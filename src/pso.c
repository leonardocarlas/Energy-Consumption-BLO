#include "../include/pso.h"
#include "utils.h"

#define EPS           1e-5
#define INERTIA_W     0.4
#define C1            1
#define C2            1
#define Pm            0.1
#define DELTA         0.2
#define THETA         0.001
#define G_primo       5
#define G             25
#define N             10
#define U             4



void psoUL(instance *inst, double *global_best) {


    // Generate initial population
    particle *swarm = calloc( N, sizeof(particle));
    double best_objval = 0.0;
    int best_particle_index = 0;
    double *objVector = calloc(G, sizeof(double));
    int usePerturbation = 0;

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/PSOvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int p = 0; p < N; ++p) {

        swarm[p].position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].velocity =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_personal_position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_objval = 0.0;
    }

    for (int p = 0; p < N; ++p) {
        printf("--- Particle: %i \n", p+1 );
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            swarm[p].position[i] = randomPrice(i, inst);
            swarm[p].best_personal_position[i] = swarm[p].position[i];
            printf("Position: %f \n", swarm[p].position[i]);
        }
    }



    // Repeat for G iterations
    for (int g = 0; g < G; ++g) {

        printf("\n--- ITERAZIONE: %d --- \n", g+1);

        // 2.1 Repair solution to satisfy C1 - C3 constraints
        printf("--- REPAIRING THE SWARM --- \n");
        repairSwarm(swarm, inst);
        printf("--- SWARM REPAIRED --- \n");


        // 2.2 for each particle I solve the LL problem
        for (int p = 0; p < N; ++p) {

            printf("--- PROCESSING THE LL PROBLEM FOR THE %d Particle --- \n", p+1);
            double objval = LLopt(inst, swarm[p].position);
            swarm[p].actual_objval = objval;
            printf("--- objval: %f --- \n", objval);

            // 2.3 Evaluate the fitness function and set the best solution
            if (objval > best_objval) {

                best_objval = objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    global_best[i] = swarm[p].position[i];
                }
                best_particle_index = p;
                printf("--- NEW BEST OBJVAL OBTAINED AND GLOBAL BEST UPDATED --- \n");
            }

            // Update the best personal position if it is greater than the previous value
            if (objval > swarm[p].best_objval) {

                swarm[p].best_objval = objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    swarm[p].best_personal_position[i] = swarm[p].position[i];
                }
            }
        }


        objVector[g] = best_objval;

        // Check if I should introduce the perturbation
        int check = 1;
        for (int i = 1; i < G_primo; ++i) {
            if ( (objVector[g - i+1] - objVector[g-i]) / objVector[g - i+1] < THETA ) {
                check = check * 1;
            } else {
                check = check * 0;
            }

        }
        if ( check == 1 ) {                 // la soluzione non è migliorata

            double r = (double) rand() / (double) RAND_MAX;
            if (r <= Pm){
                usePerturbation = 1;
                printf("\n INTRODUCO LA PERTURBAZIONE %d \n", check);
            } else {
                usePerturbation = 0;
            }
        }


        printf("\n \n La migliore particella è la %i \n \n", best_particle_index + 1);

        // 2.4 calculating the speed inertia vector
        printf("--- SPEED INERTIA --- \n");
        for (int p = 0; p < N; ++p) {

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

        // 2.5 Introduce perturbation here
        if (usePerturbation == 1) {
            for (int p = 0; p < N; ++p) {
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    double C = randomPerturbation(i, inst, DELTA);
                    printf("PERTURBATION: %f \n", C);
                    printf("Old position: %f \n", swarm[p].position[i]);
                    swarm[p].position[i] += C;
                    printf("New position: %f \n", swarm[p].position[i]);
                }
            }
            printf("\n Perturbation introduced in all the particles \n");
        }


        // Print results of the iteration
        fprintf(f, "%f", best_objval);
        for (int p = 0; p < N; ++p) {
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


void psoULBLMF(instance * instanceVector, instance * inst, double * global_best) {

    FILE *f = fopen("/home/leonardo/Scrivania/BLO/py files/PSOBLMFvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Generate initial population
    particle * swarm = calloc( N, sizeof(particle) );
    double bestObjvalue = 0.0;
    int bestParticleIndex = 0;
    double *objVector = calloc(G, sizeof(double));
    int usePerturbation = 0;

    for (int p = 0; p < N; ++p) {

        swarm[p].position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].velocity =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_personal_position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_objval = 0.0;

        printf("--- PARTICLE %i --- \n", p+1 );
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            swarm[p].position[i] = randomPrice(i, inst);
            swarm[p].best_personal_position[i] = swarm[p].position[i];
            printf("Position: %f \n", swarm[p].position[i]);
        }
    }



    for (int g = 0; g < G; ++g) {

        printf("\n--- ITERAZIONE: %d --- \n", g+1);

        // 2.1 Repair solution to satisfy C1 - C3 constraints
        printf("--- REPAIRING THE SWARM --- \n");
        repairSwarm(swarm, inst);
        printf("--- SWARM REPAIRED --- \n");


        // 2.2 for each particle I solve the LL problem for each instance of the instanceVector
        for (int p = 0; p < N; ++p) {

            printf("--- PROCESSING THE LL PROBLEM FOR THE %d Particle --- \n", p+1);

            double sum = 0.0;

            for (int u = 0; u < U; ++u)
                sum = sum + LLopt( &instanceVector[u], swarm[p].position);

            swarm[p].actual_objval = sum;
            printf("--- TOTAL OBJ VALUE FROM THE 4 CONSUMERS: %f --- \n", sum);


            // 2.3 Evaluate the fitness function. If is the case update the best solution of the particle
            if ( swarm[p].actual_objval > bestObjvalue) {

                bestObjvalue = swarm[p].actual_objval;

                for (int i = 0; i < inst->nof_subperiods; ++i)
                    global_best[i] = swarm[p].position[i];

                bestParticleIndex = p;
            }

            // Update the best personal position if it is greater than the previous value
            if ( swarm[p].actual_objval > swarm[p].best_objval) {

                swarm[p].best_objval = swarm[p].actual_objval;

                for (int i = 0; i < inst->nof_subperiods; ++i)
                    swarm[p].best_personal_position[i] = swarm[p].position[i];

            }
        }

        // History of the objValue
        objVector[g] = bestObjvalue;

        // Check if I should introduce the perturbation
        int check = introducePerturbation(objVector, g);

        if ( check == 1 ) {                 // la soluzione non è migliorata

            double r = (double) rand() / (double) RAND_MAX;
            if (r <= Pm){
                usePerturbation = 1;
                printf("\n INTRODUCO LA PERTURBAZIONE %d \n", check);
            } else {
                usePerturbation = 0;
            }
        }


        printf("\n \n ACTUAL BEST PARTICLE %i \n \n", bestParticleIndex + 1);


        // 2.4 calculating the speed inertia vector
        printf("--- UPDATING SPEED INERTIA --- \n");
        for (int p = 0; p < N; ++p) {

            if (p == bestParticleIndex)
                continue;
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                double r1 = (double)rand() / (double)RAND_MAX ;
                double r2 = (double)rand() / (double)RAND_MAX ;

                printf("Best personal position: %f \n", swarm[p].best_personal_position[i]);
                printf("Position: %f \n", swarm[p].position[i]);
                swarm[p].velocity[i] = INERTIA_W * swarm[p].velocity[i] +
                                       r1 * C1 * (swarm[p].best_personal_position[i] - swarm[p].position[i]) +
                                       r2 * C2 * (global_best[i] - swarm[p].position[i]);

                printf("Old position %d particle %d : %f \n", i+1, p+1, swarm[p].position[i]);
                swarm[p].position[i] = swarm[p].position[i] + swarm[p].velocity[i];
                printf("New position %d particle %d : %f \n", i+1, p+1, swarm[p].position[i]);

            }
        }

        // 2.5 Introduce perturbation here
        if (usePerturbation == 1) {
            for (int p = 0; p < N; ++p) {
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    double C = randomPerturbation(i, inst, DELTA);
                    printf("PERTURBATION: %f \n", C);
                    printf("Old position: %f \n", swarm[p].position[i]);
                    swarm[p].position[i] += C;
                    printf("New position: %f \n", swarm[p].position[i]);
                }
            }
            printf("\n Perturbation introduced in all the particles \n");
        }


        // Print results of the iteration
        fprintf(f, "%f", bestObjvalue);

        for (int p = 0; p < N; ++p)
            fprintf(f, " %f", swarm[p].actual_objval);

        fprintf(f, "\n");








    }






    free(swarm);
    fclose(f);
}

int introducePerturbation( double * objVector, int g) {

    int check = 1;
    for (int i = 1; i < G_primo; ++i) {
        if ( (objVector[g - i+1] - objVector[g-i]) / objVector[g - i+1] < THETA ) {
            check = check * 1;
        } else {
            check = check * 0;
        }

    }
    return check;
}

void repairSwarm(particle *swarm, instance *inst) {

    for (int p = 0; p < N; ++p) {
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
        printf("LEFT SIDE: %f >? X_AVARAGE: %f \n", sum / inst->T, inst->X_AVARAGE);
        // controllo vincolo x_avarage, abbasso tutti i valori di una determinata percentuale
        if ( sum / inst->T > inst->X_AVARAGE)          // || sum < (inst->X_AVARAGE - EPS) * inst->T
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

