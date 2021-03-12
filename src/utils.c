#include "utils.h"
#include <stdio.h>
#include <string.h>

static bool is_flag(char c) { return c == 'R' || c == 'v' || c == 'c'; }

bool is_flag_arg(char *argument) {
  if (argument[0] != '-')
    return false;
  for (int i = 1; i < strlen(argument); ++i) {
    if (!is_flag(argument[i]))
      return false;
  }
  return true;
}
