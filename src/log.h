#ifndef LOG_H
#define LOG_H

#include <sys/types.h>
#include <stdbool.h>

#define LOG_FILE_PATH_ENV "LOG_FILENAME"
#define LOG_PARENT_INITIAL_TIME_ENV "LOG_XMOD_INITIAL_INSTANT"

typedef union EventLog{
    struct {
        int argc_info;
        char **argv_info;
    } arg;
    int exit_code;
    char* signal_received;
    struct {
        char* signal_sent;
        pid_t pid_sent;
    } sent;
    struct {
        char* file_name;
        mode_t old;
        mode_t new;
    } perms;
} EventLog;

typedef enum XMOD_EVENT { PROC_CREAT, PROC_EXIT, SIGNAL_RECV, SIGNAL_SENT, FILE_MODF } XMOD_EVENT;

int log_event(XMOD_EVENT event, const EventLog *inf);

int setup_event_logging();

int close_log_file();

clock_t get_initial_instant();

bool are_logs_enabled();

#endif