#ifndef BLO_M2EWH_H
#define BLO_M2EWH_H

#endif //BLO_M2EWH_H


/**
 * Method to define the model for the electric water heater
 * @param inst
 * @param env
 * @param lp
 * @param counter
 * @param cname
 * @param vVector
 * @param nVector
 * @param PlossVector
 * @param tempVector
 * @return
 */

int model_m2ewh(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *vVector,
                int *nVector, int *PlossVector, int *tempVector);