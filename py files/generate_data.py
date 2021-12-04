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
    
   
    plt.title("Water temperature in the EWH according to M4")
    plt.plot(x,y)
    plt.ylabel('Temperature (°C)')
    plt.xlabel('Time (m)')
    plt.show()
    
def find_between(s, start, end):
    return (s.split(start))[1].split(end)[0]

def print_power_graph(solution_file, minutes):

    # Opening file
    file = open(solution_file, 'r')
    power_vector = []
    # Using for loop
    for line in file:
        power_string = "PG2H"
        if power_string in line:
            vec = re.split('\s+', line) 
            power_vector.append( float( vec[1] ) )

    # Closing files
    file.close()
    
    x = np.array(minutes)
    #print(power_vector)
    y = np.array(power_vector)
    
    plt.title("Power consume according to M4")
    plt.plot(x,y)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()


def print_ev_graph(solution_file, minutes, ev_charge_vector):

    # Opening file
    file = open(solution_file, 'r')
    
    # Using for loop
    for line in file:
        charge_string = "VehicleE"
        if charge_string in line:
            time_t = int( find_between(line, "VehicleE(", ")") ) 
            vec = re.split('\s+', line) 
            ev_charge_vector[time_t] += float( vec[1] )

    # Closing files
    file.close()
    
    x = np.array(minutes)
    #print(power_vector)
    y = np.array(ev_charge_vector)
    
    plt.title("EV energy charge according to M5")
    plt.plot(x,y)
    plt.ylabel('Energy charge (Wh)')
    plt.xlabel('Time (m)')
    plt.show()


def print_sb_graph(solution_file, minutes):

    #sb_charge_vector = np.empty(1440, dtype=object)
     
    # Opening file
    file = open(solution_file, 'r')
    vector_check = []
    
    sb_charge_vector = []

    for line in file:
        charge_string = "BatteryE"
        if charge_string in line:
            ausiliary_vector = []
            vec = re.split('\s+', line)
            time_t = int( find_between(line, "BatteryE(", ")") )
            ausiliary_vector.append( int(time_t) )
            ausiliary_vector.append( float(vec[1]) )   
            vector_check.append(ausiliary_vector)
                
    #print(vector_check)

    for i in range(1,1441):
        found = False
        for vec in vector_check:
            if vec[0] == i:
                found = True
                print("TROVATO")
                sb_charge_vector.append(vec[1])
        if found == False:
            print("NON TROVATO")
            sb_charge_vector.append(0)
  
    # Closing files
    file.close()
    
    x = np.array(minutes)
    y = np.array(sb_charge_vector)

    plt.title("SB energy charge according to M5")
    plt.plot(x,y)
    plt.ylabel('Energy charge (Wh)')
    plt.xlabel('Time (m)')
    plt.show()

def print_intemp_graph(solution_file, minutes):
    
    file = open(solution_file, 'r')
       
    intemp_vector = []
    for line in file:
        string = "inTemp"
        if string in line:
            vec = re.split('\s+', line)
            print(vec)
            intemp_vector.append( float (vec[1]) )
    
        # Closing files
    file.close()
    
    x = np.array(minutes)
    y = np.array(intemp_vector)

    plt.title("Home internal temperature according to M5")
    plt.plot(x,y)
    plt.ylabel('Grades (°C)')
    plt.xlabel('Time (m)')
    plt.show()



def print_dirty_power_graph(solution_file, minutes):
     
    # Opening file
    file = open(solution_file, 'r')
    vector_check = []
    
    sb_charge_vector = []

    for line in file:
        charge_string = "PG2H"
        if charge_string in line:
            ausiliary_vector = []
            vec = re.split('\s+', line)
            time_t = int( find_between(line, "PG2H(", ")") )
            ausiliary_vector.append( int(time_t) )
            ausiliary_vector.append( float(vec[1]) )   
            vector_check.append(ausiliary_vector)
             
    #print(vector_check)

    for i in range(1,1441):
        found = False
        for vec in vector_check:
            if vec[0] == i:
                found = True
                #print("TROVATO")
                sb_charge_vector.append(vec[1])
        if found == False:
            print(str(i), " NON TROVATO")
            sb_charge_vector.append("MISSIGNO")

    with open("created_data.txt", "w") as outfile:
        outfile.write("\n".join( str( sb_charge_vector) ))
  
    # Closing files
    file.close()


