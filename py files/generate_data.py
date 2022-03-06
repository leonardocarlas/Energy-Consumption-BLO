from tkinter import N
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

def print_two_powers(solution_file, minutes, model_name):

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


    plt.title("PG2H (blue line )and PH2G (orange line) according to " + model_name)
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


def print_objvalues(solution_file, iterations):
    
    global_best = []
    avarage = []
    N = 20

    # Population of the arrays
    file = open(solution_file, 'r')
    for line in file:
        splitted = line.split()
        global_best.append( float(splitted[0]) )

        sum = 0
        for i in range(1, N+1):
            sum += float(splitted[i])
        avarage.append( sum / N )
    

    print(global_best)
    print(avarage)
    
    x = np.array(iterations)
    y = np.array(global_best)
    z = np.array(avarage)
    
    plt.title("Best objvalues (blue line) and avarage objvalues (orange line) in each iteration")
    plt.xticks(x)
    
    plt.plot(y)
    plt.plot(z)
    plt.ylabel('Objective value')
    plt.xlabel('Number of iteration')
    plt.show()


def printSAValues(solution_file, sa_iterations):
    
    global_best = []
    temperature = []
    randomMove = []

    # Population of the arrays
    file = open(solution_file, 'r')
    for line in file:
        splitted = line.split()
        temperature.append( float(splitted[0]) )
        global_best.append( float(splitted[2]) )
        randomMove.append( int(splitted[5]) )

    x = np.array(sa_iterations)
    y = np.array(global_best)
    z = np.array(temperature)
    v = np.array(randomMove)
    
    plt.title("Temperture (orange), solution objective value (blue) and randomMove (green)")
    #plt.xticks(sa_iterations)
    plt.plot(x,y)
    plt.plot(z)
    plt.plot(v)
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()

def printGAValues(solution_file, iterations):
    
    global_best = []
    avarage = []

    # Population of the arrays
    file = open(solution_file, 'r')
    for line in file:
        splitted = line.split()
        N = len(splitted)
        print(N)
        global_best.append( float(splitted[0]) )
        sum = 0
        for i in range(0, N):
            sum += float(splitted[i])
        avarage.append( sum / N )
        
    

    print(global_best)
    print(avarage)
    
    x = np.array(iterations)
    y = np.array(global_best)
    z = np.array(avarage)
    
    plt.title("Best objvalues (blue line) and avarage objvalues (orange line) in each iteration")
    plt.xticks(x)
    plt.plot(y)
    plt.plot(z)
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()
    
def printLBSValues(solution_file, iterations):
    
    global_best = []
    avarage = []

    # Population of the arrays
    file = open(solution_file, 'r')
    for line in file:
        splitted = line.split()
        N = len(splitted)
        print(N)
        global_best.append( float(splitted[-1]) )
        sum = 0
        for i in range(0, N-1):
            sum += float(splitted[i])
        avarage.append( sum / N )
        
    

    print(global_best)
    print(avarage)
    
    x = np.array(iterations)
    y = np.array(global_best)
    z = np.array(avarage)
    
    plt.title("Best objvalues (blue line) and avarage objvalues (orange line) in each iteration")
    plt.xticks(x)
    plt.plot(y)
    plt.plot(z)
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()

