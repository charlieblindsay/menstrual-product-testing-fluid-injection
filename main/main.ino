// #include "stdlib.h" TODO: Remove this line if code works fine

// 3rd party libraries for:
#include <Adafruit_MAX31856.h> // Thermocouple amplifier board
#include <Stepper.h> // Stepper motor
#include <PID_v1.h> // PID control

// 3rd party serial communication library
#include <SPI.h>

// 3rd party LCD library
#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"

// Bespoke libraries containing constants
#include "motor.h"
#include "heating.h"

// TODO: put code in the loop into functions

// -------------OBJECT INSTANTIATION---------------
    // Creates an instance of stepper class
    // Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper stepperMotor = Stepper(stepsPerRevolution, 8, 10, 9, 11);

// TODO: Add pins information

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(5, 6, 7, 4);

PID PID_controller(&current_temperature_reading, &PID_output_for_heater_PWM_value, &temperature_setpoint, Kp, Ki, Kd, DIRECT);
Waveshare_LCD1602_RGB lcd(16,2);  //16 characters and 2 lines of show

// -------------INITIALIZING BOOLEAN VALUES---------------
// TODO: Move these boolean values into a header file

bool block_has_reached_initial_peak_temperature = 0;

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
bool temperature_was_between_20_and_30_throughout_last_checking_cycle = 0;

// -------------DECLARING AND INITIALIZING VARIABLES---------------

int delay_time_in_ms = 50;

int lcd_red_value = 0;
int lcd_green_value = 0;
int lcd_blue_value = 0;


const int LEFT_BUTTON_PIN = 13;
const int RIGHT_BUTTON_PIN = 12;
int right_button_last_state = 1;
int left_button_last_state = 1;
int right_button_current_state;
int left_button_current_state;

bool motor_reversing_has_stopped = 0;
bool user_has_selected_flow_rate = 0;
bool user_has_started_test = 0;

bool selected_flow_rate;

int delay_ms_between_button_presses = 1000;

int milliseconds_since_start_of_programme;

char buffer_current_temperature_reading[7];
char buffer_PID_output_for_heater_PWM_value[7];

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

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);

  // -------------OBJECT INITIALIZATION---------------

  thermocouple.begin();
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_K); // TODO: is this the right type of thermocouple?
    
  PID_controller.SetSampleTime(50);
  PID_controller.SetTunings(Kp, Ki, Kd);
  PID_controller.SetMode(AUTOMATIC);

  stepperMotor.setSpeed(motor_speed_in_rpm);

  lcd.init();
}

