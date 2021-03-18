#ifndef SRC_SIGNAL_SIG_H_
#define SRC_SIGNAL_SIG_H_

#include <stdbool.h>

/**
 * @brief Sets the handlers for all possible signals
 * @param curr_path Path used in the creation of the process
 * @return 0 on sucess, -1 otherwise
 */
int setup_signal_handler();

/**
 * @brief Increments by one the variable with the number of founded files
 */
void increment_nftot();

/**
 * @brief Increments by one the variable with the number of modified files
 */
void increment_nfmod();

unsigned get_nftot();

unsigned get_nfmod();

/**
 * @brief Set file path
 * @param path Path name
 */
void set_sig_file_path(char path[]);

#endif // SRC_SIGNAL_SIG_H_
