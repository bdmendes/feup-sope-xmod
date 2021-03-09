#ifndef RETRIEVERS_H
#define RETRIEVERS_H

#include <sys/stat.h>
#include "xmod.h"

void retrieve_file_info(FileInfo *file_info, char *file_path);

int retrieve_file_mode(const char *path_name, mode_t *file_mode);

void assemble_file_info(FileInfo *file_info, const mode_t mode, const char *path);

#endif