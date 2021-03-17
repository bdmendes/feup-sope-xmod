#ifndef PARSERS_H
#define PARSERS_H

#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>

typedef struct {
    bool read;
    bool write;
    bool execute;
} PermissionTypes;

typedef struct {
    PermissionTypes owner;
    PermissionTypes group;
    PermissionTypes other;
} FilePermissions;

typedef struct {
    mode_t octal_mode;
    struct {
        bool changes;
        bool verbose;
        bool recursive;
    } options;
    char file_dir[PATH_MAX];
    unsigned file_idx;
} XmodCommand;

void parse(char **argv, XmodCommand *xmodCommand);

/**
    * Assembles an octal number from a struct with all the three users' permissions.
    * 
    * @param permissions struct with all the three users' permissions.
    * @return octal number
*/
mode_t get_octal_mode(FilePermissions *permissions);

#endif
