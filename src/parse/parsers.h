#ifndef PARSERS_H
#define PARSERS_H

#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>

/**
* Struct which encapsulates the permissions' flags.
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
* Struct which encapsulates all the permissions for the three user types.
*/ 
typedef struct {
    /**
     * Struct containing the owner permissions.
    */
    PermissionTypes owner;
     /**
     * Struct containing the group permissions.
    */
    PermissionTypes group;
     /**
     * Struct containing the other permissions.
    */
    PermissionTypes other;
} FilePermissions;

/**
* Struct which encapsulates all arguments received by input.
*/ 
typedef struct {
    /**
     * Octal mode.
     */
    mode_t octal_mode;

    /**
     * Struct which encapsulates the options' flags.
     */
    struct {
        bool changes;
        bool verbose;
        bool recursive;
    } options;

    /**
     *  Path to the file/directory.
     */
    char file_dir[PATH_MAX];

    /**
     * Index of the file path in argv.
     */
    unsigned file_idx;
} XmodCommand;

/**
* Parses argv into a xmod command struct
* 
* @param argv argv
* @param xmodCommand xmod command arguments
*/
void parse(char **argv, XmodCommand *xmodCommand);

/**
* Assembles an octal number from a struct with all the three users' permissions.
* 
* @param permissions struct with all the three users' permissions.
* @return octal number
*/
mode_t get_octal_mode(FilePermissions *permissions);

#endif
