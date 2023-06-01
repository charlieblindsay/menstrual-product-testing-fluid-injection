#ifndef __heating_H__
#define __heating_H__



int heater_PWM_pin = 3;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=10, Ki=2,Kd=10; 

// TODO: change the value below

int num_loops_before_fluid_reaches_body_temperature = 0;

double temperature_reading;
double temperature_reading_offset = 0;

double PID_output;

double current_PWM_output;

// TODO: change the datatypes of these to int
double initial_temperature_setpoint = 40;
double temperature_setpoint = 37;
double body_temperature = 30;
int initial_PID_output = 255;

#endif