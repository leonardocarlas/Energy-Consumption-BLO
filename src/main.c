#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include "ll.h"
#include "read_input.h"
#include "plot.h"
#include "utils.h"
#include "../include/pso.h"
#include "../include/sa.h"
#include "../include/ga.h"
#include "../include/lbs.h"
#include "../include/csa.h"
#include "../include/de.h"
#include "../include/grasp.h"
#include "../include/root.h"

// una struct per ogni tabella, poi un array di struct
// ordinato con le variabili, prima aggiungo le colonne delle varibili con i bound delle variabili
// poi i vincoli uno alla volta
// salvare il file lp, controllare le variabili, controllare che i vincoli che ho aggiunto siano quelli
// First shiftable loads, block
// When the MILP is functioning for all the appliances, we will then implement a PSO for the UL



int main(int argc, char **argv) {
	
	//If the command line arguments are minor than 2 exits from the program
	if (argc < 2) {
		printf("Usage error");
		exit(1);
	}

	instance inst;



    // ------  THINGS FOR THE BLMF  ------
    document doc;
    readResume("/home/leonardo/Scrivania/BLO/data/retailerDoc.txt", &doc);

    instance * instanceVector = calloc( doc.nUSERS, sizeof(instance) );



    for (int u = 0; u < doc.nUSERS; ++u) {


        char * file1 = "../data/datac1.txt";
        char * file2 = "../data/datac2.txt";
        char * file3 = "../data/datac3.txt";
        char * file4 = "../data/datac4.txt";

        /*
        char * name_with_extension;
        name_with_extension = calloc(18, sizeof(char));
        char filename[18] = "../data/datac1.txt";
        for (int c = 0; c < sizeof(filename); ++c) {
            printf(" %c \n", filename[c]);
            name_with_extension[c] = filename[c];
            if ( c == 13)
                name_with_extension[c] = 1;
        }
         */

        /*
        char n = (char) (u + 1);
        char * extension = ".txt";
        char * name_with_extension;
        name_with_extension = malloc(strlen(filename)+1+4);
        strcpy(name_with_extension, filename);
        strcat(name_with_extension, n);
        strcat(name_with_extension, extension);
        */

        printf("-------------- INFORMATIONS INPUT FILE %d -------------- \n\n", u+1);
        if ( u == 0 )
            readInput( file1 , &instanceVector[u]);
        if ( u == 1 )
            readInput( file2 , &instanceVector[u]);
        if ( u == 2 )
            readInput( file3 , &instanceVector[u]);
        if ( u == 3 )
            readInput( file4 , &instanceVector[u]);

    }




	//Parse the command line and read the input file
	printf("--------------INPUT FILE INFORMATIONS-------------- \n\n");
	parse_command_line(argc, argv, &inst); // & serve a passare il puntatore dell'istanza
	read_input(&inst);

    /** RUNNING ONLY THE LOWER LEVEL PROBLEM
    //Setting the initial prices (or the prices only for the LL problem)
    double *initial_prices = calloc(inst.nof_subperiods, sizeof(double ));
    initial_prices[0] = 0.001660;
    initial_prices[1] = 0.004565;
    initial_prices[2] = 0.004713;
    initial_prices[3] = 0.001362;
    initial_prices[4] = 0.002580;
    initial_prices[5] = 0.002397;


	//Calculate the solution of the problem
	printf("\n--------------OPTIMIZATION INFORMATIONS--------------\n\n");


    double start_time = seconds();
	LLopt(&inst, initial_prices);
    double end_time = seconds();

    //printf("\nTime to solve the BLO problem: %lf \n ", (end_time - start_time));
    **/

    double *global_best = calloc(inst.nof_subperiods, sizeof(double));

    //printf("\n-------------- BLO --------------\n\n");

    //printf("\n--------------STARTING THE PSO--------------\n\n");
    //psoUL(&inst, global_best);
    //printf("\n--------------STARTING THE SA--------------\n\n");
    //saUL(&inst, global_best);
    //printf("\n--------------STARTING THE GA--------------\n\n");
    //gaUL(&inst, global_best);
    //printf("\n--------------STARTING THE LBS--------------\n\n");
    //lbsUL(&inst, global_best);
    //printf("\n--------------STARTING THE CSA--------------\n\n");
    //csaUL(&inst, global_best);
    //printf("\n--------------STARTING THE DE--------------\n\n");
    //deUL(&inst, global_best);
    //printf("\n--------------STARTING THE GRASP--------------\n\n");
    //graspUL(&inst, global_best);


    printf("\n-------------- BLMF --------------\n\n");
    psoULBLMF(instanceVector, &inst, global_best);



    for (int i = 0; i < inst.nof_subperiods; ++i)
        printf("Price: %f  \n", global_best[i]);


    free_instance(&inst);
    free(global_best);
    free(instanceVector);

    return 0;

}










