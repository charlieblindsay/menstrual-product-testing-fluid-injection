# TODO: Do function comments

# All values are in SI units

import math
import numpy as np
total_time = 60 * 60 * 4
diameter_of_syringe = 0.02
motor_RPM=1
number_motor_steps_in_one_revolution=2038
number_motor_steps_per_motor_movement=10
loop_delay_duration_in_ms=50
lead_screw_pitch = 2 * 10 ** (-3)

def calc_time_period_of_step(volume_fluid_injected):
    area_of_syringe = math.pi * (diameter_of_syringe / 2) ** 2
    syringe_distance_per_step = (number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution) * lead_screw_pitch
    volume_injected_per_step = syringe_distance_per_step * area_of_syringe

    volume_flow_rate = volume_fluid_injected / total_time
    time_period_of_step = volume_injected_per_step / volume_flow_rate

    time_period_of_step_in_ms = time_period_of_step * 10 ** 3

    return np.round(time_period_of_step_in_ms,0)

light_flow_time_period_of_step = calc_time_period_of_step(volume_fluid_injected=5 * 10 ** (-6))
heavy_flow_time_period_of_step = calc_time_period_of_step(volume_fluid_injected=15 * 10 ** (-6))

print(f'Time period for light flow in ms: {light_flow_time_period_of_step}')
print(f'Time period for heavy flow in ms: {heavy_flow_time_period_of_step}')
