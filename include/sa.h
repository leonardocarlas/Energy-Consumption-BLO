#ifndef BLO_SA_H
#define BLO_SA_H

#include "ll.h"

#endif //BLO_SA_H







/**
 * Solve the UL problem and write the solution in the global best vector
 * @param inst
 * @param global_best
 */

void saUL(instance *inst, double *global_best);

/**
 * Routine that repairs the solution in order to satisfy C1 - C3 constraints
 */
void repairSolution(double *solution, instance *inst);