void loop() {

  if(!user_has_started_test){

    if(!motor_reversing_has_stopped){
      // read the state of the switch/button:
      left_button_current_state = digitalRead(LEFT_BUTTON_PIN);
      
      if(left_button_last_state == LOW && left_button_current_state == HIGH){
        motor_reversing_has_stopped = 1;
        Serial.println("Motor has stopped");
        delay(delay_ms_between_button_presses);
      }

      // save the last state
      left_button_last_state = left_button_current_state;
    }

    if(motor_reversing_has_stopped && !user_has_selected_flow_rate){
      left_button_current_state = digitalRead(LEFT_BUTTON_PIN);
      right_button_current_state = digitalRead(RIGHT_BUTTON_PIN);
      
      if(left_button_last_state == LOW && left_button_current_state == HIGH){
        user_has_selected_flow_rate = 1;
        selected_flow_rate = 0;
        Serial.println("Light flow selected");    
        delay(delay_ms_between_button_presses);
      }

      if(right_button_last_state == LOW && right_button_current_state == HIGH){
        user_has_selected_flow_rate = 1;
        selected_flow_rate = 1;
        Serial.println("Heavy flow selected");      
        delay(delay_ms_between_button_presses);
      }

      // save the last state
      left_button_last_state = left_button_current_state;
      right_button_last_state = right_button_current_state;
    }

    if(motor_reversing_has_stopped && user_has_selected_flow_rate && !user_has_started_test){
      // read the state of the switch/button:
      left_button_current_state = digitalRead(LEFT_BUTTON_PIN);
      
      if(left_button_last_state == LOW && left_button_current_state == HIGH){
        user_has_started_test = 1;
        Serial.println("Test started");
        delay(delay_ms_between_button_presses);
      }

      // save the last state
      left_button_last_state = left_button_current_state;
    }

  }

  if(user_has_started_test){
    // -------------MOTOR CONTROL---------------
    milliseconds_since_start_of_programme = millis();

    if(!fluid_has_reached_body_temperature){
      if(milliseconds_since_start_of_programme > number_of_milliseconds_before_fluid_reaches_body_temperature){
        fluid_has_reached_body_temperature = 1;
      }
    }

    if(fluid_has_reached_body_temperature){
      milliseconds_since_motor_started = milliseconds_since_start_of_programme - number_of_milliseconds_before_fluid_reaches_body_temperature;
      time_since_last_motor_movement_in_ms = milliseconds_since_motor_started - motor_movement_counter * time_period_of_one_motor_movement_in_ms;
      number_of_motor_movements_to_perform = floor(time_since_last_motor_movement_in_ms / time_period_of_one_motor_movement_in_ms); 
      number_of_motor_steps_to_perform = number_of_motor_movements_to_perform * number_motor_steps_per_motor_movement;
      stepperMotor.step(number_of_motor_steps_to_perform);
      motor_movement_counter = motor_movement_counter + number_of_motor_movements_to_perform;
    }


    // -------------TEMPERATURE READING---------------

    current_temperature_reading = thermocouple.readThermocoupleTemperature();


    // -------------SAFETY MEASURES & THERMOCOUPLE CHECKS---------------

    // Check for FIRST SAFETY MEASURE (explained above)
    // Checks that temperature reading between 20 and 30 and that previous temperature was also between 20 and 30
    // if (current_temperature_reading > 20 && current_temperature_reading < 30 && bool_indicating_temperature_between_20_and_30 == 1){
    //   bool_indicating_temperature_between_20_and_30 = 1;
    // }
    // else{
    //   bool_indicating_temperature_between_20_and_30 = 0;
    // }

    // If check for FIRST SAFETY MEASURE in previous checking cycle indicated 
    // temperature reading was always between 20 and 30
    // if (temperature_was_between_20_and_30_throughout_last_checking_cycle){
    //   lcd_red_value = 255;
    //   current_heater_PWM_value = 0;
    //   lcd.clear();
    //   lcd.setCursor(0,0);
    //   lcd.send_string("Temp not rising");
    //   lcd.setCursor(0,1);
    //   lcd.send_string("See thermocouple");
    // }

    // A checking cycle is 100 loops.
    // At the end of the checking cycle, if bool_indicating_temperature_between_20_and_30 is still equal to 1,
    // it indicates that all temperature readings during the checking cycle were between 20 and 30 degrees C
    // so temperature_was_between_20_and_30_throughout_last_checking_cycle set equal to 1
    // if (loop_counter % number_of_loops_per_checking_cycle){
    //   if (bool_indicating_temperature_between_20_and_30 == 1){
    //     temperature_was_between_20_and_30_throughout_last_checking_cycle = 1;
    //   }

    //   // Reset bool so next checking cycle for FIRST SAFETY MEASURE can be carried out
    //   bool_indicating_temperature_between_20_and_30 = 1;
    // }


      // Check for FIRST SAFETY MEASURE (explained above)
    // Checks that temperature reading between 20 and 30 and that previous temperature was also between 20 and 30
    if(!block_has_reached_constant_temperature){
      // TODO: try to reduce steady state error so can reduce 2 (see below) to 1
      if (current_temperature_reading > temperature_setpoint - 2 && current_temperature_reading < temperature_setpoint + 1 && bool_indicating_temperature_within_one_degree_of_temperature_setpoint == 1){
        bool_indicating_temperature_within_one_degree_of_temperature_setpoint = 1;
      }
      else{
        bool_indicating_temperature_within_one_degree_of_temperature_setpoint = 0;
      }

      // If statement for performing constant temperature check at the end of the checking cycle
      if (millis() / time_period_of_constant_temperature_checks > number_of_temperature_checks_completed + 1){
        number_of_temperature_checks_completed = number_of_temperature_checks_completed + 1;
        if (bool_indicating_temperature_within_one_degree_of_temperature_setpoint == 1){
          block_has_reached_constant_temperature = 1;

          motor_start_time = millis();
        }
        else{
          // Reset bool so next checking cycle for FIRST SAFETY MEASURE can be carried out
          bool_indicating_temperature_within_one_degree_of_temperature_setpoint = 1;
        }

      }
    }


    // SECOND SAFETY MEASURE
    // If temperature goes below 20 degrees, it is likely that the thermocouple is wired the wrong way 
    // around, alert the user and turn the heaters off
    // if(current_temperature_reading < 20){
    //   lcd_red_value = 255;
    //   current_heater_PWM_value = 0;
    //   lcd.clear();
    //   lcd.setCursor(0,0);
    //   lcd.send_string("Temp < 20. See");
    //   lcd.setCursor(0,1);
    //   lcd.send_string("Thermocoup wires");
    // }

    // If the thermocouple is not working (i.e. is reading a null value):
    if(isnan(current_temperature_reading)){
      current_heater_PWM_value = 0;
    }

    // -------------IF THERMOCOUPLE WORKING CORRECTLY AND READING CORRECT TEMPERATURE---------------
    if(!isnan(current_temperature_reading) && current_temperature_reading > 20 && !temperature_was_between_20_and_30_throughout_last_checking_cycle){
      lcd_red_value = 0; // turn off previous red warning light on LCD
      
      if(!block_has_reached_initial_peak_temperature){
        current_heater_PWM_value = max_PWM_value; // set the heaters to maximum power

        if(current_temperature_reading > initial_peak_temperature){
          block_has_reached_initial_peak_temperature = 1;
        }
      }

      if(block_has_reached_initial_peak_temperature){
        //PID calculation sets the value of PID_output_for_heater_PWM_value using the current_temperature_reading and temperature_setpoint
        PID_controller.Compute(); 
        current_heater_PWM_value = PID_output_for_heater_PWM_value;
      }

    }

    // -------------SETTING HEATER PWM VALUE---------------
    analogWrite(heater_PWM_pin, current_heater_PWM_value);   // Write the current_heater_PWM_value to the heater PWM pin

      // -------------PRINTING TO LCD---------------

    // write_text_and_number_to_lcd(0, "Temp: ", current_temperature_reading, buffer_current_temperature_reading, 5);
    // write_text_and_number_to_lcd(1, "PWM out: ", PID_output_for_heater_PWM_value, buffer_PID_output_for_heater_PWM_value, 9);

    // Printing to LCD and serial
    // lcd.clear();
    lcd.setCursor(0,0);
    lcd.send_string("Temp: "); // TODO: convert current_temperature_reading to a string and print it to LCD

    lcd.setCursor(6, 0);
      // converts current_temperature_reading number to a string (base 10) and sends to LCD
    lcd.send_string(itoa(current_temperature_reading, buffer_current_temperature_reading, 10));

    lcd.setCursor(0,1);
    lcd.send_string("PWM out: ");

    lcd.setCursor(10, 1);
    lcd.send_string(itoa(PID_output_for_heater_PWM_value, buffer_PID_output_for_heater_PWM_value, 10));

    // -------------PRINTING TO SERIAL---------------

    // TODO: Remove and add to testing branch
    // Serial.print(" ");
    // Serial.print(number_of_temperature_checks_completed);
    // Serial.print(" ");
    // Serial.print(block_has_reached_constant_temperature);
    // Serial.print(" ");
    Serial.print(current_temperature_reading);
    Serial.print(" ");
    Serial.print(millis() / 1000);
    Serial.print(" ");
    Serial.println(current_heater_PWM_value);

    lcd.setRGB(lcd_red_value,lcd_green_value,lcd_blue_value);

    delay(delay_time_in_ms);
  }
}