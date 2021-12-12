#include <ilcplex/cplex.h>
#include "ll.h"
#include "utils.h"


int model_mul(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *xVector) {

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

    // Definition of x(i) >= 0
    for ( int i = 0; i < inst->nof_subperiods; i++) {

        sprintf(cname[0], "x(%i)", i+1);
        double lb = inst->table_sm1[i].min_price;
        double ub = inst->table_sm1[i].max_price;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status ) print_error("Wrong CPXnewcols on x(t) variables");

        xVector[i] = counter;
        printf("Posizione variabile x(%i): %i \n ", i+1, xVector[i]);
        counter++;
    }

    /** CONSTRAINT NUMBER: variable x(i)
     * 1/T * ampP1 * x(1) + ... 1/T * ampPi * x(i) = X_AVARAGE */

    double *c1_rmatval = calloc(inst->nof_subperiods, sizeof(double));
    int  *c1_rmatbeg = calloc(2, sizeof(int));
    c1_rmatbeg[0] = 0;
    c1_rmatbeg[1] = inst->nof_subperiods;
    double *c1_rhs = calloc(1, sizeof(double));
    c1_rhs[0] = inst->X_AVARAGE;
    for ( int i = 0; i < inst->nof_subperiods; i++) {

       c1_rmatval[i] = (1.0 / inst->T) * (inst->table_sm1[i].end_interval - inst->table_sm1[i].start_interval + 1);
    }
    status = CPXaddrows (env, lp, 0, 1, inst->nof_subperiods,
                         c1_rhs, sense_equal, c1_rmatbeg, xVector, c1_rmatval,
                         NULL, NULL);
    if ( status ) fprintf(stderr, "CPXaddrows failed.\n");
    free(c1_rmatbeg);
    free(c1_rhs);
    free(c1_rmatval);



    return counter;
}
