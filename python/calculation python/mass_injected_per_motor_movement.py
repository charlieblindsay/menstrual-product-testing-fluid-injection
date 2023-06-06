import math
density_of_fluid = 1.17 # g/ml
radius_of_syringe = 0.01
area_of_syringe = math.pi * radius_of_syringe ** 2
steps_per_motor_movement = 40
steps_per_revolution = 2038
pitch = 0.002

volume_injected_in_m3 = steps_per_motor_movement / steps_per_revolution * pitch * area_of_syringe
volume_injected_in_ml = volume_injected_in_m3 * 1000000
mass_injected = volume_injected_in_ml * density_of_fluid

print(f'Mass injected per motor movement = {mass_injected}')