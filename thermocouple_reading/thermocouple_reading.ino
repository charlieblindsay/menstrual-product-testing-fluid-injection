#include <Adafruit_MAX31856.h>

/*    Max6675 Module  ==>   Arduino
 *    CS              ==>     D10
 *    SO              ==>     D12
 *    SCK             ==>     D13
 *    Vcc             ==>     Vcc (5v)
 *    Gnd             ==>     Gnd      */

// //LCD config
// #include <Wire.h> 
// #include <LiquidCrystal_I2C.h>    //If you don't have the LiquidCrystal_I2C library, download it and install it
// LiquidCrystal_I2C lcd(0x3f,20,4);  //sometimes the adress is not 0x3f. Change to 0x27 if it dosn't work.

/*    i2c LCD Module  ==>   Arduino
 *    SCL             ==>     A5
 *    SDA             ==>     A4
 *    Vcc             ==>     Vcc (5v)
 *    Gnd             ==>     Gnd      */

#include <SPI.h>

#define MAX6675_CS   10
#define MAX6675_SO   12
#define MAX6675_SCK  13

Adafruit_MAX31856 thermocouple = Adafruit_MAX31856(10, 11, 12, 13);

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  // Use software SPI: CS, DI, DO, CLK

  thermocouple.begin();
  thermocouple.setThermocoupleType(MAX31856_TCTYPE_T);
}

void loop() {
  float temperature_reading = thermocouple.readThermocoupleTemperature();
  Serial.println(temperature_reading);
  delay(1000);
}