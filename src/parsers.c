#include "xmod.h"
#include "parsers.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void assign_permissions(struct XmodPermissionsTypes *permissions, mode_t octal_mode){
    permissions->read = octal_mode & 0b100;
    permissions->write = octal_mode & 0b010;
    permissions->execute = octal_mode & 0b001;
}

FilePermissions parse_octal_mode_to_permissions_struct(mode_t *octal_mode){
    FilePermissions file_permissions;

    assign_permissions(&file_permissions.other_permissions, (*octal_mode & 0007));
    assign_permissions(&file_permissions.group_permissions, (*octal_mode & 0070) >> 3);
    assign_permissions(&file_permissions.user_permissions, (*octal_mode & 0700) >> 6);

    return file_permissions;
}

void sum_permissions(struct XmodPermissionsTypes* old_permissions, struct XmodPermissionsTypes* new_permissions){
    new_permissions->read += old_permissions->read;
    new_permissions->write += old_permissions->write;
    new_permissions->execute += old_permissions->execute;
}

void remove_permissions(struct XmodPermissionsTypes* old_permissions, struct XmodPermissionsTypes* new_permissions){
    new_permissions->read = new_permissions->read ? false : new_permissions->read + old_permissions->read;
    new_permissions->write = new_permissions->write ? false : new_permissions->write + old_permissions->write;
    new_permissions->execute = new_permissions->execute ? false : new_permissions->execute + old_permissions->execute;
}

int transform_symbolic_mode_to_octal_mode(const char *symbolic_mode, FilePermissions *old_mode, FilePermissions *new_mode){

    int operator_index = (symbolic_mode[1] == '+' || symbolic_mode[1] == '-' || symbolic_mode[1] == '=') ? 1 : 0;

    struct XmodPermissionsTypes permissions;
    permissions.read = strchr(symbolic_mode, 'r') != NULL;
    permissions.write = strchr(symbolic_mode, 'w') != NULL;
    permissions.execute = strchr(symbolic_mode, 'x') != NULL;

    switch(symbolic_mode[0]){
        case 'u':
            new_mode->user_permissions = permissions;
            break;
        case 'g':
            new_mode->group_permissions = permissions;
            break;
        case 'o':
            new_mode->other_permissions = permissions;
            break;
        case 'a':
        default:
            new_mode->user_permissions = permissions;
            new_mode->group_permissions = permissions;
            new_mode->other_permissions = permissions;

            break;
    }
    switch(symbolic_mode[operator_index]){
        case '+':
            sum_permissions(&old_mode->user_permissions, &new_mode->user_permissions);
            sum_permissions(&old_mode->group_permissions, &new_mode->group_permissions);
            sum_permissions(&old_mode->other_permissions, &new_mode->other_permissions);
            break;
        case '-':
            remove_permissions(&old_mode->user_permissions, &new_mode->user_permissions);
            remove_permissions(&old_mode->group_permissions, &new_mode->group_permissions);
            remove_permissions(&old_mode->other_permissions, &new_mode->other_permissions);
            break;
        case '=':
            break;
        default:
            return 1; //invalid input
    }
    *old_mode = *new_mode;
    return 0;
}

static mode_t get_octal_mode(FilePermissions *permissions){
    return permissions->user_permissions.read*S_IRUSR | permissions->user_permissions.write*S_IWUSR | permissions->user_permissions.execute*S_IXUSR |
        permissions->group_permissions.read*S_IRGRP | permissions->group_permissions.write*S_IWGRP | permissions->group_permissions.execute*S_IXGRP |
        permissions->other_permissions.read*S_IROTH | permissions->other_permissions.write*S_IWOTH | permissions->other_permissions.execute*S_IXOTH;
}

static int parse_symbolic_mode(char *mode, XmodCommand *xmodCommand){
    mode_t old_mode;
    FileInfo file_info;
    retrieve_file_info(&file_info, xmodCommand->file_dir);

    FilePermissions old_mode_permissions = parse_octal_mode_to_permissions_struct(&old_mode);
    FilePermissions new_mode_permissions;

    const char sep[2] = ",";
    char *symbolic_mode;   
    symbolic_mode = strtok(mode, sep);
   
    while(symbolic_mode != NULL) {
        transform_symbolic_mode_to_octal_mode(symbolic_mode, &old_mode_permissions, &new_mode_permissions);
        symbolic_mode = strtok(NULL, sep);
    }

    xmodCommand->octal_mode = get_octal_mode(&new_mode_permissions);
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