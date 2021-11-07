#ifndef BLO_M1_H
#define BLO_M1_H

#endif //BLO_M1_H

/*!
* Function that switches the problem's build from a model to another using the type save into the instance
* @param	inst is a pointer to the instance of the problem created using tsp.h
* @param	env is the environment of CPLEX
* @param	lp is the problem written in CPLEX
*/
void model_m1(instance *inst, CPXENVptr env, CPXLPptr lp);