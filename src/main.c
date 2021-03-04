#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

/**
 * @brief reads the current mode of a file
 *
 * @param file_descriptor the descriptor correspondent to the file
 * @param old_mode the mode read
 * @return int 0 on success
 */
int read_old_mode(int file_descriptor, mode_t *old_mode);

int main(int argc, char **argv) {
  int file = open(argv[1], O_RDONLY);
  mode_t mode = 0;
  read_old_mode(file, &mode);
  printf("Old mode: %o\n", mode);
  return 0;
}

int read_old_mode(int file_descriptor, mode_t *old_mode) {
  struct stat sb;
  if (fstat(file_descriptor, &sb) < 0) {
    perror("fstat");
    return 0;
  }
  *old_mode = sb.st_mode;
  return 0;
}
