#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        // Will end the reading of the file
        if (strcmp(line, "EOF") == 0) {
            break;
        }

        if(strchr(line, ':') != NULL) {

            parameter_name = strtok(line, ":");
            value = atoi ( strtok(NULL, ":") );

            printf("Parameter: %s, Value: %i \n", parameter_name, value);

            // TABLE SM1
            if ( strncmp(parameter_name, "NUMBER_OF_SUB_PERIODS", 21) == 0 ) {

                inst->nof_subperiods = value;
                inst->table_sm1 = calloc(inst->nof_subperiods, sizeof(TABLE_SM1));
            }

            // TABLE SM2
            if ( strncmp(parameter_name, "NUMBER_OF_POWER_LEVELS", 23) == 0 ) {

                inst->nof_powerlevels = value;
                inst->table_sm2 = calloc(inst->nof_powerlevels, sizeof(TABLE_SM2));
            }

            // TABLE SM3
            if ( strncmp(parameter_name, "NUMBER_OF_BASE_LOAD_INTERVALS", 29) == 0 ) {

                inst->nof_baseloadintervals = value;
                inst->table_sm3 = calloc(inst->nof_baseloadintervals, sizeof(TABLE_SM3));
            }

            // TABLE SM5
            if ( strncmp(parameter_name, "NUMBER_OF_APPLIANCES", 21) == 0 ) {

                inst->nof_appliances = value;
                inst->table_sm5 = calloc(inst->nof_appliances, sizeof(TABLE_SM5));
            }

            // TABLE SM6
            if ( strncmp(parameter_name, "NUMBER_OF_APPLIANCE_STAGES", 26) == 0 ) {

                inst->nof_appliancestages = value;
                inst->table_sm6 = calloc(inst->nof_appliances, sizeof(TABLE_SM6));
            }

            if ( strncmp(parameter_name, "MAX_DJ", 6) == 0 ) {

                inst->max_dj = value;
            }

            if ( strncmp(parameter_name, "MODEL_TYPE", 11) == 0 ) {

                inst->model_type = value;
            }

            if ( strncmp(parameter_name, "TABLE_SM", 8) == 0) {
                active_session = 1;
                continue;
            }

        }


        if ( active_session == 1 &&  value == 1 ) {

            // Numero di sub-period
            token = strtok(line, " ");
            int in = atoi ( token );

            token = strtok(NULL, " ");
            inst->table_sm1[in - 1].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm1[in - 1].end_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm1[in - 1].price_subperiod = atof( token );
            //printf( " %lf\n", inst->table_sm1[in - 1].price_subperiod);

            if (in == inst->nof_subperiods) active_session = 0;

        }

        if ( active_session == 1 &&  value == 2 ) {

            token = strtok(line, " ");
            int in = atoi ( token );
            inst->table_sm2[in - 1].level = in;

            token = strtok(NULL, " ");
            inst->table_sm2[in - 1].price_at_day = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm2[in - 1].watt= atof( token );
            //printf("Test: %lf \n", inst->table_sm2[in - 1].watt);

            if (in == inst->nof_subperiods) active_session = 0;
        }

        if ( active_session == 1 && value == 3 ) {

            token = strtok(line, " ");
            int in = atoi ( token );

            token = strtok(NULL, " ");
            inst->table_sm3[in - 1].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm3[in - 1].end_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm3[in - 1].power_required = atoi( token );

            if (in == inst->nof_powerlevels) active_session = 0;
        }



        if ( active_session == 1 && value == 5 ) {

            token = strtok(line, " ");
            int in = atoi ( token );

            token = strtok(NULL, " ");
            inst->table_sm5[in - 1].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm5[in - 1].end_inteval= atoi( token );

            if (in == inst->nof_appliances) active_session = 0;
        }


        if ( active_session == 1 && value == 6 ) {

            token = strtok(line, " ");
            int in = atoi ( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_first_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_second_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_third_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_fourth_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_fifth_interval= atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_sixth_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm6[in - 1].power_seventh_interval = atoi( token );

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