#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <stdbool.h>

/**
    *Checks whether argv is a valid input in the xmod context. 
    * 
    * @param argv
    * @param argc
    * @return true, if it is invalid; false, otherwise
*/
bool is_invalid_input(char** argv, int argc);

#endif