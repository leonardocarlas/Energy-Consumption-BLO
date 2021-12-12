#include <ilcplex/cplex.h>
#include "ll.h"
#include "utils.h"

/*
 * Method to define problem variables and constraints
 */
void model_m1a(instance *inst, CPXENVptr env, CPXLPptr lp) {

    char binary = 'B';
    char integer = 'I';
    char sense_greater[1] = {'G'};
    char sense_less[1] = {'L'};
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



    int (*wMatrix)[inst->MAX_DJ][inst->T];
    wMatrix = calloc( inst->J, sizeof *wMatrix);

    // Declaration of structure for variable P(t)
    int *pVector = calloc( inst->T, sizeof (int) );
    // Declaration of structure for variable u(l)
    int *uVector = calloc(inst->L, sizeof (int) );
    // Declaration of structure for variable P(j,t)
    //int pMatrix[inst->J][inst->T];
    int (*pMatrix)[inst->T];
    pMatrix =  calloc( inst->J, sizeof *pMatrix);




    // Definition of w(j,r,t) = 0/1

    for (int j = 0; j < inst->J; ++j) {
        for (int r = 0; r < inst->MAX_DJ; ++r) {
            for (int t = 0; t < inst->T; ++t) {

                if ( r < inst->table_sm20[j].duration && t >= (inst->table_sm5[j].start_interval - 1) && t < inst->table_sm5[j].end_inteval)
                {
                    // I valori sono validi
                    sprintf(cname[0], "w(%d,%d,%d)", j+1, r+1, t+1);
                    double lb = 0.0;
                    double ub = 1.0;
                    double obj = 0.0;

                    status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
                    if (status) print_error("Error CPXnewcols on w(j,r,t)");

                    wMatrix[j][r][t] = counter;
                    counter++;
                    //printf("Posizione variabile w(%d,%d,%d): %d \n", j+1, r+1, t+1, wMatrix[j][r][t]);
                } else {
                    wMatrix[j][r][t] = -1;
                }
            }
        }
    }


    // Definition of P(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "P(%i)", t+1);
        double lb = 0.0;
        double ub = 10000.0;
        double obj = powerCostTimet(t+1, inst);        // cost

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname);
        if ( status ){
            fprintf (stderr,"CPXnewcols failed on P(t) variables.\n");
        }

        pVector[t] = counter;
        //printf("Posizione variabile P(%i): %i \n ", t+1, pVector[t]);
        counter++;
    }





    // Definition of u(l) = 0/1
    for (int l = 0; l < inst->L; l++) {

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





    // Definition of P(j,t) = contiguos
    int (*c4_rmatind)[1];
    c4_rmatind = calloc(inst->J * inst->T, sizeof (*c4_rmatind));
    //int c4_rmatind[(inst->J * inst->T)][1];
    int ptj_rmatind[(inst->T)][inst->J];
    k = 0;

    for (int j = 0; j < inst->J; j++) {
        for (int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "P(%d,%d)", j+1, t+1);
            double lb = 0.0;
            double ub = 10000.0;
            double obj = 0.0;

            //Creates the columns of the new variable
            if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname))
                print_error("Wrong CPXnewcols on x var.s");

            c4_rmatind[k][0] = pMatrix[j][t] = counter;
            ptj_rmatind[t][j] = counter;
            counter++;

            //printf("Posizione variabile P(%i,%i): %i \n", j+1, t+1, pMatrix[j][t]);
            k++;
        }
    }





    /* Constraint 1: example, variable P(t)
     * Power required at time t is calculated as the sum of the power required by the J appliances
     P(1) = P(1,1) + P(2,1) + P(3,1)
     P(2) = P(1,2) + P(2,2) + P(3,2)
    */

    int c1_rmatbeg[2] = {0, inst->J};
    for (int t = 0; t < inst->T ; ++t) {

        // creo un nuovo wMatrix e ci aggiungo P (t) alla fine
        int *c1_rmatind = calloc(inst->J + 1, sizeof(int));
        double *c1_rmatval = calloc(inst->J + 1, sizeof(double));
        for (int i = 0; i < inst->J; ++i) {
            c1_rmatind[i] = ptj_rmatind[t][i];
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
    printf("Constraint n 1 OK \n");



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


    /* Constraint number 3: example P(j,t)
    *
   P(1,1) = f(1,1) * w(1,1,1) + f(1,2) * w(1,2,1) + ... + f(1,90) * w(1,90,1)
   P(1,2) = f(1,1) * w(1,1,2) + f(1,2) * w(1,2,2) + ... + f(1,90) * w(1,90,2)
   ...
   */
    for (int j = 0; j < inst->J; ++j) {
        for (int t = 0; t < inst->T; ++t) {

            int duration = inst->table_sm20[j].duration;
            int *c3_rmatind = calloc(duration +1, sizeof(int));
            double *c3_rmatval = calloc(duration + 1, sizeof(double));

            for (int r = 0; r < duration; ++r) {
                if ( wMatrix[j][r][t] == -1 ) {
                    continue;
                } else {
                    c3_rmatind[r] = wMatrix[j][r][t];
                    c3_rmatval[r] = powerRequiredShiftableStageR(inst, j, r);
                    //printf("ind:  %d   val   %lf \n", c3_rmatind[r], c3_rmatval[r]);
                }
            }
            c3_rmatval[duration] = -1.0;
            c3_rmatind[duration] = pMatrix[j][t];
            int c3_rmatbeg[2] = {0, duration + 1 };


            status = CPXaddrows(env, lp, 0, 1, duration + 1,
                                zero, sense_equal, c3_rmatbeg, c3_rmatind, c3_rmatval,
                                NULL, NULL);
            if (status) {
                fprintf(stderr, "CPXaddrows failed.\n");
            } else {
                free(c3_rmatind);
                free(c3_rmatval);
            }

        }
    }
    printf("Constraint n 3 OK \n");


    /* Constraint number 4: example, variable P(j,t)
     *
     P(2,1) = 0         RIGUARDARE LA CREAZIONE DELL'INDICE
     P(2,2) = 0
     P(2,3) = 0
    */

    int c4_rmatbeg[2] = {0, 1};
    k = 0;
    for (int j = 0; j < inst->J; j++ ) {
        for (int t = 0; t < inst->T; ++t) {

            if( t+1 < inst->table_sm5[j].start_interval || t+1 > inst->table_sm5[j].end_inteval ) {

                status = CPXaddrows (env, lp, 0, 1, 1,
                                     zero, sense_equal, c4_rmatbeg, c4_rmatind[k], one,
                                     NULL, NULL);
                if ( status ) {
                    fprintf(stderr, "CPXaddrows failed.\n");
                }
            }
            k++;
        }
    }
    printf("Constraint n 4 OK \n");



    /* Constraint number 5: example, w(j,r,t)
     * Load j starts its operation cycle in the allowed time slot
     * w(1,1,1) + w(1,1,2) + w(1,1,3) + w(1,1,4) + ... + w(1,1,391) = 1
     * w(2,1,406) + w(2,1,407) + w(2,1,408) + w(2,1,409) + ... + w(2,1,766) = 1
     * w(3,1,1126) + w(3,1,1127) + w(3,1,1128) + w(3,1,1129) + ... + w(3,1,1381) = 1
     */

    for (int j = 0; j < inst->J; ++j) {

        int end = inst->table_sm5[j].end_inteval - inst->table_sm20[j].duration + 1;
        int t_counter = end - inst->table_sm5[j].start_interval;

        int *c5_rmatind = calloc(t_counter + 1 , sizeof(int));
        double *c5_rmatval = calloc(t_counter + 1, sizeof(double ));
        int c5_rmatbeg[2] = {0, k};
        k = 0;

        //printf("Tcounter: %d  end  %d \n", t_counter, end);
        for (int t = (inst->table_sm5[j].start_interval - 1) ; t < end ; ++t) {
            c5_rmatind[k] = wMatrix[j][0][t];
            //printf("TEST c9ind: %i\n", c9_rmatind[k]);
            c5_rmatval[k] = 1.0;
            k++;
        }
        //printf("K: %d\n",k);


        status = CPXaddrows (env, lp, 0, 1, t_counter + 1,
                             one, sense_equal, c5_rmatbeg, c5_rmatind, c5_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c5_rmatind);
            free(c5_rmatval);
        }
    }
    printf("Constraint n 5 OK \n");



    /* Constraint number 6: example, w(j,r,t)
     *  Load j should operate only once in his time slot
      w(1,1,1) + w(1,1,2) + w(1,1,3) + w(1,1,4) + ... + w(1,1,480) = 1
      w(1,2,1) + w(1,2,2) + w(1,2,3) + w(1,2,4) + ... + w(1,2,480)= 1
      ...
    */

    for (int j = 0; j < inst->J; ++j) {
        for (int r = 0; r < inst->table_sm20[j].duration; ++r) {

            int t_counter = inst->table_sm5[j].end_inteval - inst->table_sm5[j].start_interval + 1;
            int *c6_rmatind = calloc(t_counter, sizeof(int));
            double *c6_rmatval = calloc(t_counter, sizeof(double));
            k = 0;
            for (int t = inst->table_sm5[j].start_interval - 1; t < inst->table_sm5[j].end_inteval; ++t) {

                c6_rmatind[k] = wMatrix[j][r][t];
                c6_rmatval[k] = 1.0;
                k++;

            }
            int c6_rmatbeg[2] = {0, t_counter};

            status = CPXaddrows (env, lp, 0, 1, t_counter,
                                 one, sense_equal, c6_rmatbeg, c6_rmatind, c6_rmatval,
                                 NULL, NULL);
            if ( status ) {
                fprintf (stderr,"CPXaddrows failed.\n");
            } else {
                // libero la memoria dei val e ind
                free(c6_rmatind);
                free(c6_rmatval);
            }

        }

    }
    printf("Constraint n 6 OK \n");





    /* Constraint number 7: example, variable w(j,r,t)
     * At each instant t, each Load j is on or off at only one stage r
     * w(1,1,1) + w(1,2,1) + w(1,3,1) + w(1,4,1) + w(1,5,1) + w(1,6,1) + ... + w(1,90,1) <= 1
     * w(1,1,2) + w(1,2,2) + w(1,3,2) + w(1,4,2) + w(1,5,2) + w(1,6,2) + ... + w(1,90,2) <= 1
    */
    for (int j = 0; j < inst->J; ++j) {
        for (int t = inst->table_sm5[j].start_interval - 1; t < inst->table_sm5[j].end_inteval; ++t) {

            int *c7_rmatind = calloc(inst->table_sm20[j].duration, sizeof(int));
            double *c7_rmatval = calloc(inst->table_sm20[j].duration, sizeof(double));
            int c7_rmatbeg[2] = {0, inst->table_sm20[j].duration};
            k = 0;
            for (int r = 0; r < inst->table_sm20[j].duration; ++r) {
                c7_rmatind[k] = wMatrix[j][r][t];
                c7_rmatval[k] = 1.0;
                k++;
            }
            status = CPXaddrows (env, lp, 0, 1, inst->table_sm20[j].duration,
                                 one, sense_less, c7_rmatbeg, c7_rmatind, c7_rmatval,
                                 NULL, NULL);
            if ( status ) {
                fprintf(stderr, "CPXaddrows failed.\n");
            } else {
                free(c7_rmatind);
                free(c7_rmatval);
            }
        }
    }
    printf("Constraint n 7 OK \n");




    /* Constraint number 8: example, variable w(j,r,t)
     * When load j starts at some t, for the next t is on for his entire duration
     * w(1,2,2) - w(1,1,1) > =0
     *  w(1,2,3) - w(1,1,2) > =0
    */

    for (int j = 0; j < inst->J; ++j) {
        for (int r = 0; r < inst->MAX_DJ; ++r) {

            if ( r < inst->table_sm20[j].duration - 1 ) {

                for (int t = ( inst->table_sm5[j].start_interval - 1); t < (inst->table_sm5[j].end_inteval - 1) ; ++t) {

                    if ( wMatrix[j][r][t] == -1  ){
                        continue;
                    } else {
                        int c8_rmatind[2] = {wMatrix[j][r + 1][t + 1], wMatrix[j][r][t] };

                        if ( wMatrix[j][r+1][t+1] == -1 ){
                            printf("j: %d   r+1: %d   t+1: %d\n", j, r+1, t+1);
                        }

                        double c8_rmatval[2] = {1.0, -1.0};
                        int c8_rmatbeg[2] = {0, 1};

                        status = CPXaddrows (env, lp, 0, 1, 2,
                                             zero, sense_greater, c8_rmatbeg, c8_rmatind, c8_rmatval,
                                             NULL, NULL);
                        if ( status ) {
                            fprintf(stderr, "CPXaddrows failed.\n");
                        } else {
                            //free(c8_rmatind);
                            //free(c8_rmatval);
                        }

                    }

                }

            }
        }
    }
    printf("Constraint n 8 OK \n");



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


    free(wMatrix);
    free(cname[0]);
    free(cname);
    free(pVector);
    free(uVector);



}

