from typing import Counter
import matplotlib.pyplot as plt
import numpy as np
import math

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
    start = [6, 406, 1381]
    end =  [start[0]+ d_j[0], start[1]+ d_j[1], start[2]+ d_j[2]]
    power_matrix = [
    [1750, 1250, 120, 1600, 640, 220, 0],
    [1840, 980, 160, 220, 300, 340, 120],
    [1660, 1720, 300, 220, 0, 0, 0] ]
    minutes = []
    power = []
    T = 1440
    j = 0
    v_t = [20, 36, 40, 46, 64, 83, 89, 104, 121, 133, 159, 194, 209, 211, 252, 259, 264, 299, 316, 335, 361, 378, 381, 417, 418,419, 420, 481, 485, 486, 489, 491, 511, 515, 520, 542, 563, 585, 608, 633, 639, 671, 701, 727, 752, 780, 783, 807, 841, 863, 890, 913, 923, 938, 965, 971, 989, 1016, 1043, 1079, 1080, 1113, 1118, 1120, 1123, 1129, 1151, 1173, 1201, 1227, 1231, 1255, 1292, 1310, 1315, 1332, 1334, 1357, 1377, 1417]
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


if __name__ == "__main__":
    #generate_prices_power_level()
    #generate_power_requested_stage_r()
    print_graph()





"""
    for i in range(1,T):
        minutes.append(i)
        
        if i == start[j]:
            for r in range(0, d_j[j]):
                power.append(power_matrix[j][stage])
                counter = counter + 1
                if counter == 15:
                    counter = 0
                    stage = stage + 1
        elif i in range(0,start[j]) or range(end[j]+1, start[j]) or range():
            power.append(0)
"""