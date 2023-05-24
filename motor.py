# TODO: Do function comments

import math
total_time = 60 * 60 * 4
diameter_of_syringe = 0.02
motor_RPM=1
number_motor_steps_in_one_revolution=2038
number_motor_steps_per_motor_movement=10
loop_delay_duration_in_ms=50
lead_screw_pitch = 2 * 10 ** (-3)

def calc_number_of_delays(volume_fluid_injected):
    area_of_syringe = math.pi * (diameter_of_syringe / 2) ** 2
    syringe_distance_per_step = (number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution) * lead_screw_pitch
    volume_injected_per_step = syringe_distance_per_step * area_of_syringe

    volume_flow_rate = volume_fluid_injected / total_time
    time_period_of_step = volume_injected_per_step / volume_flow_rate

    num_steps_per_minute = 60 / time_period_of_step
    RPM = num_steps_per_minute / number_motor_steps_in_one_revolution

    time_for_one_revolution = 60 / motor_RPM
    time_for_one_step = number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution * time_for_one_revolution
    
    delay_time = time_period_of_step - time_for_one_step

    delay_time_in_ms = delay_time * 10 ** 3
    number_of_delays_rounded_down = delay_time_in_ms // loop_delay_duration_in_ms

    return number_of_delays_rounded_down

light_flow_number_of_delays = calc_number_of_delays(5 * 10 ** (-6))

heavy_flow_number_of_delays = calc_number_of_delays(15 * 10 ** (-6))

print(f'Number of delays for light flow: {light_flow_number_of_delays}')
print(f'Number of delays for heavy flow: {heavy_flow_number_of_delays}')
