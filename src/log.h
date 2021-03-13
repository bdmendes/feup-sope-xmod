#ifndef LOG_H
#define LOG_H

#include <sys/types.h>

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

int log_close(int file);

#endif