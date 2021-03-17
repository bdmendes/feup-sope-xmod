#ifndef SRC_SIGNAL_SIG_H_
#define SRC_SIGNAL_SIG_H_

#include <stdbool.h>

void handler_SIGINT(int signo);

void set_file(char* file);

void set_nftot();

void set_nfmod();

bool get_running();

#endif  // SRC_SIGNAL_SIG_H_
