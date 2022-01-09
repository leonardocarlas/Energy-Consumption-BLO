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

    printf("\n--------------STARTING THE PSO--------------\n\n");
    double *global_best = calloc(inst.nof_subperiods, sizeof(double));
    psoUL(&inst, global_best);
    //printf("\n--------------STARTING THE SA--------------\n\n");
    //saUL(&inst, global_best);


    free_instance(&inst);

    return 0;

}








