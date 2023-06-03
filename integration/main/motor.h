#ifndef __motor_H__
#define __motor_H__

const int stepsPerRevolution = 2038;

int time_period_of_one_motor_movement_in_ms = 2960;
int motor_movement_counter = 0;
int milliseconds_since_block_reached_steady_state_temperature;

int motor_start_time;

int number_of_motor_steps_to_perform;
int number_of_motor_movements_to_perform;
int number_motor_steps_per_motor_movement = 40;

double time_since_last_motor_movement_in_ms;

#endif