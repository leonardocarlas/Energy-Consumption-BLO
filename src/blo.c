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

    build_model(inst, env, lp);    // populate the data

    // Obtain the number of variable and print them on screen
    int cur_numcols = CPXgetnumcols(env, lp);
    inst->nvariables = cur_numcols;
    printf("Number of variables: %d\n", inst->nvariables);

    inst->solution = (double *) calloc(inst->nvariables, sizeof(double));

    // If the problem have a solution it saves it into the instance structure
    status = CPXgetx(env, lp, inst->solution, 0, inst->nvariables - 1);
    if (status) {
        printf("Failed to optimize MIP (retrieving the solution -> BLOopt() %d", status);
    }

    // Print the values of the solution
    double solution = -1;
    if (CPXgetobjval(env, lp, &solution)) {
        print_error("Failed to optimize MIP (getobjval() -> BLOopt).\n");
    }
    printf("Objval: %f\n", solution);



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


    // Free and close cplex model
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    return 0;
}




/*
 * Method to define problem variables and constraints
 */
void build_model(instance *inst, CPXENVptr env, CPXLPptr lp) {

    char binary = 'B';
    char integer = 'I';
    char sense_greater[1] = {'G'};
    char    sense_equal[1] = {'E'};
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





    // P(t) = contiguos
    for (int t = 0; t < inst->T; t++) {

        sprintf( cname[0], "P(%i)", t+1);
        double lb = 0.0;
        double ub = 2000.0;
        double obj = powerCostTimet(t+1, inst);        // cost

        status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname);
        if ( status ){
            fprintf (stderr,"CPXnewcols failed on P(t) variables.\n");
        }

        pVector[t] = counter;
        printf("Posizione variabile P(%i): %i \n ", t+1, pVector[t]);
        counter++;
    }





    // u( l ) = 0 / 1
    double  u_rmatval[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    int  u_rmatbeg[2] = {0, inst->nof_powerlevels};
    double  u_rhs[1] = {1.0};



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







    // s(j, t)  = 0 / 1

    double  s_rhs[3]    = {1.0, 1.0, 1.0};
    char    s_sense[3]  = {'E', 'E', 'E'};
    /* Note - use a trick for rmatbeg by putting the total nonzero count in
              the last element.  This is not required by the CPXaddrows call. */
    int     s_rmatbeg[4] = {0, 10, 20, 30};
    double  s_rmatval[(inst->nof_appliances * inst->T)];
    int s_rmatind[(inst->nof_appliances * inst->T)];
    int k = 0;


    for ( int j = 0; j < inst->nof_appliances; j++) {
        for ( int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "s(%i,%i)", j+1, t+1);
            double lb = 0.0;
            double ub = 1.0;
            double obj = 1.0;

            status = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
            if (status) print_error("Error CPXnewcols on s_jt");

            s_rmatind[k] = sMatrix[j][t] = counter;
            s_rmatval[k] = 1.0;
            counter++;

            printf("Posizione variabile s(%i,%i): %i \n", j+1, t+1, sMatrix[j][t]);
            k++;
        }
    }






    // P(j,t) = contiguos

    int pjt_rmatind[(inst->nof_appliances * inst->T)][1];
    int ptj_rmatind[(inst->T)][inst->nof_appliances];
    k = 0;

    for ( int j = 0; j < inst->nof_appliances; j++) {
        for (int t = 0; t < inst->T; t++) {

            sprintf(cname[0], "P(%d,%d)", j+1, t+1);
            double lb = 0.0;
            double ub = 2000.0;
            double obj = 1.0;

            //Creates the columns of the new variable
            if (CPXnewcols(env, lp, 1, &obj, &lb, &ub, &integer, cname))
                print_error("Wrong CPXnewcols on x var.s");

            pjt_rmatind[k][0] = pMatrix[j][t] = counter;
            ptj_rmatind[t][j] = counter;
            counter++;

            printf("Posizione variabile P(%i,%i): %i \n", j+1, t+1, pMatrix[j][t]);
            k++;
        }
    }









    /* Constraint number 6: example, variable u(l)
     *
     * u(1) + u(2) + u(3) + u(4) + u(5) + u(6) + u(7) + u(8) + u(9)  = 1
     */
    status = CPXaddrows (env, lp, 0, 1, inst->nof_powerlevels,
                         u_rhs, sense_equal, u_rmatbeg, uVector, u_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");
        //goto TERMINATE;
    } else {
        printf("Vincolo 6 creato \n");
    }





    /* Constraint number 1: example, variable s(j,t)
     *
     s(1,1) + s(1,2) + s(1,3) + s(1,4) + s(1,5) + s(1,6) + s(1,7) + s(1,8) + s(1,9) + s(1,10) = 1
     s(2,1) + s(2,2) + s(2,3) + s(2,4) + s(2,5) + s(2,6) + s(2,7) + s(2,8) + s(2,9) + s(2,10) = 1
     s(3,1) + s(3,2) + s(3,3) + s(3,4) + s(3,5) + s(3,6) + s(3,7) + s(3,8) + s(3,9) + s(3,10) = 1
     */

    status = CPXaddrows (env, lp, 0, inst->nof_appliances, (inst->T * inst->nof_appliances),
                         s_rhs, s_sense, s_rmatbeg, s_rmatind, s_rmatval,
                         NULL, NULL);
    if ( status ) {
        fprintf (stderr,"CPXaddrows failed.\n");

    } else {
        printf("Vincolo 1 creato \n");
    }



    /* Constraint number 3: example, variable P(j, t)
     *
     P(2,1) = 0
     P(2,2) = 0
     P(2,3) = 0 ... meglio aggiungerli uno alla volta
    */

    int pjt_matbeg[2] = {0, 1};
    double pjt_rmatval[1] = {1.0};
    double test_rhs[1] = {0.0};
    k = 0;
    for ( int j = 0; j < inst->nof_appliances; j++ ) {
        for (int t = 0; t < inst->T; ++t) {

            if( t+1 < inst->table_sm5[j].start_interval || t+1 > inst->table_sm5[j].end_inteval ) {

                status = CPXaddrows (env, lp, 0, 1, 1,
                                     test_rhs, sense_equal, pjt_matbeg, pjt_rmatind[k], pjt_rmatval,
                                     NULL, NULL);
                if ( status ) {
                    fprintf(stderr, "CPXaddrows failed.\n");
                }
            }
            k++;
        }
    }
    printf("Vincolo 3 creato \n");




    /* Constraint number 4: example, variable P(t)
     *
     P_11 + P_21 + P_31 = P(1)
     P_12 + P_22 + P_32 = P(2)
    */
    const double test[1] = { 1.0 };
    double pt_rmatval[3] = { 1.0, 1.0, 1.0 };
    int pt_matbeg[2] = {0, 2};

    for (int t = 0; t < inst->T ; ++t) {

        status = CPXaddrows (env, lp, 0, 1, inst->nof_appliances,
                             test, sense_equal, pt_matbeg, ptj_rmatind[t], pt_rmatval,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");

        }

    }
    printf("Vincolo 4 creato \n");







    /* Constraint number 5: example, variable P(t)
     *
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(1)
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(2)
     P_1 * u_1 + P_2 * u_2 + P_3 * u_3 + P_4 * u_4 + ... + P_9 * u_9  >= P(3)
     *
     P_12 + P_22 + P_32 >= P(2)
    */

    int ul_rmatbeg[2] = {0, inst->nof_powerlevels-1};
    double ul_rmatval[inst->nof_powerlevels];
    for (int l = 0; l < inst->nof_powerlevels; ++l) {

        ul_rmatval[l] =  inst->table_sm2[l].watt;
    }
    for (int t = 0; t < inst->T; ++t) {

        status = CPXaddrows (env, lp, 0, 1, inst->nof_powerlevels,
                             test, sense_greater, ul_rmatbeg, uVector, ul_rmatval ,
                             NULL, NULL);
        if ( status ) {
            fprintf(stderr, "CPXaddrows failed.\n");

        }

    }

    printf("Vincolo 5 creato \n");




    /* Constraint number 2: example P_jt
     *
    P_11 = g_11 * s_11 + g_12 * s_13 + ... +
    P_21 =
    P_31 =
    P_12 =
    P_22 =
    P_33 =
    */

    int r_counter = 0;
    int *r_indixes = calloc(inst->max_dj, sizeof(int));

    for (int j = 0; j < inst->nof_appliances; ++j) {
        for (int t = 0; t < inst->T; ++t) {


            for (int r = 0; r < inst->max_dj; ++r) {

                if ( r == 0 || r <= t || r <= t - inst->table_sm5[j].start_interval) {

                    r_indixes[r_counter] = r;
                    r_counter++;
                }
            }

            //printf("Numero di r_counter: %i \n", r_counter);

            int rmatind[r_counter];
            double rmatval[r_counter];

            for (int r = 0; r < r_counter; ++r) {

                int right_index = t - r_indixes[r];
                int s_j_right_index = sMatrix[j][right_index];
                int g_jr = powerRequiredShiftableStageR(inst, j, r);
                //printf("Indice giusto, sj(t-r+1), g_jr %i %i %i %i\n",j ,right_index + 1, s_j_right_index, g_jr);
                rmatind[r] = s_j_right_index;
                rmatval[r] = g_jr;
            }

            int rmatbeg[2] = {0, r_counter};

            status = CPXaddrows (env, lp, 0, 1, r_counter,
                                 test, sense_equal, rmatbeg, rmatind, rmatval ,
                                 NULL, NULL);
            if ( status ) {
                fprintf(stderr, "CPXaddrows failed.\n");
                goto TERMINATE;
            }

            r_counter = 0;


        }

    }

    printf("Vincolo 2 creato \n");




    CPXwriteprob(env, lp, "model.lp", NULL );


    TERMINATE:

    free(cname[0]);
    free(cname);


}










/*
    //  g(j, r) = contiguos
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
*/








