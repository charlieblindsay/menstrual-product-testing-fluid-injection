
#include <Wire.h>
#include "Waveshare_LCD1602_RGB.h"
#include <Keypad.h>
#include <SPI.h>

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {30, 32, 34, 36};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Waveshare_LCD1602_RGB lcd(16,2);  //16 characters and 2 lines of show
int r,g,b,t=0;
void setup() {
    // initialize
    lcd.init();
    Serial.begin(9600);
    lcd.setCursor(0,0);
    lcd.send_string("Waveshare");
    lcd.setCursor(0,1);
    lcd.send_string("Hello,World!");
}

void loop() {
  char customKey = customKeypad.getKey();
  if (customKey == '5'){
    lcd.clear();
    lcd.send_string("5");
  }
  r = (abs(sin(3.14*t/180)))*255;
  g = (abs(sin(3.14*(t + 60)/180)))*255;
  b = (abs(sin(3.14*(t + 120)/180)))*255;
  t = t + 3;
  lcd.setRGB(r,g,b);
  delay(150);
}