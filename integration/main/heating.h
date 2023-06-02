#ifndef __heating_H__
#define __heating_H__



int heater_PWM_pin = 3;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=10, Ki=7,Kd=8; 

// TODO: change the value below

double temperature_reading;
double temperature_reading_offset = 0;

double time_period_of_constant_temperature_checks = 60000;
int number_of_temperature_checks_completed = 0;

double PID_output;

double current_PWM_output;

// TODO: change the datatypes of these to int
double initial_temperature_setpoint = 55; // TODO: change back to 50
double temperature_setpoint = 47;
int initial_PID_output = 255;

#endif