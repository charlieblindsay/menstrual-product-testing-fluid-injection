//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper stepperMotor = Stepper(stepsPerRevolution, 8, 10, 9, 11);

int time_period_of_one_motor_movement_in_ms = 2960;
int motor_movement_counter = 0;
int milliseconds_since_start_of_program;

int number_of_motor_steps_to_perform;
int number_of_motor_movements_to_perform;
int number_motor_steps_per_motor_movement = 40;

double time_since_last_motor_movement_in_ms;

void setup() {
  stepperMotor.setSpeed(1);
}

void loop() {
  milliseconds_since_start_of_program = millis();
  time_since_last_motor_movement_in_ms = milliseconds_since_start_of_program - motor_movement_counter * time_period_of_one_motor_movement_in_ms;
  number_of_motor_movements_to_perform = floor(time_since_last_motor_movement_in_ms / time_period_of_one_motor_movement_in_ms); 
  number_of_motor_steps_to_perform = number_of_motor_movements_to_perform * number_motor_steps_per_motor_movement;
  stepperMotor.step(number_of_motor_steps_to_perform);
  motor_movement_counter = motor_movement_counter + number_of_motor_movements_to_perform;


}