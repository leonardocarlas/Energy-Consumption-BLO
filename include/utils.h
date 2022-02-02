#include "ll.h"
#include <ilcplex/cplex.h>


void print_error(const char *err);


void debug();


/*!
 * Frees the memory of the instance passed as argument
 * @param inst The instance that we want to kill
 * @param free_solution 1 if we want to free the memeory occupied by the solution of cplex
 */
void free_instance(instance *inst);


double ambientTemperatureAtTimet(int t, instance *inst);


double waterWithdrawlAtTimet(int t, instance *inst);


double powerRequiredShiftable(int j, int t, instance *inst);


double powerLevelCost(int l, instance *inst);


double powerCostTimet(int t, instance *inst);


double powerSellTimet(int t, instance *inst);


int powerRequiredShiftableStageR(instance *inst, int j, int r);


double externalTemperatureAtTimet(instance *inst, int t);


double baseLoadAtTime(int t, instance *inst);


double pvGenerationAtTime(int t, instance *inst);


double randomPrice(int i, instance *inst);


double randomPerturbation(int i, instance *inst, double DELTA);

/**
 * Calcolo epsilon come il 10 % dell'amplitude, quindi posso pescare valori
 * con un 20% di amplitude sul current_value
 * @param i
 * @param inst
 * @param current_value
 * @return 1 price (double)
 */
double randomNeighbour(int i, instance *inst, double current_value);


int randomSubperiod(instance *inst);


void sortingSolutions(int *indexesVector, double *valuesVector, int N);

/**
 * method to draw random an individual  between 1 and size of the NewPopulation
 * @param m
 * @return
 */
int randomNewIndividual(int m);


/*!
* Return the seconds of the current program
*/
double seconds();