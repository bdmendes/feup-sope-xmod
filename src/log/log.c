#include "log.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

static long double time_init;
static int log_fd;
static bool make_logs = true;
static bool is_group_leader = true;

/**
 * @brief Writes in the log file in the correct format
 * @param event Type of event that occurred
 * @param inf Arguments of the specific event
 * @return 0 in case of success, -2 in fail of flock, -1 for the restant problems
 */
static int log_event(XMOD_EVENT event, const EventLog *inf);

/**
 * @brief Get the number of milissecs since January 1, 1970
 * @return Number of milissecs since January 1, 1970
 */
static double get_milisecs() {
    struct timeval tim;
    gettimeofday(&tim, 0);
    double sec = tim.tv_sec;
    double microsec = tim.tv_usec;
    return sec * 1000.0 + microsec / 1000.0;
}

static int log_event(XMOD_EVENT event, const EventLog *inf) {
    if (!make_logs)
        return -1;
    char buf[PATH_MAX];

    // time
    long double time = get_milisecs();
    long double time_passed_mili_secs = time - time_init;
    snprintf(buf, sizeof(buf), "%Lf ; ", time_passed_mili_secs);

    // pid
    pid_t pid = getpid();
    snprintf(buf + strlen(buf), sizeof(buf), "%d ; ", pid);

    // event and info
    switch (event) {
        case PROC_CREAT:
            snprintf(buf + strlen(buf), sizeof(buf), "PROC_CREAT ;");
            for (int i = 0; i < inf->arg.argc_info; i++) {
                snprintf(buf + strlen(buf), sizeof(buf), " %s",
                         inf->arg.argv_info[i]);
            }
            snprintf(buf + strlen(buf), sizeof(buf), "%s", "\n");
            break;
        case PROC_EXIT:
            snprintf(buf + strlen(buf), sizeof(buf), "PROC_EXIT ; %d\n",
                     inf->exit_code);
            break;
        case SIGNAL_RECV:
            snprintf(buf + strlen(buf), sizeof(buf), "SIGNAL_RECV ; %s\n",
                     inf->signal_received);
            break;
        case SIGNAL_SENT:
            snprintf(buf + strlen(buf), sizeof(buf), "SIGNAL_SENT ; %s : %d\n",
                     inf->sent.signal_sent, inf->sent.pid_sent);
            break;
        case FILE_MODF:
            snprintf(buf + strlen(buf), sizeof(buf),
                     "FILE_MODF ; %s : %o : %o\n", inf->perms.file_name,
                     inf->perms.old_perms, inf->perms.new_perms);
            break;
        default:
            return -1;
    }

    // Lock file for writing
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    if (fcntl(log_fd, F_SETLKW, &lock) == -1) {
        if (errno != EINTR) {
            perror("log file log acquire");
            return -1;
        }
    }

    // Write assembled log to file
    lseek(log_fd, 0, SEEK_END);
    write(log_fd, buf, strlen(buf));

    // Release the lock for writing
    lock.l_type = F_UNLCK;
    if (fcntl(log_fd, F_SETLKW, &lock) == -1) {
        if (errno != EINTR) {
            perror("log file log release");
            return -1;
        }
    }

    return 0;
}

int setup_event_logging() {
    is_group_leader = getpid() == getpgrp();

    // Get log file path
    char *log_file_path = getenv(LOG_FILE_PATH_ENV);
    if (log_file_path == NULL) {
        make_logs = false;
        return 0;
    }

    // Get current instant as initial, or retrieve from group leader
    if (!is_group_leader) {
        char *saved_time_init = getenv(LOG_PARENT_INITIAL_TIME_ENV);
        if (saved_time_init == NULL) {
            fprintf(stderr, "%s environment variable not found\n",
                    LOG_PARENT_INITIAL_TIME_ENV);
            make_logs = false;
            return -1;
        } else {
            time_init = strtold(saved_time_init, NULL);
        }
    } else {
        time_init = get_milisecs();
    }

    // Parent opens the file and truncates it once
    log_fd = open(log_file_path,
                  O_WRONLY | (is_group_leader ? O_CREAT | O_TRUNC : O_APPEND));
    if (log_fd == -1) {
        perror("log file open");
        make_logs = false;
        return -1;
    }

    make_logs = true;
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

int log_proc_creat_creat(int argc, char **argv) {
    EventLog args;
    args.arg.argc_info = argc;
    args.arg.argv_info = argv;
    return log_event(PROC_CREAT, &args);
}

int log_proc_exit_creat(int exit_code) {
    EventLog args;
    args.exit_code = exit_code;
    return log_event(PROC_EXIT, &args);
}

int log_proc_sign_recev_creat(char *sign_receiv) {
    EventLog args;
    args.signal_received = sign_receiv;
    return log_event(SIGNAL_RECV, &args);
}

int log_proc_sign_sent_creat(char *sign_sent, pid_t pid_sent) {
    EventLog args;
    args.sent.signal_sent = sign_sent;
    args.sent.pid_sent = pid_sent;
    return log_event(SIGNAL_SENT, &args);
}

int log_proc_file_mod_creat(char *file_name, mode_t old_perms,
                            mode_t new_perms) {
    EventLog args;
    args.perms.file_name = file_name;
    args.perms.old_perms = old_perms;
    args.perms.new_perms = new_perms;
    return log_event(FILE_MODF, &args);
}

