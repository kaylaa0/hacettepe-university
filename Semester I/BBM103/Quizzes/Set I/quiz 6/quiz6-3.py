# Hacettepe University
# Kayla Akyüz
# 21726914

import matplotlib.pyplot as plot

data_file = open("occupancy.data", "r")

for line in data_file:
    line = line.split(";")
    id = line[:1]
    for i in id:
        id = i
    time = line[1:2]
    for i in time:
        time = i
    temp = line[2:3]
    for i in temp:
        temp = i
    humid = line[3:4]
    for i in humid:
        humid = i
    light = line[4:5]
    for i in light:
        light = i
    co2 = line[5:6]
    for i in co2:
        co2 = i
    hrat = line[6:7]
    for i in hrat:
        hrat = i
    ocp = line[7:8]
    for i in ocp:
        ocp = i.strip("\n")
    timedit = time.split(" ")
    for i in timedit[:1]:
        timedit = i
    timedit = [i for i in timedit]
    timedit = timedit[-2:]
    timedit = int("".join(timedit))
    pt1x = {timedit: ocp}

for k, v in pt1x.items():
    x = k
    if v == 0:
        yk = yk + 1
    elif v == 1:
        ym = ym + 1

plot.bar(x, y)
