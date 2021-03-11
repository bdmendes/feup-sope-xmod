#include "utils.h"

#include <string.h>

char* get_top_dir(char str[]) {
  if (str == NULL) return NULL;
  return strtok(str, "/");
}

char* prune_top_dir(char str[]) {
  if (str == NULL) return NULL;
  int size = strlen(str);
  for (int i = 0; i < size; ++i) {
    if (str[i] == '/' && i != size - 1) {
      return str + i + 1;
    }
  }
  return NULL;
}