#define NUMBER_OF_SIG 32

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "sig.h"
#include "../log/log.h"

static char* file_dir;
static unsigned int nftot = 0;
static unsigned int nfmod = 0;

void handler(int signo){
	if(signo == SIGINT){
		log_proc_sign_recev_creat("SIGINT");
		if(getpid() == getpgrp()){
			printf("\n%d ; %s ; %d ; %d \n",  getpid(), file_dir, nftot, nfmod);
			printf("Do you wish to continue? [Y/N]\n");
			char buf[100];
			scanf("%c", buf);
			if(buf[0] != 'y' && buf[0] != 'Y'){
				kill(0, SIGKILL);
				log_proc_sign_sent_creat("SIGKILL", 0);
			}
			else{ 
				kill(0, SIGCONT);
				log_proc_sign_sent_creat("SIGCONT", 0);
			}
			while ( getchar() != '\n' );
		}
		else {
			pause();
		}
	}
	else{
		log_proc_sign_recev_creat(strsignal(signo));
	}

}

int set_handler(){
	struct sigaction new, old;
	sigset_t smask;	
	for(int i = 1; i < NUMBER_OF_SIG; i++){
		if(i!=SIGKILL && i!=SIGSTOP){
			if (sigemptyset(&smask)==-1){
				perror ("sigsetfunctions");
				if(i==SIGINT) return -1;  //it will be really bad if we can't treat the 'main' signal
			}
				
			new.sa_handler = handler;
			new.sa_mask = smask;
			new.sa_flags = 0;

			if(sigaction(i, &new, &old) == -1){
				perror ("sigaction");
				if(i==SIGINT) return -1;  //it will be really bad if we can't treat the 'main' signal
			}
		}
	}

	nftot = 0;
	nfmod = 0;
	
	return 0;
}

void set_file(char* file){
	file_dir = file;
}

void increment_nftot(){
	nftot += 1;
}

void increment_nfmod(){
	nfmod += 1;
}

