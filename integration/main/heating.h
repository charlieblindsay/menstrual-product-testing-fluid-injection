#ifndef __heating_H__
#define __heating_H__

int heater_PWM_pin = 3;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=10, Ki=7,Kd=8;

double current_temperature_reading;

double time_period_of_constant_temperature_checks = 120000;

bool block_has_reached_constant_temperature = 0;
bool bool_indicating_temperature_within_one_degree_of_temperature_setpoint = 1;

int number_of_temperature_checks_completed = 0;

double PID_output_for_heater_PWM_value;

double current_heater_PWM_value;
int max_PWM_value = 255;

// TODO: change the datatypes of these to int
double initial_peak_temperature = 55; // TODO: change back to 50
double temperature_setpoint = 47;


#endif