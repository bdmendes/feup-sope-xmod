#ifndef SRC_LOG_LOG_H_
#define SRC_LOG_LOG_H_

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

#define LOG_FILE_PATH_ENV "LOG_FILENAME"
#define LOG_PARENT_INITIAL_TIME_ENV "LOG_XMOD_INITIAL_INSTANT"

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

typedef enum XMOD_EVENT {
    PROC_CREAT,
    PROC_EXIT,
    SIGNAL_RECV,
    SIGNAL_SENT,
    FILE_MODF
} XMOD_EVENT;

int setup_event_logging();

int close_log_file();

long double get_initial_instant();

int log_proc_creat_creat(int argc, char **argv);

int log_proc_exit_creat(int exit_code);

int log_proc_sign_recev_creat(char* sign_receiv);

int log_proc_sign_sent_creat(char* sign_sent, pid_t pid_sent);

int log_proc_file_mod_creat(char *file_name, mode_t old_perms, mode_t new_perms);

#endif  // SRC_LOG_LOG_H_
