#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "parsers.h"
#include "retrievers.h"
#include "transverse.h"
#include "utils.h"

int transverse(XmodCommand* cmd) {
  char* curr_path = cmd->file_dir;

  printf("Changing file permissions: %s\n", curr_path);
  FileInfo file_info;
  if (retrieve_file_info(&file_info, curr_path) != 0) {
    printf("Stat failed!!!!\n");
    return -1;
  }
  chmod(curr_path, cmd->mode.octal_mode);

  if (cmd->options.recursive && file_info.file_type == DIRECTORY) {
    DIR* dp = opendir(cmd->file_dir);
    struct dirent* dirent;

    while ((dirent = readdir(dp)) != NULL) {
      if (dirent->d_type == DT_DIR) {
        if (strcmp(dirent->d_name, ".") != 0 &&
            strcmp(dirent->d_name, "..") != 0) {
          printf("Entering directory : %s\n", dirent->d_name);
          pid_t i = fork();
          if (i == 0) {
            char dash[] = "/";
            cmd->file_dir = strcat(strcat(curr_path, dash), dirent->d_name);
            sleep(3);
            transverse(cmd);
          }
        }
      } else {
        printf("Changing file permissions: %s\n", dirent->d_name);
        sleep(3);
        chmod(curr_path, cmd->mode.octal_mode);
      }
    }
  }
  return 0;
}

int main(int argc, char** argv) {
  char str[] = "dir";

  XmodCommand cmd;
  cmd.file_dir = str;
  cmd.mode.octal_mode = 0744;
  cmd.mode_type = OCTAL_MODE;
  cmd.options.recursive = true;
  transverse(&cmd);
}
