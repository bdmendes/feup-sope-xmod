#ifndef SRC_INPUT_VALIDATION_INPUT_VALIDATION_H_
#define SRC_INPUT_VALIDATION_INPUT_VALIDATION_H_

#include <stdbool.h>

/**
* @brief Checks whether argv is a valid input in the xmod context. 
* 
* @param argv
* @param argc
* @return true, if it is invalid; false, otherwise
*/
bool is_invalid_input(char** argv, int argc);

#endif  // SRC_INPUT_VALIDATION_INPUT_VALIDATION_H_