#ifndef RETRIEVERS_H
#define RETRIEVERS_H

#include <sys/stat.h>

typedef unsigned char file_type;

typedef struct FileInfo {
    const char *path;
    mode_t octal_mode;
    file_type type;
} FileInfo;

int retrieve_file_info(FileInfo *file_info, char *file_path);

#endif