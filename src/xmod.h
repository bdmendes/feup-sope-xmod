#ifndef XMOD_H
#define XMOD_H

#include <stdbool.h>
#include <fcntl.h>

#include <stdbool.h>
#include <sys/types.h>

//static const char *PROGRAM_NAME = "xmod";

enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE };

enum XMOD_USER_TYPE { USER, GROUP, OTHER, ALL };

enum FILE_TYPE { 
    REGULAR,
    DIRECTORY,
    CHAR_SPECIAL,
    BLOCK_SPECIAL,
    FIFO,
    SYMBOLIC_LINK,
    SOCKET
};

struct XmodOptions {
    bool changes;
    bool verbose;
    bool recursive;
};

struct XmodPermissions{
    bool read;
    bool write;
    bool execute;
};

struct XmodSymbolicMode{
    enum XMOD_USER_TYPE user_type;
    struct XmodPermissions permissions;
};

union XmodMode {
    struct XmodSymbolicMode symbolic_mode;
    mode_t octal_mode;
};

typedef struct XmodCommand {
    enum XMOD_MODE_TYPE mode_type;
    union XmodMode mode;
    struct XmodOptions options;
    char *file_dir;
} XmodCommand;

typedef struct FileInfo {
    const char *path;
    mode_t octal_mode;
    enum FILE_TYPE file_type;
} FileInfo;

#endif
