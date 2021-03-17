#ifndef SRC_VERBOSE_VERBOSE_H_
#define SRC_VERBOSE_VERBOSE_H_

#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief the three kinds of user that can interact with a file (the one who
 * created it, the ones in the same group, others)
 *
 */
enum XMOD_USER_TYPE { OWNER, GROUP, OTHER };

/**
 * @brief
 *
 */
enum XMOD_PERMISSION { READ, WRITE, EXECUTE };

#define NUMBER_OF_PERM_FIELDS 9

/**
 * @brief Print to standard output the corresponding message (mode changed, mode
 * retained, call failed) with the file's permission modes' old and new values.
 *
 * @param file_name name of the file
 * @param old_mode permission mode before the chmod call
 * @param new_mode permission mode after the chmod call
 * @param changed whether or not the file's permission mode was changed by the
 * chmod call
 * @param success whether or not the chmod call was successful
 */
void print_verbose_message(const char *file_name, mode_t old_mode,
                           mode_t new_mode, bool changed, int success);

#endif // SRC_VERBOSE_VERBOSE_H_
