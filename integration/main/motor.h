#ifndef __motor_H__
#define __motor_H__

const int stepsPerRevolution = 2038;

// Function which returns the number of delays between motor steps
// Inputs:
// - flow_rate_option (int), possible options are: 
//       1 - light flow rate (5 ml over 4 hours)
//       2 - heavy flow rate (15 ml over 4 hours)

// Outputs:
// - number_of_delays_between_motor_steps (int).
//      This is calculated using the calc_delay_time motor.py

int get_number_of_delays_between_motor_steps(int flow_rate_option){
  if (flow_rate_option == 1){
    return 171;
  }

  if (flow_rate_option == 2){
    return 53;
  }
}

// int time_period_of_one_motor_movement_in_ms = 2960;
int time_period_of_one_motor_movement_in_ms = 2960;
int motor_movement_counter = 0;
int milliseconds_since_start_of_program;

int number_of_motor_steps_to_perform;
int number_of_motor_movements_to_perform;
int number_motor_steps_per_motor_movement = 40;

double time_since_last_motor_movement_in_ms;

#endif