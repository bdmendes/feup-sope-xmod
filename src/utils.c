#include "utils.h"

#include <string.h>

char *get_top_dir(char str[]) {
    if (str == NULL)
        return NULL;
    return strtok(str, "/");
}

char *prune_top_dir(char str[]) {
    if (str == NULL)
        return NULL;
    int size = strlen(str);
    for (int i = 0; i < size; ++i) {
        if (str[i] == '/' && i != size - 1) {
            return str + i + 1;
        }
    }
    return NULL;
}

bool is_ref_path(char *path) {
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

void replace_first_occurence(char *arr[], int size, char *old, char *new) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(arr[i], old) == 0) {
            arr[i] = new;
            return;
        }
    }
}

char *append_path(char *res, char *path) {
    return strcat(strcat(res, "/"), path);
}