#include "xmod.h"
#include "parsers.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static int parse_symbolic_mode(const char *mode, XmodCommand *xmodCommand){

    switch(mode[0]){
        case 'u':
            xmodCommand->mode.symbolic_mode.user_type = USER;
            break;
        case 'g':
            xmodCommand->mode.symbolic_mode.user_type = GROUP;
            break;
        case 'o':
            xmodCommand->mode.symbolic_mode.user_type = OTHER;
            break;
        case 'a':
        case '+':
        case '-':
        case '=':
            xmodCommand->mode.symbolic_mode.user_type = ALL;
            break;
        default:
            return 1; //invalid command
    }

    /*
    if(mode[1] == '-') xmodCommand->mode.symbolic_mode.permission_operator_type = SUBTRACT;
    else if(mode[1] == '+') xmodCommand->mode.symbolic_mode.permission_operator_type = ADD;
    else if(mode[1] == '=') xmodCommand->mode.symbolic_mode.permission_operator_type = ASSIGN;
    else return 1; //invalid command
    */

    xmodCommand->mode.symbolic_mode.permissions.read = strchr(mode, 'r') != NULL;
    xmodCommand->mode.symbolic_mode.permissions.write = strchr(mode, 'w') != NULL;
    xmodCommand->mode.symbolic_mode.permissions.execute = strchr(mode, 'x') != NULL;
    
    return 0;
}

/*static int transform_symbolic_to_octal(XmodCommand *xmodCommand){
}*/

static int parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand){
    char* buf;
    mode_t mode = strtoul(mode_str, &buf, 8) & 0777;
    xmodCommand->mode_type = OCTAL_MODE;
    xmodCommand->mode.octal_mode = mode;
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