#include "ll.h"

#ifndef BLO_GRASP_H
#define BLO_GRASP_H

#endif //BLO_GRASP_H


void graspUL(instance * inst, double * global_best);


void constructGreedyRandomizedSolution(double * solution, instance * inst);


void localSearch(double * solution, instance * inst);


double selectBestPriceFromRCL(int i, double * rcl, instance * inst );


void makeRLC(int i, double * rcl, instance * inst);