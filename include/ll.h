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
    double min_price;
    double max_price;
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

// Table SM4, PV generations intervals
typedef struct {
    int start_interval;
    int end_interval;
    int generated_power;
} TABLE_SM4;

// Table SM5, comfort time slots, allowed operations for each appliance
typedef struct {
    int start_interval;
    int end_inteval;
} TABLE_SM5;

// Table SM6, power required at each stage of appliance operation cycle
typedef struct {
    int power_first_interval;
    int power_second_interval;
    int power_third_interval;
    int power_fourth_interval;
    int power_fifth_interval;
    int power_sixth_interval;
    int power_seventh_interval;
} TABLE_SM6;

// TABLE SM20, duration of each appliance
typedef struct {
    int duration;
} TABLE_SM20;

//Table SM7, one array containing the parameter about the water heater
typedef struct {
    int power_required;
    int M;
    double AU;
    double termal_coeff;
    int inlet_temperature;
    int min_temperature;
    int max_temperature;
    int required_temperature;
    int required_time;
    int initial_temperature;
    int initial_on_off;
    int initial_power_losses;
} TABLE_SM7;

//Table SM8, water withdrawl intervals and volumes
typedef struct {
    int start_interval;
    int end_interval;
    double kg_water;
} TABLE_SM8;

//Table SM9, EWH intervals and temperature
typedef struct{
    int start_interval;
    int end_interval;
    double temperature;
} TABLE_SM9;

//Table SM10, one array containing the parameter about the air conditioner system
typedef struct {
    double maximum_temperature_allowed;
    double minimum_temperature_allowed;
    double initial_indoor_temperature;
    int nominal_power_AC;
    int initial_start;
    double alpha;
    double beta;
    double gamma;
} TABLE_SM10;

//Table SM11, intervals of outdoor temperature
typedef struct {
    int start_interval;
    int end_interval;
    double outdoor_temperature;
} TABLE_SM11;


//Table SM12, one array containing the parameter about the SB static battery
typedef struct {
    double sb_charging_efficiency;
    double sb_discharging_efficiency;
    int sb_minimum_charge;
    int sb_maximum_charge;
    int sb_maximum_charge_power;
    int sb_maximum_discharge_power;
    int sb_initial_battery_charge;
} TABLE_SM12;

//Table SM13, one array containing the parameter about the EV
typedef struct {
    double ev_charging_efficiency;
    double ev_discharging_efficiency;
    int ev_minimum_charge;
    int ev_maximum_charge;
    int requested_charge;
    int ev_maximum_charge_power;
    int ev_maximum_discharge_power;
    int time_arrival;
    int time_departure;
    int ev_initial_battery_charge;
} TABLE_SM13;


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
    int L;
    TABLE_SM2* table_sm2;

    // Table SM3, base load intervals of power required
    int nof_baseloadintervals;
    TABLE_SM3* table_sm3;

    // Table SM4, base load intervals of power required
    int nof_pvgeneration_intervals;
    TABLE_SM4* table_sm4;

    //Table SM5, comfort time slots, allowed operations for each appliance
    int J;
    TABLE_SM5* table_sm5;

    //Table SM6, duration and power required at each stage of appliance operation cycle
    int nof_appliancestages;
    int MAX_DJ;
    TABLE_SM6* table_sm6;

    //Table SM20, duration R of each appliance
    TABLE_SM20* table_sm20;

    //Table SM7, one array containing the parameter about the water heater
    TABLE_SM7 table_sm7;

    //Table SM8, water withdrawl intervals and volumes
    int nof_waterwithdrawl_intervals;
    TABLE_SM8* table_sm8;

    //Table SM9, ambient temperature around the EWH
    int nof_amb_temp_intervals;
    TABLE_SM9* table_sm9;

    //Table SM10, one array containing the parameter about the air conditioner system
    TABLE_SM10 table_sm10;

    //Table SM11, outdoor temperature
    int nof_outdoor_temperature_intervals;
    TABLE_SM11* table_sm11;

    //Table SM12, EV batteries parameters
    TABLE_SM12 table_sm12;

    //Table SM13, EV batteries parameters
    TABLE_SM13 table_sm13;

    // Global data
    // Variable that will contain global data
    double timelimit;
    double start_time;
    double *solution;
    double best_value;
    int nvariables;
    int *successors;
    int *component;
    int ncomp;


    int T;
    int N;
    int G;
    int MAX_PG;
    double INITIAL_OUTDOOR_TEMPERATURE;
    double X_AVARAGE;
} instance;




#endif /* BLO_H_ */




/*!
* Calculate the solution of the problem built into an instance
* @param	inst is a pointer to the instance where is stored the problem
* @return	0 if the solution is found. Other values otherwise
*/
int BLOopt(instance *inst);

