import matplotlib.pyplot as plt
import os
import pandas as pd
from datetime import datetime

# appdata_path = os.getenv('APPDATA')
# path = appdata_path + '\Realterm\capture.txt'

path = '26-5 15-20-28.txt'

# path = 'capture.txt'

with open(path, 'r') as read_file:
    lines = read_file.readlines()
    temperature_data = [float(line.split(' ')[0]) for line in lines]
    # PID_output = [float(line.split(' ')[2].split('\n')[0]) for line in lines]

now = datetime.now()
with open(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}.txt', 'w+') as write_file:
    write_file.writelines([f'{str(i)}\n' for i in temperature_data])

time_scaler = (37/35)*1.5

time = [i * time_scaler for i in range(len(temperature_data))]

plt.plot(time, temperature_data)
plt.xlabel('Time (s)')
plt.ylabel('Temperature (deg C)')
plt.savefig(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}')
plt.show()