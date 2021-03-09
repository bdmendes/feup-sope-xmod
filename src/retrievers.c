#include <stdio.h>

#include "retrievers.h"
#include "xmod.h"

void retrieve_file_info(FileInfo *file_info, char *file_path) {
    mode_t mode = 0;
    retrieve_file_mode(file_path, &mode);
    assemble_file_info(file_info, mode, file_path);
}

void assemble_file_info(FileInfo *file_info, const mode_t mode,
                        const char *path) {
    file_info->path = path;
    file_info->octal_mode = mode & 0777;

    if (S_ISREG(mode))
        file_info->file_type = REGULAR;
    else if (S_ISDIR(mode))
        file_info->file_type = DIRECTORY;
    else if (S_ISCHR(mode))
        file_info->file_type = CHAR_SPECIAL;
    else if (S_ISBLK(mode))
        file_info->file_type = BLOCK_SPECIAL;
    else if (S_ISFIFO(mode))
        file_info->file_type = FIFO;
    else if (S_ISLNK(mode))
        file_info->file_type = SYMBOLIC_LINK;
    else if (S_ISSOCK(mode))
        file_info->file_type = SOCKET;
    else
        file_info->file_type = -1;
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
