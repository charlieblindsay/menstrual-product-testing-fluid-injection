# Welcome!

## Who made this repository?
The Fluid Injection subassembly team who helped make the Menstrual Product Testing Rig.

### Contact info:
- Charlie Lindsay, cbl20@ic.ac.uk

### GitHub Repo Link: https://github.com/charlieblindsay/menstrual-product-testing-fluid-injection

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
- Testing: Python files that can be run during the tests to display a real-time temperature against time graph for the heating test & the expected mass of fluid injected for the injection test.

### What is the purpose of individual files in this repository?

#### Arduino
- main.ino: Contains void setup() and void loop() functions; the arduino executes setup() once before repeating execution of loop() until the Arduino is switched off.
- heating.h: Contains constants related to heating up the fluid simulant and maintaining its temperature.
- motor.h: Contains constants related to stepper motor movements
- Waveshare_LCD1602_RGB.cpp and Waveshare_LCD1602_RGB.h: 3rd party files made by the manufacturers of the LCD used.

#### Calculation
- time_period_between_motor_movements: Based on several parameters, e.g. volume of fluid injected and total duration of test, run this program to print the required time period between motor movements. These values are used in Arduino/motor.h
- mass_injected_per_motor_movement: Based on several parameters, e.g. number of steps per motor movement, calculate the mass of fluid injected per motor movement.

#### Data Processing
- heating_test_data_saving.py: Reads the capture.txt file which contains proxy temperature data and the corresponding time. Then saves this as a text file and a scatter of temperature against time. Both are given names related to the datetime at which the file was run. These files can then be transferred to an appropriately named data folder.
- heating_test_data_processing.py: Used to recreate plots from the aforementioned saved text files. Can delete lines from the txt file to crop the corresponding plot. The temperature/time data is then saved to a csv file so it can be combined with the fluid temperature data. 

#### Testing
NOTE: Since the files in this directory are streamlit files, to run them, change directory into this directory by entering 'cd Testing' in the terminal. Then enter 'streamlit run [FILE NAME].py' in the terminal.  
- heating_test.py: After pressing 'Capture data' in Realterm to save the Serial printouts of the proxy temperature to the capture.txt file, run this file. Then it will create a live plot of temperature against time. 
- injection_test.py: Perform same steps in Realterm as with heating_test.py. This streamlit programme prints the expected mass of fluid injected from the number of motor movements performed. This can then be compared with the actual mass of fluid injected.

#### reverse_motor
- reverse_motor.ino: Run this Arduino programme to turn the stepper motor in the opposite direction. This will move the pusher block backwards. This allows the user to reset the test without having to turn the lead screw by hand.
