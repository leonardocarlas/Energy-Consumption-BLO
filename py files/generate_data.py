from typing import Counter
import matplotlib.pyplot as plt
import numpy as np
import math
import sys
import re

def generate_prices_power_level():
    IPstart_interval = [1, 421, 661, 841, 1081, 1261];
    IPend_interval = [420, 660, 840, 1080, 1260, 1440];
    Prices = [0.0996, 0.2739, 0.2828, 0.0817, 0.1548, 0.1438];
    T = 1440
    instantPrices = []
    k = 0
    counter = 0
    s = "instantPrices = ["

    for i in range(0,T):
        if ( (i+1) >= IPstart_interval[k] and (i+1) <= IPend_interval[k] ):
            instantPrices.append( Prices[k] )
            counter = counter + 1
            print("T: " + str(i+1) + " Price: " + str(Prices[k]) )
            s = s + str(Prices[k]) + ", "
        else:
            instantPrices.append( Prices[k+1] )
            counter = counter + 1
            print("T: " + str(i+1) + " Price: " + str(Prices[k+1]))
            s = s + str(Prices[k]) + ", "
            k = k + 1
            

    print("N ELEMENTI STAMPATI: " + str(counter) )
    s = s[:-1]
    s = s[:-1]
    s = s + "];"

    file = open("created_data.txt","w", encoding='utf-8')
    file.write(s)
    file.close()


def generate_power_requested_stage_r():

    f_J = [ [1750, 1250, 120, 1600, 640, 220], [1840, 980, 160, 220, 300, 340, 120], [1660, 1720, 300, 220] ]
    divisore = 15
    s = "f_J = [ "
    
    for power_array in f_J:
        for power in power_array:
            s = s + (str(power) + ", ")* 15
        s = s + "\n"
    
    s = s[:-1]
    s = s[:-1]
    s = s + "];"
    
    
    file = open("created_data.txt","w", encoding='utf-8')
    file.write(s)
    file.close()


def calculate_power(j, t, start, number_of_stage, power_matrix):
    power = 0
    d = t - start[j]
    trunc = math.trunc(d / 15)
    #print("t: ", t, "j: ", j, "d: ", d, " ", trunc )
    # Little trick to avoid out of index
    if trunc == number_of_stage[j]:
        trunc = trunc - 1

    power = power_matrix[j][ trunc ]
    #
    #if  math.floor( d / 15) < 0 :
    #    power = power_matrix[j][d]
    #else:
    #    power = power_matrix[j][ math.floor( d / 15) ]   
    #    print( d / 15 )
    return power


def print_graph():
    
    d_j = [90, 105, 60]
    number_of_stage = [6,7,4]
    start = [236, 406, 1264]
    end =  [start[0]+ d_j[0], start[1]+ d_j[1], start[2]+ d_j[2]]
    power_matrix = [
    [1750, 1250, 120, 1600, 640, 220, 0],
    [1840, 980, 160, 220, 300, 340, 120],
    [1660, 1720, 300, 220, 0, 0, 0] ]
    minutes = []
    power = []
    T = 1440
    j = 0
    v_t = [20, 36, 52, 83, 106, 124, 141, 168, 190, 214, 236, 258, 280, 300, 324, 346, 369, 390, 412, 419, 420, 466, 482, 487, 491, 511, 516, 521, 538, 559, 586, 603, 636, 659, 685, 707, 733, 761, 787, 795, 833, 843, 870, 896, 925, 952, 977, 1004, 1030, 1058, 1080, 1108, 1110, 1118, 1123, 1131, 1158, 1184, 1209, 1233, 1258, 1270, 1301, 1343, 1370, 1389, 1417]
    EWHPower = 1500

    for i in range(1,T+1):
        minutes.append(i)
        power.append(0)
    


    # Shiftable appliances 
    for j in range(0,3):
        for t in range(0,T):
            if t >= start[j] and t <= end[j]:
                power[t] = power[t] + calculate_power(j, t, start, number_of_stage, power_matrix)
            else:
                power[t] = power[t] + 0
    # EWH
    for t in range(0,T):
        for k in range(0, len(v_t)):
            if (t+1) == v_t[k]:
                power[t] = power[t] + EWHPower
                



    x = np.array(minutes)
    y = np.array(power)
    
    plt.title("Power requested in model M1 + M2EWH")
    plt.plot(x,y)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()


def print_temperature_graph(solution_file, minutes):

    # Opening file
    file = open(solution_file, 'r')
    count = 1
    temp_vector = []
    
    # Using for loop
    for line in file:
        temp_string = "temp(" + str(count) + ")"
        if temp_string in line:
            vec = re.split('\s+', line)
            temp_vector.append( float(vec[1]) )
            print(vec)
            count += 1

    # Closing files
    file.close()
    
    x = np.array(minutes)
    print(temp_vector)
    y = np.array(temp_vector)
    
   
    plt.title("Water temperature in the EWH according to M1 + M2EWH")
    plt.plot(x,y)
    plt.ylabel('Temperature (°C)')
    plt.xlabel('Time (m)')
    plt.show()
    
def find_between(s, start, end):
    return (s.split(start))[1].split(end)[0]

def print_power_graph(solution_file, minutes, power_vector):

    # Opening file
    file = open(solution_file, 'r')
    
    # Using for loop
    for line in file:
        power_string = "PG2H"
        if power_string in line:
            time_t = int( find_between(line, "PG2H(", ")") ) 
            vec = re.split('\s+', line) 
            power_vector[time_t] += float( vec[1] )

    # Closing files
    file.close()
    
    x = np.array(minutes)
    print(power_vector)
    y = np.array(power_vector)
    
    plt.title("Power consume according to M1 + M2EWH + M2EV")
    plt.plot(x,y)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()



if __name__ == "__main__":

    solution_file = "/home/leonardo/Scrivania/BLO/solutions/m12ewhev.sol"
    T = 1440
    temp_vector = []
    power_vector = []
    minutes = []
    for i in range(1,T+1):
        minutes.append(i)
        power_vector.append(0)

    #print_temperature_graph(solution_file, minutes)
    print_power_graph(solution_file, minutes, power_vector)
    #generate_prices_power_level()
    #generate_power_requested_stage_r()
    #print_graph()



