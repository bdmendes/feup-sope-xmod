#define NUMBER_OF_SIG 32

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../log/log.h"
#include "sig.h"

static char file_dir[PATH_MAX];
static unsigned int nftot = 0;
static unsigned int nfmod = 0;

void handler(int signo) {
    if (signo == SIGINT) {
        log_proc_sign_recev_creat("SIGINT");
        bool is_group_leader = getpid() == getpgrp();
        if (is_group_leader) {
            for (;;) {
                printf("\n%d ; %s ; %d ; %d \n", getpid(), file_dir, nftot,
                       nfmod);
                printf("Do you wish to continue? [Y/N]\n");
                fflush(stdout);
                char buf[2];
                scanf("%c", buf);
                if (buf[0] == 'N' || buf[0] == 'n') {
                    log_proc_sign_sent_creat("SIGKILL", 0);
                    kill(0, SIGKILL);
                    break;
                } else if (buf[0] == 'Y' || buf[0] == 'y') {
                    log_proc_sign_sent_creat("SIGUSR1", 0);
                    kill(0, SIGUSR1);
                    break;
                }
                while (getchar() != '\n')
                    ;
            }
        } else {
            pause();
        }
    } else {
        log_proc_sign_recev_creat(strsignal(signo));
    }
}

int setup_signal_handler() {
    struct sigaction new, old;
    sigset_t smask;
    for (int i = 1; i < NUMBER_OF_SIG; i++) {
        if (i != SIGKILL && i != SIGSTOP) {
            if (sigemptyset(&smask) == -1) {
                perror("sigsetfunctions");
            }

            new.sa_handler = handler;
            new.sa_mask = smask;
            new.sa_flags = 0;

            if (sigaction(i, &new, &old) == -1) {
                perror("sigaction");
            }
        }
    }

    nftot = 0;
    nfmod = 0;
    return 0;
}

void increment_nftot() {
    nftot += 1;
}

void increment_nfmod() {
    nfmod += 1;
}

void set_sig_file_path(char path[]) {
    snprintf(file_dir, sizeof(file_dir), "%s", path);
}
