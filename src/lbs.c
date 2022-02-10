#include "../include/lbs.h"
#include "../include/sa.h"
#include "utils.h"

#define  K  10
#define  S  3
#define  ITER 27



void lbsUL(instance *inst, double *global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/LBSvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int n = K * S;
    printf("DIMENSIONE POPOLAZIONE ESPANSA: %d \n", n);


    beam * kSol = calloc( K , sizeof( beam ));
    beam (* dSol)[ K ];
    dSol = calloc( S , sizeof( *dSol ));

    double globalBestObjvalue = 0.0;


    for (int b = 0; b < K; ++b) {
        kSol[b].solution = calloc(inst->nof_subperiods, sizeof(double));
        for (int i = 0; i < inst->nof_subperiods; ++i)
            kSol[b].solution[i] = randomPrice(i, inst);
        repairSolution(kSol[b].solution, inst);
        printf("Beam created \n");
    }


    for (int s = 0; s < S; ++s) {
        for (int k = 0; k < K; ++k)
            dSol[s][k].solution = calloc(inst->nof_subperiods, sizeof(double ));
    }


    //dSol[1][0].solution[0] = 0.223;
    //printf("Test accesso %f \n", dSol[0][0].solution[0]);


    for (int g = 0; g < ITER; ++g) {

        // Generazione degli s vicini
        for (int s = 0; s < S; ++s) {
            for (int k = 0; k < K; ++k) {

                double * son = calloc(inst->nof_subperiods, sizeof(double ));
                beam currentBeam = kSol[k];
                //beam newBeam;
                for (int i = 0; i < inst->nof_subperiods; ++i)
                    //newBeam.solution[i] = randomNeighbour(i, inst, currentBeam.solution[i]);
                    son[i] = randomNeighbour(i, inst, currentBeam.solution[i]);

                repairSolution(son, inst);

                printf("PRE VAlue: %f %f %f %f %f %f \n", son[0], son[1], son[2],
                       son[3], son[4], son[5]);

                for (int i = 0; i < inst->nof_subperiods; ++i)
                    dSol[s][k].solution[i] = son[i];

                printf("POST VAlue: %f %f %f %f %f %f \n", dSol[s][k].solution[0], dSol[s][k].solution[1], dSol[s][k].solution[2],
                       dSol[s][k].solution[3], dSol[s][k].solution[4], dSol[s][k].solution[5]);

                free(son);
            }
        }



        // Valutazione degli s vicini
        for (int s = 0; s < S; ++s) {
            for (int k = 0; k < K; ++k) {
                dSol[s][k].objvalue = LLopt(inst, dSol[s][k].solution);
                if (dSol[s][k].objvalue > globalBestObjvalue) {
                    globalBestObjvalue = dSol[s][k].objvalue;
                    for (int i = 0; i < inst->nof_subperiods; ++i)
                        global_best[i] = dSol[s][k].solution[i];
                }
                printf("Objevalue = %f \n", dSol[s][k].objvalue);
                printf("POST VAlue: %f %f %f %f %f %f \n",dSol[s][k].solution[0],
                       dSol[s][k].solution[1], dSol[s][k].solution[2],
                       dSol[s][k].solution[3], dSol[s][k].solution[4],
                       dSol[s][k].solution[5]);
            }
        }



        // Selezione e ridimensionamento
        double * allValues = calloc(K, sizeof(double ));
        for (int k = 0; k < K; ++k) {

            // Selezionare l'indice
            double *values = calloc(S, sizeof(double));
            for (int s = 0; s < S; ++s)
                values[s] = dSol[s][k].objvalue;
            int max_index =  maxObjvalue(values, S);
            printf("SOLUTION SELECTED: %d \n", max_index);
            beam chosen = dSol[max_index][k];
            printf("PRE VAlue: %f %f %f %f %f %f \n",kSol[k].solution[0],
                   kSol[k].solution[1], kSol[k].solution[2],
                   kSol[k].solution[3], kSol[k].solution[4],
                   kSol[k].solution[5]);
            kSol[k] = chosen;
            printf("POST VAlue: %f %f %f %f %f %f \n",kSol[k].solution[0],
                   kSol[k].solution[1], kSol[k].solution[2],
                   kSol[k].solution[3], kSol[k].solution[4],
                   kSol[k].solution[5]);

            allValues[k] = kSol[k].objvalue;
            fprintf(f,"%f ", kSol[k].objvalue);
        }
        int max_obj = maxObjvalue(allValues, K);
        fprintf(f,"%f ", kSol[max_obj].objvalue);
        fprintf(f,"\n");

    }

    /*
    // Cerco la soluzione con il maggior obj value
    double * kValues = calloc(K, sizeof(double ));
    for (int k = 0; k < K; ++k)
        kValues[k] = kSol[k].objvalue;
    int max = maxObjvalue(kValues, K);
    printf("BEST SOLUTION: %f \n", kSol[max].objvalue);
    for (int i = 0; i < inst->nof_subperiods; ++i)
        printf("Price: %f  \n", kSol[max].solution[i]);
    */

    printf("BEST SOLUTION: %f \n", globalBestObjvalue);



    fclose(f);
}

