#ifndef SRC_LOG_LOG_H_
#define SRC_LOG_LOG_H_

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

#define LOG_FILE_PATH_ENV "LOG_FILENAME"
#define LOG_PARENT_INITIAL_TIME_ENV "LOG_XMOD_INITIAL_INSTANT"

/**
 * @brief Arguments needed to register a log in each case
 */
typedef union {
    struct {
        int argc_info;
        char **argv_info;
    } arg;
    int exit_code;
    char *signal_received;
    struct {
        char *signal_sent;
        pid_t pid_sent;
    } sent;
    struct {
        char *file_name;
        mode_t old_perms;
        mode_t new_perms;
    } perms;
} EventLog;

/**
 * @brief All the different types of log registers
 */
typedef enum XMOD_EVENT {
    PROC_CREAT,
    PROC_EXIT,
    SIGNAL_RECV,
    SIGNAL_SENT,
    FILE_MODF
} XMOD_EVENT;

/**
 * @brief Setup of all aspects related to the log
 * @return 0 in case of success, -1 otherwise
 */
int setup_event_logging();

/**
 * @brief Closes the log file
 * @return 0 in case of success, -1 otherwise
 */
int close_log_file();

/**
 * @brief Get the number of milissecs since January 1, 1970 until the beginnig of the principle process
 * @return Number of milissecs since January 1, 1970 until the beginnig of the principle process
 */
long double get_initial_instant();

/**
 * @brief Abstraction of the log register when a new process is created
 * @param argc Argc of the caller
 * @param argv argv of the caller
 * @return 0 uppon success, -1 otherwise
 */
int log_proc_creat_creat(int argc, char **argv);

/**
 * @brief Abstraction of the log register when a process exits
 * @param exit_code exit code of the caller
 * @return 0 uppon success, -1 otherwise
 */
int log_proc_exit_creat(int exit_code);

/**
 * @brief Abstraction of the log register when a process receives a signal
 * @param sign_receiv Name of the signal received
 * @return 0 uppon success, -1 otherwise
 */
int log_proc_sign_recev_creat(char* sign_receiv);

/**
 * @brief Abstraction of the log register when a process sends a signal
 * @param sign_sent Name of the signal sent
 * @param pid_sent PID of the target process
 * @return 0 uppon success, -1 otherwise
 */
int log_proc_sign_sent_creat(char* sign_sent, pid_t pid_sent);

/**
 * @brief Abstraction of the log register when a permission is changed
 * @param file_name Name of the file
 * @param old_perms Old permissions
 * @param nem_perms New permissions
 * @return 0 uppon success, -1 otherwise
 */
int log_proc_file_mod_creat(char *file_name, mode_t old_perms, mode_t new_perms);

#endif  // SRC_LOG_LOG_H_
