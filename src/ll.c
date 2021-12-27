#include "ll.h"
#include "utils.h"
#include "models/m1.h"
#include "models/m2ewh.h"
#include "models/m2ev.h"
#include "models/m2sb.h"
#include "models/m3.h"
#include "models/mo.h"
#include "ul.h"
#include <ilcplex/cplex.h>

#define INTERNAL_TIME_LIMIT 60.0



double LLopt(instance *inst, double *prices) {

    double objval;
    int status;          // code of an error, 0 if is all correct

    CPXENVptr env = CPXopenCPLEX(&status);                     // return the pointer of the envinroment

    /* Turn on output to the screen */

    status = CPXsetintparam (env, CPXPARAM_ScreenOutput, CPX_ON);
    if ( status ) fprintf (stderr,"Failure to turn on screen indicator, error %d.\n", status);

    status = CPXsetintparam (env, CPXPARAM_Simplex_Display, 2);
    if ( status ) fprintf (stderr,"Failed to turn up simplex display level.\n");

    CPXLPptr lp = CPXcreateprob(env, &status, "BLO");          // env, pointer to error to comunicate, a name

    char **cname = ( char ** ) calloc( 1, sizeof (char *));
    cname[0] = ( char * ) calloc( 100, sizeof(char) );
    int counter = 0;

    /** SETTING THE PRICES TO BUY **/

    for (int i = 0; i < inst->nof_subperiods; ++i)
        inst->table_sm1[i].price_subperiod = prices[i];


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
    int *sH2VVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable sv2h(t)
    int *sV2HVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Ph2v(t)
    int *PH2VVector = calloc(inst->T, sizeof(int));
    // Declaration of structure for variable Pv2h(t)
    int *PV2HVector = calloc(inst->T, sizeof(int));
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

    /** STRUTTURE DATI MODELLO MUL */
    //Declaration of structure for variable sg2h(t)
    int *xVector = calloc(inst->nof_subperiods, sizeof(int));


    /** MODELS */

    //counter = model_m1(inst, env, lp, counter, cname, PSHVector, sMatrix, pMatrix);
    //printf("MODEL M1 INSTANTIATED \n Counter: %d \n", counter);
    counter = model_m2ewh(inst, env, lp, counter, cname, vVector, nVector, PlossVector, tempVector);
    printf("MODEL M2EWH INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m2ev(inst, env, lp, counter, cname, sH2VVector, sV2HVector, PH2VVector, PV2HVector, EvVector);
    //printf("MODEL M2EV INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m2sb(inst, env, lp, counter, cname, sh2bVector, sb2hVector, Ph2bVector, Pb2hVector, BatteryEVector);
    //printf("MODEL M2SB INSTANTIATED \n Counter: %d \n", counter);
    //counter = model_m3(inst, env, lp, counter, cname, sACVector, yVector, zVector, inTempVector);
    //printf("MODEL M3 INSTANTIATED \n Counter: %d \n", counter);
    /** MODEL M4: JUST MODIFY THE MO ADDING THE BASE LOAD **/
    /** MODEL M5: JUST MODIFY THE MO ADDING THE PV PRODUCTION **/

    counter = model_mo(inst, env, lp, counter, cname, PSHVector, vVector, PH2VVector, PV2HVector, Ph2bVector, Pb2hVector, sACVector, sg2hVector, PG2HVector, sh2gVector, PH2GVector, uVector);
    printf("MODEL MO INSTANTIATED \n Counter: %d \n", counter);

    //counter = model_mul(inst, env, lp, counter, cname, xVector);
    //printf("MODEL MUL INSTANTIATED \n Counter: %d \n", counter);

    CPXwriteprob(env, lp, "ll.lp", NULL );
    free(cname[0]);
    free(cname);


    // Obtain the number of variable and print them on screen
    int cur_numcols = CPXgetnumcols(env, lp);
    inst->nvariables = cur_numcols;
    printf("--- NUMBER OF VARIABLES: %d\n", inst->nvariables);
    inst->solution = (double *) calloc(inst->nvariables, sizeof(double));


    inst->start_time = seconds();
    //Change the internal time limit of cplex
    CPXsetdblparam(env, CPX_PARAM_TILIM, INTERNAL_TIME_LIMIT);
    printf("--- CALCULATING THE SOLUTION\n");


    //If the time remaining is lower than the INTERNAL_TIME_LIMIT than
    //change the cplex time limit to the actual time remaining
    //double timeleft = inst->start_time + inst->timelimit - seconds();
    //if (timeleft <= 0)
    //    break;
    //if (timeleft < INTERNAL_TIME_LIMIT) {
    status = CPXsetdblparam(env, CPX_PARAM_TILIM, INTERNAL_TIME_LIMIT);
    if ( status ) fprintf (stderr, "Failed to set parameter time limit\n");
    //}

    status = CPXmipopt (env, lp);
    if ( status ) fprintf (stderr, "Failed to optimize MIP.\n");


    status = CPXgetobjval (env, lp, &objval);
    if ( status ) fprintf (stderr,"CPXgetobjval failed\n");


    // If the problem have a solution it saves it into the instance structure
    status = CPXgetx(env, lp, inst->solution, 0, inst->nvariables - 1);
    if (status) printf("Failed to optimize MIP (retrieving the solution -> LLopt()) %d", status);




    status = CPXsolwrite(env, lp, "ll.sol");


    // Free and close cplex model
    free(PSHVector);
    free(vVector);
    free(nVector);
    free(tempVector);
    free(PlossVector);
    free(uVector);
    free(PG2HVector);
    free(sg2hVector);
    free(PH2VVector);
    free(PV2HVector);

    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return objval;
}


















