#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "xmod.h"

/*typedef struct{ 
  char* cmd;
  int exit_code;
  char* signal;   //mudar isto para o "tipo" de sinal
  int process_target;  
  char* direct;
  int old_perms;
  int new_perms;
} info;*/

int log_file(FILE* file, enum XMOD_EVENT event, union info inf);

FILE* log_init();

int log_close(FILE* file);

#endif