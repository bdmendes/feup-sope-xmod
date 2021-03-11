#include <stdio.h>
#include <stdlib.h>
#include "xmod.h"
#include "log.h"

//static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

int main(int argc, char **argv, char *envp[]) {
  FILE* my_file = setup_event_logging();
  union EventLog nj;
  nj.exit_code = 1;
  log_file(my_file, PROC_EXIT, &nj);
  nj.signal_received = "helo";
  log_file(my_file, SIGNAL_RECV, &nj);
  nj.sent.signal_sent = "hello";
  nj.sent.pid_sent = 4;
  log_file(my_file, SIGNAL_SENT, &nj);
  nj.perm.name_file = "~";
  nj.perm.old_perms = 0666;
  nj.perm.new_perms = 0666;
  log_file(my_file,FILE_MODF , &nj);
  nj.arg.argc_info = argc;
  nj.arg.argv_info = argv;
  log_file(my_file, PROC_CREAT, &nj);

  log_close(my_file);
  return 0;

/*#include "retrievers.h"

int main(int argc, char **argv) {
    char *file_path = argv[1];
    FileInfo file_info;
    retrieve_file_info(&file_info, file_path);
    printf("Relative path: %s\n", file_info.path);
    printf("Current mode: %o\n", file_info.octal_mode);
    printf("File type: %d\n", file_info.file_type);
    return 0;*/
}
