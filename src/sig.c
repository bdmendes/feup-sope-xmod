#include <stdio.h>

#include "sig.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

static char* file_dir;
static unsigned int nftot = 0;
static unsigned int nfmod = 0;
static bool running = 0;

void handler_SIGINT(int signo){
    printf("\n%d ; %s ; %d ; %d \n",  getpid(), file_dir, nftot, nfmod);

}

int set_handler_SIGINT(){
    struct sigaction new, old;
	sigset_t smask;	
	
	if (sigemptyset(&smask)==-1){	// block no signal
		perror ("sigsetfunctions");
		return -1;
	}
		
	new.sa_handler = handler_SIGINT;
	new.sa_mask = smask;
	new.sa_flags = 0;	// usually works

	if(sigaction(SIGINT, &new, &old) == -1){
		perror ("sigaction");
		return -1;
	}

	nftot = 0;
	nfmod = 0;
	
	return 0;
}

void set_file(char* file){
	file_dir = file;
}

void set_nftot(char* file){
	nftot += 1;
}

void set_nfmod(char* file){
	nfmod += 1;
}

bool get_running(){
	return running;
}