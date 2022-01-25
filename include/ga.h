#ifndef BLO_GA_H
#define BLO_GA_H

#include "ll.h"

typedef struct {

    double *solution;
    double objvalue;
} individual;

#endif //BLO_GA_H



/**
 * Solve the UL problem and write the solution in the global best vector
 * @param inst
 * @param global_best
 */

void gaUL(instance *inst, double *global_best);


