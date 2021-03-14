#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool is_ref_path(char *path);

bool is_flag_arg(char *argument);

bool is_number_arg(char *argument);

bool has_permissions_flags(const char* argument);

#endif