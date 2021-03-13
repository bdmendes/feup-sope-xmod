#include "utils.h"
#include <ctype.h>
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

bool is_number_arg(char *argument) {
  for (int i = 0; i < strlen(argument); ++i) {
    if (!isdigit(argument[i]))
      return false;
  }
  return true;
}
