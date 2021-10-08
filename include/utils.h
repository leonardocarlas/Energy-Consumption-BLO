#include "blo.h"

void print_error(const char *err);






void debug();


/*!
 * Frees the memory of the instance passed as argument
 * @param inst The instance that we want to kill
 * @param free_solution 1 if we want to free the memeory occupied by the solution of cplex
 */
void free_instance(instance *inst);



