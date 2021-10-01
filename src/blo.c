#include "blo.h"
#include "utils.h"
#include <ilcplex/cplex.h>


int BLOopt(instance *inst) {
    
    int error;                               // code of an error, 0 if is all correct
    CPXENV env = CPXopenCPLEX(&error);          // return the pointer of the envinromente
    CPXLPptr lp = CPXcreateprob(env, &error, "BLO");       // env, pointer to error to comunicate, a name

    build_model(inst, env, lp);    // populate the data

    // Cplex parameter settings
    // ...

    if ( CPXmipopt(env, lp) ) print_error("CPXmipopt() error");





    // Free and close cplex model
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}