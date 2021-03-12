#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "parsers.h"

int test();

int main(int argc, char **argv) {

    /*XmodCommand cmd;
    parse(argv, &cmd);
    printf("mode: %o\n", cmd.octal_mode);
    if (chmod(cmd.file_dir, cmd.octal_mode) == -1){
        perror("chmod call failed");
        return -1;
    }*/
    test();

    /*char *file_path = argv[1];
    FileInfo file_info;
    retrieve_file_info(&file_info, file_path);
    printf("Relative path: %s\n", file_info.path);
    printf("Current mode: %o\n", file_info.octal_mode);
    printf("File type: %d\n", file_info.file_type);*/
    return 0;
}

int test(){
    /*int octal = 00124;
    int binary = 0b000000100;
    printf("%d\n", (octal & binary));*/
    /*OctalNumber old;
    old.ls = 7;
    old.middle = 7;
    old.ms = 1;

    OctalNumber new;

    transform_symbolic_mode_to_octal_mode("u+rw", &old, &new);

    printf("%o", new.ls);
    printf("%o", new.middle);
    printf("%o\n", new.ms);*/
    return 0;
}