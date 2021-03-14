#include <dirent.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "log/log.h"
#include "parse/parsers.h"
#include "retrieve/retrievers.h"
#include "signal/sig.h"
#include "util/utils.h"

int traverse(char *argv[], char dir_path[], unsigned file_idx);
int process(char **argv);

int main(int argc, char **argv) {
    if (setup_event_logging() != 0) {
        exit(EXIT_FAILURE);
    }
    EventLog event_log;
    event_log.perms.file_name = "my-file-name";
    event_log.perms.new = 0777;
    event_log.perms.old = 0333;
    log_event(FILE_MODF, &event_log);
    process(argv);
    close_log_file();
}

int process(char **argv) { // pass log too
    XmodCommand cmd;
    parse(argv, &cmd);
    printf("file_name: %s, mode: %o\n", cmd.file_dir, cmd.octal_mode);

    FileInfo file_info;

    if (retrieve_file_info(&file_info, cmd.file_dir) != 0) {
        perror("could not retrieve file info");
        return -1;
    }

    // Must be wrapped after log is ready
    chmod(cmd.file_dir, cmd.octal_mode);
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
                continue;
            }

            if (file_info.type == DT_DIR) {
                pid_t id = fork();
                if (id == 0) {
                    char time_str[PATH_MAX];
                    sprintf(time_str, "%lu", get_initial_instant());
                    setenv(LOG_PARENT_INITIAL_TIME_ENV, time_str, 0);
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
