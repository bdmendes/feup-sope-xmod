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
    FilePermissions old;
    old.user_permissions.read = true;
    old.user_permissions.write = false;  //5
    old.user_permissions.execute = true;

    old.group_permissions.read = true;
    old.group_permissions.write = false;  //5
    old.group_permissions.execute = true;

    old.other_permissions.read = true;
    old.other_permissions.write = false;  //5
    old.other_permissions.execute = true;

    transform_symbolic_mode_to_octal_mode("-rw",&old);

    printf("%o\n", get_octal_mode(&old)); //must be 655

    return 0;
}