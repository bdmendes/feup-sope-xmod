#ifndef PARSERS_H
#define PARSERS_H

#include <string.h>
#include "xmod.h"
#include "retrievers.h"

int parse(char **argv, XmodCommand *xmodCommand);

int transform_symbolic_mode_to_octal_mode(const char *symbolic_mode, OctalNumber *old_digits, OctalNumber *new_digits);

#endif
