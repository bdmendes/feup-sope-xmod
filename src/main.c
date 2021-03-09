
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "xmod.h"

int retrieve_file_mode(const char *path_name, mode_t *file_mode);
void assemble_file_info(FileInfo *file_info, const mode_t mode,
                        const char *path);
void destroy_file_info(FileInfo *file_info);

int main(int argc, char **argv) {
    char *file_path = argv[1];
    mode_t mode = 0;
    retrieve_file_mode(file_path, &mode);
    FileInfo *file_info = (FileInfo *)malloc(sizeof(FileInfo));
    assemble_file_info(file_info, mode, file_path);
    printf("Relative path: %s\n", file_info->path);
    printf("Old mode: %o\n", file_info->old_mode);
    printf("File type: %d\n", file_info->type);
    destroy_file_info(file_info);
    return 0;
}

int retrieve_file_mode(const char *path_name, mode_t *file_mode) {
    struct stat sb;
    if (stat(path_name, &sb) < 0) {
        perror("stat");
        return 1;
    }
    *file_mode = sb.st_mode;
    return 0;
}

void assemble_file_info(FileInfo *file_info, const mode_t mode,
                        const char *path) {
    file_info->path = path;
    file_info->old_mode = mode & 0x1FF;

    if (S_ISREG(mode))
        file_info->type = REGULAR;
    else if (S_ISDIR(mode))
        file_info->type = DIRECTORY;
    else if (S_ISCHR(mode))
        file_info->type = CHAR_SPECIAL;
    else if (S_ISBLK(mode))
        file_info->type = BLOCK_SPECIAL;
    else if (S_ISFIFO(mode))
        file_info->type = FIFO;
    else if (S_ISLNK(mode))
        file_info->type = SYMBOLIC_LINK;
    else if (S_ISSOCK(mode))
        file_info->type = SOCKET;
    else
        file_info->type = -1;
}

void destroy_file_info(FileInfo *file_info) {
    free(file_info);
}
