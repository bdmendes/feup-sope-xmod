#include <stdio.h>

#include "retrievers.h"

int main(int argc, char **argv) {
    char *file_path = argv[1];
    FileInfo file_info;
    retrieve_file_info(&file_info, file_path);
    printf("Relative path: %s\n", file_info.path);
    printf("Current mode: %o\n", file_info.octal_mode);
    printf("File type: %d\n", file_info.file_type);
    return 0;
}