def printCSAValues(solution_file, iterations, nNESTS, nEGGS):
    
    global_best = []
    #avarage = []
    #list = []
    #matrix = []
    sum = 0

    z = []
    w = []
    v = []
    r = []
    t = []

    # Population of the arrays
    file = open(solution_file, 'r')
    c = 0
    for line in file:
        max = 0.0
        splitted = line.split()
        N = len(splitted)
        for i in range(0, N):
            if float(splitted[i]) > max:
                max = float(splitted[i])
            sum = sum + float(splitted[i])
            if (i + 1)  % nNESTS == 0:
                print("Avarage nido %f", sum / nEGGS)
                if i >= 0 and i < 5:
                    z.append(sum / nEGGS)
                if i >= 5 and i < 10:
                    w.append(sum / nEGGS)
                if i >= 10 and i < 15:
                    v.append(sum / nEGGS)
                if i >= 15 and i < 20:
                    r.append(sum / nEGGS)
                if i >= 25 and i < 30:
                    t.append(sum / nEGGS)
                sum = 0
        
        
        #matrix.append(list)
        #list = []
        global_best.append( max )
            
        
    
    #print(matrix)
    print(global_best)
    #print(avarage)
    
    x = np.array(iterations)
    y = np.array(global_best)

    plt.title("Best objvalues (blue line) and avarage objvalues (other colors) in each iteration")
    plt.xticks(x)
    plt.plot(y)

    plt.plot(np.array(z))
    plt.plot(np.array(w))
    plt.plot(np.array(v))
    plt.plot(np.array(r))
    plt.plot(np.array(t))
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()

def printDEValues(solution_file, iterations):
    
    global_best = []
    avarage = []
    
    # Population of the arrays
    file = open(solution_file, 'r')
    for line in file:
        max = 0.0
        splitted = line.split()
        N = len(splitted)
        for i in range(0,N):
            if float(splitted[i]) > max:
                max = float(splitted[i])
        sum = 0
        for i in range(0, N):
            sum += float(splitted[i])
        avarage.append( sum / N )
        global_best.append(max)
        
    print(global_best)
    print(avarage)
    
    x = np.array(iterations)
    y = np.array(global_best)
    z = np.array(avarage)
    
    plt.title("Best objvalues (blue line) and avarage objvalues (orange line) in each iteration")
    plt.xticks(x)
    plt.plot(y)
    plt.plot(z)
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()


def printGRASPValues(solution_file, iterations):

    global_best = []
    
    file = open(solution_file, 'r')
    for line in file:
        #splitted = line.split()
        #print(splitted[0])
        global_best.append(float(line))
    
    
    print(global_best)
    
    x = np.array(iterations)
    y = np.array(global_best)
    
    plt.title("Best objvalues in each iteration")
    plt.xticks(x)
    plt.plot(y)
    plt.ylabel('Values')
    plt.xlabel('Number of iteration')
    plt.show()
    


if __name__ == "__main__":

    solution_file = "/home/leonardo/Scrivania/BLO/cmake-build-debug/ll.sol"
    model_name = "M5"
    T = 10
    SAT = 1380
    G = 30
    iterations = []
    sa_iterations = []
    minutes = []
    for i in range(1,T+1):
        minutes.append(i)
    for i in range(1, G+1):
        iterations.append(i)
    for i in range(1, SAT+1):
        sa_iterations.append(i)

    #print_power_sol(solution_file, minutes, model_name)
    #print_two_powers(solution_file, minutes, model_name)
    #print_temperature_graph(solution_file, minutes, model_name)
    #print_ev_graph(solution_file, minutes, model_name)
    #print_sb_graph(solution_file, minutes, model_name)
    #print_intemp_graph(solution_file, minutes, model_name)
    #obj_values = "/home/leonardo/Scrivania/BLO/py files/PSOvalues.txt"
    #print_objvalues(obj_values, iterations)
    #sa_values = "/home/leonardo/Scrivania/BLO/py files/SAvalues.txt"
    #ga_values = "/home/leonardo/Scrivania/BLO/py files/GAvalues.txt"
    de_values = "/home/leonardo/Scrivania/BLO/py files/DEvalues.txt"
    lbs_values = "/home/leonardo/Scrivania/BLO/py files/LBSvalues.txt"
    csa_values = "/home/leonardo/Scrivania/BLO/py files/CSAvalues.txt"
    grasp_values = "/home/leonardo/Scrivania/BLO/py files/GRASPvalues.txt"
    #printSAValues(sa_values, sa_iterations)
    #printGAValues(ga_values, iterations)
    #printLBSValues(lbs_values, iterations)
    #printCSAValues(csa_values, iterations, 5, 5)
    #printDEValues(de_values, iterations)
    printGRASPValues(grasp_values, iterations)

