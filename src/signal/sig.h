#ifndef SRC_SIGNAL_SIG_H_
#define SRC_SIGNAL_SIG_H_

#include <stdbool.h>

/**
 * @brief Sets the handlers for all possible signals
 * @param curr_path Path used in the creation of the process
 * @return 0 on sucess, -1 otherwise
 */
int setup_signal_handler(char curr_path[]);

/**
 * @brief Increments by one the variable with the number of founded files
 */
void set_nftot();

/**
 * @brief Increments by one the variable with the number of modified files
 */
void set_nfmod();

#endif // SRC_SIGNAL_SIG_H_

