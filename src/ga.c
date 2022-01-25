#include "../include/ga.h"
#include "../include/sa.h"
#include "utils.h"
#include <math.h>


#define POP  6
#define GEN  5
#define Pm   0.3



void gaUL(instance *inst, double *global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/GAvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Random generation of the initial population
    individual *population = calloc( POP * (inst->nof_subperiods + 1), sizeof (double ));
    int NOFBEST = POP / 2;
    individual *new_population = calloc( NOFBEST * (inst->nof_subperiods + 1), sizeof (double ));
    for (int i = 0; i < NOFBEST; ++i)
        new_population[i].solution = calloc( inst->nof_subperiods, sizeof(double));
    int *indexesVector = calloc( POP, sizeof(int));
    double *valuesVector = calloc( POP, sizeof(double));
    int *indexBestVector = calloc( NOFBEST, sizeof(int));


    // Istantiation
    for (int p = 0; p < POP; ++p) {

        population[p].solution = calloc( inst->nof_subperiods, sizeof(double ));

        for (int i = 0; i < inst->nof_subperiods; ++i)
            population[p].solution[i] = randomPrice(i, inst);

        // Repair solution
        repairSolution(population[p].solution, inst);

        for (int i = 0; i < inst->nof_subperiods; ++i)
            printf("Price: %f \n", population[p].solution[i]);

        //printf("ITERATION \n");
    }




    for (int g = 0; g < GEN; ++g) {


        // Evaluate every solution
        for (int p = 0; p < POP; ++p) {

            population[p].objvalue = LLopt(inst, population[p].solution);
            printf("OBJECTIVE VALUE: %f \n", population[p].objvalue);

            indexesVector[p] = p;
            valuesVector[p] = population[p].objvalue;
        }
        fprintf(f,"\n");

        // Sorting the solutions and printing
        fprintf(f,"%d ", g);
        sortingSolutions(indexesVector, valuesVector, POP);
        for (int i = 0; i < POP; ++i) {
            fprintf(f,"%f ", valuesVector[i]);
        }
        fprintf(f,"\n\n");
        for (int i = 0; i < POP; ++i) {
            for (int j = 0; j < inst->nof_subperiods; ++j) {
                fprintf(f,"%f ", population[indexesVector[i]].solution[j] );
            }
            fprintf(f,"\n");
        }

        // Selection of the NOFBEST best individuals
        for(int i = POP - 1; i >= NOFBEST; i--) {

            // printf("Index: %d Value: %f \n", indexesVector[i], valuesVector[i] );
            indexBestVector[POP - i] = indexesVector[i];
            printf("Index: %d \n", indexBestVector[i] );
        }


        // Generation of combinations of the best individuals
        int c = 0;
        for (int i = 0; i < NOFBEST; ++i) {
            for (int j = 0; j < NOFBEST; ++j) {
                if ( indexBestVector[j] > indexBestVector[i] ){

                    // Work on the combination
                    printf("COMBINATION OF INDIVIDUAL: %d %d \n", indexBestVector[i] + 1 , indexBestVector[j] + 1);

                    // Recombine individuals
                    int cutPoint = randomSubperiod(inst) - 1;
                    for (int k = 0; k < inst->nof_subperiods; ++k) {
                        if (k <= cutPoint)                                 // ci incollo sempre la prima fino al cutpoint
                            new_population[c].solution[k] = population[indexBestVector[i]].solution[k];
                        else                                                   // ci incollo sempre la seconda dopo il cutpoint
                            new_population[c].solution[k] = population[indexBestVector[j]].solution[k];
                        printf( "GEN 1: %f\n", population[indexBestVector[i]].solution[k]);
                        printf( "GEN 2: %f\n", population[indexBestVector[j]].solution[k]);
                        printf( "SON  : %f\n", new_population[c].solution[k]);
                    }

                    // Mutation with probability
                    double r = (double) rand() / (double ) RAND_MAX;
                    if (r <= Pm) {
                        int mutationPoint = randomSubperiod(inst) - 1;
                        new_population[c].solution[mutationPoint] = randomPrice(mutationPoint, inst);
                        printf("Mutation completed");
                    }

                    // Repairing the new solution
                    repairSolution(new_population[c].solution, inst);

                    c++;
                }
                else
                    continue;
            }
        }



        // Cleanings of the populations (via i vecchi peggiori e aggiungi i nuovi della popolazione)
        for (int i = 0; i < NOFBEST; i++) {
            population[i].solution = new_population[i].solution;
            for (int j = 0; j < inst->nof_subperiods; ++j) {
                printf("Individual %d Price: %f \n", i, population[i].solution[j]);
            }
        }
        // free(new_population);



    }


    // Print out the global best that contains the best solution
    sortingSolutions(indexesVector, valuesVector, POP);
    //printf("Best individual: %d", indexesVector[POP-1]);
    for (int i = 0; i < POP; ++i) {
        printf("Index Individual: %d Value: %f \n", indexesVector[i], valuesVector[i] );
    }
    printf("--- BEST SOLUTION: %f ---\n", population[indexesVector[POP-1]].objvalue);

    for (int i = 0; i < inst->nof_subperiods; ++i) {
        printf("Price: %f \n", population[indexesVector[POP-1]].solution[i] );
    }




    free(population);
    fclose(f);


}




