#include "transverse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void transverse_dir(DIR *dp) {
    struct dirent *dirent;

    while ((dirent = readdir(dp)) != NULL) {
        if (dirent->d_type == DT_REG) {
            printf("Regular file : %s\n", dirent->d_name);
            // to things to regual files
        } else if (dirent->d_type == DT_DIR) {
            if (strcmp(dirent->d_name, ".") != 0 &&
                strcmp(dirent->d_name, "..") != 0) {
                // recurse
                printf("Directory : %s\n", dirent->d_name);
            }
        }
    }
}