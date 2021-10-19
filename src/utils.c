#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "utils.h"


void print_error(const char *err) {

    printf(" \n\n Error: %s \n\n ", err);
    fflush(NULL);
    exit(1);
}


void free_instance(instance *inst) {

    free(inst->table_sm1);
    free(inst->table_sm2);
    free(inst->table_sm3);
    free(inst->table_sm5);
    free(inst->table_sm6);

}

void debug() {

    printf("Debug \n");
}


// Computes the position
int xpos(int j, int t, instance *inst) {

    int pos = inst->T * (j-1) + t - 1;
    return pos;
}

// Power required by the appliance j at time t, P_jt
double powerRequiredShiftable(int j, int t, instance *inst) {

    int max_duration = inst->max_dj;
    int nofinterval = inst->nof_appliancestages;

    // check when s(j,t) = 1, return the t_start

    // difference = t - t_start

    // calculate the interval

    // access to the the data structure and return the power
    return 1.4;
}


/*
 * Return the price associated at the power level l provided
 */
double powerLevelCost(int l, instance *inst) {

    double cost = inst->table_sm2[l-1].price_at_day;
    //printf("Costo : %lf ", cost);
    return cost;
}



double powerCostTimet(int t, instance *inst) {

    double cost = 0.0;
    for (int i = 0; i < inst->nof_subperiods; i++) {
        if ( t >= inst->table_sm1[i].start_interval && t <= inst->table_sm1[i].end_interval )
            cost = inst->table_sm1[i].price_subperiod;
    }
    return cost;
}


/*
 * Power required by the shiftable load j at its stage r
 */
int powerRequiredShiftableStageR(instance *inst, int j, int r) {

    int time_interval = inst->max_dj / inst->nof_appliancestages;   //15
    int integer_part = r / time_interval;
    int power_at_stage_r = 0;
    switch (integer_part)
    {
        case 0:
            power_at_stage_r = inst->table_sm6[j].power_first_interval;
        break;
        case 1:
            power_at_stage_r = inst->table_sm6[j].power_second_interval;
        break;
        case 2:
            power_at_stage_r = inst->table_sm6[j].power_third_interval;
        break;
        case 3:
            power_at_stage_r = inst->table_sm6[j].power_fourth_interval;
        break;
        case 4:
            power_at_stage_r = inst->table_sm6[j].power_fifth_interval;
        break;
        case 5:
            power_at_stage_r = inst->table_sm6[j].power_sixth_interval;
        break;
        case 6:
            power_at_stage_r = inst->table_sm6[j].power_seventh_interval;
        break;
    }

    return power_at_stage_r;
}