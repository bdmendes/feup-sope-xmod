#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parsers.h"
#include "retrievers.h"
#include "utils.h"

int main(int argc, char **argv) {
    char str[] = "testdir";

    // Hard coded command content. Waiting for parser
    XmodCommand cmd;
    strcpy(cmd.file_dir, str);
    cmd.mode.octal_mode = 0744;
    cmd.mode_type = OCTAL_MODE;
    cmd.options.recursive = true;

    char *curr_path = cmd.file_dir;
    printf("Changing file permissions: %s\n", curr_path);
    FileInfo file_info;
    if (retrieve_file_info(&file_info, curr_path) != 0) {
        perror("could not retrieve file info\n");
        return -1;
    }
    chmod(curr_path, cmd.mode.octal_mode);

    if (cmd.options.recursive && file_info.type == DT_DIR) {
        DIR *dp = opendir(cmd.file_dir);
        struct dirent *dirent;

        while ((dirent = readdir(dp)) != NULL) {
            if (dirent->d_type == DT_DIR) {
                if (strcmp(dirent->d_name, ".") != 0 &&
                    strcmp(dirent->d_name, "..") != 0) {
                    printf("Entering directory : %s\n", dirent->d_name);

                    pid_t i = fork();
                    if (i == 0) {
                        char dash[] = "/";
                        for (int i = 0; i < argc; ++i) {
                            if (argv[i] == cmd.file_dir) {
                                strcpy(cmd.file_dir,
                                       strcat(strcat(curr_path, dash),
                                              dirent->d_name));
                                argv[i] = cmd.file_dir;
                                break;
                            }
                        }
                        execvp("xmod", argv);
                    } else {
                        wait(NULL);
                    }
                }
            } else {
                printf("Changing file permissions: %s\n", dirent->d_name);
                chmod(curr_path, cmd.mode.octal_mode);
            }
        }
    }
    return 0;
}
