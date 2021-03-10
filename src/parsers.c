#include "xmod.h"
#include "parsers.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static int get_octal_digit(XmodPermissions permissions){
   int octal_digit = 0;
   octal_digit = permissions.execute ? 1 : 0;
   octal_digit += permissions. write ? 2 : 0;
   octal_digit += permissions.read ? 4 : 0;
   return octal_digit;
}

static void calculate_octal_sum(mode_t *old_mode, mode_t *new_mode){
    *new_mode += *old_mode;
    *new_mode = *new_mode > 7 ? 7 : *new_mode;
}

static void calculate_octal_subtration(mode_t *old_mode, mode_t *new_mode){
    *new_mode = old_mode < new_mode ? 0 : old_mode - new_mode;
}

static OctalNumber get_digits(mode_t *mode){
    OctalNumber mode_digits;
    mode_digits.ls = *mode & 0007;
    mode_digits.middle = *mode & 0070;
    mode_digits.ms = *mode & 0700;
    return mode_digits;
}

static int transform_symbolic_mode_to_octal_mode(const char *symbolic_mode, OctalNumber *old_digits, OctalNumber *new_digits){

    int operator_index = (symbolic_mode[1] == '+' || symbolic_mode[1] == '-' || symbolic_mode[1] == '=') ? 1 : 0;

    XmodPermissions permissions;
    permissions.read = strchr(symbolic_mode, 'r') != NULL;
    permissions.write = strchr(symbolic_mode, 'w') != NULL;
    permissions.execute = strchr(symbolic_mode, 'x') != NULL;

    switch(symbolic_mode[0]){
        case 'u':
            new_digits->ms = get_octal_digit(permissions);
            break;
        case 'g':
            new_digits->middle = get_octal_digit(permissions);
            break;
        case 'o':
            new_digits->ls = get_octal_digit(permissions);
            break;
        case 'a':
        default:
            new_digits->ms = get_octal_digit(permissions);
            new_digits->middle = get_octal_digit(permissions);
            new_digits->ls = get_octal_digit(permissions);
            break;
    }
    switch(symbolic_mode[operator_index]){
        case '+':
            calculate_octal_sum(&old_digits->ls, &new_digits->ls);
            calculate_octal_sum(&old_digits->middle, &new_digits->middle);
            calculate_octal_sum(&old_digits->ms, &new_digits->ms);
            break;
        case '-':
            calculate_octal_subtration(&old_digits->ls, &new_digits->ls);
            calculate_octal_subtration(&old_digits->middle, &new_digits->middle);
            calculate_octal_subtration(&old_digits->ms, &new_digits->ms);
            break;
        case '=':
            break;
        default:
            return 1; //invalid input
    }
    *old_digits = *new_digits;
    return 0;
}

static mode_t get_mode(OctalNumber *digits){
    mode_t octal_mode = digits->ls;
    octal_mode |= (digits->middle << 4);
    octal_mode |= (digits->ms << 8);
    return octal_mode;
}

static int parse_symbolic_mode(char *mode, XmodCommand *xmodCommand){
    mode_t old_mode;
    retrieve_file_mode(xmodCommand->file_dir, &old_mode);

    OctalNumber old_digits = get_digits(&old_mode);
    OctalNumber new_digits;

    const char sep[2] = ",";
    char *symbolic_mode;   
    symbolic_mode = strtok(mode, sep);
   
    while(symbolic_mode != NULL) {
        transform_symbolic_mode_to_octal_mode(symbolic_mode, &old_digits, &new_digits);
        symbolic_mode = strtok(NULL, sep);
    }

    xmodCommand->octal_mode = get_mode(&new_digits);
    return 0;
}

static int parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand){
    char* buf;
    mode_t mode = strtoul(mode_str, &buf, 8) & 0777;
    //xmodCommand->mode_type = OCTAL_MODE;
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