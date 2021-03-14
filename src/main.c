#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "log.h"
#include "parsers.h"
#include "retrievers.h"
#include "sig.h"
#include "utils.h"

int traverse(char *argv[], char dir_path[], unsigned file_idx);
int process(char **argv);

int main(int argc, char **argv) {
    process(argv);
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
    if (cmd.options.verbose) {
        char *old_mode = (char *)malloc(9);
        char *new_mode = (char *)malloc(9);
        get_symbolic_string(file_info.octal_mode, old_mode);
        get_symbolic_string(cmd.octal_mode, new_mode);
        printf("mode of '%s' changed from %o (%s) to %o (%s)\n", file_info.path,
               file_info.octal_mode, old_mode, cmd.octal_mode, new_mode);
    }
    chmod(cmd.file_dir, cmd.octal_mode);

    if (cmd.options.recursive && file_info.type == DT_DIR) {
        traverse(argv, cmd.file_dir, cmd.file_idx);
    }
    return 0;
}

int traverse(char *argv[], char dir_path[], unsigned file_idx) {
    printf("Transversing...\n");
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
/* Tests */

int test_conversion_of_symbolic_mode_to_octal_mode();

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
/*
void test_signal(){
    setup_event_logging();
    set_file("nckdjsnc");
    set_handler_SIGINT();
    while(get_running()){
        printf("hello\n");
        sleep(1);
    }
    printf("heillo");
}*/
