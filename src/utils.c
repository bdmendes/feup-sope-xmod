#include "utils.h"

#include <string.h>

bool is_ref_path(char *path) {
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}
