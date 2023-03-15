#include <Adafruit_MAX31856.h>
#include <SPI.h>
#include <PID_v1.h>
//We define the SPI pìns
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

int PWM_pin = 3;

double temperature_reading = 0.0;
double temperature_setpoint = 37; // body temperature
double PWM_output ; //LED

int loop_counter = 0;

//PID parameters
double Kp=0, Ki=10, Kd=0; 
 
//create PID instance 
PID myPID(&temperature_reading, &PWM_output, &temperature_setpoint, Kp, Ki, Kd, DIRECT);

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
  // put your main code here, to run repeatedly:

  temperature_reading = thermocouple.readThermocoupleTemperature();

  //PID calculation
  myPID.Compute();

  //Write the output as calculated by the PID function
  analogWrite(4,PWM_output); //LED is set to digital 3 this is a pwm pin. 
  
  if(loop_counter % 10 == 0){  
    Serial.print("Setpoint Value = ");  
    Serial.println(temperature_setpoint);
    Serial.print("Temperature Reading = ");
    Serial.println(temperature_reading);
    Serial.print("PWM Output Value = ");
    Serial.println(PWM_output);
  }

  loop_counter ++;

  delay(100); 
}
