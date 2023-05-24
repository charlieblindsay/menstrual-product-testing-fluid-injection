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

bool has_reached_setpoint = 0;
double temperature_reading;
double PID_output;
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
      analogWrite(heater_PWM_pin,0); // turn off heater
    }

    // If the thermocouple reads a value:
    if(!isnan(temperature_reading)){
      
      // If has_reached_setpoint bool equal to false, keep the PID output constant
      if(!has_reached_setpoint){
        analogWrite(heater_PWM_pin,initial_PID_output);
        
        // If current temperature reading above setpoint temperature, set the has_reached_setpoint bool to true 
        if(temperature_reading > temperature_setpoint){
          has_reached_setpoint = 1;
        }
      }

      // If setpoint temperature reached, use PID control
      if(has_reached_setpoint){
        myPID.Compute(); //PID calculation
        analogWrite(heater_PWM_pin,PID_output);   //Write the output to the mosfet pin as calculated by the PID function
      }

    }

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
    lcd.send_string(itoa(temperature_reading, buffer_PID_output, 10));

    if (counter % number_of_delays_between_motor_steps == 0){
      myStepper.step(10); // TODO: test different number of steps to see which injects the most accurate volume of fluid
    }

    // r = (abs(sin(3.14*counter/180)))*255;
    // g = (abs(sin(3.14*(counter + 60)/180)))*255;
    // b = (abs(sin(3.14*(counter + 120)/180)))*255;
    counter = counter + 1;

    Serial.println(temperature_reading);
    Serial.println(PID_output);

    lcd.setRGB(r,g,b);

    delay(50);
  // }

}

// TODO: see README for help if stuck