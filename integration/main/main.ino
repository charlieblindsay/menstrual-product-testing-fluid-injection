#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"
#include <SPI.h>
#include <Stepper.h>
#include <PID_v1.h>
#include "motor.h"
#include "heating.h"
#include "stdlib.h"
#include <Adafruit_MAX31856.h>

// TODO: put code in the loop into functions

// bool has_user_input_been_entered = 0;

// USER INPUT POINT
// This is how the user changes between light and heavy flow rates
// Set flow_rate_option = 1 for light flow (5 ml over 4 hours) - CURRENT SETTING
// Set flow_rate_option = 2 for heavy flow (15 ml over 4 hours)

int flow_rate_option = 1;

// -------------OBJECT INSTANTIATION---------------
    // Creates an instance of stepper class
    // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

// TODO: Add pins information

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(5, 6, 7, 4);
PID PID_controller(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);
Waveshare_LCD1602_RGB lcd(16,2);  //16 characters and 2 lines of show

// -------------INITIALIZING BOOLEAN VALUES---------------
bool has_reached_initial_setpoint = 0;
bool has_fluid_reached_body_temperature = 0;

// FIRST SAFETY MEASURE
// If the thermocouple reads between 20 and 30 degrees for a minute, it is likely that the 
// thermocouple has been removed from the block.
// The heater should then be turned off and the user should be alerted

// This boolean is set equal to 1 at the start of each checking cycle, 
// but will be set to 0 if a single temperature reading is not between 20 and 30
// during the checking cycle.
// A checking cycle is equivalent to 100 loops
bool bool_indicating_temperature_between_20_and_30 = 1;

// This boolean is set equal to 0 and is only set equal to 1 if the above boolean
// is 1 at the end of the checking cycle.
// NOTE: this only occurs if all temperature readings in the checking cycle are between 20 and 30
// For reasons mentioned above, the heaters should then be turned off
bool bool_indicating_temperature_was_always_between_20_and_30_throughout_last_checking_cycle = 0;

// -------------DECLARING AND INITIALIZING VARIABLES---------------
int loop_counter=0;
int number_of_loops_per_checking_cycle = 100;
int delay_time_in_ms = 50;
int number_of_delays_between_motor_steps;

int lcd_red_value = 0;
int lcd_green_value = 0;
int lcd_blue_value = 0;

char buffer_temperature_reading[7];
char buffer_PID_output[7];

// TODO: Move function to header file
void write_text_and_number_to_lcd(Waveshare_LCD1602_RGB lcd, int row_number, char text_to_write, int number_to_write, char buffer, int text_length){
    lcd.setCursor(0,row_number);
    lcd.send_string(text_to_write);

    lcd.setCursor(text_length + 1, text_length);
    lcd.send_string(itoa(number_to_write, buffer, 10));
}

void setup() {
  Serial.begin(9600); // TODO: Remove this

    // TODO: put these 4 blocks below in init functions - pass the object instance as input?

  // -------------OBJECT INITIALIZATION---------------

  thermocouple.begin();
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_K); // TODO: is this the right type of thermocouple?
    
  PID_controller.SetSampleTime(50);
  PID_controller.SetTunings(Kp, Ki, Kd);
  PID_controller.SetMode(AUTOMATIC);

  myStepper.setSpeed(1);
  number_of_delays_between_motor_steps = get_number_of_delays_between_motor_steps(flow_rate_option);

  lcd.init();
}

