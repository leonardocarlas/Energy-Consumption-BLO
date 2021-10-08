#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include "blo.h"
#include "read_input.h"
#include "plot.h"
#include "utils.h"


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

    time_t start = time(NULL);
	instance inst;

	//Commands for gnuplot
	// char *commandsForGnuplot[] = { "set title \"FIRST POINTS\"", "plot \"data.dat\" with linespoints linestyle 1" };

	//Parse the command line and read the input file
	printf("--------------INPUT FILE INFORMATIONS-------------- \n\n");
	parse_command_line(argc, argv, &inst); // & serve a passare il puntatore dell'istanza
	read_input(&inst);

	//Calculate the solution of the problem
	//printf("\n--------------OPTIMIZATION INFORMATIONS--------------\n");
	//BLOopt(&inst);

    time_t end = time(NULL);

    printf("\nTime to solve the BLO problem: %lf \n ", difftime(end, start));

    free_instance(&inst);

    return 0;

	//Plot the solution with the passed commands
	//printf("\n----------------------PLOTTING------------------------\n");
	//plot(commandsForGnuplot, 2, &inst);
}
