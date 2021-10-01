#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <errno.h>
#include "read_input.h"
#include "utils.h"


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

        if ( strcmp( argv[i], "-m" ) == 0 ) {
            inst->model_type = atoi( argv[++i] );
            continue;
        }

    }

    printf("Selected file: %s \n Model type: %i \n", inst->input_file, inst->model_type);

    if (help)
        exit(1);

}


void read_input(instance *inst)
{

    FILE *file = fopen(inst->input_file, "r");
    if ( file == NULL )
        print_error("Cannot open the file");


    char *parameter_name;
    int value;
    int active_session = 0;
    char line[180];
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

            parameter_name = strtok(line, ":");
            value = atoi ( strtok(NULL, ":") );

            printf("Parameter: %s, Value: %i \n", parameter_name, value);

            if ( strncmp(parameter_name, "NUMBER_OF_INTERVAL_PRICES", 26) == 0 ) {

                inst->nof_interval_prices = value;
                // calloc inizializza già a zero, malloc la inizializza vuota
                inst->earliest_interval_sm1 = (int *)calloc(inst->nof_interval_prices, sizeof(int));
                inst->latest_interval_sm1 = (int *)calloc(inst->nof_interval_prices, sizeof(int));
                inst->price_interval_sm1 = (double *)calloc(inst->nof_interval_prices, sizeof(double));;

            }

            if ( strncmp(parameter_name, "X_AVARAGE", 10) == 0 ) {

                inst->x_avarage = value;
            }

            if ( strncmp(parameter_name, "MODEL_TYPE", 11) == 0 ) {

                inst->model_type = value;
            }

            if ( strncmp(parameter_name, "SUB_PERIODS", 12) == 0 ) {

                inst->nof_subperiods = value;
                inst->start_subperiod_sm2 = (int *)calloc(inst->nof_subperiods, sizeof(int));
                inst->end_subperiod_sm2 = (int *)calloc(inst->nof_subperiods, sizeof(int));
                inst->maximum_prices_sm2 = (double *)calloc(inst->nof_subperiods, sizeof(double));
                inst->minimum_prices_sm2 = (double *)calloc(inst->nof_subperiods, sizeof(double));

            }


            if ( strncmp(parameter_name, "NUMBER_OF_APPLIANCES", 21) == 0 ) {

                inst->nof_appliances = value;
                inst->start_appliance = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->end_appliance = (int *)calloc(inst->nof_appliances, sizeof(int));

                inst->power_stage1_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage2_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage3_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage4_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage5_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage6_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage7_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));
                inst->power_stage8_sm6 = (int *)calloc(inst->nof_appliances, sizeof(int));

            }


            if ( strncmp(parameter_name, "NUMBER_OF_POWER_LEVELS", 23) == 0 ) {

                inst->nof_powerlevels = value;
                inst->prices_powerlevel_sm3 = (double *)calloc(inst->nof_powerlevels, sizeof(double));
                inst->maximum_power_sm3 = (int *)calloc(inst->nof_powerlevels, sizeof(int));
            }

            if ( strncmp(parameter_name, "NUMBER_OF_INTERVAL_POWER_REQUESTED", 35) == 0 ) {

                inst->nof_intervals_power_requested = value;
                inst->start_interval_pr_sm4 = (int *)calloc(inst->nof_powerlevels, sizeof(int));
                inst->end_interval_pr_sm4 = (int *)calloc(inst->nof_powerlevels, sizeof(int));
                inst->power_requested_sm4 = (int *)calloc(inst->nof_powerlevels, sizeof(int));
            }



            if ( strncmp(parameter_name, "TABLE_SM", 8) == 0) {
                active_session = 1;
                continue;
            }

        }

        if ( active_session == 1 &&  value == 1 ) {

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

            if (in == inst->nof_interval_prices) active_session = 0;

        }


        if ( active_session == 1 &&  value == 2 ) {

            //Primo token
            token = strtok(line, " ");
            int in = atoi ( token );
            //printf( " %i\n", in - 1 );

            token = strtok(NULL, " ");
            inst->start_subperiod_sm2[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->end_subperiod_sm2[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->minimum_prices_sm2[in - 1] = atof( token );

            token = strtok(NULL, " ");
            inst->maximum_prices_sm2[in - 1] = atof( token );

            //printf("Test: %lf \n", inst->maximum_prices_sm2[in - 1]);
            if (in == inst->nof_subperiods) active_session = 0;
        }

        if ( active_session == 1 && value == 3 ) {

            //Primo token
            token = strtok(line, " ");
            int in = atoi ( token );
            //printf( " %i\n", in - 1 );

            token = strtok(NULL, " ");
            inst->prices_powerlevel_sm3[in - 1] = atof( token );

            token = strtok(NULL, " ");
            inst->maximum_power_sm3[in - 1] = atoi( token );

            if (in == inst->nof_powerlevels) active_session = 0;
        }

        if ( active_session == 1 && value == 4 ) {

            //Primo token
            token = strtok(line, " ");
            int in = atoi ( token );
            //printf( " %i\n", in - 1 );

            token = strtok(NULL, " ");
            inst->start_interval_pr_sm4[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->end_interval_pr_sm4[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_requested_sm4[in - 1] = atoi( token );

            if (in == inst->nof_powerlevels) active_session = 0;
        }

        if ( active_session == 1 && value == 5 ) {

            //Primo token
            token = strtok(line, " ");
            int in = atoi ( token );
            //printf( " %i\n", in - 1 );

            token = strtok(NULL, " ");
            inst->start_appliance[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->end_appliance[in - 1] = atoi( token );

            if (in == inst->nof_appliances) active_session = 0;

        }

        if ( active_session == 1 && value == 6 ) {

            //Primo token
            token = strtok(line, " ");
            int in = atoi ( token );
            //printf( " %i\n", in - 1 );

            token = strtok(NULL, " ");
            inst->power_stage1_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage2_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage3_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage4_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage5_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage6_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage7_sm6[in - 1] = atoi( token );

            token = strtok(NULL, " ");
            inst->power_stage8_sm6[in - 1] = atoi( token );


            if (in == inst->nof_appliances) active_session = 0;
        }

        if ( value == 7 ) {
            continue;
        }
        if ( value == 8 ) {
            continue;
        }


    }


}