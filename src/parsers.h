#ifndef MODE_H
#define MODE_H

#include <string.h>
#include "xmod.h"

int read_mode(const char **argv, XmodCommand *xmodCommand);

int parse_symbolic_mode(const char *argv, XmodCommand *xmodCommand);

void parse_options(const char *options, XmodCommand *xmodCommand);

#endif
