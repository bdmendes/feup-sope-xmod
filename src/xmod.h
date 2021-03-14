#ifndef XMOD_H
#define XMOD_H

#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>

enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE };

enum XMOD_USER_TYPE { USER, GROUP, OTHER, ALL };

enum XMOD_PERMISSION { READ, WRITE, EXECUTE };

#define NUMBER_OF_PERM_FIELDS 9

struct XmodOptions {
    bool changes;
    bool verbose;
    bool recursive;
};

typedef struct OctalNumber {
    mode_t ls;
    mode_t middle;
    mode_t ms;
} OctalNumber;

typedef struct XmodCommand {
    enum XMOD_MODE_TYPE mode_type;
    mode_t octal_mode;
    struct XmodOptions options;
    char file_dir[PATH_MAX];
    unsigned file_idx;
} XmodCommand;

#endif
