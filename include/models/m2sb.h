#ifndef BLO_M2SB_H
#define BLO_M2SB_H

#endif //BLO_M2SB_H


/**
 * Method that instantiates the model for the static battery SB
 * @param inst
 * @param env
 * @param lp
 * @param counter
 * @param cname
 * @param sh2bVector
 * @param sb2hVector
 * @param Ph2bVector
 * @param Pb2hVector
 * @param BatteryEVector
 * @return
 */
int model_m2sb(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname,
               int *sh2bVector, int *sb2hVector, int *Ph2bVector, int *Pb2hVector, int *BatteryEVector);


