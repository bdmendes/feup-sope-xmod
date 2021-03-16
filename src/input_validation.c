#include "input_validation.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool is_invalid_octal_number(const char *octal_number){
     int counter_octal_digits = 0;
     
    for(int i = strlen(octal_number) - 1; i >= 0; --i){
        if(octal_number[i] > '7') return true;
        counter_octal_digits++;
        if(counter_octal_digits > 3 && octal_number[i] != '0') return true;
    }

    return false;
}

static bool is_invalid_symbolic_mode(char* symbolic_mode){
    //symbolic mode cannot start or end with a comma
    if(symbolic_mode[0] == ',' || symbolic_mode[strlen(symbolic_mode)-1] == ',') return true;

    const char sep[2] = ",";
    for (char *i = strtok(symbolic_mode, sep); i != NULL;) {
        if(strlen(i) < 2) {
            strcpy(symbolic_mode, i);
            return true;
        }
        int operator_index = 0;

        if(!is_permission_operator(i[0])){
            if(strlen(i) < 3 || !is_user_flag(i[0])) {
                strcpy(symbolic_mode, i);
                return true;
            }
             operator_index++;
        }
        if(!is_permission_operator(i[operator_index]) || !has_permissions_flags(i, ++operator_index)) {
             strcpy(symbolic_mode, i);
            return true;
        }
        i = strtok(NULL, sep);
    }
    return false;
}

bool is_invalid_input(char** argv, int argc){
    if(argc < 3){
        printf("xmod: missing operand\n");
        return true;
    }
    int mode_index = 1;

    while(argv[mode_index][0] == '-'){
        if(has_permissions_flags(argv[mode_index], 1)) break;

        if(!is_flag_arg(argv[mode_index])){
            printf("xmod: invalid option '%s'\n", argv[mode_index]);
            return true;
        }
        mode_index++;
    }
    char *symbolic_mode_to_be_modified = (char *)malloc(sizeof(char)*strlen(argv[mode_index])); //strtok cannot modify argv
    strcpy(symbolic_mode_to_be_modified, argv[mode_index]);

    if(is_number_arg(argv[mode_index]) && is_invalid_octal_number(argv[mode_index])){
        printf("xmod: invalid mode '%s'\n", argv[mode_index]);
        return true;
    }else if(!is_number_arg(argv[mode_index]) && is_invalid_symbolic_mode(symbolic_mode_to_be_modified)){
        printf("xmod: invalid mode '%s'\n", symbolic_mode_to_be_modified);
        return true;
    }


    return false;
}