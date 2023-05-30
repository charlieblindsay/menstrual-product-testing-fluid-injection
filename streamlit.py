import streamlit as st
import os
import time
from datetime import datetime
import matplotlib.pyplot as plt

# appdata_path = os.getenv('APPDATA')
# path = appdata_path + '\Realterm\capture.txt'
document_path = '/Users/alan/Documents/GitHub/menstrual-product-testing-fluid-injection/Saved\Data'
path = document_path + ''

start_time = datetime.now()

placeholder = st.empty()

while(True):
    now = datetime.now()
    with open(path, 'r') as read_file:
        lines = read_file.readlines()
        temperature_data = [float(line.split(' ')[0]) for line in lines]
        last_read_temperature = [float(line.split(' ')[0]) for line in lines][-1]
        last_read_counter = [float(line.split(' ')[1]) for line in lines][-1]
        last_read_PID_output = [float(line.split(' ')[2].split('\n')[0]) for line in lines][-1]

    time_scaler = (37/35)*1.5

    time_list = [i * time_scaler for i in range(len(temperature_data))]

    fig, ax = plt.subplots()

    ax.plot(time_list, temperature_data)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Temperature (deg C)')

    placeholder.empty()
    with placeholder.container():
        st.write(f'Counter: {last_read_counter}')
        st.write(f'Seconds since start: {(datetime.now() - start_time).total_seconds()}')
        st.write(f'@ {now.hour}:{now.minute}:{now.second}, temperature = {last_read_temperature}, PID output = {last_read_PID_output}')
        
        st.pyplot(fig)

    time.sleep(5)





