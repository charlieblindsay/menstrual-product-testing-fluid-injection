/*    Max6675 Module  ==>   Arduino
 *    CS              ==>     D10
 *    SO              ==>     D12
 *    SCK             ==>     D13
 *    Vcc             ==>     Vcc (5v)
 *    Gnd             ==>     Gnd      */

//LCD config (i2c LCD screen, you need to install the LiquidCrystal_I2C if you don't have it )
// #include <Wire.h> 
#include <Adafruit_MAX31856.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x3f,20,4);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

/*    i2c LCD Module  ==>   Arduino
 *    SCL             ==>     A5
 *    SDA             ==>     A4
 *    Vcc             ==>     Vcc (5v)
 *    Gnd             ==>     Gnd      */

#include <SPI.h>
//We define the SPI pìns
#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

//Pins
int PWM_pin = 3;


//Variables

float temperature_read = 0.0;
float set_temperature = 37; // body temperature
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
int PID_value = 0;

//PID constants
int kp = 9.1;   int ki = 0.3;   int kd = 1.8;
int PID_p = 0;    int PID_i = 0;    int PID_d = 0;

//Thermocouple object
Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(10, 11, 12, 13);
// max31856_noise_filter_t noise_filter = MAX31856_NOISE_FILTER_50HZ;



void setup() {
  pinMode(PWM_pin,OUTPUT);
  TCCR2B = TCCR2B & B11111000 | 0x03;    // pin 3 and 11 PWM frequency of 980.39 Hz
  Time = millis(); 
  // lcd.init();
  // lcd.backlight();

  Serial.begin(9600); // open the serial port at 9600 bps:  

  thermocouple.begin();
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_T);
  // setNoiseFilter(MAX31856_NOISE_FILTER_50HZ);
}


void loop() {
 // First we read the real value of temperature
  float temperature_read = thermocouple.readThermocoupleTemperature();
  //Next we calculate the error between the setpoint and the real value
  PID_error = set_temperature - temperature_read;
  //Calculate the P value
  PID_p = kp * PID_error;
  //Calculate the I value in a range on +-3
  if(-3 < PID_error <3)
  {
    PID_i = PID_i + (ki * PID_error);
  }

  //For derivative we need real time to calculate speed change rate
  timePrev = Time;                            // the previous time is stored before the actual time read
  Time = millis();                            // actual time read
  elapsedTime = (Time - timePrev) / 1000; 
  //Now we can calculate the D calue
  PID_d = kd*((PID_error - previous_error)/elapsedTime);
  //Final total PID value is the sum of P + I + D
  PID_value = PID_p + PID_i + PID_d;

  //We define PWM range between 0 and 255
  if(PID_value < 0)
  {    PID_value = 0;    }
  if(PID_value > 255)  
  {    PID_value = 255;  }
  //Now we can write the PWM signal to the mosfet on digital pin D3
  analogWrite(PWM_pin,255-PID_value);
  Serial.println(temperature_read);
  Serial.println(255-PID_value);
  previous_error = PID_error;     //Remember to store the previous error for next loop.

  delay(300);
  //lcd.clear();
  
  // lcd.setCursor(0,0);
  // lcd.print("PID TEMP control");
  // lcd.setCursor(0,1);
  // lcd.print("S:");
  // lcd.setCursor(2,1);
  // lcd.print(set_temperature,1);
  // lcd.setCursor(9,1);
  // lcd.print("R:");
  // lcd.setCursor(11,1);
  // lcd.print(temperature_read,1);
}
