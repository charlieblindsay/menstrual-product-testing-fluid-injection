//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 12, 10, 13, 11);

void setup() {
	// Nothing to do (Stepper Library sets pins as outputs)
}

void loop() {
	// Rotate CW slowly at 10 RPM
	myStepper.setSpeed(1);
	myStepper.step(10); // TODO: test different number of steps to see which injects the most accurate volume of fluid
  delay(2665);
	
	// Rotate CCW quickly at 10 RPM
	// myStepper.setSpeed(10);
	// myStepper.step(-50);
	// delay(1000);
}