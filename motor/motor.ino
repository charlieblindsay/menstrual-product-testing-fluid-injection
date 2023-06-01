//Includes the Arduino Stepper Library
#include <Stepper.h>

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;
int motor_speed_in_RPM = 1;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper stepperMotor = Stepper(stepsPerRevolution, 8, 10, 9, 11); 

int time_period_of_one_motor_movement_in_ms = 8879;
int motor_movement_counter = 0;
int milliseconds_since_start_of_program;

int number_of_motor_steps_to_perform;
int number_of_motor_movements_to_perform;
int number_motor_steps_per_motor_movement = 40;

double time_since_last_motor_movement_in_ms;

double mass_injected_per_motor_movement = 0.01442851189283634;
double total_mass_injected;

bool countdown_completed = 0;
int countdown_counter = 60;

void setup() {
  stepperMotor.setSpeed(motor_speed_in_RPM);
  Serial.begin(9600);
}

void loop() {

  if(countdown_completed == 0){
    countdown_counter -= 1;
    Serial.println("Counting down");

    if(countdown_counter == 0){
      countdown_completed = 1;
    }

    delay(1000);
  }

  if(countdown_completed == 1){
    total_mass_injected = motor_movement_counter * mass_injected_per_motor_movement;
    Serial.println(total_mass_injected);

    milliseconds_since_start_of_program = millis();
    time_since_last_motor_movement_in_ms = milliseconds_since_start_of_program - motor_movement_counter * time_period_of_one_motor_movement_in_ms;
    number_of_motor_movements_to_perform = floor(time_since_last_motor_movement_in_ms / time_period_of_one_motor_movement_in_ms); 
    number_of_motor_steps_to_perform = number_of_motor_movements_to_perform * number_motor_steps_per_motor_movement;
    stepperMotor.step(number_of_motor_steps_to_perform);
    motor_movement_counter = motor_movement_counter + number_of_motor_movements_to_perform;
    delay(50);    
  }

}