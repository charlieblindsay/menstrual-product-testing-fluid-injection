#ifndef __heating_H__
#define __heating_H__

int heater_PWM_pin = 3;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=10, Ki=2,Kd=10; 

// TODO: change the value below

int num_loops_before_fluid_reaches_body_temperature = 500;

// TODO: change the datatypes of these to int
double initial_temperature_setpoint = 45;
double temperature_setpoint = 37.6;
double body_temperature = 37.5;
double initial_PID_output = 255;

#endif