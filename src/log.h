#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "xmod.h"


int log_file(FILE* file, enum XMOD_EVENT event, union info inf);

FILE* log_init();

int log_close(FILE* file);

#endif