#include <ilcplex/cplex.h>
#include "ll.h"
#include "utils.h"


int model_m3(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *sACVector,
             int* yVector, int* zVector, int* inTempVector) {

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


    // Definition of sAC(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "sAC(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on sAC(t)");
        sACVector[t] = counter;
        counter++;
        printf("Posizione variabile sh2v(%d): %d \n", t+1 , sACVector[t]);
    }

    // Definition of y(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "y(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on y(t)");
        yVector[t] = counter;
        counter++;
        printf("Posizione variabile y(%d): %d \n", t+1 , yVector[t]);
    }

    // Definition of z(t) = 0/1
    for (int t = 0; t < inst->T; ++t) {

        sprintf(cname[0], "z(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if (status) print_error("Error CPXnewcols on z(t)");
        zVector[t] = counter;
        counter++;
        printf("Posizione variabile z(%d): %d \n", t+1 , zVector[t]);
    }

    // Definition of inTemp(t) = continuos
    for (int t = 0; t < inst->T; ++t) {
        sprintf(cname[0], "inTemp(%i)", t+1);
        double lb;
        double ub;
        if (t == 0) {
            lb = inst->table_sm10.initial_indoor_temperature;
            ub = inst->table_sm10.initial_indoor_temperature;
        } else {
            lb = 0.0;
            ub = CPX_INFBOUND;
        }
        double obj = 0.0;
        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if (status) print_error("Error CPXnewcols on inTemp(t)");
        inTempVector[t] = counter;
        counter++;
        printf("Posizione variabile inTempVector(%d): %d \n", t+1 , inTempVector[t]);

    }

    /** CONSTRAINT N 1, variable inTemp(t)
     *  inTemp(t) - (1-B) * inTemp(t-1)  - gamma * PAC * sAC(t-1) = B extTemp(t-1)
     *  inTemp(1) lo calcolo separatamente
    */
    for (int t = 1; t < inst->T; ++t) {

        int *c1_rmatind = calloc(4, sizeof(int));
        double *c1_rmatval = calloc(4, sizeof(double ));
        int *c1_rmatbeg = calloc(2, sizeof(int));
        double *c1_rhs = calloc(1, sizeof(double ));

        c1_rhs[0] = inst->table_sm10.beta * externalTemperatureAtTimet(inst, t-1);
        c1_rmatbeg[0] = 0;
        c1_rmatbeg[1] = 2;
        c1_rmatind[0] = inTempVector[t];
        c1_rmatind[1] = inTempVector[t-1];
        c1_rmatind[2] = sACVector[t-1];
        c1_rmatval[0] = 1.0;
        c1_rmatval[1] = -1 *(1 - inst->table_sm10.beta);
        c1_rmatval[2] = -1 * inst->table_sm10.gamma * inst->table_sm10.nominal_power_AC;

        status = CPXaddrows (env, lp, 0, 1, 3,
                             c1_rhs, sense_equal, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c1_rmatbeg);
            free(c1_rmatval);
            free(c1_rmatind);
        }
    }

    /** CONSTRAINT N 2, variable inTemp(t)
     *  inTemp(t) + M * sAC(t) >= min_indoor_temp
    */
    int M = 100;
    for (int t = 0; t < inst->T; ++t) {

        int *c2_rmatind = calloc(2, sizeof(int));
        double *c2_rmatval = calloc(2, sizeof(double ));
        int *c2_rmatbeg = calloc(2, sizeof(int));
        double *c2_rhs = calloc(1, sizeof(double ));

        c2_rhs[0] = inst->table_sm10.minimum_temperature_allowed;
        c2_rmatbeg[0] = 0;
        c2_rmatbeg[1] = 1;
        c2_rmatind[0] = inTempVector[t];
        c2_rmatind[1] = sACVector[t];
        c2_rmatval[0] = 1.0;
        c2_rmatval[1] = M;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c2_rhs, sense_greater, c2_rmatbeg, c2_rmatind, c2_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c2_rmatbeg);
            free(c2_rmatval);
            free(c2_rmatind);
            free(c2_rhs);
        }
    }

    /** CONSTRAINT N 3, variable inTemp(t)
     *  inTemp(t) - M * z(t) <= min_indoor_temp
    */
    for (int t = 0; t < inst->T; ++t) {

        int *c3_rmatind = calloc(2, sizeof(int));
        double *c3_rmatval = calloc(2, sizeof(double ));
        int *c3_rmatbeg = calloc(2, sizeof(int));
        double *c3_rhs = calloc(1, sizeof(double ));

        c3_rhs[0] = inst->table_sm10.minimum_temperature_allowed;
        c3_rmatbeg[0] = 0;
        c3_rmatbeg[1] = 1;
        c3_rmatind[0] = inTempVector[t];
        c3_rmatind[1] = zVector[t];
        c3_rmatval[0] = 1.0;
        c3_rmatval[1] = -1* M;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c3_rhs, sense_less, c3_rmatbeg, c3_rmatind, c3_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c3_rmatbeg);
            free(c3_rmatval);
            free(c3_rmatind);
            free(c3_rhs);
        }
    }

    /** CONSTRAINT N 4, variable inTemp(t)
     *  inTemp(t) + M * y(t) >= max_indoor_temp
    */
    for (int t = 0; t < inst->T; ++t) {

        int *c4_rmatind = calloc(2, sizeof(int));
        double *c4_rmatval = calloc(2, sizeof(double ));
        int *c4_rmatbeg = calloc(2, sizeof(int));
        double *c4_rhs = calloc(1, sizeof(double ));

        c4_rhs[0] = inst->table_sm10.maximum_temperature_allowed;
        c4_rmatbeg[0] = 0;
        c4_rmatbeg[1] = 1;
        c4_rmatind[0] = inTempVector[t];
        c4_rmatind[1] = yVector[t];
        c4_rmatval[0] = 1.0;
        c4_rmatval[1] = M;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c4_rhs, sense_greater, c4_rmatbeg, c4_rmatind, c4_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c4_rmatbeg);
            free(c4_rmatval);
            free(c4_rmatind);
            free(c4_rhs);
        }
    }

    /** CONSTRAINT N 5 : variable sAC(t)
     * z(t) + y(t) - sAC(t-1) + sAC(t) <= 2
     */
    for (int t = 1; t < inst->T; ++t) {

        int *c5_rmatind = calloc(4, sizeof(int));
        double *c5_rmatval = calloc(4, sizeof(double ));
        int *c5_rmatbeg = calloc(2, sizeof(int));
        double *c5_rhs = calloc(1, sizeof(double ));

        c5_rmatbeg[0] = 0;
        c5_rmatbeg[1] = 3;
        c5_rhs[0] = 2.0;
        c5_rmatind[0] = zVector[t];
        c5_rmatind[1] = yVector[t];
        c5_rmatind[2] = sACVector[t-1];
        c5_rmatind[3] = sACVector[t];
        c5_rmatval[0] = 1.0;
        c5_rmatval[1] = 1.0;
        c5_rmatval[2] = -1.0;
        c5_rmatval[3] = 1.0;

        status = CPXaddrows (env, lp, 0, 1, 4,
                             c5_rhs, sense_less, c5_rmatbeg, c5_rmatind, c5_rmatval,
                             NULL, NULL);
        if ( status ) fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c5_rmatbeg);
            free(c5_rmatval);
            free(c5_rmatind);
            free(c5_rhs);
        }
    }

    /** CONSTRAINT N 6 : variable sAC(t)
     * z(t) + y(t) + sAC(t-1) - sAC(t) <= 2
     */
    for (int t = 1; t < inst->T; ++t) {

        int *c6_rmatind = calloc(4, sizeof(int));
        double *c6_rmatval = calloc(4, sizeof(double ));
        int *c6_rmatbeg = calloc(2, sizeof(int));
        double *c6_rhs = calloc(1, sizeof(double ));

        c6_rmatbeg[0] = 0;
        c6_rmatbeg[1] = 3;
        c6_rhs[0] = 2.0;
        c6_rmatind[0] = zVector[t];
        c6_rmatind[1] = yVector[t];
        c6_rmatind[2] = sACVector[t-1];
        c6_rmatind[3] = sACVector[t];
        c6_rmatval[0] = 1.0;
        c6_rmatval[1] = 1.0;
        c6_rmatval[2] = 1.0;
        c6_rmatval[3] = -1.0;

        status = CPXaddrows (env, lp, 0, 1, 4,
                             c6_rhs, sense_less, c6_rmatbeg, c6_rmatind, c6_rmatval,
                             NULL, NULL);
        if ( status ) fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c6_rmatbeg);
            free(c6_rmatval);
            free(c6_rmatind);
            free(c6_rhs);
        }
    }

    /** CONSTRAINT N 7, variable inTemp(t)
     *  inTemp(t) + M * sAC(t) <= max_indoor_temp + M
    */
    for (int t = 0; t < inst->T; ++t) {

        int *c4_rmatind = calloc(2, sizeof(int));
        double *c4_rmatval = calloc(2, sizeof(double ));
        int *c4_rmatbeg = calloc(2, sizeof(int));
        double *c4_rhs = calloc(1, sizeof(double ));

        c4_rhs[0] = inst->table_sm10.maximum_temperature_allowed + M;
        c4_rmatbeg[0] = 0;
        c4_rmatbeg[1] = 1;
        c4_rmatind[0] = inTempVector[t];
        c4_rmatind[1] = sACVector[t];
        c4_rmatval[0] = 1.0;
        c4_rmatval[1] = M;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c4_rhs, sense_less, c4_rmatbeg, c4_rmatind, c4_rmatval,
                             NULL, NULL);
        if ( status )
            fprintf(stderr, "CPXaddrows failed.\n");
        else {
            free(c4_rmatbeg);
            free(c4_rmatval);
            free(c4_rmatind);
            free(c4_rhs);
        }
    }




    return counter;
}

