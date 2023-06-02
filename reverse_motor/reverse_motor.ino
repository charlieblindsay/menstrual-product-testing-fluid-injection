#include <Stepper.h>
const int stepsPerRevolution = 2038;
int motor_speed_in_rpm = 10;

Stepper stepperMotor = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // put your setup code here, to run once:
  stepperMotor.setSpeed(motor_speed_in_rpm);

}

void loop() {
  // put your main code here, to run repeatedly:
  stepperMotor.step(-1);

}
