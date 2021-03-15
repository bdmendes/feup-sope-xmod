#include "log.h"
#include "xmod.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

static long double time_init;
static int log_fd = -1;

int setup_event_logging() {
    char *dir = getenv("LOG_FILENAME");
    if (dir == NULL) {
        fprintf(stderr, "LOG_FILENAME environment variable not found\n");
        return -1;
    }
    int fd = open(dir, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd == -1) {
        perror("log file open");
        return -1;
    }
    time_init = get_milisecs();
    log_fd = fd;
    return 0;
}

int log_event(XMOD_EVENT event, const EventLog *inf) {
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

long double get_milisecs(){
    struct timeval tim;
    gettimeofday(&tim, 0);
    long sec = tim.tv_sec;
    long microsec = tim.tv_usec;
    long double mili_secs = sec * 1000.0 + microsec/1000.0;
    return mili_secs;
}
