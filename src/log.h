#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include "xmod.h"

union EventLog{
    struct {
        int argc_info;
        char **argv_info;
    } arg;
    int exit_code;
    char* signal_received;
    struct {
        char* signal_sent;
        int pid_sent;
    } sent;
    struct {
        char* name_file;
        mode_t old_perms;
        mode_t new_perms;
    } perm;
};

enum XMOD_EVENT { PROC_CREAT, PROC_EXIT, SIGNAL_RECV, SIGNAL_SENT, FILE_MODF };

int log_file(int file, enum XMOD_EVENT event, const union EventLog *inf);

int setup_event_logging();

int log_close(int file);

#endif