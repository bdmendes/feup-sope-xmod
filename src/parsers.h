#ifndef PARSERS_H
#define PARSERS_H

#include <string.h>
#include "xmod.h"

typedef struct XmodPermissionsTypes{
    bool read;
    bool write;
    bool execute;
} XmodPermissionsTypes;

typedef struct FilePermissions{
    struct XmodPermissionsTypes user;
    struct XmodPermissionsTypes group;
    struct XmodPermissionsTypes other;
}FilePermissions;

int parse(char **argv, XmodCommand *xmodCommand);

int update_permissions(const char *symbolic_mode, FilePermissions *octal_mode);

mode_t get_octal_mode(FilePermissions *permissions);

#endif
