#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "blo.h"


// First shiftable loads, block
// When the MILP is functioning, we will then implement a PSO for the UL 


void print_error(const char *err);
void read_input(instance *inst);
void parse_command_line(int argc, char** argv, instance *inst);
int BLOopt(instance *inst);

/*
void free_instance(instance *inst) {
    
    free(inst->input_file);
    free(inst->model_type);
    free(inst->nappliances);
}
*/


int main(int argc, char **argv) {
	
	//If the command line arguments are minor than 2 exits from the program
	if (argc < 2) {
		printf("Usage error");
		exit(1);
	}

    //Variables
    time_t t1 = time(NULL);
	instance inst;
    

	//Commands for gnuplot
	// char *commandsForGnuplot[] = { "set title \"FIRST POINTS\"", "plot \"data.dat\" with linespoints linestyle 1" };

	//Parse the command line and read the input file
	printf("Input file information \n\n");
	parse_command_line(argc, argv, &inst); // & serve a passare il puntatore dell'istanza
	read_input(&inst);

	//Calculate the solution of the problem
	//printf("\n--------------OPTIMIZATION INFORMATIONS--------------\n");
	//BLOopt(&inst);

    time_t t2 = time(NULL);

    printf("\nTime to solve the BLO problem: %lf \n ", difftime(t2, t1));

    //free_instance(&inst);
    return 0;

	//Plot the solution with the passed commands
	//printf("\n----------------------PLOTTING------------------------\n");
	//plot(commandsForGnuplot, 2, &inst);
}



/*!
 * This function will parse the command line and saves it into instance
 * @param	argc is the number if string pointed by argv
 * @param	argv is a pointer to a pointer of strings
 * @param	inst pointer to an instance
 * @modify	saves the value passed in argv into inst
 */
void parse_command_line(int argc, char** argv, instance *inst) {

    //allocate the model type of the struct
    inst->model_type = 0;
    strcpy(inst->input_file, "NULL");

    int help = 0;

    if ( argc < 1 ) 
        help = 1;

    for ( int i = 1; i < argc; i++) 
    {
        if ( strcmp( argv[i], "-f" ) == 0 ) {
            strcpy( inst->input_file, argv[++i] );
            continue;
        }
        
        if ( strcmp( argv[i], "-help" ) == 0 ) {
            help = 1;
            continue;
        }

    }

    printf("Selected file: %s \n", inst->input_file);

    if (help)
        exit(1);

}


void read_input(instance *inst)
{

    FILE *file = fopen(inst->input_file, "r");
    if ( file == NULL )
        print_error("Cannot open the file");

    
    char *parameter_name;
    int number_table;

    char line[180];
    const char delimiter[2] = ":";
    char *token;

    while ( fgets( line, sizeof(line), file) != NULL )
    {

        // Skip the empty lines
        if ( strlen (line) <= 1 ) continue;
        //Will end the reading of the file
		if (strcmp(line, "EOF") == 0) {
			break;
		}

        if(strchr(line, ':') != NULL) 
        {
            //printf("Tabella trovata \n");
            
            //Primo token
            token = strtok(line, ":");
            //printf( " %s\n", token );
            parameter_name = token;

            
            //Secondo token
            token = strtok(NULL, ":");
            //printf( " %s\n", token );
            number_table = atoi( token );

            printf("Table number: %i \n", number_table);
            continue;

        }

        if ( number_table == 1 ) {

            //Primo token (numero di intervallo)
            token = strtok(line, " ");
            //printf( " %s\n", token );
            int in = atoi ( token );

            
            //Secondo token (inizio) 
            token = strtok(NULL, " ");
            //printf( " %s\n", token );
            inst->earliest_interval_sm1[in - 1] = atoi( token );
            //printf( " %i\n", inst->earliest_interval_sm1[in - 1] );
            

            //Terzo token (fine)
            token = strtok(NULL, " ");
            //printf( " %s\n", token );
            inst->latest_interval_sm1[in - 1] = atoi( token );
            //printf( " %i\n", inst->latest_interval_sm1[in - 1] );
            
            
            //Quarto token
            token = strtok(NULL, " ");
            //printf( " %s\n", token );
            inst->price_interval_sm1[in - 1] = atof( token );
            //printf( " %lf\n", inst->price_interval_sm1[in - 1] );
            
            
        }

        if ( number_table == 2 ) {
            continue;
        }

        if ( number_table == 3 ) {
            continue;
        }
        if ( number_table == 4 ) {
            continue;
        }
        if ( number_table == 5 ) {
            continue;
        }
        if ( number_table == 6 ) {
            continue;
        }
        if ( number_table == 7 ) {
            continue;
        }
        if ( number_table == 8 ) {
            continue;
        }
        if ( number_table == 99 ) {
            continue;
        }

    }

    
}



void print_error(const char *err)
{

    printf(" \n\n Error: %s \n\n ", err);
    fflush(NULL);
    exit(1);
}


// O alloco dimanicamente un heap 
// Oppure sono creo un array di lunghezza fissa (posso farlo avendo dei dati)