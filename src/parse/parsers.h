#ifndef SRC_PARSE_PARSERS_H_
#define SRC_PARSE_PARSERS_H_

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief Struct which encapsulates the permissions' flags.
 */
typedef struct {
    /**
     * Read flag.
     */
    bool read;
    /**
     * Write flag.
     */
    bool write;
    /**
     * Execute flag.
     */
    bool execute;
} PermissionTypes;

/**
 * @brief Struct which encapsulates all the permissions for the three user
 * types.
 */
typedef struct {
    /**
     * @brief Struct containing the owner permissions.
     */
    PermissionTypes owner;
    /**
     * @brief Struct containing the group permissions.
     */
    PermissionTypes group;
    /**
     * @brief Struct containing the other permissions.
     */
    PermissionTypes other;
} FilePermissions;

/**
 * @brief Struct which encapsulates all arguments received by input.
 */
typedef struct {
    /**
     * @brief Octal mode.
     */
    mode_t octal_mode;

    /**
     * @brief Struct which encapsulates the options' flags.
     */
    struct {
        /**
         * @brief changes flag
         */
        bool changes;
        /**
         * @brief verbose flag
         */
        bool verbose;
        /**
         * @brief recursive flag
         */
        bool recursive;
    } options;

    /**
     *  @brief Path to the file/directory.
     */
    char file_dir[PATH_MAX];

    /**
     * @brief Index of the file path in argv.
     */
    unsigned file_idx;
} XmodCommand;

/**
 * @brief Parses argv into a xmod command struct
 *
 * @param argv argv
 * @param xmodCommand xmod command arguments
 */
void parse(char **argv, XmodCommand *xmodCommand);

/**
 * @brief Assembles an octal number from a struct with all the three users'
 * permissions.
 *
 * @param permissions struct with all the three users' permissions.
 * @return octal number
 */
mode_t get_octal_mode(FilePermissions *permissions);

#endif // SRC_PARSE_PARSERS_H_
