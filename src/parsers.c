#include "xmod.h"
#include "parsers.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static void assign_permissions(struct XmodPermissionsTypes *permissions, mode_t octal_mode){
    permissions->read = octal_mode & 0b100;
    permissions->write = octal_mode & 0b010;
    permissions->execute = octal_mode & 0b001;
}

static FilePermissions parse_octal_mode_to_permissions_struct(mode_t *octal_mode){
    FilePermissions file_permissions;

    assign_permissions(&file_permissions.other_permissions, (*octal_mode & 0007));
    assign_permissions(&file_permissions.group_permissions, (*octal_mode & 0070) >> 3);
    assign_permissions(&file_permissions.user_permissions, (*octal_mode & 0700) >> 6);

    return file_permissions;
}

static void sum_permissions(struct XmodPermissionsTypes* old_permissions, struct XmodPermissionsTypes* new_permissions){
    old_permissions->read += new_permissions->read;
    old_permissions->write += new_permissions->write;
    old_permissions->execute += new_permissions->execute;
}

static void remove_permissions(struct XmodPermissionsTypes *old_permissions, struct XmodPermissionsTypes* new_permissions){
    old_permissions->read = new_permissions->read ? false : old_permissions->read;
    printf("%d", old_permissions->read);
    old_permissions->write = new_permissions->write ? false : old_permissions->write;
    printf("%d", old_permissions->write);
    old_permissions->execute = new_permissions->execute ? false : old_permissions->execute;
    printf("%d\n", old_permissions->execute);
}

int transform_symbolic_mode_to_octal_mode(const char *symbolic_mode, FilePermissions *octal_mode){

    int operator_index = (symbolic_mode[1] == '+' || symbolic_mode[1] == '-' || symbolic_mode[1] == '=') ? 1 : 0;

    struct XmodPermissionsTypes new_permissions;
    new_permissions.read = strchr(symbolic_mode, 'r') != NULL;
    new_permissions.write = strchr(symbolic_mode, 'w') != NULL;
    new_permissions.execute = strchr(symbolic_mode, 'x') != NULL;

    switch(symbolic_mode[0]){
        case 'u':
            switch(symbolic_mode[operator_index]){
                case '+':
                    sum_permissions(&octal_mode->user_permissions, &new_permissions);
                    break;
                case '-':
                    remove_permissions(&octal_mode->user_permissions, &new_permissions);
                    break;
                case '=':
                    octal_mode->user_permissions = new_permissions;
                    break;
                default:
                    return 1; //invalid input
            }
            break;
        case 'g':
            switch(symbolic_mode[operator_index]){
                case '+':
                    sum_permissions(&octal_mode->group_permissions, &new_permissions);
                    break;
                case '-':
                    remove_permissions(&octal_mode->group_permissions, &new_permissions);
                    break;
                case '=':
                    octal_mode->group_permissions = new_permissions;
                default:
                    return 1; //invalid input
            }
            break;
        case 'o':
            switch(symbolic_mode[operator_index]){
                case '+':
                    sum_permissions(&octal_mode->other_permissions, &new_permissions);
                    break;
                case '-':
                    remove_permissions(&octal_mode->other_permissions, &new_permissions);
                    break;
                case '=':
                    octal_mode->other_permissions = new_permissions;
                default:
                    return 1; //invalid input
            }
            break;
        case 'a':
        default:
            switch(symbolic_mode[operator_index]){
                case '+':
                    sum_permissions(&octal_mode->user_permissions, &new_permissions);
                    sum_permissions(&octal_mode->group_permissions, &new_permissions);
                    sum_permissions(&octal_mode->other_permissions, &new_permissions);
                    break;
                case '-':
                    remove_permissions(&octal_mode->user_permissions, &new_permissions);
                    remove_permissions(&octal_mode->group_permissions, &new_permissions);
                    remove_permissions(&octal_mode->other_permissions, &new_permissions);
                    break;
                case '=':
                    octal_mode->user_permissions = new_permissions;
                    octal_mode->group_permissions = new_permissions;
                    octal_mode->other_permissions = new_permissions;
                    break;
                default:
                    return 1; //invalid input
            }
            break;
    }
    return 0;
}

mode_t get_octal_mode(FilePermissions *permissions){
    return permissions->user_permissions.read*S_IRUSR | permissions->user_permissions.write*S_IWUSR | permissions->user_permissions.execute*S_IXUSR |
        permissions->group_permissions.read*S_IRGRP | permissions->group_permissions.write*S_IWGRP | permissions->group_permissions.execute*S_IXGRP |
        permissions->other_permissions.read*S_IROTH | permissions->other_permissions.write*S_IWOTH | permissions->other_permissions.execute*S_IXOTH;
}

int parse_symbolic_mode(char *mode, XmodCommand *xmodCommand){
    mode_t old_mode;
    FileInfo file_info;
    retrieve_file_info(&file_info, xmodCommand->file_dir);

    FilePermissions mode_permissions = parse_octal_mode_to_permissions_struct(&old_mode);
    const char sep[2] = ",";
    char *symbolic_mode;   
    symbolic_mode = strtok(mode, sep);
   
    while(symbolic_mode != NULL) {
        transform_symbolic_mode_to_octal_mode(symbolic_mode, &mode_permissions);
        symbolic_mode = strtok(NULL, sep);
    }

    xmodCommand->octal_mode = get_octal_mode(&mode_permissions);
    return 0;
}

static int parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand){
    char* buf;
    mode_t mode = strtoul(mode_str, &buf, 8) & 0777;
    xmodCommand->octal_mode = mode;
    return 0;
}

static void parse_options(const char *options, XmodCommand *xmodCommand){
    xmodCommand->options.recursive = strchr(options, 'R') != NULL;
    xmodCommand->options.verbose = strchr(options, 'v') != NULL;
    xmodCommand->options.changes = strchr(options, 'c') != NULL && !xmodCommand->options.verbose;
}
    
int parse(char **argv, XmodCommand *xmodCommand){
    int mode_index = 1;

    if(argv[mode_index][0] == '-'){
        mode_index++;
        parse_options(argv[2], xmodCommand);
    }

    if(isdigit(argv[mode_index][0])){ // create util to check entire string
        parse_octal_mode(argv[mode_index], xmodCommand);
    } else {
        parse_symbolic_mode(argv[mode_index], xmodCommand);
    }

    // should pass argc to this function to check here
    xmodCommand->file_dir = argv[mode_index + 1];

    return 0;
}