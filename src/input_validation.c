#include "input_validation.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

static bool is_invalid_flag(char *argument){

    return !is_flag_arg(argument) && !has_permissions_flags(argument);
}

static bool is_invalid_octal_number(const char *octal_number){
    int counter_octal_digits = 0;

    for(int i = strlen(octal_number) - 1; i >= 0; --i){
        if(octal_number[i] > '7') return true;
        counter_octal_digits++;
        if(counter_octal_digits > 3 && octal_number[i] != '0') return true;
    }
    return false;
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
    if(is_number_arg(argv[mode_index]) && is_invalid_octal_number(argv[mode_index])){
        printf("xmod: invalid mode '%s'\n", argv[mode_index]);
        return true;
    }

    return false;
}