#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"


/*
 * Method to define problem variables and constraints
 */
void model_m1(instance *inst, CPXENVptr env, CPXLPptr lp) {

    char binary = 'B';
    char integer = 'I';
    char sense_greater[1] = {'G'};
    char    sense_equal[1] = {'E'};
    const double zero[1] = {0.0};
    double  one[1] = {1.0};
    int status;
    int k = 0;
    double P = 10000.0;

    // each name of the variable are string, organizzare le variabili ( column name )
    char **cname = ( char ** ) calloc( 1, sizeof (char *));
    cname[0] = ( char * ) calloc( 100, sizeof(char) );

    int counter = 0;

    // Declaration of structure for variable P(t)
    int *pVector = calloc( inst->T, sizeof (int) );
    // Declaration of structure for variable u(l)
    int *uVector = calloc( inst->L, sizeof (int) );
    // Declaration of structure for variable s(j,t)
    int (*sMatrix)[inst->T];
    sMatrix =  calloc( inst->J, sizeof *sMatrix);
    // Declaration of structure for variable P(j,t)
    int (*pMatrix)[inst->T];
    pMatrix =  calloc( inst->J, sizeof *pMatrix);
    int (*ReversepMatrix)[inst->J];
    ReversepMatrix = calloc(inst->T, sizeof *ReversepMatrix);





    // Definition of P(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "P(%i)", t+1);
        double lb = 0.0;
        double ub = P;
        double obj = powerCostTimet(t+1, inst);

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname);
        if ( status ){
            fprintf (stderr,"CPXnewcols failed on P(t) variables.\n");
        }

        pVector[t] = counter;
        //printf("Posizione variabile P(%i): %i \n ", t+1, pVector[t]);
        counter++;
    }





    // Definition of u(l) = 0/1
    for ( int l = 0; l < inst->L; l++) {

        sprintf(cname[0], "u(%i)", l+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = powerLevelCost(l+1,inst);     // cost

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status ) {

            print_error("Wrong CPXnewcols on u variables");
        }

        uVector[l] = counter;
        //printf("Posizione variabile u(%i): %i \n ", l+1, uVector[l]);
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
            double ub = P;
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
    printf("CONSTRAINT n 1: CREATED \n");






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
    printf("Constraint HARD OK \n");





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
    printf("Constraint n 4 OK \n");





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
    printf("Constraint OK \n");







    /* Constraint number 2: example, variable P(t)
     * The power at any instant shuold be less then the amount contracted
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(1)
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(2)
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(3)
    */

    int c2_rmatbeg[2] = {0, inst->L};
    double *c2_rmtval = calloc( inst->L + 1, sizeof (double));
    int *c2_rmatind = calloc( inst->L + 1, sizeof (int));

    for (int l = 0; l < inst->L; ++l) {
        c2_rmatind[l] = uVector[l];
        c2_rmtval[l] =  inst->table_sm2[l].watt;
    }
    c2_rmtval[inst->L] = -1.0;

    for (int t = 0; t < inst->T; ++t) {

        c2_rmatind[inst->L] = pVector[t];

        status = CPXaddrows (env, lp, 0, 1, inst->L + 1,
                             zero, sense_greater, c2_rmatbeg, c2_rmatind, c2_rmtval ,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            //free(c2_rmatind);
            //free(c2_rmtval);
        }
    }

    printf("Constraint n 2 OK \n");









    /* Constraint number 9: example, variable u(l)
     *
     * u(1) + u(2) + u(3) + u(4) + u(5) + u(6) + u(7) + u(8) + u(9)  = 1
     */

    double  *c9_rmatval = calloc( inst->L, sizeof( double ));
    for (int i = 0; i < inst->L; ++i) {
        c9_rmatval[i] = 1.0;
    }
    int  c9_rmatbeg[2] = {0, inst->L};

    status = CPXaddrows (env, lp, 0, 1, inst->L,
                         one, sense_equal, c9_rmatbeg, uVector, c9_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
        //goto TERMINATE;
    } else {
        free(c9_rmatval);
    }
    printf("Constraint n 9 OK \n");




    CPXwriteprob(env, lp, "model.lp", NULL );




    free(cname[0]);
    free(cname);
    free(pVector);
    free(uVector);

}

