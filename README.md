# Welcome!

NOTE: this will be finished after the other deadlines.

TODO: Add testing files from other branch

## Who made this repository?
The Fluid Injection subassembly team who helped make the Menstrual Product Testing Rig.

### Contact info:
- Charlie Lindsay, cbl20@ic.ac.uk

## User Instructions
1. Wire up the electronics components 
2. Plug the Arduino into the computer using a USB cable
3. Open the Arduino folder in the Arduino IDE
4. Open the main.ino file
5. Click upload
6. Turn on the mains power
7. Follow the instructions on the LCD screen

## Developer Instructions

### What is the purpose of each folder in this repository?
- Arduino: Contains Arduino programme for normal operation of the device.
- Calculation: Python files for determining constants used in Arduino programme, e.g. time period of motor movements
- Data Processing: Reads Realterm capture.txt file from heating test and saves new txt file and plot of temperature against time. Suitable for Windows users since reliant on use of Realterm.
- Testing Demo: Python files that can be run during the tests to display a real-time temperature against time graph for the heating test & the expected mass of fluid injected for the injection test.

### What is the purpose of individual files in this repository?

#### Arduino
- main.ino: Contains void setup() and void loop() functions; the arduino executes setup() once before repeating execution of loop() until the Arduino is switched off.
- heating.h: Contains constants related to heating up the fluid simulant and maintaining its temperature.
- motor.h: Contains constants related to stepper motor movements
- Waveshare_LCD1602_RGB.cpp and Waveshare_LCD1602_RGB.h: 3rd party files made by the manufacturers of the LCD used.

#### Calculation
- time_period_between_motor_movements: Based on several parameters, e.g. volume of fluid injected and total duration of test, run this program to print the required time period between motor movements. These values are used in Arduino/motor.h
- mass_injected_per_motor_movement: Based on several parameters, e.g. number of steps per motor movement, calculate the mass of fluid injected per motor movement.