#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"
#include <Keypad.h>
#include <SPI.h>
#include <Stepper.h>
#include <Adafruit_MAX31856.h>
#include <PID_v1.h>
#include "motor.h"
#include "keypad.h"
#include "heating.h"

// TODO: put code in the loop into functions

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(10, 11, 12, 13);

bool has_reached_setpoint = 0;
double temperature_reading;
double PID_output;
 
//create PID instance 
PID myPID(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

// Defines the number of steps per rotation

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 12, 10, 13, 11);

// Create keypad instance
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Waveshare_LCD1602_RGB lcd(16,2);  //16 characters and 2 lines of show
int r,g,b,counter=0;

void setup() {
    lcd.init();
    Serial.begin(9600);

    myStepper.setSpeed(1);

    thermocouple.begin();
    thermocouple.setThermocoupleType(MAX31856_TCTYPE_T);
    myPID.SetSampleTime(50);
    myPID.SetTunings(Kp, Ki, Kd);
    myPID.SetMode(AUTOMATIC);
}

void loop() {
  temperature_reading = thermocouple.readThermocoupleTemperature();

  if(!has_reached_setpoint){
    if(!isnan(temperature_reading)){
      analogWrite(PWM_pin,initial_PID_output);
    }

    if(temperature_reading > temperature_setpoint){
      has_reached_setpoint = 1;
    }
  }
  
  if(has_reached_setpoint){
    if(!isnan(temperature_reading)){
      myPID.Compute(); //PID calculation
      analogWrite(PWM_pin,PID_output);   //Write the output to the mosfet pin as calculated by the PID function
  }}

  // Printing to LCD and serial
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.send_string("Temp: "); // TODO: convert temperature_reading to a string and print it to LCD
  lcd.setCursor(0,1);
  lcd.send_string("PWM out: ");

  Serial.print(temperature_reading);
  Serial.print(" ");
  Serial.println(PID_output);

  if (counter % 53 == 0){
    // A delay of 53 * 50 = 2650ms is required between motor steps
    myStepper.step(10); // TODO: test different number of steps to see which injects the most accurate volume of fluid
  }

  r = (abs(sin(3.14*counter/180)))*255;
  g = (abs(sin(3.14*(counter + 60)/180)))*255;
  b = (abs(sin(3.14*(counter + 120)/180)))*255;
  counter = counter + 1;
  lcd.setRGB(r,g,b);

  delay(50);
}

// TODO: see README for help if stuck