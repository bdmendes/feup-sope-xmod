#include "input_validation.h"
#include "utils.h"

#include <stdio.h>

static bool is_invalid_flag(char *argument){

    return !is_flag_arg(argument) && !has_permissions_flags(argument);
}

bool is_invalid_input(char** argv, int argc){
    int mode_index = 1;
    
    while(argv[mode_index][0] == '-'){
        if(is_invalid_flag(argv[mode_index])){
            printf("xmod: invalid option '%s'\n", argv[mode_index]);
            return true;
        }
        mode_index++;
    }
    return false;
}