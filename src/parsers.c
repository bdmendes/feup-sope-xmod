#include "parsers.h"


int read_mode(const char **argv, XmodCommand *xmodCommand){

    int mode_index = 2;

    if(strncmp(argv[2], "-", 1) != 0){
        mode_index++;
        parse_options(argv[2], xmodCommand);
    } 

    // work with hidden user/group
    if(argv[mode_index][0]>'0') parse_symbolic_mode(argv[mode_index], xmodCommand);
    //else parse_octal_mode

    return 0;
}

int parse_symbolic_mode(const char *mode, XmodCommand *xmodCommand){

  // convert to switch

    if(mode[0] == 'u') xmodCommand->mode.symbolic_mode.user_type = USER;
    else if(mode[0] =='g') xmodCommand->mode.symbolic_mode.user_type = GROUP;
    else if(mode[0] == 'o') xmodCommand->mode.symbolic_mode.user_type = OTHER;
    else if(mode[0] =='a') xmodCommand->mode.symbolic_mode.user_type = ALL;
    else return 1; //invalid command

    if(mode[1] == '-') xmodCommand->mode.symbolic_mode.permission_operator_type = SUBTRACT;
    else if(mode[1] == '+') xmodCommand->mode.symbolic_mode.permission_operator_type = ADD;
    else if(mode[1] == '=') xmodCommand->mode.symbolic_mode.permission_operator_type = ASSIGN;
    else return 1; //invalid command

    xmodCommand->mode.symbolic_mode.permissions.read = strchr(mode[2], 'r') != NULL;
    xmodCommand->mode.symbolic_mode.permissions.write = strchr(mode[2], 'w') != NULL;
    xmodCommand->mode.symbolic_mode.permissions.execute = strchr(mode[2], 'x') != NULL;
    
    return 0;
}

void parse_options(const char *options, XmodCommand *xmodCommand){
    xmodCommand->options.recursive = strchr(options, 'R') != NULL;
    xmodCommand->options.verbose = strchr(options, 'v') != NULL;
    xmodCommand->options.changes = strchr(options, 'c') != NULL && !xmodCommand->options.verbose;
}
    