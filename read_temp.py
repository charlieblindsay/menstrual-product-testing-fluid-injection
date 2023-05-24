import os
import time
from datetime import datetime

appdata_path = os.getenv('APPDATA')
path = appdata_path + '\Realterm\capture.txt'

while(True):
    now = datetime.now()
    with open(path, 'r') as read_file:
        lines = read_file.readlines()
        last_read_temperature = [float(line.split(' ')[0]) for line in lines][-1]
        last_read_PID_output = [float(line.split(' ')[1].split('\n')[0]) for line in lines][-1]

    print(f'@ {now.hour}:{now.minute}:{now.second}, temperature = {last_read_temperature}, PID output = {last_read_PID_output}')
    time.sleep(5)