#include <dirent.h>
#include <errno.h>
#include <stdio.h>
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

/**
 * @brief Convert command line arguments to a xmod command and execute it.
 *
 * @param argv command line arguments
 * @return 0 if no error
 */
int process(char *argv[]);

/**
 * @brief Change permissions for all files in a directory, and recursively enter
 * subdirectories, in newly created instances.
 *
 * @param argv command line arguments
 * @param dir_path directory path
 * @param file_idx file path argument index
 * @return 0 if no error
 */
int traverse(char *argv[], const char dir_path[], unsigned file_idx);

int main(int argc, char *argv[]) {
    int is_setup = setup_event_logging();
    log_proc_creat_creat(argc, argv);
    if (setup_signal_handler() != 0) {
        fprintf(stderr, "setup signal handling");
    }
    if (is_invalid_input(argv, argc) || is_setup!= 0) {
        log_proc_exit_creat(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    process(argv);
    log_proc_exit_creat(0);
}

int process(char *argv[]) {
    XmodCommand cmd;
    parse(argv, &cmd);
    increment_nftot();

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
            success = -1;
            print_verbose_message(cmd.file_dir, file_info.octal_mode,
                                  cmd.octal_mode, changed, success);
        }
    }

    if (success == 0) {
        increment_nfmod();
        log_proc_file_mod_creat(cmd.file_dir, file_info.octal_mode,
                                cmd.octal_mode);
    }
    return success;
}

int traverse(char *argv[], const char dir_path[], unsigned file_idx) {
    DIR *dp = opendir(dir_path);
    if (dp == NULL) {
        fprintf(stderr, "xmod: cannot read directory %s: %s\n", dir_path,
                strerror(errno));
        return -1;
    }

    struct dirent *dirent;
    while ((dirent = readdir(dp)) != NULL) {
        if (!is_ref_path(dirent->d_name)) {
            char new_path[PATH_MAX];
            snprintf(new_path, sizeof(new_path), "%s/%s", dir_path,
                     dirent->d_name);
            argv[file_idx] = new_path;

            FileInfo file_info;
            if (retrieve_file_info(&file_info, new_path) != 0) {
                perror("could not retrieve file info");
                closedir(dp);
                return -1;
            }

            if (file_info.type == DT_DIR) {
                pid_t id = fork();
                if (id == 0) {
                    char time_str[PATH_MAX];
                    snprintf(time_str, sizeof(time_str), "%Lf",
                             get_initial_instant());
                    setenv(LOG_PARENT_INITIAL_TIME_ENV, time_str, 1);
                    execvp(argv[0], argv);
                } else if (id != -1) {
                    int status;
                    if (wait(&status) == -1) {
                        perror("could not wait for child process");
                        closedir(dp);
                        return -1;
                    } else if (status != 0) {
                        perror("child process exited abnormally");
                        closedir(dp);
                        return -1;
                    }
                } else {
                    perror("fork failed");
                    closedir(dp);
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
