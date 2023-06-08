# TODO: Do function comments

# All quantity values have SI units

import math
import numpy as np
total_time = 14400 # 4 hours in secods
diameter_of_syringe = 0.02
lead_screw_pitch = 0.002
number_motor_steps_in_one_revolution=2038
number_motor_steps_per_motor_movement=10

def calc_time_period_of_motor_movement(volume_fluid_injected):
    area_of_syringe = math.pi * (diameter_of_syringe / 2) ** 2
    syringe_distance_per_movement = (number_motor_steps_per_motor_movement / number_motor_steps_in_one_revolution) * lead_screw_pitch
    volume_injected_per_movement = syringe_distance_per_movement * area_of_syringe

    print(volume_injected_per_movement)
    volume_flow_rate = volume_fluid_injected / total_time
    print(volume_flow_rate)
    time_period_of_motor_movement = volume_injected_per_movement / volume_flow_rate

    time_period_of_motor_movement_in_ms = time_period_of_motor_movement * 10 ** 3

    return np.round(time_period_of_motor_movement_in_ms,0)

light_flow_time_period_of_motor_movement = calc_time_period_of_motor_movement(volume_fluid_injected=5 * 10 ** (-6))
heavy_flow_time_period_of_motor_movement = calc_time_period_of_motor_movement(volume_fluid_injected=15 * 10 ** (-6))

print(f'Time period for light flow in ms: {light_flow_time_period_of_motor_movement}') # 8879
print(f'Time period for heavy flow in ms: {heavy_flow_time_period_of_motor_movement}') # 2960
