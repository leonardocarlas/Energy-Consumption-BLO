#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "utils.h"


void print_error(const char *err) {

    printf(" \n\n Error: %s \n\n ", err);
    fflush(NULL);
    exit(1);
}


void free_instance(instance *inst) {

    free(inst->earliest_interval_sm1);
    free(inst->latest_interval_sm1);
    free(inst->price_interval_sm1);
}

void debug() {

    printf("Debug \n");
}