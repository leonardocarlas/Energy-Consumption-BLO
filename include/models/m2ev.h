#ifndef BLO_M2EV_H
#define BLO_M2EV_H

#endif //BLO_M2EV_H

/**
 * model for the EV
 * @param inst
 * @param env
 * @param lp
 * @param counter
 * @param cname
 * @param sh2vVector
 * @param sv2hVector
 * @param Ph2vVector
 * @param Pv2hVector
 * @param EvVector
 * @return
 */
int model_m2ev(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *sh2vVector, int *sv2hVector,
               int *Ph2vVector, int *Pv2hVector, int *EvVector);