#include "../include/pso.h"
#include "utils.h"

#define EPS           1e-5
#define INERTIA_W     0.4
#define C1            1
#define C2            1

int test_pso(instance *inst) {

    // Generate initial population
    particle *swarm = calloc( inst->N, sizeof(particle));

    for (int p = 0; p < inst->N; ++p) {
        swarm[p].position =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].velocity =  calloc(inst->nof_subperiods, sizeof(double));
        swarm[p].best_personal_position =  calloc(inst->nof_subperiods, sizeof(double));

    }
    double *global_best = calloc(inst->nof_subperiods, sizeof(double));

    for (int p = 0; p < inst->N; ++p) {
        printf("--- Particle: %i \n", p+1 );
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            swarm[p].position[i] = randomPrice(i, inst);
            printf("Position: %f \n", swarm[p].position[i]);
        }
    }

    // Repeat for G iterations
    for (int g = 0; g < inst->G; ++g) {
        // 2.1 Repair solution to satisfy C1 - C3 constraints
        repairSwarm(swarm, inst);

        double best_objval = 1000.0;
        // 2.2 for each particle I solve the LL problem
        for (int p = 0; p < inst->N; ++p) {

            double objval = 7.0919; //ll(inst, swarm[p].position);

            // 2.3 Evaluate the fitness function and set the best solution
            if (objval < best_objval) {
                best_objval = objval;
                for (int i = 0; i < inst->nof_subperiods; ++i) {
                    global_best[i] = swarm[p].position[i];
                }
            }
        }

        // 2.4 calculating the speed inertia vector
        for (int p = 0; p < inst->N; ++p) {
            for (int i = 0; i < inst->nof_subperiods; ++i) {

                double r1 = rand() / RAND_MAX;
                double r2 = rand() / RAND_MAX;
                double velocity_comp_i = INERTIA_W * swarm[p].velocity[i] +
                        r1 * C1 * (swarm[p].best_personal_position[i] - swarm[p].position[i]) +
                        r2 * C2 * (global_best[i] - swarm[p].position[i]);
                swarm[p].position[i] = swarm[p].position[i] + velocity_comp_i;

            }
        }

    }

    free(swarm);

    // output of the global best that contains the best solution
    return 0;
}


void repairSwarm(particle *swarm, instance *inst) {

    for (int p = 0; p < inst->N; ++p) {
        double sum = 0.0;
        for (int i = 0; i < inst->nof_subperiods; ++i) {

            // caso sotto al lower bound
            if (swarm[p].position[i] < inst->table_sm1[i].min_price)
                swarm[p].position[i] = inst->table_sm1[i].min_price;
            // caso sopra all'upper bound
            if (swarm[p].position[i] > inst->table_sm1[i].max_price)
                swarm[p].position[i] = inst->table_sm1[i].max_price;

            // x_avarage condition
            double amplitudePi = inst->table_sm1[i].end_interval - inst->table_sm1[i].start_interval + 1;
            sum = sum + amplitudePi * swarm[p].position[i];
        }
        if ( sum > inst->X_AVARAGE * inst->T || sum < (inst->X_AVARAGE - EPS) * inst->T )
            printf("Bad Values \n");
            // repair (how?)
    }



}

