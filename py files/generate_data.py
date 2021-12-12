from typing import Counter
import matplotlib.pyplot as plt
import numpy as np
import math
import sys
import re

def print_temperature_graph(solution_file, minutes, model_name):


    file = open(solution_file, 'r')
    temp_vector = []
    string = "temp"
    for line in file:
        if string in line:
            value = float( find_between(line, "value=\"", "\"") )
            temp_vector.append( float( value ) )

    file.close()
    x = np.array(minutes)
    y = np.array(temp_vector)
   
    plt.title("Water temperature in the EWH according to " + model_name)
    
    plt.plot(x,y)
    plt.ylabel('Temperature (°C)')
    plt.xlabel('Time (m)')
    plt.show()


def find_between(s, start, end):
    return (s.split(start))[1].split(end)[0]

def print_ev_graph(solution_file, minutes, model_name):

    file = open(solution_file, 'r')
    ev_charge_vector = []
    string = "VehicleE"
    for line in file:
        if string in line:
            value = float( find_between(line, "value=\"", "\"") )
            ev_charge_vector.append( float( value ) )
        

    file.close()
    
    x = np.array(minutes)
    y = np.array(ev_charge_vector)
    
    plt.title("EV energy charge according to " + model_name)
    plt.plot(x,y)
    plt.ylabel('Energy charge (Wh)')
    plt.xlabel('Time (m)')
    plt.show()


def print_sb_graph(solution_file, minutes, model_name):

    #sb_charge_vector = np.empty(1440, dtype=object)
     
    # Opening file
    file = open(solution_file, 'r')

    sb_charge_vector = []

    string = "BatteryE"
    for line in file:
        if string in line:
            value = float( find_between(line, "value=\"", "\"") )
            sb_charge_vector.append( float( value ) )

  
    # Closing files
    file.close()
    
    x = np.array(minutes)
    y = np.array(sb_charge_vector)

    plt.title("SB energy charge according to " + model_name)
    plt.plot(x,y)
    plt.ylabel('Energy charge (Wh)')
    plt.xlabel('Time (m)')
    plt.show()


def print_intemp_graph(solution_file, minutes, model_name):
    
    file = open(solution_file, 'r')
    intemp_vector = []
    string = "inTemp"
    for line in file:
        if string in line:
            value = float( find_between(line, "value=\"", "\"") )
            intemp_vector.append( float( value ) )

    file.close()

    x = np.array(minutes)
    y = np.array(intemp_vector)

    plt.title("Home internal temperature according to " + model_name)
    plt.plot(x,y)
    plt.ylabel('Grades (°C)')
    plt.xlabel('Time (m)')
    plt.show()

def print_two_powers(solution_file, minutes):

    PG2H_vector = []
    PH2G_vector = []

    file = open(solution_file, 'r')

    string_pg2h = "PG2H"
    string_ph2g = "PH2G"

    for line in file:

        if string_pg2h in line:
            value = float( find_between(line, "value=\"", "\"") )
            PG2H_vector.append( float( value ) )
        if string_ph2g in line:
            value = float( find_between(line, "value=\"", "\"") )
            PH2G_vector.append( float( value ) )

    file.close()

    x = np.array(minutes)
    y = np.array(PG2H_vector)
    z = np.array(PH2G_vector)


    plt.title("PG2H and PH2G according to M5")
    plt.plot(x,y,z)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()


def print_power_sol(solution_file, minutes, model_name):

    file = open(solution_file, 'r')
    power_vector = []
    string = "PG2H"
    for line in file:
        if string in line:
            value = float( find_between(line, "value=\"", "\"") )
            power_vector.append( float( value ) )

    file.close()
    
    x = np.array(minutes)
    y = np.array(power_vector)
    
    plt.title("Power consume according to " + model_name)
    plt.plot(x,y)
    plt.ylabel('Power (W)')
    plt.xlabel('Time (m)')
    plt.show()

if __name__ == "__main__":

    solution_file = "/home/leonardo/Scrivania/BLO/solutions/M1.sol"
    model_name = "M1"
    T = 1440
    temp_vector = []
    full_vector = []
    ev_charge_vector = []
    minutes = []
    for i in range(1,T+1):
        minutes.append(i)
        ev_charge_vector.append(0)
        full_vector.append(i)

    print_power_sol(solution_file, minutes, model_name)
    #print_two_powers(solution_file, minutes, model_name)
    #print_temperature_graph(solution_file, minutes, model_name)
    #print_ev_graph(solution_file, minutes, model_name)
    #print_sb_graph(solution_file, minutes, model_name)
    #print_intemp_graph(solution_file, minutes, model_name)







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


