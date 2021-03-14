#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static clock_t time_init;
static int log_fd = -1;
static bool make_logs = true;

int setup_event_logging() {
    char *log_file_path = getenv(LOG_FILE_PATH_ENV);
    if (log_file_path == NULL) {
        fprintf(stderr, "%s environment variable not found\n",
                LOG_FILE_PATH_ENV);
        make_logs = false;
        return -1;
    }

    int fd;
    bool is_top_process = getpid() == getpgrp();
    fd = open(log_file_path,
              O_WRONLY | (is_top_process ? O_CREAT | O_TRUNC : O_APPEND),
              S_IRWXU);
    if (fd == -1) {
        perror("log file open");
        return -1;
    }

    if (!is_top_process) {
        char *saved_time_init = getenv(LOG_PARENT_INITIAL_TIME_ENV);
        if (saved_time_init == NULL) {
            fprintf(stderr, "%s environment variable not found\n",
                    LOG_PARENT_INITIAL_TIME_ENV);
            if (close(fd) != 0) {
                perror("could not close file");
            }
            return -1;
        }
        time_init = strtoul(saved_time_init, NULL, 10);
        printf("GETTING SAVED TIME INIT: %lu\n", time_init);
        fflush(stdout);
    } else {
        time_init = clock();
        printf("GETTING TIME INFO FOR FIRST TIME: %lu\n", time_init);
    }

    log_fd = fd;
    return 0;
}

int log_event(XMOD_EVENT event, const EventLog *inf) {
    char buf[PATH_MAX];
    char *curr_buf = buf;

    // time
    clock_t time = clock();
    double time_passed_mili_secs =
        (double)(time - time_init) / CLOCKS_PER_SEC *
        1000.0; // calcs not needed after proper use of wall time
    curr_buf += sprintf(curr_buf, "%f ; ", time_passed_mili_secs);

    // pid
    pid_t pid = getpid();
    curr_buf += sprintf(curr_buf, "%d ; ", pid);

    // event and info
    switch (event) {
        case PROC_CREAT:
            curr_buf += sprintf(curr_buf, "PROC_CREAT ;");
            for (int i = 0; i < inf->arg.argc_info; i++) {
                curr_buf += sprintf(curr_buf, " %s", inf->arg.argv_info[i]);
            }
            curr_buf += sprintf(curr_buf, "\n");
            break;
        case PROC_EXIT:
            curr_buf += sprintf(curr_buf, "PROC_EXIT ; %d\n", inf->exit_code);
            break;
        case SIGNAL_RECV:
            curr_buf +=
                sprintf(curr_buf, "SIGNAL_RECV ; %s\n", inf->signal_received);
            break;
        case SIGNAL_SENT:
            curr_buf += sprintf(curr_buf, "SIGNAL_SENT ; %s : %d\n",
                                inf->sent.signal_sent, inf->sent.pid_sent);
            break;
        case FILE_MODF:
            curr_buf +=
                sprintf(curr_buf, "FILE_MODF ; %s : %o : %o\n",
                        inf->perms.file_name, inf->perms.old, inf->perms.new);
            break;
        default:
            return -1;
    }

    write(log_fd, buf, strlen(buf));
    return 0;
}

int close_log_file() {
    if (close(log_fd) != 0) {
        perror("log file close");
        return -1;
    }
    return 0;
}

clock_t get_initial_instant() {
    return time_init;
}

inline bool are_logs_enabled() {
    return make_logs;
}