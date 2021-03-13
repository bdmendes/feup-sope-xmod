#ifndef SIG_H
#define SIG_H
#include <stdbool.h>

int set_handler_SIGINT();

void handler_SIGINT(int signo);

void set_file(char* file);

void set_nftot(char* file);

void set_nfmod(char* file);

bool get_running();

#endif