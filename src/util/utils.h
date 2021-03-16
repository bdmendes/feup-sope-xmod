#ifndef SRC_UTIL_UTILS_H_
#define SRC_UTIL_UTILS_H_

#include <stdbool.h>

bool is_ref_path(char *path);

bool is_flag_arg(char *argument);

bool is_number_arg(char *argument);

bool has_permissions_flags(const char *argument, int start_index);

bool has_digits(char *argument);

bool is_permission_operator(char c);

bool is_user_flag(char c);

bool has_irregular_coma(const char str[]);

void strip_trailing_slashes(char str[]);

#endif // SRC_UTIL_UTILS_H_