#include "../include/lbs.h"
#include "../include/sa.h"
#include "../include/ga.h"
#include "../include/de.h"
#include "utils.h"
#include "math.h"


#define   N    20
#define   GEN  70
#define   C    0.25



void deUL(instance *inst, double *global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/DEvalues.txt", "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }



    // INSTANTIATION

    individual *pop = calloc(N, sizeof(individual));
    individual *newPop = calloc(N, sizeof(individual));

    double globalBestObjval = 0.0;

    for (int n = 0; n < N; ++n) {

        pop[n].solution = calloc(inst->nof_subperiods, sizeof(double));
        newPop[n].solution = calloc(inst->nof_subperiods, sizeof(double));

        for (int i = 0; i < inst->nof_subperiods; ++i) {
            pop[n].solution[i] = randomPrice(i, inst);
        }
        repairSolution(pop[n].solution, inst);
        pop[n].objvalue = LLopt(inst, pop[n].solution);
        pop[n].rank = n;
    }


    for (int g = 0; g < GEN; ++g) {

        // Creation of the new population mutated
        for (int n = 0; n < N; ++n) {

            int r1 = randomNewIndividual(N) - 1;
            int r2 = randomNewIndividual(N) - 1;
            int r3 = randomNewIndividual(N) - 1;

            while (r2 == r1)
                r2 = randomNewIndividual(N) - 1;

            while (r3 == r1 || r3 == r2)
                r3 = randomNewIndividual(N) - 1;


            int delta = randomNewIndividual(inst->nof_subperiods) - 1;
            printf("INDICI SELEZIONATI:  %d %d %d \n", r1, r2, r3);


            for (int i = 0; i < inst->nof_subperiods; ++i) {

                double r = randomValue01();
                if (r <= C || i == delta) {

                    double difference = pop[r2].solution[i] - pop[r3].solution[i];
                    newPop[n].solution[i] = pop[r1].solution[i] + difference;

                } else {

                    newPop[n].solution[i] = pop[r1].solution[i];
                }

            }
            repairSolution(newPop[n].solution, inst);

            newPop[n].objvalue = LLopt(inst, newPop[n].solution);

        }

        // Selection of the best individuals
        for (int n = 0; n < N; ++n) {

            if (newPop[n].objvalue < pop[n].objvalue) {

                continue;
            } else {

                for (int i = 0; i < inst->nof_subperiods; ++i)
                    pop[n].solution[i] = newPop[n].solution[i];
                pop[n].objvalue = newPop[n].objvalue;
            }
        }

        for (int n = 0; n < N; ++n) {

            if (pop[n].objvalue > globalBestObjval) {
                for (int i = 0; i < inst->nof_subperiods; ++i)
                    global_best[i] = pop[n].solution[i];
            }
            fprintf(f, "%f ", pop[n].objvalue);
        }
        fprintf(f, "\n");

        erasePopulation(inst, newPop);

    }

    fclose(f);
    free(pop);

}


void erasePopulation(instance * inst, individual * pop) {

    for (int n = 0; n < N; ++n) {

        pop[n].objvalue = 0.0;
        for (int i = 0; i < inst->nof_subperiods; ++i) {
            pop[n].solution[i] = 0.0;
        }
    }
}