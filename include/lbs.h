#ifndef BLO_LBS_H
#define BLO_LBS_H

#include "ll.h"

typedef struct {

    double *solution;
    double objvalue;
} beam;


#endif //BLO_LBS_H


void lbsUL(instance *inst, double *global_best);


