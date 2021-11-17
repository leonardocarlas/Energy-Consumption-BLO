#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"

int model_m2sb(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname,
               int *sh2bVector, int *sb2hVector, int *Ph2bVector, int *Pb2hVector, int *BatteryEVector) {

    char continuos = 'C';
    char binary = 'B';
    char integer = 'I';
    char sense_greater[1] = {'G'};
    char sense_equal[1] = {'E'};
    char sense_less[1] = {'L'};
    const double zero[1] = {0.0};
    double  one[1] = {1.0};
    int status;
    int k = 0;


    // Definition of sh2b(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "sh2b(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on sh2b");

        sh2bVector[t] = counter;
        counter++;
        printf("Posizione variabile sh2b(%d): %d \n",t+1 ,sh2bVector[t]);
    }

    // Definition of sb2h(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "sb2h(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on sb2h");

        sb2hVector[t] = counter;
        counter++;
        printf("Posizione variabile sb2h(%d): %d \n",t+1 ,sb2hVector[t] );
    }


    // Definition of Pb2h(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "Pb2h(%i)", t+1);
        double lb = 0.0;
        double ub = CPX_INFBOUND;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on Pb2h(t) variables.\n");

        Pb2hVector[t] = counter;
        printf("Posizione variabile Pb2h(%i): %i \n ", t+1, Pb2hVector[t]);
        counter++;
    }

    // Definition of Ph2b(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "Ph2b(%i)", t+1);
        double lb = 0.0;
        double ub = CPX_INFBOUND;
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on Pv2h(t) variables.\n");
        Ph2bVector[t] = counter;
        printf("Posizione variabile Ph2v(%i): %i \n ", t+1, Ph2bVector[t]);
        counter++;
    }

    // Definition of BatteryE(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "BatteryE(%i)", t+1);
        double lb = inst->table_sm12.sb_minimum_charge;
        double ub = inst->table_sm12.sb_maximum_charge;

        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on BatteryE(t) variables.\n");
        BatteryEVector[t] = counter;
        printf("Posizione variabile BatteryE(%i): %i \n ", t+1, BatteryEVector[t]);
        counter++;
    }


    /** Constraint number 1: example, variable Ev
     *  Eb(t) - Eb(t-1) - nch * Ph2b(t) + (1/ndch) Pb2h(t) = 0
     */

    for (int t = 0 ; t < inst->T; ++t) {

        int *c1_rmatind = calloc( 4 , sizeof (int) );
        double *c1_rmatval = calloc( 4 , sizeof (double) );
        int *c1_rmatbeg = calloc( 2 , sizeof (int) );

        c1_rmatbeg[0] = 0;
        c1_rmatbeg[1] = 3;

        c1_rmatind[0] = BatteryEVector[t];
        c1_rmatind[1] = BatteryEVector[t-1];
        c1_rmatind[2] = Ph2bVector[t];
        c1_rmatind[3] = Pb2hVector[t];

        c1_rmatval[0] = 1.0;
        c1_rmatval[1] = -1.0;
        c1_rmatval[2] = -1.0 * inst->table_sm12.sb_charging_efficiency;
        c1_rmatval[3] = ( 1 / inst->table_sm12.sb_discharging_efficiency) * Pb2hVector[t];

        status = CPXaddrows (env, lp, 0, 1, 4,
                             zero, sense_equal, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c1_rmatind);
            free(c1_rmatval);
            free(c1_rmatbeg);
        }
    }
    printf("CONSTRAINT SB n 1: CREATED \n");


    /** Constraint number 3: example, variable Ev
     *  Ph2b(t) - Pmaxcharge * sh2b(t) <= 0
     */

    for (int t = 0; t < inst->T; ++t) {

        int *c3_rmatind = calloc( 2 , sizeof (int) );
        double *c3_rmatval = calloc( 2 , sizeof (double) );
        int *c3_rmatbeg = calloc( 2 , sizeof (int) );

        c3_rmatbeg[0] = 0;
        c3_rmatbeg[1] = 1;
        c3_rmatind[0] = Ph2bVector[t];
        c3_rmatind[1] = sh2bVector[t];
        c3_rmatval[0] = 1.0;
        c3_rmatval[1] = -1.0 * inst->table_sm12.sb_maximum_charge_power;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             zero, sense_less, c3_rmatbeg, c3_rmatind, c3_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c3_rmatind);
            free(c3_rmatval);
            free(c3_rmatbeg);
        }
    }
    printf("CONSTRAINT SB n 3: CREATED \n");

    /** Constraint number 4: example, variable Ev
     *  Pb2h(t) - Pmaxdischarge * sb2h(t) <= 0
     */
    for (int t = 0 ; t < inst->T; ++t) {

        int *c4_rmatind = calloc( 2 , sizeof (int) );
        double *c4_rmatval = calloc( 2 , sizeof (double) );
        int *c4_rmatbeg = calloc( 2 , sizeof (int) );

        c4_rmatbeg[0] = 0;
        c4_rmatbeg[1] = 1;
        c4_rmatind[0] = Pb2hVector[t];
        c4_rmatind[1] = sb2hVector[t];
        c4_rmatval[0] = 1.0;
        c4_rmatval[1] = -1.0 * inst->table_sm12.sb_maximum_discharge_power;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             zero, sense_less, c4_rmatbeg, c4_rmatind, c4_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c4_rmatind);
            free(c4_rmatval);
            free(c4_rmatbeg);
        }
    }
    printf("CONSTRAINT SB n 4: CREATED \n");

    /** Constraint number 5: example, variable Ev
     *  sh2b(t) + sb2h(t) <= 1
     */
    for (int t = inst->table_sm13.time_arrival ; t < inst->table_sm13.time_departure; ++t) {

        int *c5_rmatind = calloc( 2 , sizeof (int) );
        double *c5_rmatval = calloc( 2 , sizeof (double) );
        int *c5_rmatbeg = calloc( 2 , sizeof (int) );

        c5_rmatbeg[0] = 0;
        c5_rmatbeg[1] = 1;
        c5_rmatind[0] = sh2bVector[t];
        c5_rmatind[1] = sb2hVector[t];
        c5_rmatval[0] = 1.0;
        c5_rmatval[1] = 1.0;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             one, sense_less, c5_rmatbeg, c5_rmatind, c5_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf (stderr,"CPXaddrows failed.\n");
        } else {
            free(c5_rmatind);
            free(c5_rmatval);
            free(c5_rmatbeg);
        }
    }
    printf("CONSTRAINT SB n 5: CREATED \n");

    /** Constraint number 2: example, variable Eb
    *  Eb(T) >= initial battery charge
    */
    int *c2_rmatind = calloc( 1 , sizeof (int) );
    double *c2_rmatval = calloc( 1 , sizeof (double) );
    int *c2_rmatbeg = calloc( 2, sizeof (int) );
    int *c2_rhs = calloc( 1, sizeof (int) );

    c2_rhs[0] = inst->table_sm12.sb_initial_battery_charge;
    c2_rmatbeg[0] = 0;
    c2_rmatbeg[1] = 0;
    c2_rmatind[0] = BatteryEVector[inst->T - 1];
    c2_rmatval[0] = 1.0;

    status = CPXaddrows (env, lp, 0, 1, 1,
                         zero, sense_greater, c2_rmatbeg, c2_rmatind, c2_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
    } else {
        free(c2_rmatind);
        free(c2_rmatval);
        free(c2_rmatbeg);
    }
    printf("CONSTRAINT SB n 2: CREATED \n");




    return counter;

}