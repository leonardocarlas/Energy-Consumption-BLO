#include "blo.h"
#include "utils.h"
#include "models/m1a.h"
#define BINARY_VARIABLE 'B'
#define INTEGER_VARIABLE 'I'
#define EQUAL 'E'
#define LESS_EQUAL 'L'
#define GREAT_EQUAL 'G'
#define LOWER_BOUND 'L'
#define EPS 1e-5

#include <ilcplex/cplex.h>




int BLOopt(instance *inst) {

    double objval;
    int status;          // code of an error, 0 if is all correct

    CPXENVptr env = CPXopenCPLEX(&status);                     // return the pointer of the envinroment

    /* Turn on output to the screen */

    status = CPXsetintparam (env, CPXPARAM_ScreenOutput, CPX_ON);
    if ( status ) {
        fprintf (stderr,
                 "Failure to turn on screen indicator, error %d.\n", status);
        //goto TERMINATE;
    }

    status = CPXsetintparam (env, CPXPARAM_Simplex_Display, 2);
    if ( status ) {
        fprintf (stderr,"Failed to turn up simplex display level.\n");
        //goto TERMINATE;
    }

    CPXLPptr lp = CPXcreateprob(env, &status, "BLO");          // env, pointer to error to comunicate, a name

    model_m1a(inst, env, lp);    // populate the data

    // Obtain the number of variable and print them on screen
    int cur_numcols = CPXgetnumcols(env, lp);
    inst->nvariables = cur_numcols;
    printf("Number of variables: %d\n", inst->nvariables);

    inst->solution = (double *) calloc(inst->nvariables, sizeof(double));


    status = CPXmipopt (env, lp);
    if ( status ) {
        fprintf (stderr, "Failed to optimize MIP.\n");
        //goto TERMINATE;
    }

    status = CPXgetobjval (env, lp, &objval);
    if ( status ) {
        fprintf (stderr,"CPXgetobjval failed\n");
        //goto TERMINATE;
    }


    // If the problem have a solution it saves it into the instance structure
    status = CPXgetx(env, lp, inst->solution, 0, inst->nvariables - 1);
    if (status) {
        printf("Failed to optimize MIP (retrieving the solution -> BLOopt()) %d", status);
    }

    // Print the values of the solution
    double solution = -1;
    if (CPXgetobjval(env, lp, &solution)) {
        print_error("Failed to optimize MIP (getobjval() -> BLOopt).\n");
    }
    printf("Objval: %f\n", solution);

    for (int j = 0; j < inst->J; ++j) {
        for (int t = 0; t < inst->T; ++t) {
            //printf("VAL s(%i, %i): %i ", j+1, t+1, CPXgetx(env, lp))
        }
    }


    status = CPXsolwrite(env, lp, "test.sol");

    // Free and close cplex model
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}






















