#ifndef XMOD_H
#define XMOD_H

#include <fcntl.h>

#include <stdbool.h>
#include <sys/types.h>

//static const char *PROGRAM_NAME = "xmod";

//enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE };

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

typedef struct XmodPermissions{
    bool read;
    bool write;
    bool execute;
    bool all;
}XmodPermissions;

typedef struct OctalNumber{
    mode_t ls;
    mode_t middle;
    mode_t ms;
}OctalNumber;

typedef struct XmodCommand {
    //enum XMOD_MODE_TYPE mode_type;
    mode_t octal_mode;
    struct XmodOptions options;
    char *file_dir;
} XmodCommand;

typedef struct FileInfo {
    const char *path;
    mode_t octal_mode;
    enum FILE_TYPE file_type;
} FileInfo;

#endif