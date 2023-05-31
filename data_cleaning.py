import pandas as pd

path = 'data/up to 45, down to 37.5, long test/26-5 15-20-28.txt'

with open(path, 'r') as read_file:
    lines = read_file.readlines()
    temperature_data = [float(line) for line in lines]

i = 0
j = 1

while j < len(temperature_data):
    if abs(temperature_data[j] - temperature_data[i]) > 1:
        temperature_data[j] = 0
        # print(j)
        j += 1
    else:
        i += 1
        j += 1

for i in range(len(temperature_data)):
    if temperature_data[i] == 37.34:
        print(i)

print(temperature_data)

