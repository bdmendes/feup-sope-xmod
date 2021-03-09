#include <stdio.h>

#include "retrievers.h"

int main(int argc, char **argv) {
    char *file_path = argv[1];
    mode_t mode = 0;
    retrieve_file_mode(file_path, &mode);
    FileInfo file_info;
    assemble_file_info(&file_info, mode, file_path);
    printf("Relative path: %s\n", file_info.path);
    printf("Current mode: %o\n", file_info.octal_mode);
    printf("File type: %d\n", file_info.file_type);
    return 0;
}
