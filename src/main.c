#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parsers.h"
#include "retrievers.h"
#include "utils.h"

int traverse(char *argv[], char dir_path[], unsigned file_idx);
int process(char **argv);

int main(int argc, char **argv) {
  // init logs too...
  process(argv);
}

int process(char **argv) {  // pass log too
  XmodCommand cmd;
  // Hard coded; parse here
  cmd.mode.octal_mode = 0777;
  cmd.mode_type = OCTAL_MODE;
  cmd.options.recursive = true;
  cmd.file_idx = 3;
  strcpy(cmd.file_dir, argv[3]);

  FileInfo file_info;
  if (retrieve_file_info(&file_info, cmd.file_dir) != 0) {
    perror("could not retrieve file info");
    return -1;
  }

  // Must be wrapped after log is ready
  chmod(cmd.file_dir, cmd.mode.octal_mode);
  printf("Changing file permissions: %s\n", cmd.file_dir);

  if (cmd.options.recursive && file_info.type == DT_DIR) {
    traverse(argv, cmd.file_dir, cmd.file_idx);
  }
  return 0;
}

int traverse(char *argv[], char dir_path[], unsigned file_idx) {
  DIR *dp = opendir(dir_path);
  if (dp == NULL) {
    perror("could not open directory");
    return -1;
  }

  struct dirent *dirent;
  while ((dirent = readdir(dp)) != NULL) {
    if (!is_ref_path(dirent->d_name)) {
      char new_path[PATH_MAX];
      sprintf(new_path, "%s/%s", dir_path, dirent->d_name);
      argv[file_idx] = new_path;

      FileInfo file_info;
      if (retrieve_file_info(&file_info, new_path) != 0) {
        perror("could not retrieve file info");
        return -1;
      }

      if (file_info.type == DT_DIR) {
        pid_t id = fork();
        if (id == 0) {
          execvp(argv[0], argv);
        } else if (id != -1) {
          wait(NULL);
        } else {
          perror("fork failed");
          return -1;
        }
      } else if (file_info.type != DT_LNK) {
        process(argv);
      }
    }
  }

  if (closedir(dp) == -1) {
    perror("error closing directory");
    return -1;
  }
  return 0;
}