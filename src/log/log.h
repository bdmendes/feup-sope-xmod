#ifndef LOG_H
#define LOG_H

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

int log_event(XMOD_EVENT event, const EventLog *inf);

int setup_event_logging();

int close_log_file();

long double get_initial_instant();

int log_process_exit_creat(int exit_code);

#endif