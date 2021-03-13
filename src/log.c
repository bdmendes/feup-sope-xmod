#include "log.h"
#include "xmod.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static clock_t time_init;
static int log_fd = -1;

int setup_event_logging() {
    char *dir = getenv("LOG_FILENAME");
    if (dir == NULL) {
        perror("LOG_FILENAME environment variable not found");
        return -1;
    }
    int fd = open(dir, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd == -1) {
        fprintf(stderr, "could not open %s", dir);
        return -1;
    }
    time_init = clock();
    log_fd = fd;
    return 0;
}

int log_event(XMOD_EVENT event, const EventLog *inf) {
    char buf[PATH_MAX];
    char *curr_buf = buf;

    // time
    clock_t time = clock();
    double time_passed_mili_secs =
        (double)(time - time_init) / CLOCKS_PER_SEC * 1000.0;
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

int log_close(int file) {
    if (close(file) != 0) {
        perror("could not close log file");
        return -1;
    }
    return 0;
}