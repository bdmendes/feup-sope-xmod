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
    return 0;
}

int test(){
    OctalNumber old;
    old.ls = 7;
    old.middle = 7;
    old.ms = 1;

    OctalNumber new;

    transform_symbolic_mode_to_octal_mode("u-rw", &old, &new);

    printf("%o", new.ls);
    printf("%o", new.middle);
    printf("%o\n", new.ms);
    return 0;
}