def print_two_powers(solution_file, minutes):

    PG2H_vector = []
    PH2G_vector = []

    for i in range(1,T+1):
        PG2H_vector.append(0)
        PH2G_vector.append(0)

    file = open(solution_file, 'r')

    for line in file:
        charge_string = "PG2H"
        if charge_string in line:
            vec = re.split('\s+', line)
            time_t = int( find_between(line, "PG2H(", ")") )
            PG2H_vector[ time_t - 1] = float( vec[1] )
            #print(time_t, " ", float( vec[1] ) )

    file.close()
    file = open(solution_file, 'r')

    for line in file:
        string = "PH2G"
        if string in line:
            vec = re.split('\s+', line)
            print("LOG")
            time_t = int( find_between(line, "PH2G(", ")") )
            PH2G_vector[ time_t - 1] = float( vec[1] ) * 1000
            #print(time_t, " ", float( vec[1] ) )

    file.close()

    print(PH2G_vector)

    x = np.array(minutes)
    y = np.array(PG2H_vector)
    z = np.array(PH2G_vector)


    plt.title("PG2H and PH2G according to M5")
    plt.plot(x,y,z)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()

if __name__ == "__main__":

    solution_file = "/home/leonardo/Scrivania/BLO/solutions/m12ewh.sol"
    T = 1440
    temp_vector = []
    full_vector = []
    ev_charge_vector = []
    minutes = []
    for i in range(1,T+1):
        minutes.append(i)
        ev_charge_vector.append(0)
        full_vector.append(i)

    #print_two_powers(solution_file, minutes)
    print_temperature_graph(solution_file, minutes)
    #print_power_graph(solution_file, minutes)
    #print_dirty_power_graph(solution_file, minutes)
    #print_ev_graph(solution_file, minutes, ev_charge_vector)
    #print_sb_graph(solution_file, minutes)
    #generate_prices_power_level()
    #generate_power_requested_stage_r()
    print_intemp_graph(solution_file, minutes)
    #print_graph()






"""
    full = []
    for i in range(1, 1441):
        full.append(i)
    
    set_difference = set(full) - set(vector_check)
    list_difference = list(set_difference)
    print(list_difference)


    for i in range(0, 1440):
        if i in vector_check:
            np.put(sb_charge_vector, i, ausiliary_vector[i])
        else:
            np.put(sb_charge_vector, i, 0)

            c = 1
for line in file:
    charge_string = "BatteryE"
    if charge_string in line:
        time_t = int( find_between(line, "BatteryE(", ")") )
        if time_t == c:
            #appendo il valore
            vec = re.split('\s+', line)
            sb_charge_vector.append( float(vec[1]) )
        else:
            #appendo 0
            sb_charge_vector.append(0)
        c +=1
        #vec = re.split('\s+', line)
        #vector_check.append(time_t)
        #ausiliary_vector.append(float( vec[1]) )




                        charge_string = "BatteryE"
                if charge_string in line:
                    time_t = int( find_between(line, "BatteryE(", ")") )
                    if time_t == i:
                        vec = re.split('\s+', line)
                        sb_charge_vector.append( float(vec[1]) )


                            for i in range(1,10):
        
        stringa = "BatteryE(" + str(i) + ")"
        found = False
        for line in file:
            vec = re.split('\s+', line)
            if vec[0] == stringa:
                found = True
                sb_charge_vector.append(1)              
                
        if found == False:
            sb_charge_vector.append(0)
        print(found," ", i)  

"""