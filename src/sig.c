#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "sig.h"
#include "log.h"

static char* file_dir;
static unsigned int nftot = 0;
static unsigned int nfmod = 0;
static bool running = true;

void handler(int signo){
	EventLog sig;
	if(signo == SIGINT){
		sig.signal_received = "SIGINT";
		log_event(SIGNAL_RECV, &sig);
		printf("\n%d ; %s ; %d ; %d \n",  getpid(), file_dir, nftot, nfmod);
		printf("Do you wish to continue? [Y/N]\n");
		char buf[100];
		scanf("%c", buf);
		if(buf[0] != 'y' && buf[0] != 'Y'){
			running = false;
		}
		while ( getchar() != '\n' );
	}
	else{
		sig.signal_received = strsignal(signo);
		log_event(SIGNAL_RECV, &sig);
	}

}

int set_handler(){
	for(int i = 1; i <= 64; i++){
		if(i!=SIGKILL && i!=SIGSTOP){
			struct sigaction new, old;
			sigset_t smask;	
			
			if (sigemptyset(&smask)==-1){
				perror ("sigsetfunctions");
				if(i==SIGINT) return -1;  //it will be really bad if we caan't treat the 'main' signal
			}
				
			new.sa_handler = handler;
			new.sa_mask = smask;
			new.sa_flags = 0;

			if(sigaction(i, &new, &old) == -1){
				perror ("sigaction");
				if(i==SIGINT) return -1;  //it will be really bad if we caan't treat the 'main' signal
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

bool get_running(){
	return running;
}