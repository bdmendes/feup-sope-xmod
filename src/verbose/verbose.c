
#include "verbose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void get_symbolic_string(mode_t mode, char *string) {
    string[USER * 3 + READ] = mode & S_IRUSR ? 'r' : '-';
    string[USER * 3 + WRITE] = mode & S_IWUSR ? 'w' : '-';
    string[USER * 3 + EXECUTE] = mode & S_IXUSR ? 'x' : '-';

    string[GROUP * 3 + READ] = mode & S_IRGRP ? 'r' : '-';
    string[GROUP * 3 + WRITE] = mode & S_IWGRP ? 'w' : '-';
    string[GROUP * 3 + EXECUTE] = mode & S_IXGRP ? 'x' : '-';

    string[OTHER * 3 + READ] = mode & S_IROTH ? 'r' : '-';
    string[OTHER * 3 + WRITE] = mode & S_IWOTH ? 'w' : '-';
    string[OTHER * 3 + EXECUTE] = mode & S_IXOTH ? 'x' : '-';

    string[NUMBER_OF_PERM_FIELDS] = '\0';
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
