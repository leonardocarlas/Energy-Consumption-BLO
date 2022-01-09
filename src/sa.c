#include "../include/sa.h"
#include "utils.h"
#include <math.h>
#define T0         15.0
#define TFINAL     0.001
#define a          0.1
#define ITER       15
#define K          0.1
#define alpha      0.9

// ITER mi dice quante altre soluzioni devo considerare ad ogni temperatura




void saUL(instance *inst, double *global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/SAvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    double *solution = calloc(inst->nof_subperiods, sizeof(double ));
    double *new_solution = calloc(inst->nof_subperiods, sizeof(double ));

    double current_objval = 0.0;
    double new_objval = 0.0;
    int isRandomMovedToNeighbour = 0;

    // Istanzio la soluzione iniziale
    for (int i = 0; i < inst->nof_subperiods; ++i) {

        solution[i] = randomPrice(i, inst);
        printf("Position: %f \n", solution[i]);
    }
    repairSolution(solution, inst);
    //current_objval = LLopt(inst, solution);

    double T = T0;

    while (T > TFINAL) {

        printf("TEMP: %f \n", T);

        // Da sostituire con un while su condizione
        for (int j = 0; j < ITER; ++j) {

            // 2.2 Genero la nuova soluzione
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                //printf("Old position: %f \n", solution[i]);
                new_solution[i] = randomNeighbour(i, inst, solution[i]);
                printf("New position: %f \n", new_solution[i]);
            }

            // 2.1 Riparo la soluzione ad ogni iterazione
            repairSolution(solution, inst);

            new_objval = LLopt(inst, new_solution);
            printf("NEW obj val: %f OLD obj val: %f \n", new_objval, current_objval);

            // 2.3 confronto le soluzioni
            if (new_objval > current_objval) {

                current_objval = new_objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    solution[i] = new_solution[i];
                }
                printf("TROVATO VALORE MIGLIORE \n");
                isRandomMovedToNeighbour = 0;

            } else {

                double delta = current_objval - new_objval;
                double r = (double)rand() / (double)RAND_MAX ;
                if ( r <= exp( - delta / T ) ){

                    printf("r : %f  exp : %f \n", r, exp(- delta / (K * T) ) );
                    current_objval = new_objval;
                    for (int i = 0; i < inst->nof_subperiods; ++i) {
                        solution[i] = new_solution[i];
                    }
                    printf("RANDOMICAMENTE SPOSTATO VERSO UN VALORE VICINO \n");
                    isRandomMovedToNeighbour = 1;
                }
            }
            fprintf(f,"%f %d %f %d\n", T, j+1, current_objval, isRandomMovedToNeighbour);
        }

        T *= alpha;

    }




    // print out the global best that contains the best solution
    for (int i = 0; i < inst->nof_subperiods; ++i) {
        global_best[i] = solution[i];
        printf("%i BEST VALUE FOUND: %f \n", i+1, solution[i]);
    }


    free(solution);
    fclose(f);


}







void repairSolution(double *solution, instance *inst) {

    double sum = 0.0;
    for (int i = 0; i < inst->nof_subperiods; ++i) {

        // caso sotto al lower bound
        if (solution[i] < inst->table_sm1[i].min_price) {
            solution[i] = inst->table_sm1[i].min_price;
            printf("Particella riparata con il lower bound \n");
        }
        // caso sopra all'upper bound
        if (solution[i] > inst->table_sm1[i].max_price) {
            solution[i] = inst->table_sm1[i].max_price;
            printf("Particella riparata con l'upper bound \n");
        }
        double amplitudePi = inst->table_sm1[i].end_interval - inst->table_sm1[i].start_interval + 1;
        sum = sum + amplitudePi * solution[i];
    }
    printf("LEFT SIDE: %f >? X_AVARAGE: %f \n", sum / inst->T, inst->X_AVARAGE);
    // controllo vincolo x_avarage, abbasso tutti i valori di una determinata percentuale
    if ( sum / inst->T > inst->X_AVARAGE)          // || sum < (inst->X_AVARAGE - EPS) * inst->T
    {
        double difference = sum - inst->X_AVARAGE * inst->T;
        double percentage = difference / sum;
        for (int i = 0; i < inst->nof_subperiods; ++i) {
            solution[i] -= solution[i] * percentage;
        }
        printf("Particella riparata con l'avarage \n");
    }



}