#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../log/log.h"
#include "../signal/sig.h"

static char *file_dir;
static unsigned int nftot = 0;
static unsigned int nfmod = 0;
static bool running = true;

void handler_SIGINT(int signo) {
    EventLog sig;
    sig.signal_received = "SIGINT";
    log_event(SIGNAL_RECV, &sig);
    printf("\n%d ; %s ; %d ; %d \n", getpid(), file_dir, nftot, nfmod);
    printf("Do you wish to continue? [Y/N]\n");
    char buf[100];
    scanf("%c", buf);
    if (buf[0] != 'y' && buf[0] != 'Y') {
        running = false;
    }
    while (getchar() != '\n')
        ;
}

int set_handler_SIGINT() {
    struct sigaction new, old;
    sigset_t smask;

    if (sigemptyset(&smask) == -1) { // block no signal
        perror("sigsetfunctions");
        return -1;
    }

    new.sa_handler = handler_SIGINT;
    new.sa_mask = smask;
    new.sa_flags = 0; // usually works

    if (sigaction(SIGINT, &new, &old) == -1) {
        perror("sigaction");
        return -1;
    }

    nftot = 0;
    nfmod = 0;

    return 0;
}

void set_file(char *file) {
    file_dir = file;
}

void increment_nftot() {
    nftot += 1;
}

void increment_nfmod() {
    nfmod += 1;
}

bool get_running() {
    return running;
}