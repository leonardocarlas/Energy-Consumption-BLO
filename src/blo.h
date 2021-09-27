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
    int x_avarage;

    // Table SM1, Electricity Prices divided by time intervals
    int nof_interval_prices;
    int *earliest_interval_sm1;
    int *latest_interval_sm1;
    double *price_interval_sm1;

    //Table SM2, maximum and minimum prices for each sub-periods
    int nof_subperiods;
    int *start_subperiod_sm2;
    int *end_subperiod_sm2;
    double *minimum_prices_sm2;
    double *maximum_prices_sm2;

    //Table SM3, Power levels
    int nof_powerlevels;
    double *prices_powerlevel_sm3;
    int *maximum_power_sm3;

    //Table SM4, Power requested from the grid for each interval
    int nof_intervals_power_requested;
    int *start_interval_pr_sm4;
    int *end_interval_pr_sm4;
    int *power_requested_sm4;

    //Table SM5, comfort time slots, allowe operations for each appliance
    int nof_appliances;
    int *start_appliance; // array di int (starting point)
    int *end_appliance; // array di int (starting point)
      
    //Table SM6, Power required by the appliance at each stage of its operation cycle
    //1, 2, 3, 4, 5, 6, 7, 8-36
    int *power_stage1_sm6;
    int *power_stage2_sm6;
    int *power_stage3_sm6;
    int *power_stage4_sm6;
    int *power_stage5_sm6;
    int *power_stage6_sm6;
    int *power_stage7_sm6;
    int *power_stage8_sm6;
    
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