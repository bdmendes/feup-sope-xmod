#ifndef SRC_VERBOSE_VERBOSE_H_
#define SRC_VERBOSE_VERBOSE_H_

#include <stdbool.h>
#include <sys/types.h>

enum XMOD_USER_TYPE { OWNER, GROUP, OTHER };

enum XMOD_PERMISSION { READ, WRITE, EXECUTE };

#define NUMBER_OF_PERM_FIELDS 9

void print_verbose_message(const char *file_name, mode_t old_mode,
                           mode_t new_mode, bool changed, int success);

#endif // SRC_VERBOSE_VERBOSE_H_
