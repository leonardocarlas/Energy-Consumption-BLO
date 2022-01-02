#include "../include/pso.h"
#include "utils.h"

#define EPS           1e-5
#define INERTIA_W     0.4
#define C1            1
#define C2            1


void psoUL(instance *inst, double *global_best) {


    // Generate initial population
    particle *swarm = calloc( inst->N, sizeof(particle));
    double best_objval = 1000.0;
    int best_particle_index = 0;

    for (int p = 0; p < inst->N; ++p) {

        swarm[p].position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].velocity =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_personal_position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_objval = 1000;
    }

    for (int p = 0; p < inst->N; ++p) {
        printf("--- Particle: %i \n", p+1 );
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            swarm[p].position[i] = randomPrice(i, inst);
            swarm[p].best_personal_position[i] = swarm[p].position[i];
            printf("Position: %f \n", swarm[p].position[i]);
        }
    }

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/objvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Repeat for G iterations
    for (int g = 0; g < inst->G; ++g) {

        // 2.1 Repair solution to satisfy C1 - C3 constraints
        printf("--- REPAIRING THE SWARM --- \n");
        repairSwarm(swarm, inst);
        printf("--- SWARM REPAIRED --- \n");


        // 2.2 for each particle I solve the LL problem
        for (int p = 0; p < inst->N; ++p) {

            printf("--- PROCESSING THE LL PROBLEM FOR THE %d Particle --- \n", p+1);
            double objval = LLopt(inst, swarm[p].position);
            swarm[p].actual_objval = objval;
            printf("--- objval: %f --- \n", objval);

            // 2.3 Evaluate the fitness function and set the best solution
            if (objval < best_objval) {
                best_objval = objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    global_best[i] = swarm[p].position[i];
                }
                best_particle_index = p;
                printf("--- NEW BEST OBJVAL OBTAINED AND GLOBAL BEST UPDATED --- \n");
            }

            // Update the best personal position if it is lower than the previous value
            if (objval < swarm[p].best_objval) {

                swarm[p].best_objval = objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    swarm[p].best_personal_position[i] = swarm[p].position[i];
                }
            }
        }

        printf("\n \n La migliore particella è la %i \n \n", best_particle_index + 1);

        // 2.4 calculating the speed inertia vector
        printf("--- SPEED INERTIA --- \n");
        for (int p = 0; p < inst->N; ++p) {

            if (p == best_particle_index)
                continue;
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                double r1 = (double)rand() / (double)RAND_MAX ;
                double r2 = (double)rand() / (double)RAND_MAX ;

                printf("Best personal position: %f \n", swarm[p].best_personal_position[i]);
                printf("Position: %f \n", swarm[p].position[i]);
                swarm[p].velocity[i] = INERTIA_W * swarm[p].velocity[i] +
                        r1 * C1 * (swarm[p].best_personal_position[i] - swarm[p].position[i]) +
                        r2 * C2 * (global_best[i] - swarm[p].position[i]);
                //swarm[p].velocity[i] = (global_best[i] - swarm[p].position[i]);

                //printf("velocity comp: %f \n", swarm[p].velocity[i]);
                printf("Old position %d particle %d : %f \n", i+1, p+1, swarm[p].position[i]);
                swarm[p].position[i] = swarm[p].position[i] + swarm[p].velocity[i];
                printf("New position %d particle %d : %f \n", i+1, p+1, swarm[p].position[i]);

            }
        }


        // Print results of the iteration
        fprintf(f, "%f", best_objval);
        for (int p = 0; p < inst->N; ++p) {
            fprintf(f, " %f", swarm[p].actual_objval);
        }
        fprintf(f, "\n");


    }

    free(swarm);
    fclose(f);

    // print out the global best that contains the best solution
    for (int i = 0; i < inst->nof_subperiods; ++i)
        printf("%i GLOBAL BEST VALUE: %f \n", i+1, global_best[i]);

}


void repairSwarm(particle *swarm, instance *inst) {

    for (int p = 0; p < inst->N; ++p) {
        double sum = 0.0;
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            // caso sotto al lower bound
            if (swarm[p].position[i] < inst->table_sm1[i].min_price) {
                swarm[p].position[i] = inst->table_sm1[i].min_price;
                printf("Particella riparata con il lower bound \n");
            }
            // caso sopra all'upper bound
            if (swarm[p].position[i] > inst->table_sm1[i].max_price) {
                swarm[p].position[i] = inst->table_sm1[i].max_price;
                printf("Particella riparata con l'upper bound \n");
            }
            double amplitudePi = inst->table_sm1[i].end_interval - inst->table_sm1[i].start_interval + 1;
            sum = sum + amplitudePi * swarm[p].position[i];
        }
        // controllo vincolo x_avarage, abbasso tutti i valori di una determinata percentuale
        if ( sum > inst->X_AVARAGE * inst->T )          // || sum < (inst->X_AVARAGE - EPS) * inst->T
        {
            double difference = sum - inst->X_AVARAGE * inst->T;
            double percentage = difference / sum;
            for (int i = 0; i < inst->nof_subperiods; ++i) {
                swarm[p].position[i] -= swarm[p].position[i] * percentage;
            }
            printf("Particella riparata con l'avarage \n");
        }

    }



}

