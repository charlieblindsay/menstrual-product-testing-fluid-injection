import matplotlib.pyplot as plt
import os
import pandas as pd
from datetime import datetime
import numpy as np

number_of_data_points = 3483
end_time = 2150
path = 'data/heating_up_to_50_cooling_to_47_30_minute_test_5_minutes_until_steady_state_so_offset_fluid_data_short_needle/2-6 14-34-13 cropped data elongated.txt'

with open(path, 'r') as read_file:
    lines = read_file.readlines()
    temperature_data = [float(line.split(' ')[0]) for line in lines]
    time_list = np.linspace(0, end_time, number_of_data_points)

now = datetime.now()

temp_dict = {'Time': time_list, 'Temperature': temperature_data}
temp_df = pd.DataFrame(temp_dict)

temp_df.to_csv(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}.csv')

plt.plot(time_list, temperature_data)
plt.xlabel('Time (s)')
plt.ylabel('Temperature (deg C)')
plt.savefig(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}')
plt.show()