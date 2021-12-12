#ifndef BLO_MO_H
#define BLO_MO_H
#endif //BLO_MO_H


/**
 * Istanciates the model containing variables to connect all the to other models
 * @param inst
 * @param env
 * @param lp
 * @param counter
 * @param cname
 * @return the overall counter of variables
 */
int model_mo(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname,
             int *pVector, int *vVector, int *Ph2vVector, int *Pv2hVector, int *Ph2bVector, int *Pb2hVector,
             int *sACVector, int *sG2HVector, int *PG2HVector, int *sh2gVector, int *PH2GVector, int *uVector);