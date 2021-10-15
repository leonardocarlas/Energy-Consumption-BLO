#ifndef BLO_H_
// definition of an internal variable. If it is undefined, it will be defined. Otherwise not two times (when icluded another time).
#define BLO_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ilcplex/cplex.h>


// Table SM1, electricity prices for each sub-period Pi
typedef struct {
    int start_interval;
    int end_interval;
    double price_subperiod;
} TABLE_SM1;


// Table SM2, L power level values and prices(euros/day)
typedef struct {
    int level;
    double price_at_day;
    double watt;
} TABLE_SM2;

// Table SM3, base load intervals of power required
typedef struct {
    int start_interval;
    int end_interval;
    int power_required;
} TABLE_SM3;

// Table SM5, comfort time slots, allowed operations for each appliance
typedef struct {
    int start_interval;
    int end_inteval;
} TABLE_SM5;

// Table SM6, duration and power required at each stage of appliance operation cycle
typedef struct {
    int power_first_interval;
    int power_second_interval;
    int power_third_interval;
    int power_fourth_interval;
    int power_fifth_interval;
    int power_sixth_interval;
    int power_seventh_interval;
} TABLE_SM6;

// Single data structure that contains all the input of the problem
typedef struct
{
    // Parameters
    int model_type;
    char input_file[1000];

    // Table SM1, electricity prices for each sub-period Pi
    int nof_subperiods;
    TABLE_SM1* table_sm1;

    //Table SM2, Power levels and prices
    int nof_powerlevels;
    TABLE_SM2* table_sm2;

    // Table SM3, base load intervals of power required
    int nof_baseloadintervals;
    TABLE_SM3* table_sm3;

    //Table SM5, comfort time slots, allowed operations for each appliance
    int nof_appliances;
    TABLE_SM5* table_sm5;

    //Table SM6, duration and power required at each stage of appliance operation cycle
    int nof_appliancestages;
    int max_dj;
    TABLE_SM6* table_sm6;


    // Global data
    double tstart;
    double zbest;
    double tbest;
    double best_lb;

    int T;
} instance;




#endif /* BLO_H_ */




/*!
* Calculate the solution of the problem built into an instance
* @param	inst is a pointer to the instance where is stored the problem
* @return	0 if the solution is found. Other values otherwise
*/
int BLOopt(instance *inst);

/*!
* Function that switches the problem's build from a model to another using the type save into the instance
* @param	inst is a pointer to the instance of the problem created using tsp.h
* @param	env is the environment of CPLEX
* @param	lp is the problem written in CPLEX
*/
void build_model(instance *inst, CPXENVptr env, CPXLPptr lp);