#ifndef BLO_PSO_H
#define BLO_PSO_H

#include "ll.h"

typedef struct {
    double *position;
    double *velocity;
    double *best_personal_position;
    double best_objval;
    double actual_objval;
} particle;

#endif //BLO_PSO_H



/**
 * psoUL is the main method for the execution of the PSO algorithm on the ele. prices
 * @param inst
 */
void psoUL(instance *inst, double *global_best);

/**
 * Routine that repairs the particles swarm in order to satisfy C1 - C3 constraints
 */
void repairSwarm(particle *swarm, instance *inst);



