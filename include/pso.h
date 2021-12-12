#ifndef BLO_PSO_H
#define BLO_PSO_H

#include "ll.h"

typedef struct {
    double *position;
    double *velocity;
    double *best_personal_position;
} particle;

int test_pso(instance *inst);

/*
 * Routine that repairs the particles swarm in order to satisfy C1 - C3 constraints
 */
void repairSwarm(particle *swarm, instance *inst);


#endif //BLO_PSO_H
