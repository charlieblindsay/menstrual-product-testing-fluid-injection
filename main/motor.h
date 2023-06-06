#ifndef __motor_H__
#define __motor_H__

const int stepsPerRevolution = 2038;

int time_period_of_one_motor_movement_in_ms = 2960;
int motor_movement_counter = 0;
int milliseconds_since_motor_started;

int number_of_milliseconds_before_fluid_reaches_body_temperature = 975000;
int motor_start_time;

bool fluid_has_reached_body_temperature = 0;

int number_of_motor_steps_to_perform;
int number_of_motor_movements_to_perform;
int number_motor_steps_per_motor_movement = 10;

int motor_speed_in_rpm = 1;

double time_since_last_motor_movement_in_ms;

#endif