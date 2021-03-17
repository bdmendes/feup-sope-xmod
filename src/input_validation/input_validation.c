#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../retrieve/retrievers.h"
#include "../util/utils.h"
#include "input_validation.h"

/**
 * Checks whether the octal number received by input is valid in the xmod
 * context.
 *
 * @param octal_number octal number to be tested
 * @return true, if it is invalid; false, otherwise
 */
static bool is_invalid_octal_number(const char *octal_number) {
    // xmod octal mode has to start with '0'
    if (octal_number[0] != '0' || strtoul(octal_number, NULL, 8) > 0777)
        return true;
    for (int i = strlen(octal_number) - 1; i >= 0; --i)
        if (octal_number[i] > '7')
            return true;

    return false;
}
/**
 * Checks whether the symbolic mode received by input is valid.
 *
 * @param symbolic_mode symbolic mode to be tested
 * @return true, if it is invalid; false, otherwise
 */
static bool is_invalid_symbolic_mode(char *symbolic_mode) {
    if (has_irregular_coma(symbolic_mode))
        return true;

    const char sep[2] = ",";
    char *pos = NULL;
    for (char *i = strtok_r(symbolic_mode, sep, &pos); i != NULL;) {
        if (strlen(i) < 2) { // operator and permission are not provided
            return true;
        }

        int operator_index = 0;
        if (!is_permission_operator(i[0])) {
            if (strlen(i) < 3 || !is_user_flag(i[0]))
                return true;
            operator_index++;
        }
        if (!is_permission_operator(i[operator_index]) ||
            !has_permissions_flags(i, operator_index + 1)) {
            return true;
        }
        i = strtok_r(NULL, sep, &pos);
    }
    return false;
}

bool is_invalid_input(char **argv, int argc) {
    if (argc < 3) {
        printf("xmod: missing operand\n");
        return true;
    }
    int mode_index = 1;

    // Check for valid option flags
    while (argv[mode_index][0] == '-') {
        if (has_permissions_flags(argv[mode_index], 1))
            break;

        if (!is_flag_arg(argv[mode_index])) {
            printf("xmod: invalid option -- '%s'\n", argv[mode_index]);
            return true;
        }
        mode_index++;
    }

    // Check for valid mode
    char mode_str[strlen(argv[mode_index]) + 1];
    snprintf(mode_str, sizeof(mode_str), "%s", argv[mode_index]);

    if (is_number_arg(mode_str) ? is_invalid_octal_number(mode_str)
                                : is_invalid_symbolic_mode(mode_str)) {
        printf("xmod: invalid mode '%s'\n", argv[mode_index]);
        return true;
    }

    // Check for invalid number of arguments
    if (argc > mode_index + 2) {
        printf("xmod: too many arguments: multiple file paths are not "
               "supported\n");
        return true;
    }

    return false;
}
