#include "blo.h"
#include "utils.h"
#include "models/m1.h"
#include "models/m2ewh.h"
#include "models/m2ev.h"
#include "models/m2sb.h"
#include "models/m3.h"
#include "models/mo.h"
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


    /* POPULATION OF THE MODEL */

    char **cname = ( char ** ) calloc( 1, sizeof (char *));
    cname[0] = ( char * ) calloc( 100, sizeof(char) );
    int counter = 0;

    /** STRUTTURE DATI MODELLO M1 */

    int *PSHVector = calloc(inst->T, sizeof (int) );
    // Declaration of structure for variable s(j,t)
    int (*sMatrix)[inst->T];
    sMatrix =  calloc( inst->J, sizeof *sMatrix);
    // Declaration of structure for variable P(j,t)
    int (*pMatrix)[inst->T];
    pMatrix =  calloc( inst->J, sizeof *pMatrix);

    /** STRUTTURE DATI MODELLO M2EWH */

    //Declaration of structure for variable v(t)
    int *vVector = calloc( inst->T, sizeof (int) );
    // Declaration of structure for variable n(t)
    int *nVector = calloc( inst->T, sizeof (int) );
    // Declaration of structure for variable Ploss(t)
    int *PlossVector = calloc( inst->T, sizeof (int) );
    // Declaration of structure for variable at(t) (Ambient temperature)
    int *tempVector = calloc( inst->T, sizeof (int) );


    /** STRUTTURE DATI MODELLO M2EV */

    // Declaration of structure for variable sh2v(t)
    int *sh2vVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable sv2h(t)
    int *sv2hVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Ph2v(t)
    int *Ph2vVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Pv2h(t)
    int *Pv2hVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Ev(t)
    int *EvVector = calloc(inst->T, sizeof(int));

    /** STRUTTURE DATI MODELLO M2SB */

    // Declaration of structure for variable sh2v(t)
    int *sb2hVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable sv2h(t)
    int *sh2bVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Ph2v(t)
    int *Ph2bVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Pv2h(t)
    int *Pb2hVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Ev(t)
    int *BatteryEVector = calloc(inst->T, sizeof(int));

    /** STRUTTURE DATI MODELLO M3 */

    // Declaration of structure for variable sAC(t)
    int *sACVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable y(t)
    int *yVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable z(t)
    int *zVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable inTemp(t)
    int *inTempVector = calloc(inst->T, sizeof(int));

    /** STRUTTURE DATI MODELLO M4 */
    //

    /** STRUTTURE DATI MODELLO M5 */
    //Declaration of structure for variable sg2h(t)
    int *sh2gVector = calloc(inst->T, sizeof(int));
    //Declaration of structure for variable PG2H(t)
    int *PH2GVector = calloc(inst->T, sizeof(int));

    /** STRUTTURE DATI MODELLO MO */
    //Declaration of structure for variable sg2h(t)
    int *sg2hVector = calloc(inst->T, sizeof(int));
    //Declaration of structure for variable PG2H(t)
    int *PG2HVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable u(l)
    int *uVector = calloc(inst->L, sizeof (int));

    /** MODELS */

    //counter = model_m1(inst, env, lp, counter, cname, PSHVector, sMatrix, pMatrix);
    //printf("MODEL M1 INSTANTIATED \n Counter: %d \n", counter);
    counter = model_m2ewh(inst, env, lp, counter, cname, vVector, nVector, PlossVector, tempVector);
    printf("MODEL M2EWH INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m2ev(inst, env, lp, counter, cname, sh2vVector, sv2hVector, Ph2vVector, Pv2hVector, EvVector);
    //printf("MODEL M2EV INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m2sb(inst, env, lp, counter, cname, sh2bVector, sb2hVector, Ph2bVector, Pb2hVector, BatteryEVector);
    //printf("MODEL M2SB INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m3(inst, env, lp, counter, cname, sACVector, yVector, zVector, inTempVector);
    //printf("MODEL M3 INSTANTIATED \n Counter: %d \n", counter);
    /** MODEL M4: JUST MODIFY THE MO ADDING THE BASE LOAD **/
    /** MODEL M5: JUST MODIFY THE MO ADDING THE PV PRODUCTION **/

    counter = model_mo(inst, env, lp, counter, cname, PSHVector, vVector, Ph2vVector, Pv2hVector, Ph2bVector, Pb2hVector, sACVector, sg2hVector, PG2HVector, sh2gVector, PH2GVector, uVector);
    printf("MODEL MO INSTANTIATED \n Counter: %d \n", counter);


    CPXwriteprob(env, lp, "modelmewh.lp", NULL );
    free(cname[0]);
    free(cname);


    // Obtain the number of variable and print them on screen
    int cur_numcols = CPXgetnumcols(env, lp);
    inst->nvariables = cur_numcols;
    printf("Number of variables: %d\n", inst->nvariables);

    inst->solution = (double *) calloc(inst->nvariables, sizeof(double));


    status = CPXmipopt (env, lp);
    if ( status )
        fprintf (stderr, "Failed to optimize MIP.\n");


    status = CPXgetobjval (env, lp, &objval);
    if ( status )
        fprintf (stderr,"CPXgetobjval failed\n");


    // If the problem have a solution it saves it into the instance structure
    status = CPXgetx(env, lp, inst->solution, 0, inst->nvariables - 1);
    if (status)
        printf("Failed to optimize MIP (retrieving the solution -> BLOopt()) %d", status);


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
    free(PSHVector);
    free(vVector);
    free(nVector);
    free(tempVector);
    free(PlossVector);
    free(uVector);
    free(PG2HVector);
    free(sg2hVector);
    free(Ph2vVector);
    free(Pv2hVector);

    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}






















