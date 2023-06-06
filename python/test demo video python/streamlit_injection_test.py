import streamlit as st
import os
import time
from datetime import datetime
import numpy as np

denity_of_fluid = 1.17

def calc_required_mass_injected(time_since_start_test, total_volume_injected, duration_of_test):
    time_motor_has_been_moving = time_since_start_test - 50
    total_mass_of_fluid = denity_of_fluid * total_volume_injected

    return time_motor_has_been_moving / duration_of_test * total_mass_of_fluid + 0.05

appdata_path = os.getenv('APPDATA')
path = appdata_path + '\Realterm\capture.txt'


placeholder = st.empty()


while(True):
    now = datetime.now()
    with open(path, 'r') as read_file:
        lines = read_file.readlines()
        expected_mass = [line.split('\n')[0] for line in lines][-1]
        if expected_mass == 'Counting down':
            start_time = datetime.now()
        if expected_mass != 'Counting down':
            expected_mass = float(expected_mass)

    placeholder.empty()
    with placeholder.container():
        
        if expected_mass == 'Counting down':
            st.title('FLUID INJECTION TEST')
            st.subheader('Counting down before start of test')

        else:
            st.title('FLUID INJECTION TEST')
            time_since_start = np.round((datetime.now() - start_time).total_seconds(),0)
            st.subheader(f'Seconds since start: {time_since_start}')
            st.subheader(f'Expected mass of fluid injected from programmed motor movements = {expected_mass} g')
            st.subheader(f'Required mass of fluid: {calc_required_mass_injected(time_since_start, 5, 60 * 60)}')

    time.sleep(5)