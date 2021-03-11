#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

char *get_top_dir(char str[]);

char *prune_top_dir(char str[]);

bool is_ref_path(char *path);

void replace_first_occurence(char *arr[], int size, char *old, char *new);

char *append_path(char *res, char *path);

#endif