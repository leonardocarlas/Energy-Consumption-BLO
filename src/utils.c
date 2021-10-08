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

    free(inst->table_sm1);
    free(inst->table_sm2);
    free(inst->table_sm3);
    free(inst->table_sm5);
    free(inst->table_sm6);

}

void debug() {

    printf("Debug \n");
}

int xpos(int j, int r, int t, instance *inst) {
    /*
    if (i == j) print_error("i == j in xpos");
    if (i > j) return xpos(j, i, inst);
    int pos = i * inst->nnodes + j - ((i + 1) * (i + 2) / 2);
    return pos;
     */
    return 0;
}