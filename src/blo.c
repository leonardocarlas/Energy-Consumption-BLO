#include "blo.h"
#include "utils.h"
#include <ilcplex/cplex.h>


void build_model(instance *inst, CPXENVptr pCpxenv, CPXLPptr pCpxlp);

int xpos(int j, int r, int t, instance *inst);

int BLOopt(instance *inst) {
    
    int error;                                                            // code of an error, 0 if is all correct
    CPXENVptr env = CPXopenCPLEX(&error);                                // return the pointer of the envinroment
    CPXLPptr lp = CPXcreateprob(env, &error, "BLO");          // env, pointer to error to comunicate, a name

    build_model(inst, env, lp);    // populate the data

    // Cplex parameter settings
    // ...

    // solve the problem
    int status = CPXmipopt(env, lp);
    if ( status != 0 )
        printf("Cplex error code: %i", status);

    // Use the optimal solution found by cplex

    int nof_columns = CPXgetnumcols(env, lp);
    double *xstar = (double *) calloc(nof_columns, sizeof (double ));

    if ( CPXgetx(env, lp, xstar, 0, nof_columns - 1) )
        print_error("CPXgetx() error");

    // interpretation of the solution


    free(xstar);


    // Free and close cplex model
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}

void build_model(instance *inst, CPXENVptr env, CPXLPptr lp) {

    double zero = 0.0;
    char binary = 'B';

    // each name of the variable are string, organizzare le variabili ( column name )
    char **cname = ( char ** ) calloc( 1, sizeof (char *));
    cname[0] = ( char * ) calloc( 100, sizeof(char) );

    /*
    // add binary variable for the appliances
    for ( int j = 0; j < inst->nof_appliances; j++ ) {
        for ( int r = 0; r < 8; r++ ) {
            for ( int t = 0; t < (inst->end_appliance_sm5[j] - inst->start_appliance_sm5[j]); t++) {
                sprintf(cname[0], "w(%i,%i,%i)", j+1, r+1, t+1);
                // costo di prendere quella soluzione (f_jr)
                double lb = 0.0;
                double ub = 1.0;

                // cost of taking that time r for that load j
                double obj = 1.4; //inst->power_stage1_sm6[j];

                //Creates the columns of the new variable
                if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname))
                    print_error("Wrong CPXnewcols on x var.s");

                if (CPXgetnumcols(env, lp) - 1 != xpos(j, r, t, inst))
                    print_error("Wrong position for x var.s");

            }

        }
    }
     */
}



