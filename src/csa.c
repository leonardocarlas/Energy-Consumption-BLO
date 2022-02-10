#include "../include/lbs.h"
#include "../include/sa.h"
#include "../include/csa.h"
#include "utils.h"
#include "math.h"

#define  nEGGS  3
#define  nNESTS 3
#define  GEN    1
#define  B      1.5
#define  DEVu   0.6966
#define  Pa     0.25


void csaUL(instance *inst, double *global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/CSAvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    double globalBestObjvalue = 0.0;



    // INSTATIATION

    nest * nests = calloc(nNESTS, sizeof(nest));

    for (int n = 0; n < nNESTS; ++n) {

        nests[n].eggs = calloc(nEGGS, sizeof(egg));
        for (int e = 0; e < nEGGS; ++e) {

            nests[n].eggs[e].solution = calloc(inst->nof_subperiods, sizeof(double));

            // Random generation
            for (int i = 0; i < inst->nof_subperiods; ++i)
                nests[n].eggs[e].solution[i] = randomPrice(i, inst);

            repairSolution(nests[n].eggs[e].solution, inst);

            nests[n].eggs[e].rank = e;

            nests[n].eggs[e].objvalue = LLopt(inst, nests[n].eggs[e].solution);

            if ( nests[n].eggs[e].objvalue > globalBestObjvalue ) {

                globalBestObjvalue = nests[n].eggs[e].objvalue;
                for (int i = 0; i < inst->nof_subperiods; ++i)
                    global_best[i] = nests[n].eggs[e].solution[i];
            }

            printf("Egg created. Objvalue: %f \n", nests[n].eggs[e].objvalue);
        }

        orderNest(nests[n].eggs, nEGGS);
    }
    printf("%f %f %f \n", nests[0].eggs[0].objvalue, nests[0].eggs[1].objvalue, nests[0].eggs[2].objvalue);



    for (int g = 0; g < GEN; ++g) {

        for (int n = 0; n < nNESTS; ++n) {

            // CUCKOO EGG CREATION

            double *cuckooEgg = calloc(inst->nof_subperiods, sizeof(double));
            double cuckooEggObjvalue = 0.0;

            // CREAZIONE CON AVARAGE
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                double sum = 0.0;
                for (int e = 0; e < nEGGS; ++e) {
                    sum += nests[n].eggs[e].solution[i];
                }
                cuckooEgg[i] = sum / nEGGS;
            }

            // CREAZIONE CON LEVY FLIGHT
            /*
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                int d1 = randomNewIndividual(nEGGS) - 1;
                int d2 = randomNewIndividual(nEGGS) - 1;
                cuckooEgg[i] = nests[n].eggs[e].solution[i] +
                            randomValue01() * (nests[n].eggs[d1].solution[i] - nests[n].eggs[d2].solution[i]);
            }
            */

            repairSolution(cuckooEgg, inst);
            printf("CUCKOO EGG AVARAGE: %f %f %f %f %f %f \n", cuckooEgg[0],
                   cuckooEgg[1], cuckooEgg[2], cuckooEgg[3], cuckooEgg[4],
                   cuckooEgg[5]);
            cuckooEggObjvalue = LLopt(inst, cuckooEgg);

            if ( cuckooEggObjvalue > globalBestObjvalue ) {
                globalBestObjvalue = cuckooEggObjvalue;
                for (int i = 0; i < inst->nof_subperiods; ++i)
                    global_best[i] = cuckooEgg[i];
            }


            // Butto giù un uovo del nido peggiore scelto a caso

            orderNest(nests[n].eggs, nEGGS);


            printf("SELECTED EGG TO THROW WITH OBJVAL: %d %f", nests[n].eggs[nEGGS - 1].rank, nests[n].eggs[nEGGS - 1].objvalue);
            for (int i = 0; i < inst->nof_subperiods; ++i)
                nests[n].eggs[ nEGGS - 1 ].solution[i] = cuckooEgg[i];
            nests[n].eggs[nEGGS - 1].objvalue = cuckooEggObjvalue;
            printf("EGG REPLACED \n");
            free(cuckooEgg);


            orderNest(nests[n].eggs, nEGGS );



            // Torna la vera madre e può buttare giù con prob 0.25 un qualsiasi uovo
            for (int e = 0; e < nEGGS; ++e) {

                double r = randomValue01();
                if ( r <= Pa ) {

                    double * newEGG = calloc(inst->nof_subperiods, sizeof(double ));
                    double newEGGObjvalue = 0.0;

                    // LEVY FLIGHT
                    for (int i = 0; i < inst->nof_subperiods; ++i) {

                        int d1 = randomNewIndividual(nEGGS) - 1;
                        int d2 = randomNewIndividual(nEGGS) - 1;
                        newEGG[i] = nests[n].eggs[e].solution[i] +
                                randomValue01() * (nests[n].eggs[d1].solution[i] - nests[n].eggs[d2].solution[i]);
                    }
                    repairSolution(newEGG, inst);

                    newEGGObjvalue = LLopt(inst, newEGG);


                    // GREEDY CHOISE
                    if ( newEGGObjvalue > nests[n].eggs[e].objvalue ){

                        nests[n].eggs[e].objvalue = newEGGObjvalue;
                        for (int i = 0; i < inst->nof_subperiods; ++i)
                            nests[n].eggs[e].solution[i] = newEGG[i];
                        if ( newEGGObjvalue > globalBestObjvalue ) {
                            globalBestObjvalue = newEGGObjvalue;
                            for (int i = 0; i < inst->nof_subperiods; ++i)
                                global_best[i] = newEGG[i];
                        }
                    }

                }
            }


            // PRINT VALUES
            for (int e = 0; e < nEGGS; ++e)
                fprintf(f, "%f ", nests[n].eggs[e].objvalue);
            //fprintf(f, " ");
        }
        fprintf(f, "\n");

        printf("BEST OBJ VALUE: %f \n", globalBestObjvalue);





    }


    fclose(f);
    free(nests);
}



