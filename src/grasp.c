#include "../include/grasp.h"
#include "../include/sa.h"
#include "../include/lbs.h"
#include "utils.h"
#include <math.h>

#define G       30
#define V       4
#define LRC     5
#define E       20
#define I       6


void graspUL(instance *inst, double *global_best) {

    double * solution = calloc(inst->nof_subperiods, sizeof(double));
    double bestObjvalue = 0.0;

    FILE *f = fopen("//home/leonardo/Scrivania/BLO/py files/GRASPvalues.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }


    for (int g = 0; g < G; ++g) {

        printf("Pre CONSTRUCTION Value: %f %f %f %f %f %f \n",solution[0],
               solution[1], solution[2], solution[3], solution[4], solution[5]);

        constructGreedyRandomizedSolution(solution, inst);

        printf("Pre REPAIRING Value: %f %f %f %f %f %f \n",solution[0],
               solution[1], solution[2], solution[3], solution[4], solution[5]);

        repairSolution(solution, inst);

        printf("After REPAIRING Value: %f %f %f %f %f %f \n",solution[0],
               solution[1], solution[2], solution[3], solution[4], solution[5]);

        localSearch(solution, inst);

        printf("Post LOCAL SEARCH Value: %f %f %f %f %f %f \n",solution[0],
               solution[1], solution[2], solution[3], solution[4], solution[5]);

        double currentObjvalue = LLopt(inst, solution);

        if ( currentObjvalue > bestObjvalue ) {

            bestObjvalue = currentObjvalue;
            for (int i = 0; i < inst->nof_subperiods; ++i)
                global_best[i] = solution[i];
        }

        fprintf(f, "%f\n", currentObjvalue);

    }


    fclose(f);
    free(solution);

}


void constructGreedyRandomizedSolution(double * solution, instance * inst) {

    for (int i = 0; i < I; ++i) {

        double * set = calloc(E, sizeof(double));
        double * rcl = calloc(LRC, sizeof(double));

        // Creation of the set of E possible prices for subperiod i

        for (int e = 0; e < E; ++e)
            set[e] = randomPrice(i, inst);

        // Random selection of the LCR prices from the set

        for (int l = 0; l < LRC; ++l) {
            rcl[l] = set[randomNewIndividual(E) - 1];
            printf("RCL element: %f \n", set[randomNewIndividual(E) - 1]);
        }

        solution[i] = selectBestPriceFromRCL(i, rcl, inst);

    }


}


double selectBestPriceFromRCL(int i, double * rcl, instance * inst ) {

    double maxAmplitudeValue = 0.0;
    int indexMaxAmplitudeValue = 0;

    for (int l = 0; l < LRC; ++l) {

        double * falseVectorPrices = calloc(inst->nof_subperiods, sizeof(double));
        for (int j = 0; j < inst->nof_subperiods; ++j)
            falseVectorPrices[j] = rcl[l];

        double objvalue = LLopt(inst, falseVectorPrices);
        double mediumValue = objvalue / inst->T;
        double amplitude = inst->table_sm1[i].end_interval - inst->table_sm1[i].start_interval + 1;
        double amplitudeValue = amplitude * falseVectorPrices[0] * mediumValue;

        if ( amplitudeValue > maxAmplitudeValue){
            maxAmplitudeValue = amplitudeValue;
            indexMaxAmplitudeValue = l;
        }
    }

    return rcl[indexMaxAmplitudeValue];
}


void localSearch(double * solution, instance * inst){

    beam * neighbours = calloc(V, sizeof(beam));
    double maxValue = 0.0;
    int indexBestNeighbour;

    for (int v = 0; v < V; ++v) {

        neighbours[v].solution = calloc(inst->nof_subperiods, sizeof(double));

        for (int i = 0; i < inst->nof_subperiods; ++i)
            neighbours[v].solution[i] = randomNeighbour(i, inst, solution[i]);

        repairSolution(neighbours[v].solution, inst);

        neighbours[v].objvalue = LLopt(inst, neighbours[v].solution);

        if ( neighbours[v].objvalue > maxValue) {

            maxValue = neighbours[v].objvalue;
            indexBestNeighbour = v;
        }

    }

    for (int i = 0; i < inst->nof_subperiods; ++i)
        solution[i] = neighbours[indexBestNeighbour].solution[i];

}