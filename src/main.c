#include <stdio.h>
#include <stdlib.h>
#include "xmod.h"
#include "log.h"

//static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

int main(int argc, char **argv, char *envp[]) {
  FILE* my_file = log_init();
  union info nj;
  nj.exit_code = 1;
  nj.signal_received = "helo";
  nj.sent.signal_sent = "hello";
  nj.sent.pid_sent = 4;
 nj.arg.argc_info = argc;
 nj.arg.argv_info = argv;
nj.perm.name_file = "~";
nj.perm.old_perms = 0666;
nj.perm.new_perms = 0666;

  
  log_file(my_file, FILE_MODF, nj);
 nj.arg.argc_info = argc;
 nj.arg.argv_info = argv;
 log_file(my_file, PROC_CREAT, nj);

  log_close(my_file);
  return 0;
}
