#include <fcntl.h>
#include <fts.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

int retrieve_file_mode(int dir_fd, const char *path_name, mode_t *file_mode,
                       int flag);

int main(int argc, char **argv) {
    int dirfd = open(argv[1], O_RDONLY);
    mode_t mode = 0;
    retrieve_file_mode(dirfd, argv[2], &mode, 0);
    printf("Old mode: %o\n", mode);
    return 0;
}

int retrieve_file_mode(int dir_fd, const char *path_name, mode_t *file_mode,
                       int flag) {
    struct stat sb;
    if (fstatat(dir_fd, path_name, &sb, flag) < 0) {
        perror("fstat");
        return 0;
    }
    *file_mode = sb.st_mode;
    return 0;
}
