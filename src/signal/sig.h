#ifndef SIG_H
#define SIG_H
#include <stdbool.h>

int set_handler();

void handler(int signo);

void set_file(char* file);

void set_nftot();

void set_nfmod();

bool get_running();

#endif