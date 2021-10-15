#include "blo.h"
#include <ilcplex/cplex.h>


void print_error(const char *err);



void debug();


/*!
 * Frees the memory of the instance passed as argument
 * @param inst The instance that we want to kill
 * @param free_solution 1 if we want to free the memeory occupied by the solution of cplex
 */
void free_instance(instance *inst);



void build_model(instance *inst, CPXENVptr pCpxenv, CPXLPptr pCpxlp);



int xpos(int j, int t, instance *inst);



double powerRequiredShiftable(int j, int t, instance *inst);


double powerLevelCost(int l, instance *inst);


double powerCostTimet(int t, instance *inst);


