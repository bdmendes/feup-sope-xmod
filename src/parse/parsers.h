#ifndef SRC_PARSE_PARSERS_H_
#define SRC_PARSE_PARSERS_H_

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

mode_t get_octal_mode(FilePermissions *permissions);

#endif  // SRC_PARSE_PARSERS_H_
