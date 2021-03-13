#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parsers.h"

int test_conversion_of_symbolic_mode_to_octal_mode();

int main(int argc, char **argv) {
  XmodCommand cmd;
  parse(argv, &cmd);
  printf("file_name: %s, mode: %o\n", cmd.file_dir, cmd.octal_mode);

  if (chmod(cmd.file_dir, cmd.octal_mode) == -1) {
    perror("chmod call failed");
    return -1;
  }
  return 0;
}

int test_conversion_of_symbolic_mode_to_octal_mode() {
  FilePermissions old;
  old.user.read = false;
  old.user.write = true; // 3
  old.user.execute = true;

  old.group.read = false;
  old.group.write = true; // 2
  old.group.execute = false;

  old.other.read = true;
  old.other.write = false; // 5
  old.other.execute = true;

  update_permissions("-w", &old);

  printf("%o\n", get_octal_mode(&old));

  return 0;
}