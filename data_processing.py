import matplotlib.pyplot as plt
import os
import pandas as pd
from datetime import datetime

# appdata_path = os.getenv('APPDATA')
# path = appdata_path + '\Realterm\capture.txt'

path = 'capture.txt'

with open(path, 'r') as read_file:
    temperature_data = [float(num.split('\n')[0]) for num in read_file.readlines()]

now = datetime.now()
with open(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}.txt', 'w+') as write_file:
    write_file.writelines([f'{str(i)}\n' for i in temperature_data])

time = [i * 0.3 for i in range(len(temperature_data))]

plt.scatter(time, temperature_data)
plt.xlabel('Time (s)')
plt.ylabel('Temperature (deg C)')
plt.show()