void orderNest(egg * eggs, int n) {

    int max_idx;
    egg temp; //temp è la variabile temporanea utilizzata per lo scambio

    for (int e = 0; e < n - 1; ++e) {

        max_idx = e;
        for (int j = e + 1; j < n; ++j)
            if ( eggs[j].objvalue > eggs[max_idx].objvalue )
                max_idx = j;

        // Scambio i valori, si trova l'elemento più piccolo dell'array
        // e si scambia con l'elemento alla posizione i
        temp = eggs[max_idx];
        eggs[max_idx] = eggs[e];
        eggs[e] = temp;

    }
}




/*
 for (int e = 0; e < nEGGS; ++e) {

                for (int i = 0; i < inst->nof_subperiods; ++i) {

                    double u = randomValue01() * DEVu;
                    double v = randomValue01();
                    double s = u / ( pow( v, 1 / B) );

                    cuckooEgg[i] = nests[n].eggs[e].solution[i] + randomValue01() * 0.01 *
                            s * (nests[n].eggs[e].solution[i] - nests[n].eggs[bestEggOfTheNest].solution[i]);

                }

                repairSolution(cuckooEgg, inst);
                cuckooEggObjvalue = LLopt(inst, cuckooEgg);

                printf("POST VALUE: %f %f %f %f %f %f \n",cuckooEgg[0],
                       cuckooEgg[1], cuckooEgg[2], cuckooEgg[3], cuckooEgg[4],
                       cuckooEgg[5]);


    }



            double maxValue = 0.0;
            int maxIndex = 0;
            for (int e = 0; e < nEGGS; ++e) {
                if (nests[n].eggs[e].objvalue > maxValue){
                    maxValue = nests[n].eggs[e].objvalue;
                    maxIndex = e;
                }
            }
            nests[n].indexBestEgg = maxIndex;
            nests[n].valueBestEgg = maxValue;
            printf("Index best egg in the nest: %d objvalue: %f \n", nests[n].indexBestEgg, nests[n].valueBestEgg);

 */