#ifndef VERBOSE_H
#define VERBOSE_H

#include <stdbool.h>
#include <sys/types.h>

void print_verbose_message(const char *file_name, mode_t old_mode,
                           mode_t new_mode, bool changed, int success);

#endif