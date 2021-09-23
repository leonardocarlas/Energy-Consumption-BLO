#ifndef BLO_H_
// definition of an internal variable. If it is undefined, it will be defined. Otherwise not two times (when icluded another time).
#define BLO_H_

#include <stdio.h>
//#include <cplex.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// Single data structure that contains all the input of the problem

typedef struct
{
    // Input data
    int nappliances;
    // Table SM1, Electricity Prices divided by time intervals
    //int nof_interval_prices = 12;
    int earliest_interval_sm1[12];
    int latest_interval_sm1[12];
    double price_interval_sm1[12];


    int *start_appliance; // array di int (starting point)
    int *end_appliance; // array di int (starting point)
      

    // Parameters
    int model_type;
    char input_file[1000];
    //double timelimit; // in seconds

    // Global data
    double tstart;
    double zbest;
    double tbest;
    double best_lb;

} instance;


#endif /* BLO_H_ */