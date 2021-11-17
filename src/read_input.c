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

        if (strcmp(argv[i], "-time_limit") == 0) {
            inst->timelimit = atoi(argv[++i]);
            printf("Time limit selected: %f\n", inst->timelimit);
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

                inst->L = value;
                inst->table_sm2 = calloc(inst->L, sizeof(TABLE_SM2));
            }

            // TABLE SM3
            if ( strncmp(parameter_name, "NUMBER_OF_BASE_LOAD_INTERVALS", 29) == 0 ) {

                inst->nof_baseloadintervals = value;
                inst->table_sm3 = calloc(inst->nof_baseloadintervals, sizeof(TABLE_SM3));
            }

            // TABLE SM5
            if ( strncmp(parameter_name, "NUMBER_OF_APPLIANCES", 21) == 0 ) {

                inst->J = value;
                inst->table_sm5 = calloc(inst->J, sizeof(TABLE_SM5));
                inst->table_sm20 = calloc(inst->J, sizeof(TABLE_SM20));
            }

            // TABLE SM6
            if ( strncmp(parameter_name, "NUMBER_OF_APPLIANCE_STAGES", 26) == 0 ) {

                inst->nof_appliancestages = value;
                inst->table_sm6 = calloc(inst->J, sizeof(TABLE_SM6));
            }

            // TABLE SM8
            if ( strncmp(parameter_name, "NUMBER_OF_WATERWITHDRAWL_INTERVALS", 34) == 0 ) {

                inst->nof_waterwithdrawl_intervals = value;
                inst->table_sm8 = calloc( value , sizeof(TABLE_SM8));
            }

            // TABLE SM9
            if ( strncmp(parameter_name, "NUMBER_OF_AMBIENTTEMPERATUREEWH_INTERVALS", 41) == 0 ) {

                inst->nof_amb_temp_intervals = value;
                inst->table_sm9 = calloc( value , sizeof(TABLE_SM9));
            }

            // TABLE SM11
            if ( strncmp(parameter_name, "NUMBER_OF_EXTERNAL_TEMPERATURE_INTERVALS", 40) == 0 ) {

                inst->nof_outdoor_temperature_intervals= value;
                inst->table_sm11 = calloc( value , sizeof(TABLE_SM11));
            }

            if ( strncmp(parameter_name, "MAX_DJ", 6) == 0 ) {

                inst->MAX_DJ = value;
            }

            if ( strncmp(parameter_name, "MAX_PG", 6) == 0 ) {

                inst->MAX_PG= value;
            }

            if ( strncmp(parameter_name, "INITIAL_OUTDOOR_TEMPERATURE", 27) == 0 ) {

                inst->INITIAL_OUTDOOR_TEMPERATURE = value / 100;
            }

            if ( strncmp(parameter_name, "TIME", 4) == 0 ) {

                inst->T = value;
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
            //printf("Test: %lf \n", inst->table_sm2[in - 1].price_at_day);

            if (in == inst->L) active_session = 0;
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

            if (in == inst->nof_baseloadintervals) active_session = 0;
        }


        if ( active_session == 1 && value == 5 ) {

            token = strtok(line, " ");
            int in = atoi ( token );

            token = strtok(NULL, " ");
            inst->table_sm5[in - 1].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm5[in - 1].end_inteval= atoi( token );

            if (in == inst->J) active_session = 0;
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

            if (in == inst->J) active_session = 0;
        }


        if ( active_session == 1 && value == 20 ) {

            token = strtok(line, " ");
            int in = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm20[in - 1].duration = atoi( token );

            if (in == inst->J) active_session = 0;
        }

        if ( active_session == 1 && value == 7 ) {

            token = strtok(line, " ");
            inst->table_sm7.power_required = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.M = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.AU = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm7.termal_coeff = atof( token );
            // La SOURCE
            token = strtok(NULL, " ");
            inst->table_sm7.inlet_temperature = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.min_temperature = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.max_temperature = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.required_temperature = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.required_time = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.initial_temperature = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.initial_on_off = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm7.initial_power_losses = atoi( token );

        }

        if ( active_session == 1 && value == 8 ) {

            token = strtok(line, " ");
            int in = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm8[ in - 1 ].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm8[ in - 1 ].end_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm8[ in - 1 ].kg_water = atof( token );

            if (in == inst->nof_waterwithdrawl_intervals) active_session = 0;

        }

        if ( active_session == 1 && value == 9 ) {

            token = strtok(line, " ");
            int in = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm9[ in - 1 ].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm9[ in - 1 ].end_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm9[ in - 1 ].temperature = atof( token );

            if (in == inst->nof_amb_temp_intervals) active_session = 0;

        }

        if ( active_session == 1 && value == 10 ) {

            token = strtok(line, " ");
            inst->table_sm10.maximum_temperature_allowed = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm10.minimum_temperature_allowed = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm10.initial_indoor_temperature = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm10.nominal_power_AC = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm10.initial_start = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm10.alpha = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm10.beta = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm10.gamma = atof( token );

        }

        if ( active_session == 1 && value == 11 ) {

            token = strtok(line, " ");
            int in = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm11[ in - 1 ].start_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm11[ in - 1 ].end_interval = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm11[ in - 1 ].outdoor_temperature = atof( token );

            if (in == inst->nof_outdoor_temperature_intervals) active_session = 0;

        }

        if ( active_session == 1 && value == 12 ) {

            token = strtok(line, " ");
            inst->table_sm12.sb_charging_efficiency = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_discharging_efficiency = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_minimum_charge = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_maximum_charge = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_maximum_charge_power = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_maximum_discharge_power = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm12.sb_initial_battery_charge = atoi( token );
        }

        if ( active_session == 1 && value == 13 ) {

            token = strtok(line, " ");
            inst->table_sm13.ev_charging_efficiency = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_discharging_efficiency = atof( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_minimum_charge = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_maximum_charge = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.requested_charge = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_maximum_charge_power = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_maximum_discharge_power = atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.time_arrival= atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.time_departure= atoi( token );

            token = strtok(NULL, " ");
            inst->table_sm13.ev_initial_battery_charge = atoi( token );
        }


    }


}