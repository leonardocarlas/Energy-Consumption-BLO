#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"


int model_m2ev(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *sh2vVector, int *sv2hVector,
                         int *Ph2vVector, int *Pv2hVector, int *EvVector) {

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


    // Definition of sh2v(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "sh2v(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on sh2v");

        sh2vVector[t] = counter;
        counter++;
        //printf("Posizione variabile sh2v(%d): %d \n",t+1 ,sh2vVector[t]);
    }

    // Definition of sv2h(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "sv2h(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on sv2h");

        sv2hVector[t] = counter;
        counter++;
        //printf("Posizione variabile sv2h(%d): %d \n",t+1 ,sv2hVector[t] );
    }


    // Definition of Pv2h(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "Pv2h(%i)", t+1);
        double lb;
        double ub;
        if ( t >= (inst->table_sm13.time_arrival - 1) && t < inst->table_sm13.time_departure ) {
            lb = 0.0;
            ub = CPX_INFBOUND;
        } else {
            lb = 0.0;
            ub = 0.0;
        }
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on Pv2h(t) variables.\n");

        Pv2hVector[t] = counter;
        //printf("Posizione variabile Pv2h(%i): %i \n ", t+1, Pv2hVector[t]);
        counter++;
    }

    // Definition of Ph2v(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "Ph2v(%i)", t+1);
        double lb;
        double ub;
        if ( t >= (inst->table_sm13.time_arrival - 1) && t < inst->table_sm13.time_departure ) {
            lb = 0.0;
            ub = CPX_INFBOUND;
        } else {
            lb = 0.0;
            ub = 0.0;
        }
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on Pv2h(t) variables.\n");
        Ph2vVector[t] = counter;
        //printf("Posizione variabile Ph2v(%i): %i \n ", t+1, Ph2vVector[t]);
        counter++;
    }

    // Definition of VehicleE(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "VehicleE(%i)", t+1);
        double lb; double ub;
        if (t == inst->table_sm13.time_arrival - 1 ) {
            lb = inst->table_sm13.ev_initial_battery_charge;
            ub = inst->table_sm13.ev_initial_battery_charge;
        }
        else if ( t >= inst->table_sm13.time_arrival && t < inst->table_sm13.time_departure) {
            lb = inst->table_sm13.ev_minimum_charge;
            ub = inst->table_sm13.ev_maximum_charge;
        } else {
            lb = 0.0;
            ub = 0.0;
        }
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            fprintf (stderr,"CPXnewcols failed on Ev(t) variables.\n");
        EvVector[t] = counter;
        //printf("Posizione variabile Ev(%i): %i \n ", t+1, EvVector[t]);
        counter++;
    }


    /** Constraint number 1: example, variable Ev
     *  Ev(t) - Ev(t-1) - nch * Ph2v(t) + (1/ndch) Pv2h(t) = 0
     */

    for (int t = inst->table_sm13.time_arrival ; t < inst->table_sm13.time_departure; ++t) {

        int *c1_rmatind = calloc( 4 , sizeof (int) );
        double *c1_rmatval = calloc( 4 , sizeof (double) );
        int *c1_rmatbeg = calloc( 2 , sizeof (int) );

        c1_rmatbeg[0] = 0;
        c1_rmatbeg[1] = 3;

        c1_rmatind[0] = EvVector[t];
        c1_rmatind[1] = EvVector[t-1];
        c1_rmatind[2] = Ph2vVector[t];
        c1_rmatind[3] = Pv2hVector[t];

        c1_rmatval[0] = 1.0;
        c1_rmatval[1] = -1.0;
        c1_rmatval[2] = -1.0 * inst->table_sm13.ev_charging_efficiency;
        c1_rmatval[3] = ( 1/inst->table_sm13.ev_discharging_efficiency );

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
    printf("CONSTRAINT n 1: CREATED \n");


    /** Constraint number 2: example, variable Ev
    *  Ev(td) >= Ev_required
    */
    int *c2_rmatind = calloc( 1 , sizeof (int) );
    double *c2_rmatval = calloc( 1 , sizeof (double) );
    int *c2_rmatbeg = calloc( 2, sizeof (int) );
    double *c2_rhs = calloc( 1, sizeof (int) );

    c2_rhs[0] = inst->table_sm13.requested_charge;
    c2_rmatbeg[0] = 0;
    c2_rmatbeg[1] = 0;
    c2_rmatind[0] = EvVector[inst->table_sm13.time_departure-1];
    c2_rmatval[0] = 1.0;

    status = CPXaddrows (env, lp, 0, 1, 1,
                         c2_rhs, sense_greater, c2_rmatbeg, c2_rmatind, c2_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
    } else {
        free(c2_rmatind);
        free(c2_rmatval);
        free(c2_rmatbeg);
    }
    printf("CONSTRAINT n 2: CREATED \n");

    /** Constraint number 3: example, variable Ev
     *  Ph2v(t) - Pmaxcharge * sh2v(t) <= 0
     */

    for (int t = inst->table_sm13.time_arrival ; t < inst->table_sm13.time_departure; ++t) {

        int *c3_rmatind = calloc( 2 , sizeof (int) );
        double *c3_rmatval = calloc( 2 , sizeof (double) );
        int *c3_rmatbeg = calloc( 2 , sizeof (int) );

        c3_rmatbeg[0] = 0;
        c3_rmatbeg[1] = 1;
        c3_rmatind[0] = Ph2vVector[t];
        c3_rmatind[1] = sh2vVector[t];
        c3_rmatval[0] = 1.0;
        c3_rmatval[1] = -1.0 * inst->table_sm13.ev_maximum_charge_power;

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
    printf("CONSTRAINT EV n 3: CREATED \n");

    /** Constraint number 4: example, variable Ev
     *  Pv2h(t) - Pmaxdischarge * sv2h(t) <= 0
     */
    for (int t = inst->table_sm13.time_arrival ; t < inst->table_sm13.time_departure; ++t) {

        int *c4_rmatind = calloc( 2 , sizeof (int) );
        double *c4_rmatval = calloc( 2 , sizeof (double) );
        int *c4_rmatbeg = calloc( 2 , sizeof (int) );

        c4_rmatbeg[0] = 0;
        c4_rmatbeg[1] = 1;
        c4_rmatind[0] = Pv2hVector[t];
        c4_rmatind[1] = sv2hVector[t];
        c4_rmatval[0] = 1.0;
        c4_rmatval[1] = -1.0 * inst->table_sm13.ev_maximum_discharge_power;

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
    printf("CONSTRAINT n 4: CREATED \n");

    /** Constraint number 5: example, variable Ev
     *  sh2v(t) + sv2h(t) <= 1
     */
    for (int t = inst->table_sm13.time_arrival ; t < inst->table_sm13.time_departure; ++t) {

        int *c5_rmatind = calloc( 2 , sizeof (int) );
        double *c5_rmatval = calloc( 2 , sizeof (double) );
        int *c5_rmatbeg = calloc( 2 , sizeof (int) );

        c5_rmatbeg[0] = 0;
        c5_rmatbeg[1] = 1;
        c5_rmatind[0] = sh2vVector[t];
        c5_rmatind[1] = sv2hVector[t];
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
    printf("CONSTRAINT n 5: CREATED \n");






    return counter;
}