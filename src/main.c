#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "input_validation/input_validation.h"
#include "log/log.h"
#include "parse/parsers.h"
#include "retrieve/retrievers.h"
#include "signal/sig.h"
#include "util/utils.h"
#include "verbose/verbose.h"

int traverse(char *argv[], const char dir_path[], unsigned file_idx);
int process(char *argv[]);

int main(int argc, char *argv[]) {
  setup_event_logging();
  EventLog nj;
  log_process_exit_creat(1);
  nj.signal_received = "helo";
  log_event(SIGNAL_RECV, &nj);
  nj.sent.signal_sent = "hello";
  nj.sent.pid_sent = 4;
  log_event(SIGNAL_SENT, &nj);
  nj.perms.file_name = "~";
  nj.perms.old_perms = 0666;
  nj.perms.new_perms = 0666;
  log_event(FILE_MODF , &nj);
  nj.arg.argc_info = argc;
  nj.arg.argv_info = argv;
  log_event(PROC_CREAT, &nj);

  close_log_file();
    /*if (is_invalid_input(argv, argc) || setup_event_logging() != 0) {
        exit(EXIT_FAILURE);
    }
    // log process creation here; include after logger functions are ready
    process(argv);*/
}

int process(char *argv[]) {
    XmodCommand cmd;
    parse(argv, &cmd);

    FileInfo file_info;
    int success = 0;

    if (retrieve_file_info(&file_info, cmd.file_dir) == 0) {
        success = chmod(cmd.file_dir, cmd.octal_mode);
    } else {
        fprintf(stderr, "xmod: cannot access '%s': %s\n", cmd.file_dir,
                strerror(errno));
        success = -1;
    }

    bool changed = cmd.octal_mode != file_info.octal_mode;

    if (cmd.options.verbose || (cmd.options.changes && changed)) {
        print_verbose_message(cmd.file_dir, file_info.octal_mode,
                              cmd.octal_mode, changed, success);
    }

    if (success == 0 && cmd.options.recursive && file_info.type == DT_DIR) {
        if (traverse(argv, cmd.file_dir, cmd.file_idx) != 0) {
            return -1;
        }
    }

    return 0;
}

int traverse(char *argv[], const char dir_path[], unsigned file_idx) {
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
                continue;
            }

            if (file_info.type == DT_DIR) {
                pid_t id = fork();
                if (id == 0) {
                    char time_str[PATH_MAX];
                    sprintf(time_str, "%Lf", get_initial_instant());
                    setenv(LOG_PARENT_INITIAL_TIME_ENV, time_str, 1);
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
