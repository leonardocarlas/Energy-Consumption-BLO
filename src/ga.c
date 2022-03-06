#include "../include/ga.h"
#include "../include/sa.h"
#include "utils.h"
#include <math.h>

#define POP  20
#define GEN  20
#define Pm   0.3



void gaUL(instance *inst, double *global_best) {


    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/GAvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }


    int nBEST = POP / 2;
    int nCOMB = 0;
    for (int i = 0; i < nBEST; ++i) {
        for (int j = 0; j < nBEST; ++j) {
            if (j > i)
                nCOMB++;
        }
    }
    int n = POP + nCOMB ;
    printf("DIMENSIONE POPOLAZIONE: %d \n", n);
    individual * pop = calloc( n, sizeof(individual) );

    // ISTANZIAZIONE
    for (int p = 0; p < n; ++p) {
        pop[p].solution = calloc(inst->nof_subperiods, sizeof(double));
        pop[p].rank = p;
        if (p < POP) {              // popolazione iniziale
            for (int i = 0; i < inst->nof_subperiods; ++i)
                pop[p].solution[i] = randomPrice(i, inst);
            repairSolution(pop[p].solution, inst);
            pop[p].objvalue = LLopt(inst, pop[p].solution);
        } else {                    // popolazione combinatoria
            pop[p].objvalue = 0.0;
        }
        printf("Rank: %d Objvalue: %f \n", pop[p].rank, pop[p].objvalue);
    }

    for (int g = 0; g < GEN; ++g) {

        orderPopulation(pop, n);

        for (int p = 0; p < n; ++p) {
            printf("TEST ORDINE %f \n", pop[p].objvalue);
            fprintf(f,"%f ", pop[p].objvalue);
        }
        fprintf(f,"\n");

        for (int p = 0; p < n; ++p) {
            printf("Previous VAlue: %f %f %f %f %f %f \n",pop[p].solution[0],
                   pop[p].solution[1], pop[p].solution[2],
                   pop[p].solution[3], pop[p].solution[4],
                   pop[p].solution[5]);
        }

        // Select the nBEST and combine
        int counter = 0;
        for (int i = 0; i < nBEST; ++i) {
            for (int j = 0; j < nBEST; ++j) {
                if ( pop[j].rank > pop[i].rank ) {

                    // Work on the combination
                    printf("COMBINATION OF INDIVIDUAL: %d %d \n", pop[i].rank , pop[j].rank );

                    double * son = calloc(inst->nof_subperiods, sizeof (double ));
                    generateSon(son, pop[i].solution, pop[j].solution, inst);
                    repairSolution(son, inst);
                    for (int k = 0; k < inst->nof_subperiods; ++k) {
                        printf("Price SON: %f \n", son[k]);
                        pop[POP + counter].solution[k] = son[k];
                    }
                    printf("SON GENERATED \n");

                    // assegno il son ad una soluzione scarsa

                    counter++;
                    free(son);
                }
                else
                    continue;
            }
        }

        for (int p = 0; p < n; ++p) {
            printf("POST VAlue: %f %f %f %f %f %f \n",pop[p].solution[0],
                   pop[p].solution[1], pop[p].solution[2],
                   pop[p].solution[3], pop[p].solution[4],
                   pop[p].solution[5]);
        }

        // Evaluate
        for (int p = 0; p < n; ++p) {
            pop[p].objvalue = LLopt(inst, pop[p].solution);
        }


    }

    // Ultimo ordinamento
    orderPopulation(pop, n);
    // Print prima soluzione
    printf("--- BEST SOLUTION: %f ---\n", pop[0].objvalue);
    for (int i = 0; i < inst->nof_subperiods; ++i)
        printf("Price: %f \n", pop[0].solution[i] );


    free(pop);
    fclose(f);




}





void orderPopulation(individual * pop, int n) {

    int max_idx;
    individual temp; //temp è la variabile temporanea utilizzata per lo scambio

    for (int p = 0; p < n - 1; ++p) {

        max_idx = p;
        for (int j = p + 1; j < n; ++j)
            if ( pop[j].objvalue > pop[max_idx].objvalue )
                max_idx = j;

        // Scambio i valori, si trova l'elemento più piccolo dell'array
        // e si scambia con l'elemento alla posizione i
        temp = pop[max_idx];
        pop[max_idx] = pop[p];
        pop[p] = temp;

    }
}


void generateSon(double * son, double * gen1, double * gen2, instance *inst){

    // Recombine individuals
    int cutPoint = randomSubperiod(inst) - 1;
    for (int k = 0; k < inst->nof_subperiods; ++k) {
        if (k <= cutPoint)                                 // ci incollo sempre la prima fino al cutpoint
            son[k] = gen1[k];
        else                                                   // ci incollo sempre la seconda dopo il cutpoint
            son[k] = gen2[k];
    }

    // Mutation with probability
    double r = (double) rand() / (double ) RAND_MAX;
    if (r <= Pm) {
        int mutationPoint = randomSubperiod(inst) - 1;
        son[mutationPoint] = randomPrice(mutationPoint, inst);
        printf("Mutation completed\n");
    }
}


void deleteOldAddNew(individual ind, double * son, instance *inst){

    for (int i = 0; i < inst->nof_subperiods; ++i) {
        ind.solution[i] = son[i];
    }
    ind.objvalue = 0.0;

}


/*
    individual uno;
    uno.solution = calloc(inst->nof_subperiods, sizeof (double ));
    for (int i = 0; i < inst->nof_subperiods; ++i) {
    uno.solution[i] = randomPrice(i, inst);
    }

    individual due = uno;
    //due.solution = uno.solution;

    if (due.solution[0] == uno.solution[0])
    printf("UGUALI \n");
*/






