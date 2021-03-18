#ifndef SRC_SIGNAL_SIG_H_
#define SRC_SIGNAL_SIG_H_

#include <stdbool.h>

int setup_signal_handler();

void increment_nftot();

void increment_nfmod();

unsigned get_nftot();

unsigned get_nfmod();

void set_sig_file_path(char path[]);

#endif // SRC_SIGNAL_SIG_H_
