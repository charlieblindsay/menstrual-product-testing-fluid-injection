#include <Adafruit_MAX31856.h>
#include <SPI.h>
#include <PID_v1.h>
//We define the SPI pìns
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

#include <Adafruit_MAX31856.h>

Adafruit_MAX31856 max = Adafruit_MAX31856(10, 11, 12, 13);

int PWM_pin = 4;

double temperature_reading;
double temperature_setpoint = 37; // body temperature
double PID_output = 0;

int loop_counter = 0;

//PID parameters
double Kp=1.875, Ki=0.46875, Kd=1.875; 
 
//create PID instance 
PID myPID(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // open the serial port at 9600 bps:
  // Use software SPI: CS, DI, DO, CLK

  max.begin();
  max.setThermocoupleType(MAX31856_TCTYPE_T);
  myPID.SetSampleTime(200);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetMode(AUTOMATIC);

}

void loop() {
  // put your main code here, to run repeatedly:

  myPID.Compute(); //PID calculation
  analogWrite(PWM_pin,PID_output);   //Write the output to the mosfet pin as calculated by the PID function

  float tip_temp = max.readThermocoupleTemperature();
  Serial.println(tip_temp);
  Serial.println(PID_output);
  // lcd.setCursor(0,0);
  // lcd.print("TEMPERATURE");
  // lcd.setCursor(7,1);  
  // lcd.print(temperature_read,1);    
  delay(1000);

}
