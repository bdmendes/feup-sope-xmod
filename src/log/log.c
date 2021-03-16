#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static long double time_init;
static int log_fd = -1;
static bool make_logs = true;

static long double get_milisecs() {
    struct timeval tim;
    gettimeofday(&tim, 0);
    long sec = tim.tv_sec;
    long microsec = tim.tv_usec;
    return sec * 1000.0 + microsec / 1000.0;
}

int setup_event_logging() {
    bool is_group_leader = getpid() == getpgrp();

    // Get log file path
    char *log_file_path = getenv(LOG_FILE_PATH_ENV);
    if (log_file_path == NULL) {
        make_logs = false;
        return 0;
    }

    // Open log file for writing
    int fd;
    fd = open(log_file_path,
              O_WRONLY | (is_group_leader ? O_CREAT | O_TRUNC : O_APPEND),
              S_IRWXU);
    if (fd == -1) {
        perror("log file open");
        make_logs = false;
        return -1;
    }
    log_fd = fd;

    // Get current instant as initial, or retrieve from group leader
    if (!is_group_leader) {
        char *saved_time_init = getenv(LOG_PARENT_INITIAL_TIME_ENV);
        if (saved_time_init == NULL) {
            fprintf(stderr, "%s environment variable not found\n",
                    LOG_PARENT_INITIAL_TIME_ENV);
            if (close(fd) != 0) {
                perror("log file close");
            }
            make_logs = false;
            return -1;
        } else {
            time_init = strtold(saved_time_init, NULL);
            printf("GETTING SAVED TIME INIT: %Lf\n", time_init);
            fflush(stdout);
        }
    } else {
        time_init = get_milisecs();
        printf("GETTING TIME INFO FOR FIRST TIME: %Lf\n", time_init);
        fflush(stdout);
    }

    make_logs = true;
    return 0;
}

int log_event(XMOD_EVENT event, const EventLog *inf) {
    if (!make_logs)
        return -1;
    char buf[PATH_MAX];
    char *curr_buf = buf;

    // time
    double long time = get_milisecs();
    double long time_passed_mili_secs = time - time_init;
    curr_buf += sprintf(curr_buf, "%Lf ; ", time_passed_mili_secs);

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

long double get_initial_instant() {
    return time_init;
}
