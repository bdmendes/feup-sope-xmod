#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "parsers.h"

int main(int argc, char **argv) {
    XmodCommand cmd;
    parse(argv, &cmd);
    printf("mode: %o\n", cmd.octal_mode);
    if (chmod(cmd.file_dir, cmd.octal_mode) == -1){
        perror("chmod call failed");
        return -1;
    }
}

