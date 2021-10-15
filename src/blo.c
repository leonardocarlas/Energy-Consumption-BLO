#include "blo.h"
#include "utils.h"
#define BINARY_VARIABLE 'B'
#define INTEGER_VARIABLE 'I'
#define EQUAL 'E'
#define LESS_EQUAL 'L'
#define GREAT_EQUAL 'G'
#define LOWER_BOUND 'L'
#define EPS 1e-5

#include <ilcplex/cplex.h>




int BLOopt(instance *inst) {
    
    int error;                                                            // code of an error, 0 if is all correct
    CPXENVptr env = CPXopenCPLEX(&error);                                // return the pointer of the envinroment
    CPXLPptr lp = CPXcreateprob(env, &error, "BLO");          // env, pointer to error to comunicate, a name

    build_model(inst, env, lp);    // populate the data

    // Cplex parameter settings
    // ...


    int status = CPXmipopt(env, lp);            // solve the problem
    if ( status != 0 )
        printf("Cplex error code: %i", status);

    // Use the optimal solution found by cplex

    int nof_columns = CPXgetnumcols(env, lp);
    double *xstar = (double *) calloc(nof_columns, sizeof (double ));


    //if ( CPXgetx(env, lp, xstar, 0, nof_columns - 1) )
    //    print_error("CPXgetx() error");

    // interpretation of the solution

    free(xstar);

    // Free and close cplex model
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}

void build_model(instance *inst, CPXENVptr env, CPXLPptr lp) {

    char binary = 'B';
    char integer = 'I';
    int status;

    // each name of the variable are string, organizzare le variabili ( column name )
    char **cname = ( char ** ) calloc( 1, sizeof (char *));
    cname[0] = ( char * ) calloc( 100, sizeof(char) );

    int counter = 0;

    // Declaration of structure for variable P_t
    int pVector[inst->T];
    // Declaration of structure for variable u_l
    int uVector[inst->nof_powerlevels];
    // Declaration of structure for variable s_jt
    int sMatrix[inst->nof_appliances][inst->T];
    // Declaration of structure for variable s_jt
    int pMatrix[inst->nof_appliances][inst->T];
    // Declaration of structure for variable g_jr
    int gMatrix[inst->nof_appliances][inst->max_dj];




    /*
    // contiguos variable for the Power at time t
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "P(%i)", t+1);
        double lb = 0.0;
        double ub = 2000.0;
        double obj = powerCostTimet(t+1, inst);        // cost

        if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname)) {
            fprintf (stderr,"CPXnewcols failed on P(t) variables.\n");
        }

        pVector[t] = counter;
        //printf("Posizione variabile P(%i): %i \n ", t+1, pVector[t]);
        counter++;
    }




    // Debug del vettore pVector
    for (int t = 0; t < inst->T; t++) {
        printf("Valore nel vettore P: %i \n", pVector[t]);
    }

*/



    // u( l ) = 0 / 1
    double  u_rmatval[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int  u_rmatbeg[2] = {0, inst->nof_powerlevels};
    double  u_rhs[1] = {1.0};
    char    u_sense[1] = {'E'};


    for ( int l = 0; l < inst->nof_powerlevels; l++) {

        sprintf(cname[0], "u(%i)", l+1);
        double lb = 0.0;
        double ub = 1.0;
        double obj = powerLevelCost(l+1,inst);     // cost

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
        if ( status ) {

            print_error("Wrong CPXnewcols on u variables");
        }

        uVector[l] = counter;
        printf("Posizione variabile u(%i): %i \n ", l+1, uVector[l]);
        counter++;
    }











    double  s_rhs[3]    = {1.0, 1.0, 1.0};
    char    s_sense[3]  = {'E', 'E', 'E'};
    /* Note - use a trick for rmatbeg by putting the total nonzero count in
              the last element.  This is not required by the CPXaddrows call. */
    int     s_rmatbeg[4] = {0, 1440, 2880, 4320};
    double  s_rmatval[(inst->nof_appliances * inst->T)];
    int s_rmatind[(inst->nof_appliances * inst->T)];
    int k = 0;


    // s(j, t)  = 0 / 1
    for ( int j = 0; j < inst->nof_appliances; j++) {
        for ( int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "s(%i,%i)", j+1, t+1);
            double lb = 0.0;
            double ub = 1.0;
            double obj = 1.0;

            status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
                if (status) print_error("Error CPXnewcols on s_jt");

            if ( t+1 >= inst->table_sm5[j].start_interval && t+1 <= inst->table_sm5[j].end_inteval ) {

                s_rmatind[k] = sMatrix[j][t] = counter;
                s_rmatval[k] = 1.0;
                counter++;
            }
            else {

                sMatrix[j][t] = -1;
                s_rmatind[k] = counter;
                s_rmatval[k] = 0.0;
            }
            printf("Posizione variabile s(%i,%i): %i \n", j+1, t+1, sMatrix[j][t]);
            k++;
        }
    }




    /*

    // contiguos variable P(j,t), power fo load j at time t
    for ( int j = 0; j < inst->nof_appliances; j++) {
        for (int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "P(%d,%d)", j+1, t+1);
            double lb = 0.0;
            double ub = 2000.0;
            double obj = 1.0;

            //Creates the columns of the new variable
            if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname))
                print_error("Wrong CPXnewcols on x var.s");
            if ( t+1 >= inst->table_sm5[j].start_interval && t+1 <= inst->table_sm5[j].end_inteval ) {
                pMatrix[j][t] = counter;
                counter++;
            }
            else
                pMatrix[j][t] = -1;
            printf("Posizione variabile P(%i,%i): %i \n", j+1, t+1, pMatrix[j][t]);

        }
    }


    // contiguos variable g(j,r), power for load j at stage r
    for ( int j = 0; j < inst->nof_appliances; j++) {
        for (int r = 0; r < inst->max_dj; r++) {

            sprintf(cname[0], "g(%d,%d)", j+1, r+1);
            double lb = 0.0;
            double ub = 2000.0;
            double obj = 1.0;

            //Creates the columns of the new variable
            if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname))
                print_error("Wrong CPXnewcols on x var.s");

            gMatrix[j][r] = counter;
            counter++;

            printf("Posizione variabile g(%i,%i): %i \n", j+1, r+1, gMatrix[j][r]);

        }
    }



    /* Constraint number 6: example, variable u(l)
     *
     * u(1) + u(2) + u(3) + u(4) + u(5) + u(6) + u(7) + u(8) + u(9)  = 1
     */
    status = CPXaddrows (env, lp, 0,1, inst->nof_powerlevels,
                         u_rhs, u_sense, u_rmatbeg, uVector, u_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
        //goto TERMINATE;
    }




    /* Constraint number 1: example, variable s(j,t)
     *
     s(1,1) + s(1,2) + s(1,3) + s(1,4) + s(1,5) + s(1,6) + s(1,7) + s(1,8) + s(1,9) + s(1,10)
     s(2,1) + s(2,2) + s(2,3) + s(2,4) + s(2,5) + s(2,6) + s(2,7) + s(2,8) + s(2,9) + s(2,10)
     s(3,1) + s(3,2) + s(3,3) + s(3,4) + s(3,5) + s(3,6) + s(3,7) + s(3,8) + s(3,9) + s(3,10)
     */

    status = CPXaddrows (env, lp, 0, inst->nof_appliances, (inst->T * inst->nof_appliances),
                         s_rhs, s_sense, s_rmatbeg, s_rmatind, s_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
        goto TERMINATE;
    }




    CPXwriteprob(env, lp, "model.lp", NULL );


    TERMINATE:

    free(cname[0]);
    free(cname);


}











