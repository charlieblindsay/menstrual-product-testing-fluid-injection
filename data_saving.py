import matplotlib.pyplot as plt
import os
import pandas as pd
from datetime import datetime
import numpy as np

appdata_path = os.getenv('APPDATA')
path = appdata_path + '\Realterm\capture.txt'

with open(path, 'r') as read_file:
    lines = read_file.readlines()
    temperature_data = [float(line.split(' ')[0]) for line in lines]
    time_list = [float(line.split(' ')[1]) for line in lines]


now = datetime.now()
with open(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}.txt', 'w+') as write_file:
    write_file.writelines([f'{str(i)}\n' for i in temperature_data])

plt.plot(time_list, temperature_data)
plt.xlabel('Time (s)')
plt.ylabel('Temperature (deg C)')
plt.savefig(f'{now.day}-{now.month} {now.hour}-{now.minute}-{now.second}')
plt.show()