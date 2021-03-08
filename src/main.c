
#include <fts.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "parsers.h"

int main(int argc, char **argv) {
    XmodCommand cmd;
    parse(argv, &cmd);
    printf("mode: %o\n", cmd.mode.octal_mode);
    if (chmod(cmd.file_dir, cmd.mode.octal_mode) == -1){
        perror("chmod call failed");
        return -1;
    }
}


/*#include <sys/stat.h>
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
    printf("Old mode: %o\n", file_info->old_mode);
    destroy_file_info(file_info);
    return 0;
}*/

int retrieve_file_mode(const char *path_name, mode_t *file_mode) {
    struct stat sb;
    if (stat(path_name, &sb) < 0) {
        perror("stat");
        return 1;
    }
    *file_mode = sb.st_mode & 0x1FF;
    return 0;
}

void assemble_file_info(FileInfo *file_info, const mode_t mode,
                        const char *path) {
    file_info->path = path;
    file_info->old_mode = mode;
}

void destroy_file_info(FileInfo *file_info) {
    free(file_info);
}
