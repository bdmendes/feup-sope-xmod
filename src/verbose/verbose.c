#include "verbose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief Get the symbolic string in the 'rwxrwxrwx' format given the octal
 * permission mode
 *
 * @param mode permission mode (octal number)
 * @param str buffer where the returned string is stored.
 */
static void get_symbolic_string(mode_t mode, char *str) {
    str[OWNER * 3 + READ] = mode & S_IRUSR ? 'r' : '-';
    str[OWNER * 3 + WRITE] = mode & S_IWUSR ? 'w' : '-';
    str[OWNER * 3 + EXECUTE] = mode & S_IXUSR ? 'x' : '-';

    str[GROUP * 3 + READ] = mode & S_IRGRP ? 'r' : '-';
    str[GROUP * 3 + WRITE] = mode & S_IWGRP ? 'w' : '-';
    str[GROUP * 3 + EXECUTE] = mode & S_IXGRP ? 'x' : '-';

    str[OTHER * 3 + READ] = mode & S_IROTH ? 'r' : '-';
    str[OTHER * 3 + WRITE] = mode & S_IWOTH ? 'w' : '-';
    str[OTHER * 3 + EXECUTE] = mode & S_IXOTH ? 'x' : '-';

    str[NUMBER_OF_PERM_FIELDS] = '\0';
}

void print_verbose_message(const char *file_name, mode_t old_mode,
                           mode_t new_mode, bool changed, int success) {
    char old_mode_str[NUMBER_OF_PERM_FIELDS + 1];
    char new_mode_str[NUMBER_OF_PERM_FIELDS + 1];
    get_symbolic_string(old_mode, old_mode_str);
    get_symbolic_string(new_mode, new_mode_str);

    if (success < 0) {
        printf("failed to change mode of '%s' from %04o (%s) to %04o (%s)\n",
               file_name, old_mode, old_mode_str, new_mode, new_mode_str);
    } else if (changed) {
        printf("mode of '%s' changed from %04o (%s) to %04o (%s)\n", file_name,
               old_mode, old_mode_str, new_mode, new_mode_str);
    } else {
        printf("mode of '%s' retained as %04o (%s)\n", file_name, old_mode,
               old_mode_str);
    }
}
