#ifndef RETRIEVERS_H
#define RETRIEVERS_H

#include <sys/stat.h>

#include "xmod.h"

typedef struct FileInfo {
    const char *path;
    mode_t octal_mode;
    enum FILE_TYPE file_type;
} FileInfo;

int retrieve_file_info(FileInfo *file_info, char *file_path);


#endif