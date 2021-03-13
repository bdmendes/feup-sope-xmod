#include <stdio.h>

#include "sig.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler_SIGINT(int signo){
    printf("heilo");
}

int set_handler_SIGINT(){
    struct sigaction new, old;
	sigset_t smask;	
	
	if (sigemptyset(&smask)==-1)	// block no signal
		perror ("sigsetfunctions");
		
	new.sa_handler = handler_SIGINT;
	new.sa_mask = smask;
	new.sa_flags = 0;	// usually works

	if(sigaction(SIGINT, &new, &old) == -1)
		perror ("sigaction");
	return 0;
}