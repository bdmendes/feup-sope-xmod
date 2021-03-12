#ifndef PARSERS_H
#define PARSERS_H

#include <string.h>
#include "xmod.h"
#include "retrievers.h"

struct XmodPermissionsTypes{
    bool read;
    bool write;
    bool execute;
};

typedef struct FilePermissions{
    struct XmodPermissionsTypes user_permissions;
    struct XmodPermissionsTypes group_permissions;
    struct XmodPermissionsTypes other_permissions;
}FilePermissions;

int parse(char **argv, XmodCommand *xmodCommand);

int transform_symbolic_mode_to_octal_mode(const char *symbolic_mode, FilePermissions *old_mode, FilePermissions *new_mode);

#endif
