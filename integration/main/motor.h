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
    return 53;
  }

  if (flow_rate_option == 2){
    return 171;
  }
}

#endif