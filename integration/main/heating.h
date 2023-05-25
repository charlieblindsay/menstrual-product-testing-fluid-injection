#ifndef __heating_H__
#define __heating_H__


int heater_PWM_pin = 3;

//PID parameters
// double Kp=1.875, Ki=0.46875, Kd=1.875; 
double Kp=10, Ki=1,Kd=10; 

// TODO: change the value below

double initial_temperature_setpoint = 45;
double temperature_setpoint = 43; // body temperature
double initial_PID_output = 180;

#endif