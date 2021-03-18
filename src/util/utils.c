#include "utils.h"

#include <ctype.h>
#include <string.h>

bool is_ref_path(char *path) {
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

/**
 * @brief Verifies if the argument is an option flag.
 *
 * @param c option flag
 * @return true, if the argument is an option flag; false, otherwise
 */
static bool is_options_flag(char c) {
    return c == 'R' || c == 'v' || c == 'c';
}

bool is_flag_arg(char *argument) {
    if (argument[0] != '-')
        return false;
    for (int i = 1; i < strlen(argument); ++i) {
        if (!is_options_flag(argument[i]))
            return false;
    }
    return true;
}

bool is_number_arg(char *argument) {
    for (int i = 0; i < strlen(argument); ++i) {
        if (!isdigit(argument[i]))
            return false;
    }
    return true;
}

/**
 * @brief Verifies if the argument is an pemrission flag.
 *
 * @param c pemrission flag
 * @return true, if the argument is an pemrission flag; false, otherwise
 */
static bool is_permission_flag(char c) {
    return c == 'r' || c == 'w' || c == 'x';
}

bool has_permissions_flags(const char *argument, int start_index) {
    for (int i = start_index; i < strlen(argument); ++i) {
        if (!is_permission_flag(argument[i]))
            return false;
    }
    return true;
}

bool is_permission_operator(char c) {
    return c == '+' || c == '-' || c == '=';
}

bool is_user_flag(char c) {
    return c == 'u' || c == 'g' || c == 'o' || c == 'a';
}

bool has_irregular_coma(const char str[]) {
    if (str[0] == ',' || str[strlen(str) - 1] == ',') {
        return true;
    }
    bool repeated = false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            if (repeated)
                return true;
            repeated = true;
        } else {
            repeated = false;
        }
    }
    return false;
}

void strip_trailing_slashes(char str[]) {
    for (int i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] != '/') {
            str[i + 1] = '\0';
            return;
        }
    }
}
