import streamlit as st
import os
import time
from datetime import datetime
import matplotlib.pyplot as plt

appdata_path = os.getenv('APPDATA')
path = appdata_path + '\Realterm\capture.txt'

start_time = datetime.now()

placeholder = st.empty()

while(True):
    now = datetime.now()
    with open(path, 'r') as read_file:
        lines = read_file.readlines()
        temperature_data = [float(line.split(' ')[0]) for line in lines]
        time_list = [float(line.split(' ')[1]) for line in lines]
        last_read_temperature = [float(line.split(' ')[0]) for line in lines][-1]
        last_read_PID_output = [float(line.split(' ')[2].split('\n')[0]) for line in lines][-1]

    fig, ax = plt.subplots()

    ax.plot(time_list, temperature_data)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Temperature (deg C)')

    placeholder.empty()
    with placeholder.container():
        st.title('HEATING TEST')
        st.subheader(f'Current temperature = {last_read_temperature} degrees C')
        st.subheader(f'PID output = {last_read_PID_output}')
        
        st.pyplot(fig)

    time.sleep(5)