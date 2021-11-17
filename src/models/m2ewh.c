#include <ilcplex/cplex.h>
#include "blo.h"
#include "utils.h"




int model_m2ewh(instance *inst, CPXENVptr env, CPXLPptr lp, int counter, char **cname, int *vVector,
            int *nVector, int *PlossVector, int *tempVector) {

    char binary = 'B';
    char integer = 'I';
    char continuos = 'C';
    char sense_greater[1] = {'G'};
    char sense_equal[1] = {'E'};
    char sense_less[1] = {'L'};
    const double zero[1] = {0.0};
    double  one[1] = {1.0};
    int status;
    int k = 0;




    // Definition of v(t) = 0/1
    for ( int t = 0; t < inst->T; t++) {

        sprintf(cname[0], "v(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;
        //double obj = ( (inst->table_sm7.power_required / 1000 )* powerCostTimet(t, inst));    //dev'essere espressa in kW

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status )
            print_error("Wrong CPXnewcols on v(t) variables");


        vVector[t] = counter;
        //printf("Posizione variabile v(%i): %i \n ", t+1, vVector[t]);
        counter++;
    }

    // Definition of n(t) = 0/1
    for ( int t = 0; t < inst->T; t++) {

        sprintf(cname[0], "n(%i)", t+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status )
            print_error("Wrong CPXnewcols on n(t) variables");

        nVector[t] = counter;
        //printf("Posizione variabile n(%i): %i \n ", t+1, nVector[t]);
        counter++;
    }


    // Definition of temp(t) >= 0
    for ( int t = 0; t < inst->T; t++) {

        if (t == 0) {
            sprintf(cname[0], "temp(%i)", t+1);
            double lb = 55.0;
            double ub = 55.0;
            double obj = 0.0;

            status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
            if ( status )
                print_error("Wrong CPXnewcols on temp(t) variables");

            tempVector[t] = counter;
            //printf("Posizione variabile temp(%i): %i \n ", t+1, tempVector[t]);
            counter++;
        }
        else {
            sprintf(cname[0], "temp(%i)", t+1);
            double lb = 0.0;
            double ub = CPX_INFBOUND;
            double obj = 0.0;

            status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
            if ( status )
                print_error("Wrong CPXnewcols on temp(t) variables");

            tempVector[t] = counter;
            //printf("Posizione variabile temp(%i): %i \n ", t+1, tempVector[t]);
            counter++;
        }

    }

    // Definition of Ploss(t) >= 0
    for ( int t = 0; t < inst->T; t++) {

        sprintf(cname[0], "Ploss(%i)", t+1);
        double lb = 0.0;
        double ub = CPX_INFBOUND;
        double obj = 0.0;

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &continuos, cname);
        if ( status )
            print_error("Wrong CPXnewcols on Ploss(t) variables");

        PlossVector[t] = counter;
        //printf("Posizione variabile Ploss(%i): %i \n ", t+1, PlossVector[t]);
        counter++;
    }





    /* Constraint n 1, variable Ploss(t)
     * Calculate the loss of Power due to the difference of temperature for every t
     * Ploss(1) - AU * temp(1) = AU * at(1)
     * Ploss(2) - AU * temp(2) = AU * at(2)
     * Ploss(3) - AU * temp(3) = AU * at(3)
     * ...
     */

    for (int t = 0; t < inst->T; ++t) {

        int *c1_rmatind = calloc(2, sizeof (int));
        double *c1_rmatval = calloc(2, sizeof (double ));
        int *c1_rmatbeg = calloc(2, sizeof (int ));
        double *c1_rhs = calloc(1, sizeof (double ));

        c1_rmatbeg[0] = 0;
        c1_rmatbeg[1] = 2;
        c1_rmatval[0] = 1.0 ;
        c1_rmatval[1] = -1.0 * inst->table_sm7.AU;
        c1_rmatind[0] = PlossVector[t] ;
        c1_rmatind[1] = tempVector[t];
        c1_rhs[0] = -1 * inst->table_sm7.AU * ambientTemperatureAtTimet(t, inst);

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c1_rhs, sense_equal, c1_rmatbeg, c1_rmatind, c1_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c1_rmatval);
            free(c1_rmatind);
            free(c1_rmatbeg);
            free(c1_rhs);
        }

    }
    printf("Constraint n 1 OK \n");


    /* Constraint n 2, variable temp(t)
     * Calculate the for each time t the temperature temp(t)
     * * // t(0) = 55
     *   coeff temp(t) + coeff v(t) - coeff Ploss(t) - temp(t+1) = - noto
     *
     */     // manca come calcolare la temp(1) = temp(0)...


    for (int t = 0; t < inst->T-1; ++t) {

        int *c2_rmatind = calloc(4, sizeof (int));
        double *c2_rmatval = calloc(4, sizeof (double ));
        int *c2_rmatbeg = calloc(2, sizeof (int ));
        double *c2_rhs = calloc(1, sizeof (double ));

        c2_rmatbeg[0] = 0;
        c2_rmatbeg[1] = 3;
        c2_rmatval[0] = 1.0;
        c2_rmatval[1] = -1 * ( inst->table_sm7.M - waterWithdrawlAtTimet(t,inst) ) / inst->table_sm7.M ;
        c2_rmatval[2] = -1 *( inst->table_sm7.power_required / ( inst->table_sm7.M * inst->table_sm7.termal_coeff));
        c2_rmatval[3] = 1 / ( inst->table_sm7.M * inst->table_sm7.termal_coeff);
        c2_rmatind[0] = tempVector[t+1];
        c2_rmatind[1] = tempVector[t];
        c2_rmatind[2] = vVector[t];
        c2_rmatind[3] = PlossVector[t];
        c2_rhs[0] =  ((waterWithdrawlAtTimet(t, inst) * inst->table_sm7.inlet_temperature) / inst->table_sm7.M) ;

        status = CPXaddrows (env, lp, 0, 1, 4,
                             c2_rhs, sense_equal, c2_rmatbeg, c2_rmatind, c2_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c2_rmatval);
            free(c2_rmatind);
            free(c2_rmatbeg);
            free(c2_rhs);
        }
    }
    printf("Constraint n 2 OK \n");



    /* Constraint n 3, variable temp(t)
     * Imposes the water temperature to be above the minimum temperature for every t
     * M * v (1) + temp(1) >= min_temp
     * M * v (2) + temp(2) >= min_temp
     * M * v (3) + temp(3) >= min_temp
     * ...
     */

    for (int t = 0; t < inst->T; ++t) {

        int *c3_rmatind = calloc(2, sizeof (int));
        double *c3_rmatval = calloc(2, sizeof (double ));
        int *c3_rmatbeg = calloc(2, sizeof (int ));
        double *c3_rhs = calloc(1, sizeof (double ));

        c3_rmatbeg[0] = 0; c3_rmatbeg[1] = 2;
        c3_rmatval[0] = inst->table_sm7.M; c3_rmatval[1] = 1.0;
        c3_rmatind[0] = vVector[t]; c3_rmatind[1] = tempVector[t];
        c3_rhs[0] = inst->table_sm7.min_temperature;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c3_rhs, sense_greater, c3_rmatbeg, c3_rmatind, c3_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c3_rmatval);
            free(c3_rmatind);
            free(c3_rmatbeg);
            free(c3_rhs);
        }

    }
    printf("Constraint n 3 OK \n");


    /* Constraint n 4, variable temp(t)
     * Imposes the water temperature to be above the minimum temperature for every t
     * temp(1) + M * v (1)  <= max_temp + M
     * temp(2) + M * v (2)  <= max_temp + M
     * temp(3) + M * v (3)  <= max_temp + M
     * ...
     */

    for (int t = 0; t < inst->T; ++t) {

        int *c4_rmatind = calloc(2, sizeof (int));
        double *c4_rmatval = calloc(2, sizeof (double ));
        int *c4_rmatbeg = calloc(2, sizeof (int ));
        double *c4_rhs = calloc(1, sizeof (double ));

        c4_rmatbeg[0] = 0; c4_rmatbeg[1] = 2;
        c4_rmatval[0] = 1.0; c4_rmatval[1] =  (inst->table_sm7.M );
        c4_rmatind[0] = tempVector[t]; c4_rmatind[1] = vVector[t];
        c4_rhs[0] = inst->table_sm7.max_temperature + inst->table_sm7.M;

        status = CPXaddrows (env, lp, 0, 1, 2,
                             c4_rhs, sense_less, c4_rmatbeg, c4_rmatind, c4_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c4_rmatval);
            free(c4_rmatind);
            free(c4_rmatbeg);
            free(c4_rhs);
        }

    }
    printf("Constraint n 4 OK \n");


    /* Constraint n 5, variable temp(t)
     * Imposes to choose only one moment of the day to start warming up the water
     * n(1) + n(2) + ... + n(1429) = 1
     */

    int end = inst->T - inst->table_sm7.required_time + 1;
    int *c5_rmatind = calloc(end, sizeof (int));
    double *c5_rmatval = calloc(end, sizeof (double ));
    int *c5_rmatbeg = calloc(2, sizeof (int ));
    c5_rmatbeg[0] = 0; c5_rmatbeg[1] = end;
    for (int t = 0; t < end; ++t) {
        c5_rmatind[t] = nVector[t];
        c5_rmatval[t] = 1.0;
    }
    status = CPXaddrows (env, lp, 0, 1, end,
                         one, sense_equal, c5_rmatbeg, c5_rmatind, c5_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf(stderr, "CPXaddrows failed.\n");
    } else {
        free(c5_rmatval);
        free(c5_rmatind);
        free(c5_rmatbeg);
    }
    printf("Constraint n 5 OK \n");



    /* Constraint n 6, variable temp(t)
     * The temperature of the water needs to be adjusted regarding when n(t) is set
     * required_temp * n ()
     */

    for (int t = 0; t < inst->T; ++t) {

        k = 0;
        int *x_array = calloc(inst->table_sm7.required_time, sizeof(int));
        // controllo sulle t', seleziono quelle giuste. Anzichè t' lo chiamo x. t' = x
        for (int x = 0; x < inst->table_sm7.required_time; ++x) {
            if ( x <= t ) {
                x_array[k] = x;
                k++;
            }
        }

        int *c6_rmatind = calloc(k + 1, sizeof (int));
        double *c6_rmatval = calloc(k + 1, sizeof (double ));
        int *c6_rmatbeg = calloc(2, sizeof (int )); c6_rmatbeg[0] = 0; c6_rmatbeg[1] = k+1;

        for (int i = 0; i < k; ++i) {
            c6_rmatind[i] = nVector[ (t - x_array[i]) ];
            c6_rmatval[i] = -1.0 * inst->table_sm7.required_temperature;
        }
        c6_rmatind[k] = tempVector[t];
        c6_rmatval[k] = +1.0;

        status = CPXaddrows (env, lp, 0, 1, k+1,
                             zero, sense_greater, c6_rmatbeg, c6_rmatind, c6_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");
        } else {
            free(c6_rmatval);
            free(c6_rmatind);
            free(c6_rmatbeg);
        }
    }
    printf("Constraint n 6 OK \n");





    return counter;


}

