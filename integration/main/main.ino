#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"
#include <SPI.h>
#include <Stepper.h>
#include <Adafruit_MAX31856.h>
#include <PID_v1.h>
#include "motor.h"
#include "heating.h"
#include "stdlib.h"

// TODO: put code in the loop into functions

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(5, 6, 7, 4);

// bool has_user_input_been_entered = 0;

// USER INPUT POINT
// This is how the user changes between light and heavy flow rates
// Set flow_rate_option = 1 for light flow (5 ml over 4 hours) - CURRENT SETTING
// Set flow_rate_option = 2 for heavy flow (15 ml over 4 hours)

int flow_rate_option = 1;

int number_of_delays_between_motor_steps;

bool has_reached_initial_setpoint = 0;
bool has_reached_num_loops_before_fluid_reaches_body_temperature = 0;
double temperature_reading;
double PID_output;
double PID_output_for_body_temperature;

double current_PWM_output;

char buffer_temperature_reading[7];
char buffer_PID_output[7];

//create PID instance 
PID myPID(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

// Defines the number of steps per rotation

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

Waveshare_LCD1602_RGB lcd(16,2);  //16 characters and 2 lines of show
int r,g,b,counter=0;

void write_text_and_number_to_lcd(int row_number, char text_to_write, int number_to_write, char buffer, int text_length){
    lcd.setCursor(0,row_number);
    lcd.send_string(text_to_write);

    lcd.setCursor(text_length + 1, text_length);
    lcd.send_string(itoa(number_to_write, buffer, 10));
}

void setup() {
    lcd.init();
    Serial.begin(9600);

    myStepper.setSpeed(1);

    thermocouple.begin();
    thermocouple.setThermocoupleType(MAX31856_TCTYPE_K); // TODO: is this the right type of thermocouple?
    myPID.SetSampleTime(50);
    myPID.SetTunings(Kp, Ki, Kd);
    myPID.SetMode(AUTOMATIC);

    number_of_delays_between_motor_steps = get_number_of_delays_between_motor_steps(flow_rate_option);

    // intial_temperature = thermocouple.readThermocoupleTemperature();
}

void loop() {

  // if(!has_user_input_been_entered){
  //   Serial.println('Enter 1 for light flow and 2 for heavy flow:');
  //   while(Serial.available() > 0){
  //     char user_input = Serial.read();
  //     Serial.println(user_input);

  //     has_user_input_been_entered = 1;
  //   }
  // }

  // if (has_user_input_been_entered){
    temperature_reading = thermocouple.readThermocoupleTemperature();

    // If the thermocouple is not working (i.e. is reading a null value):
    if(isnan(temperature_reading)){
      current_PWM_output = 0;
      analogWrite(heater_PWM_pin,current_PWM_output); // turn off heater
    }

    // If the thermocouple reads a value:
    if(!isnan(temperature_reading)){
      
      // If has_reached_initial_setpoint bool equal to false, keep the PID output constant
      if(!has_reached_initial_setpoint){
        current_PWM_output = initial_PID_output;
        analogWrite(heater_PWM_pin,current_PWM_output);
        
        // If current temperature reading above setpoint temperature, set the has_reached_initial_setpoint bool to true 
        if(temperature_reading > initial_temperature_setpoint){
          has_reached_initial_setpoint = 1;
        }
      }

      if (!has_reached_num_loops_before_fluid_reaches_body_temperature && counter > num_loops_before_fluid_reaches_body_temperature){
        has_reached_num_loops_before_fluid_reaches_body_temperature = 1;
        temperature_setpoint = body_temperature;        
      }

      // If setpoint temperature reached, use PID control
      if(has_reached_initial_setpoint){
        myPID.Compute(); //PID calculation
        current_PWM_output = PID_output;
        analogWrite(heater_PWM_pin,current_PWM_output);   //Write the output to the mosfet pin as calculated by the PID function
      }

    }

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

    if (counter % number_of_delays_between_motor_steps == 0){
      myStepper.step(10); // TODO: test different number of steps to see which injects the most accurate volume of fluid
    }

    // r = (abs(sin(3.14*counter/180)))*255;
    // g = (abs(sin(3.14*(counter + 60)/180)))*255;
    // b = (abs(sin(3.14*(counter + 120)/180)))*255;
    counter = counter + 1;

    Serial.print(temperature_reading);
    Serial.print(" ");
    Serial.print(counter);
    Serial.print(" ");
    Serial.println(current_PWM_output);

    lcd.setRGB(r,g,b);

    delay(50);
  // }

}

// TODO: see README for help if stuck