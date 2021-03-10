#include <dirent.h>
#include <stdio.h>

#include "retrievers.h"
#include "transverse.h"

int main(int argc, char **argv) {
    char *file_path = argv[1];
    FileInfo file_info;
    retrieve_file_info(&file_info, file_path);
    printf("Relative path: %s\n", file_info.path);
    printf("Current mode: %o\n", file_info.octal_mode);
    printf("File type: %d\n", file_info.file_type);

    if (file_info.file_type == DIRECTORY) {
        DIR *dp = opendir(file_info.path);
        transverse_dir(dp);
    }

    return 0;
}
