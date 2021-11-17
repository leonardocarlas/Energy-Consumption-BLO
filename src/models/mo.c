#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"
#include "models/m1.h"


int model_mo(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *pVector, int *vVector) {

    char binary = 'B';
    char integer = 'I';
    char continuos = 'C';
    char sense_greater[1] = {'G'};
    char sense_less[1] = {'L'};
    char sense_equal[1] = {'E'};
    const double zero[1] = {0.0};
    double one[1] = {1.0};
    int status;
    int k = 0;

    //Declaration of structure for variable sg2h(t)
    int *sg2hVector = calloc(inst->T, sizeof(int));
    //Declaration of structure for variable PG2H(t)
    int *PG2HVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable u(l)
    int *uVector = calloc( inst->L, sizeof (int));



    // Definition of sg2h(t) = 0/1
    for ( int t = 0; t < inst->T; t++) {

        sprintf(cname[0], "sg2h(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status )
            print_error("Wrong CPXnewcols on v(t) variables");

        sg2hVector[t] = counter;
        //printf("Posizione variabile sg2h(%i): %i \n ", t+1, sg2hVector[t]);
        counter++;
    }

    // Definition of PG2H(t) >=0
    for ( int t = 0; t < inst->T; t++) {

        sprintf(cname[0], "PG2H(%i)", t+1);
        double lb = 0.0;
        double ub = CPX_INFBOUND;
        double obj = powerCostTimet(t, inst);

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            print_error("Wrong CPXnewcols on PG2H(t) variables");

        PG2HVector[t] = counter;
        //printf("Posizione variabile PG2H(%i): %i \n ", t+1, PG2HVector[t]);
        counter++;
    }

    // Definition of u(l) = 0/1
    for ( int l = 0; l < inst->L; l++) {

        sprintf(cname[0], "u(%i)", l+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = powerLevelCost(l+1,inst);     // cost

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status )
            print_error("Wrong CPXnewcols on u variables");

        uVector[l] = counter;
        //printf("Posizione variabile u(%i): %i \n ", l+1, uVector[l]);
        counter++;
    }


    /**
     * CONSTRAINT NUMBER : example, variable PG2H
     *  PG2H(t) - P(t) - power_required_ewh * v(t) = 0
     */

    for (int t = 0; t < inst->T; ++t) {

        int *c5_rmatbeg = calloc(2, sizeof(int));
        int *c5_rmatind = calloc(3, sizeof(int));
        double *c5_rmatval = calloc(2, sizeof(int));
        c5_rmatbeg[0] = 0;
        c5_rmatbeg[1] = 2;
        c5_rmatval[0] = 1.0;
        c5_rmatval[1] = -1.0;
        c5_rmatval[2] = -1.0 * inst->table_sm7.power_required;
        c5_rmatind[0] = PG2HVector[t];
        c5_rmatind[1] = pVector[t];
        c5_rmatind[2] = vVector[t];
        status = CPXaddrows (env, lp, 0, 1, 3,
                             zero, sense_equal, c5_rmatbeg, c5_rmatind, c5_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c5_rmatbeg);
            free(c5_rmatval);
            free(c5_rmatind);
        }
    }
    printf("CONSTRAINT MO 1 OK");


    /**  CONSTRAINT NUMBER :  example, variable P(t)
     * The power at any instant shuold be less then the amount contracted
     *   poweratL(1) * u(1) + poweratL(2) * u(2) + poweratL(3) * u(3) +  ... + poweratL(9) * u(9)  >= PG2H(1)
     *   poweratL(1) * u(1) + poweratL(2) * u(2) + poweratL(3) * u(3) +  ... + poweratL(9) * u(9)  >= PG2H(2)
     *   poweratL(1) * u(1) + poweratL(2) * u(2) + poweratL(3) * u(3) +  ... + poweratL(9) * u(9)  >= PG2H(3)
    */

    int *c2_rmatbeg = calloc(2, sizeof(int)); c2_rmatbeg[0] = 0; c2_rmatbeg[1] = inst->L;
    double *c2_rmtval = calloc( inst->L + 1, sizeof (double));
    int *c2_rmatind = calloc( inst->L + 1, sizeof (int));

    for (int l = 0; l < inst->L; ++l) {

        c2_rmatind[l] = uVector[l];
        c2_rmtval[l] =  inst->table_sm2[l].watt;
    }
    c2_rmtval[inst->L] = -1.0;

    for (int t = 0; t < inst->T; ++t) {

        c2_rmatind[inst->L] = PG2HVector[t];
        status = CPXaddrows (env, lp, 0, 1, inst->L + 1,
                             zero, sense_greater, c2_rmatbeg, c2_rmatind, c2_rmtval ,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
    }
    printf("CONSTRAINT MO 2 OK\n");
    free(c2_rmatind);
    free(c2_rmtval);




    /* Constraint number 9: example, variable u(l)
     *
     * u(1) + u(2) + u(3) + u(4) + u(5) + u(6) + u(7) + u(8) + u(9)  = 1
     */

    double  *c9_rmatval = calloc( inst->L, sizeof( double ));
    int  c9_rmatbeg[2] = {0, inst->L};

    for (int i = 0; i < inst->L; ++i) {
        c9_rmatval[i] = 1.0;
    }

    status = CPXaddrows (env, lp, 0, 1, inst->L,
                         one, sense_equal, c9_rmatbeg, uVector, c9_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
    } else {
        free(c9_rmatval);
    }
    printf("CONSTRAINT MO 3 OK\n");



    /**
 * CONSTRAINT NUMBER : example, variable PG2H
 *  PG2H(t) - MAX_PG * sg2h(t) <= 0
 */
/*
    for (int t = 0; t < inst->T; ++t) {

        int *c1_rmatbeg = calloc(2, sizeof(int));
        int *c1_rmatind = calloc(2, sizeof(int));
        double *c1_rmatval = calloc(2, sizeof(int));

        c1_rmatbeg[0] = 0;
        c1_rmatbeg[1] = 1;
        c1_rmatind[0] = PG2HVector[t];
        c1_rmatind[1] = sg2hVector[t];
        c1_rmatval[0] = 1.0;
        c1_rmatval[1] = -1 * inst->MAX_PG;

        status = CPXaddrows (env, lp, 0, 1, inst->L + 1,
                             zero, sense_greater, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else { free(c1_rmatbeg); free(c1_rmatval); free(c1_rmatind); }
    }
*/

    free(uVector);
    free(PG2HVector);
    free(sg2hVector);

    return counter;
}