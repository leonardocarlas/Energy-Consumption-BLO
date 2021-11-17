#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"


/*
 * Method to define problem variables and constraints
 */
int model_m1(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *pVector, int (*sMatrix)[inst->T], int (*pMatrix)[inst->T]) {

    char binary = 'B';
    char integer = 'I';
    char continuos = 'C';
    char sense_greater[1] = {'G'};
    char    sense_equal[1] = {'E'};
    const double zero[1] = {0.0};
    double  one[1] = {1.0};
    int status;
    int k = 0;
    double P = 10000.0;


    // Declaration of auxiliary structure
    int (*ReversepMatrix)[inst->J];
    ReversepMatrix = calloc(inst->T, sizeof *ReversepMatrix);




    // Definition of P(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "P(%i)", t+1);
        double lb = 0.0;
        double ub = P;
        double obj = 0.0;
                //powerCostTimet(t+1, inst) * 1/1000;     // la potenza dev'essere espressa in kWh

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on P(t) variables.\n");

        pVector[t] = counter;
        //printf("Posizione variabile P(%i): %i \n ", t+1, pVector[t]);
        counter++;
    }


    // Definition of s(j,t) = 0/1
    for ( int j = 0; j < inst->J; j++) {
        for ( int t = 0; t < inst->T; t++) {

            if ( t >= (inst->table_sm5[j].start_interval - 1) && t <  inst->table_sm5[j].end_inteval  ) {
                sprintf(cname[0], "s(%i,%i)", j+1, t+1);
                double lb = 0.0;
                double ub = 1.0;
                double obj = 0.0;

                status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
                if (status) print_error("Error CPXnewcols on s_jt");

                sMatrix[j][t] = counter;
                counter++;
                //printf("Posizione variabile s(%i,%i): %i \n", j+1, t+1, sMatrix[j][t]);
            }
            else {
                sMatrix[j][t] = -1;
            }
        }
    }




    // Definition of P(j,t) = contiguos

    for ( int j = 0; j < inst->J; j++) {
        for (int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "P(%d,%d)", j+1, t+1);
            double lb = 0.0;
            double ub = CPX_INFBOUND;
            double obj = 0.0;

            //Creates the columns of the new variable
            if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname))
                print_error("Wrong CPXnewcols on x var.s");

            pMatrix[j][t] = counter;
            ReversepMatrix[t][j] = counter;
            counter++;
            //printf("Posizione variabile P(%i,%i): %i \n", j+1, t+1, pMatrix[j][t]);

        }
    }




    /* Constraint number: example, variable s(j,t)
     *
     s(1,1) + s(1,2) + s(1,3) + s(1,4) + s(1,5) + ... + s(1,391) = 1
     s(2,1) + s(2,2) + s(2,3) + s(2,4) + s(2,5) + ... + s(2,231) = 1
     s(3,1) + s(3,2) + s(3,3) + s(3,4) + s(3,5) + ... + s(3,10) = 1
     */

    for (int j = 0; j < inst->J; ++j) {

        int end = (inst->table_sm5[j].end_inteval - inst->table_sm20[j].duration + 1);
        int t_counter = end - inst->table_sm5[j].start_interval;

        int *c1_rmatind = calloc( t_counter + 1, sizeof (int) );
        double *c1_rmatval = calloc( t_counter + 1, sizeof (double) );
        k = 0;
        for (int t = ( inst->table_sm5[j].start_interval - 1 ); t < inst->table_sm5[j].end_inteval - inst->table_sm20[j].duration + 1 ; ++t) {
            c1_rmatind[k] = sMatrix[j][t];
            c1_rmatval[k] = 1.0;
            k++;
        }

        int c1_rmatbeg[2] = {0, t_counter + 1};
        status = CPXaddrows (env, lp, 0, 1, t_counter + 1,
                             one, sense_equal, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c1_rmatind);
            free(c1_rmatval);
        }
    }
    //printf("CONSTRAINT n 1: CREATED \n");






    /* Constraint number 3: example P(j,t)
    *
   P(1,4) = f(1,1) * s(1,4) + f(1,2) * s(1,3) + f(1,3) * s(1,2) + f(1,4) * s(1,1)
   ...
   */

    for (int j = 0; j < inst->J; ++j) {
        for (int t = (inst->table_sm5[j].start_interval - 1); t < inst->table_sm5[j].end_inteval ; ++t) {

            int r_counter = 0;

            for (int r = 0; r < inst->table_sm20[j].duration; ++r) {
                if (r == 0 || ( (r <= t) && (r <= (t + 1 - inst->table_sm5[j].start_interval)))) {
                    r_counter++;
                }
            }
            //printf("J = %d, T = %d, R_counter: %d \n", j+1, t+1, r_counter);

            int *chard_rmatind = calloc(r_counter + 1, sizeof(int));
            double *chard_rmatval = calloc(r_counter + 1, sizeof(double));
            int chard_rmatbeg[2] = {0, r_counter};
            k = 0;
            for (int r = 0; r < inst->table_sm20[j].duration; ++r) {
                if (r == 0 || (r <= t && r <= t + 1 - inst->table_sm5[j].start_interval)) {
                    if ( sMatrix[j][t-r] == -1 ) {
                        printf("J: %d Value: %d T: %d  R: %d \n ", j, t-r+1, t ,r  );
                    } else {
                        chard_rmatind[k] = sMatrix[j][t-r];
                        chard_rmatval[k] = powerRequiredShiftableStageR(inst, j, r);
                        k++;
                    }
                }
            }

            chard_rmatind[ r_counter ] = pMatrix[j][t];
            chard_rmatval[ r_counter ] = -1.0;

            status = CPXaddrows (env, lp, 0, 1, r_counter + 1,
                                 zero, sense_equal, chard_rmatbeg, chard_rmatind, chard_rmatval,
                                 NULL, NULL);
            if ( status )
                fprintf(stderr, "CPXaddrows failed.\n");
            else {
                free(chard_rmatval);
                free(chard_rmatind);
            }


        }
    }
    //printf("Constraint HARD OK \n");





    /* Constraint number 4: example, variable P(j,t)
     *
     P(2,1) = 0
     P(2,2) = 0
     P(2,3) = 0
    */

    int c4_rmatbeg[2] = {0, 1};
    for (int j = 0; j < inst->J; j++ ) {
        for (int t = 0; t < inst->T; ++t) {

            if( t+1 < inst->table_sm5[j].start_interval || t+1 > inst->table_sm5[j].end_inteval ) {

                int *c_rmatind = calloc(1, sizeof (int));
                c_rmatind[0] = pMatrix[j][t];
                status = CPXaddrows (env, lp, 0, 1, 1,
                                     zero, sense_equal, c4_rmatbeg, c_rmatind, one,
                                     NULL, NULL);
                if ( status ) {
                    fprintf(stderr, "CPXaddrows failed.\n");
                } else {
                    free(c_rmatind);
                }
            }
        }
    }
    //printf("Constraint n 4 OK \n");





    /* Constraint 1: example, variable P(t)
     * Power required at time t is calculated as the sum of the power required by the J appliances
     P(1) = P(1,1) + P(2,1) + P(3,1)
     P(2) = P(1,2) + P(2,2) + P(3,2)
    */

    int c1_rmatbeg[2] = {0, inst->J};
    for (int t = 0; t < inst->T ; ++t) {

        int *c1_rmatind = calloc(inst->J + 1, sizeof(int));
        double *c1_rmatval = calloc(inst->J + 1, sizeof(double));

        for (int i = 0; i < inst->J; ++i) {
            c1_rmatind[i] = ReversepMatrix[t][i];
            c1_rmatval[i] = 1.0;
        }
        c1_rmatind[inst->J] = pVector[t];
        c1_rmatval[inst->J] = -1.0;

        status = CPXaddrows (env, lp, 0, 1, inst->J + 1,
                             zero, sense_equal, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c1_rmatval);
            free(c1_rmatind);
        }
    }
    //printf("Constraint OK \n");











    return counter;

}