void loop() {

  // -------------MOTOR CONTROL---------------
  if (loop_counter % number_of_delays_between_motor_steps == 0){
    myStepper.step(10); // TODO: test different number of steps to see which injects the most accurate volume of fluid
  }

  // -------------TEMPERATURE READING---------------

  temperature_reading = thermocouple.readThermocoupleTemperature();

  // -------------LCD---------------

  // write_text_and_number_to_lcd(0, "Temp: ", temperature_reading, buffer_temperature_reading, 5);
  // write_text_and_number_to_lcd(1, "PWM out: ", PID_output, buffer_PID_output, 9);

  // Printing to LCD and serial
  // lcd.clear();
  lcd.setCursor(0,0);
  lcd.send_string("Temp: "); // TODO: convert temperature_reading to a string and print it to LCD

  lcd.setCursor(6, 0);
    // converts temperature_reading number to a string (base 10) and sends to LCD
  lcd.send_string(itoa(temperature_reading, buffer_temperature_reading, 10));

  lcd.setCursor(0,1);
  lcd.send_string("PWM out: ");

  lcd.setCursor(10, 1);
  lcd.send_string(itoa(PID_output, buffer_PID_output, 10));

  // -------------PRINTING TO SERIAL---------------

  // TODO: Remove and add to testing branch
  Serial.print(temperature_reading);
  Serial.print(" ");
  Serial.print(loop_counter);
  Serial.print(" ");
  Serial.println(current_PWM_output);

  lcd.setRGB(lcd_red_value,lcd_green_value,lcd_blue_value);


  // -------------SAFETY MEASURES & THERMOCOUPLE CHECKS---------------

  // Check for FIRST SAFETY MEASURE (explained above)
  // Checks that temperature reading between 20 and 30 and that previous temperature was also between 20 and 30
  if (temperature_reading > 20 && temperature_reading < 30 && bool_indicating_temperature_between_20_and_30 == 1){
    bool_indicating_temperature_between_20_and_30 = 1;
  }
  else{
    bool_indicating_temperature_between_20_and_30 = 0;
  }

  // If check for FIRST SAFETY MEASURE in previous checking cycle indicated 
  // temperature reading was always between 20 and 30
  if (bool_indicating_temperature_was_always_between_20_and_30_throughout_last_checking_cycle){
    lcd_red_value = 255;
    current_PWM_output = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.send_string("Temp not rising");
    lcd.setCursor(0,1);
    lcd.send_string("See thermocouple");
  }

  // A checking cycle is 100 loops.
  // At the end of the checking cycle, if bool_indicating_temperature_between_20_and_30 is still equal to 1,
  // it indicates that all temperature readings during the checking cycle were between 20 and 30 degrees C
  // so bool_indicating_temperature_was_always_between_20_and_30_throughout_last_checking_cycle set equal to 1
  if (loop_counter % number_of_loops_per_checking_cycle){
    if (bool_indicating_temperature_between_20_and_30 == 1){
      bool_indicating_temperature_was_always_between_20_and_30_throughout_last_checking_cycle = 1;
    }

    // Reset bool so next checking cycle for FIRST SAFETY MEASURE can be carried out
    bool_indicating_temperature_between_20_and_30 = 1;
  }

  // SECOND SAFETY MEASURE
  // If temperature goes below 20 degrees, it is likely that the thermocouple is wired the wrong way 
  // around, alert the user and turn the heaters off
  if(temperature_reading < 20){
    lcd_red_value = 255;
    current_PWM_output = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.send_string("Temp < 20. See");
    lcd.setCursor(0,1);
    lcd.send_string("Thermocoup wires");
  }

  // If the thermocouple is not working (i.e. is reading a null value):
  if(isnan(temperature_reading)){
    current_PWM_output = 0;
  }

  // -------------THERMOCOUPLE WORKING CORRECTLY---------------

  // If the thermocouple reads a value and is greater than 20:
  if(!isnan(temperature_reading) && temperature_reading > 20 && bool_indicating_temperature_was_always_between_20_and_30_throughout_last_checking_cycle == 0){
    lcd_red_value = 0;

      // If has_reached_initial_setpoint bool equal to false, keep the PID output constant
    if(!has_reached_initial_setpoint){
      current_PWM_output = initial_PID_output;
      
        
        // If current temperature reading above setpoint temperature, set the has_reached_initial_setpoint bool to true 
      if(temperature_reading > initial_temperature_setpoint){
        has_reached_initial_setpoint = 1;
      }
    }

    if (!has_fluid_reached_body_temperature && loop_counter > num_loops_before_fluid_reaches_body_temperature){
      has_fluid_reached_body_temperature = 1;
      temperature_setpoint = body_temperature;        
    }

      // If setpoint temperature reached, use PID control
    if(has_reached_initial_setpoint){
      PID_controller.Compute(); //PID calculation
      current_PWM_output = PID_output;
         //Write the output to the mosfet pin as calculated by the PID function
    }

  }

  // Write the PWM output to the heater PWM pin
  analogWrite(heater_PWM_pin,current_PWM_output);

  loop_counter = loop_counter + 1;

  delay(delay_time_in_ms);
}