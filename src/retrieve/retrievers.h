#ifndef SRC_RETRIEVE_RETRIEVERS_H_
#define SRC_RETRIEVE_RETRIEVERS_H_

#include <sys/stat.h>

typedef unsigned char file_type;

/**
 * @brief Structure that stores useful file information, such as the relative
 * path, the permission mode (in octal) and the type of file
 *
 */
typedef struct {
    /**
     * @brief relative path
     *
     */
    const char *path;
    /**
     * @brief permission mode
     *
     */
    mode_t octal_mode;
    /**
     * @brief type of file
     *
     */
    file_type type;
} FileInfo;

/**
 * @brief Retrieve a file's relevant information given it's relative path
 *
 * @param file_info structure where the info is to be stored
 * @param file_path relative path to the file
 * @return int 0 if no errors occured and -1 otherwise
 */
int retrieve_file_info(FileInfo *file_info, char *file_path);

#endif // SRC_RETRIEVE_RETRIEVERS_H_
