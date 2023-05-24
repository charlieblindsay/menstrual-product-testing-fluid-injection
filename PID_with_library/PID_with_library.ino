#include <Adafruit_MAX31856.h>

#include <PID_v1.h>
//We define the SPI pìns
#include <SPI.h>
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

#include <Adafruit_MAX31856.h>

Adafruit_MAX31856 max = Adafruit_MAX31856(10, 11, 12, 13);

bool user_input_entered = 0;

int heater_PWM_pin = 3;

double temperature_reading;
double temperature_setpoint = 40;
double PID_output = 0;

double initial_power = 140;
bool has_reached_setpoint = 0;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=9, Ki=2, Kd=9; 
 
//create PID instance 
PID myPID(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // open the serial port at 9600 bps:
  // Use software SPI: CS, DI, DO, CLK

  max.begin();
  max.setThermocoupleType(MAX31856_TCTYPE_K);
  // max.setThermocoupleType(MAX31856_TCTYPE_T);
  myPID.SetSampleTime(50);
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetMode(AUTOMATIC);

}

void loop() {
  // put your main code here, to run repeatedly:

  if (!user_input_entered){

  }

  if (user_input_entered){
    temperature_reading = max.readThermocoupleTemperature();

    if(!has_reached_setpoint){
      if(!isnan(temperature_reading)){
        analogWrite(heater_PWM_pin,initial_power);
      }

      if(temperature_reading > temperature_setpoint){
        has_reached_setpoint = 1;
      }

    }
    
    if(has_reached_setpoint){
      if(!isnan(temperature_reading)){
        myPID.Compute(); //PID calculation
        analogWrite(heater_PWM_pin,PID_output);   //Write the output to the mosfet pin as calculated by the PID function
      }
    }

    Serial.print(temperature_reading);
    Serial.print(" ");
    Serial.println(PID_output);

    delay(50);
  }

}
