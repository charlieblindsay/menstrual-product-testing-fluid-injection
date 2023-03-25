import math

number_motor_steps_in_one_revolution = 2038
volume_fluid_injected = 15 * 10 ** (-6)
total_time = 60 * 60 * 4
diameter_of_syringe = 0.02
lead_screw_pitch = 0.002
number_motor_steps_per_motor_movement = 10
motor_RPM = 1

def calc_volume_injected_per_step(diameter_of_syringe, lead_screw_pitch, number_motor_steps_per_motor_movement):
    area_of_syringe = math.pi * (diameter_of_syringe / 2) ** 2
    syringe_distance_per_step = (number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution) * lead_screw_pitch
    volume_injected_per_step = syringe_distance_per_step * area_of_syringe
    return volume_injected_per_step

def calc_time_period_of_step(volume_fluid_injected, total_time, diameter_of_syringe, lead_screw_pitch, number_motor_steps_per_motor_movement):
    volume_flow_rate = volume_fluid_injected/total_time
    volume_injected_per_step = calc_volume_injected_per_step(diameter_of_syringe, lead_screw_pitch, number_motor_steps_per_motor_movement)
    time_period_of_step = volume_injected_per_step / volume_flow_rate
    return time_period_of_step

def calc_overall_RPM(time_period_of_step):
    num_steps_per_minute = 60 / time_period_of_step
    RPM = num_steps_per_minute / number_motor_steps_in_one_revolution
    return RPM

time_period_of_step = calc_time_period_of_step( 
             volume_fluid_injected=volume_fluid_injected, 
             total_time=total_time, 
             diameter_of_syringe=diameter_of_syringe, 
             lead_screw_pitch=lead_screw_pitch, 
             number_motor_steps_per_motor_movement=number_motor_steps_per_motor_movement)
RPM = calc_overall_RPM(time_period_of_step)

print(time_period_of_step)

def check_total_injected_fluid_correct(time_period_of_step, total_time):
    V = calc_volume_injected_per_step(diameter_of_syringe, lead_screw_pitch, number_motor_steps_per_motor_movement)
    return total_time / time_period_of_step * V

print(check_total_injected_fluid_correct(time_period_of_step=time_period_of_step, total_time=total_time))

def calc_delay_time(motor_RPM, number_motor_steps_in_one_revolution, number_motor_steps_per_motor_movement, time_period_of_step):
    time_for_one_revolution = 60 / motor_RPM
    time_for_one_step = number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution * time_for_one_revolution
    return time_period_of_step - time_for_one_step

print(calc_delay_time(motor_RPM, number_motor_steps_in_one_revolution, number_motor_steps_per_motor_movement, time_period_of_step))