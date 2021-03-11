#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "retrievers.h"
#include "transverse.h"
#include "utils.h"

int main(int argc, char** argv) {
  char str[] = "file1/dd/babsb/";
  char* s1 = prune_top_dir(str);
  char* s2 = prune_top_dir(s1);
  printf("HI: %s, %s\n", s1, s2);
  fflush(stdout);
  char* s3 = get_top_dir(str);
  printf("HI: %s\n", s3);

  char* file_path = argv[1];
  FileInfo file_info;
  retrieve_file_info(&file_info, file_path);
  printf("Relative path: %s\n", file_info.path);
  printf("Current mode: %o\n", file_info.octal_mode);
  printf("File type: %d\n", file_info.file_type);

  if (file_info.file_type == DIRECTORY) {
    DIR* dp = opendir(file_info.path);
    transverse_dir(dp);
  }

  return 0;
}
