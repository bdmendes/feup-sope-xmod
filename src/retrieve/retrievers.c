#include "retrievers.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Save the information about a file's mode (and therefore the type) and
 * the relative path to an appropriate structure
 *
 * @param file_info where the values are to be saved
 * @param mode file's permission mode (in octal)
 * @param path file's relative path
 */
static void assemble_file_info(FileInfo *file_info, const mode_t mode,
                               const char *path);

/**
 * @brief Retrieve a file's permission mode given it's relative path
 *
 * @param path_name relative path to the file
 * @param file_mode mode read from the file
 * @return int 0 if no errors occured and -1 otherwise
 */
static int retrieve_file_mode(const char *path_name, mode_t *file_mode);

int retrieve_file_info(FileInfo *file_info, char *file_path) {
    memset(file_info, 0, sizeof(*file_info));
    mode_t mode = 0;
    if (retrieve_file_mode(file_path, &mode) != 0)
        return -1;
    assemble_file_info(file_info, mode, file_path);
    return 0;
}

static void assemble_file_info(FileInfo *file_info, const mode_t mode,
                               const char *path) {
    file_info->path = path;
    file_info->octal_mode = mode & 0777;

    if (S_ISREG(mode))
        file_info->type = DT_REG;
    else if (S_ISDIR(mode))
        file_info->type = DT_DIR;
    else if (S_ISCHR(mode))
        file_info->type = DT_CHR;
    else if (S_ISBLK(mode))
        file_info->type = DT_BLK;
    else if (S_ISFIFO(mode))
        file_info->type = DT_FIFO;
    else if (S_ISLNK(mode))
        file_info->type = DT_LNK;
    else if (S_ISSOCK(mode))
        file_info->type = DT_SOCK;
    else
        file_info->type = DT_UNKNOWN;
}

static int retrieve_file_mode(const char *path_name, mode_t *file_mode) {
    struct stat sb;
    if (stat(path_name, &sb) < 0) {
        return -1;
    }
    *file_mode = sb.st_mode;
    return 0;
}
