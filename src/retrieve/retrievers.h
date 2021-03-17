#ifndef SRC_RETRIEVE_RETRIEVERS_H_
#define SRC_RETRIEVE_RETRIEVERS_H_

#include <sys/stat.h>

typedef unsigned char file_type;

typedef struct {
    const char *path;
    mode_t octal_mode;
    file_type type;
} FileInfo;

int retrieve_file_info(FileInfo *file_info, char *file_path);

int retrieve_file_mode(const char *path_name, mode_t *file_mode);

#endif  // SRC_RETRIEVE_RETRIEVERS_H_
