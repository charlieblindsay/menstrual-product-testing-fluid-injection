import matplotlib.pyplot as plt

time_of_test = 240
time_interval_between_measurements = 30

density_in_g_per_cm3 = 1.17

time_intervals = [t for t in range(
    time_interval_between_measurements, 
    time_of_test + 1, 
    time_interval_between_measurements)]

def calc_expected_mass_injeted_at_time_intervals(maximum_volume_injected_in_cm3):
    expected_volume_at_time_intervals = [t / 240 * maximum_volume_injected_in_cm3 for t in time_intervals]
    return [v * density_in_g_per_cm3 for v in expected_volume_at_time_intervals]

expected_mass_for_light_flow = calc_expected_mass_injeted_at_time_intervals(
    maximum_volume_injected_in_cm3=5
)

expected_mass_for_heavy_flow = calc_expected_mass_injeted_at_time_intervals(
    maximum_volume_injected_in_cm3=15
)

plt.scatter(time_intervals, expected_mass_for_light_flow, label='Light flow')
plt.scatter(time_intervals, expected_mass_for_heavy_flow, label='Heavy flow')
plt.legend()

plt.xlabel('Time since start of test (minutes)')
plt.ylabel('Expected mass of fluid injected (g)')

plt.xticks(time_intervals)

plt.show()