#ifndef BLO_CSA_H
#define BLO_CSA_H

#include "ll.h"


typedef struct {

     double * solution;
     double objvalue;
     int rank;
} egg;

typedef struct {

    egg * eggs;
    int indexBestEgg;
    double valueBestEgg;
} nest;



#endif //BLO_CSA_H

void csaUL(instance *inst, double *global_best);


void orderNest(egg * eggs, int n);