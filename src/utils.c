#include "utils.h"

#include <ctype.h>
#include <string.h>

bool is_ref_path(char *path) {
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

static bool is_options_flag(char c) {
    return c == 'R' || c == 'v' || c == 'c';
}

bool is_flag_arg(char *argument) {
    if (argument[0] != '-')
        return false;
    for (int i = 1; i < strlen(argument); ++i) {
        if (!is_options_flag(argument[i]))
            return false;
    }
    return true;
}

bool is_number_arg(char *argument) {
    for (int i = 0; i < strlen(argument); ++i) {
        if (!isdigit(argument[i]))
            return false;
    }
    return true;
}

static bool is_permission_flag(char c){
   return c == 'r' || c == 'w' || c == 'x';
}

bool has_permissions_flags(const char* argument, int start_index){
    for(int i = start_index; i < strlen(argument); ++i){
        if(!is_permission_flag(argument[i]))
            return false;
    }
    return true;
}

bool is_permission_operator(char c){
    return c == '+' || c == '-' || c == '=';
}

bool is_user_flag(char c){
    return c == 'u' || c == 'g' || c == 'o' || c == 'a';
}