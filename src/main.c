#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parsers.h"
#include "retrievers.h"
#include "utils.h"

int main(int argc, char **argv) {
    // Hard coded command content. Waiting for parser
    XmodCommand cmd;
    cmd.mode.octal_mode = 0744;
    cmd.mode_type = OCTAL_MODE;
    cmd.options.recursive = true;
    cmd.file_idx = 3;
    strcpy(cmd.file_dir, argv[3]);

    FileInfo file_info;
    if (retrieve_file_info(&file_info, cmd.file_dir) != 0) {
        perror("could not retrieve file info\n");
        return -1;
    }

    chmod(cmd.file_dir, cmd.mode.octal_mode);
    printf("Changing file permissions: %s\n", cmd.file_dir);
    fflush(stdout);

    if (cmd.options.recursive && file_info.type == DT_DIR) {
        DIR *dp = opendir(cmd.file_dir);
        struct dirent *dirent;

        while ((dirent = readdir(dp)) != NULL) {
            if (!is_ref_path(dirent->d_name)) {

                // Not very pretty. Should rethink
                char file_path[200];
                strcpy(file_path, cmd.file_dir);
                strcpy(file_path, append_path(file_path, dirent->d_name));
                argv[cmd.file_idx] = file_path;

                if (dirent->d_type == DT_DIR) {
                    pid_t i = fork();
                    if (i == 0) {
                        execvp(argv[0], argv);
                    } else {
                        wait(NULL);
                    }
                } else if (dirent->d_type != DT_LNK) {
                    main(argc, argv); // wrapping this in a module would be
                                      // prettier, but it seems like a hard task
                                      // due to the difficult structure
                }
            }
        }
    }
    return 0;
}
