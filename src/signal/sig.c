#define NUMBER_OF_SIGNALS 31

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../log/log.h"
#include "sig.h"

static char *file_dir;
static unsigned int nftot = 0;
static unsigned int nfmod = 0;

void handler(int signo) {
    switch (signo) {
        case SIGINT: {
            log_proc_sign_recev_creat("SIGINT");
            killpg(0, SIGUSR2);
            log_proc_sign_sent_creat("SIGUSR2", 0);
            break;
        }
        case SIGUSR2: {
            log_proc_sign_recev_creat("SIGUSR2");
            bool is_group_leader = getpid() == getpgrp();
            if (!is_group_leader) {
                pause();
            } else {
                printf("\n%d ; %s ; %d ; %d \n", getpid(), file_dir, nftot,
                       nfmod);
                printf("Do you wish to continue? [Y/N]\n");
                char buf[2];
                scanf("%c", buf);
                if (buf[0] != 'y' && buf[0] != 'Y') {
                    killpg(0, SIGKILL);
                    log_proc_sign_sent_creat("SIGKILL", 0);
                } else {
                    killpg(0, SIGCONT);
                    log_proc_sign_sent_creat("SIGCONT", 0);
                }
                while (getchar() != '\n')
                    ;
            }
            break;
        }
        default: {
            log_proc_sign_recev_creat(strsignal(signo));
        }
    }
}

int set_handler() {
    struct sigaction new, old;
    sigset_t smask;
    for (int i = 1; i < NUMBER_OF_SIGNALS; i++) {
        if (i != SIGKILL && i != SIGSTOP) {
            if (sigemptyset(&smask) == -1) {
                perror("sigsetfunctions");
                if (i == SIGINT)
                    return -1; // it will be really bad if we can't treat
                               // the 'main' signal
            }

            new.sa_handler = handler;
            new.sa_mask = smask;
            new.sa_flags = 0;

            if (sigaction(i, &new, &old) == -1) {
                perror("sigaction");
                if (i == SIGINT)
                    return -1; // it will be really bad if we can't treat
                               // the 'main' signal
            }
        }
    }

    nftot = 0;
    nfmod = 0;

    return 0;
}

void set_log_file(char *file) {
    file_dir = file;
}

void increment_nftot() {
    nftot += 1;
}

void increment_nfmod() {
    nfmod += 1;
}
