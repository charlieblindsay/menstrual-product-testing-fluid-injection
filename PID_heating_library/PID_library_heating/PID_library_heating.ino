#include <Adafruit_MAX31856.h>
#include <SPI.h>
#include <PID_v1.h>
//We define the SPI pìns
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

int PWM_pin = 4;

double temperature_reading;
double temperature_setpoint = 37; // body temperature
double PID_output = 0;

int loop_counter = 0;

//PID parameters
double Kp=1.875, Ki=0.46875, Kd=1.875; 
 
//create PID instance 
PID myPID(&temperature_reading, &PID_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(10, 11, 12, 13);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);   
  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  //Adjust PID values
  myPID.SetTunings(Kp, Ki, Kd);

}

void loop() {
  temperature_reading = thermocouple.readThermocoupleTemperature();

  myPID.Compute(); //PID calculation
  analogWrite(PWM_pin,PID_output);   //Write the output to the mosfet pin as calculated by the PID function
  
  if(loop_counter % 10 == 0){  
    Serial.print("Setpoint Value = ");  
    Serial.println(temperature_setpoint);
    Serial.print("Temperature Reading = ");
    Serial.println(temperature_reading);
    Serial.print("PID Output Value = ");
    Serial.println(PID_output);
  }

  loop_counter ++;

  delay(300); 
}
