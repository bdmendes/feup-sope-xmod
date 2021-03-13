#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "xmod.h"
#include "log.h"
#include <signal.h>
#include <fcntl.h>

//static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file
static clock_t time_init;


int setup_event_logging(){
    time_init = clock();
    char* dir = getenv("LOG_FILENAME");
    if(dir == NULL){
        fprintf(stderr, "ENVP not found\n");
        return -1;
    }
    int val_open = open(dir, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if(val_open == -1){
        fprintf(stderr, "Not open\n");
        return -1;
    }
    return val_open;
}

int log_file(int file, enum XMOD_EVENT event, const union EventLog *inf){   
  //time  
  clock_t time = clock();
  double mili_secs = (double) (time - time_init) / CLOCKS_PER_SEC *1000.0;
  char to_wrt[50];
  sprintf(to_wrt, "%f ; ", mili_secs);
  write(file, to_wrt, strlen(to_wrt));
  
  //pid
  pid_t pid = getpid();
  sprintf(to_wrt, "%d ; ", pid);
  write(file, to_wrt, strlen(to_wrt));
  //event and info
  switch(event){
    case PROC_CREAT:
        sprintf(to_wrt, "PROC_CREAT ;");
        write(file, to_wrt, strlen(to_wrt));
        for(int i= 0; i<inf->arg.argc_info; i++){
            sprintf(to_wrt, " %s", inf->arg.argv_info[i]);
            write(file, to_wrt, strlen(to_wrt));
        }
        sprintf(to_wrt, "\n");
        write(file, to_wrt, strlen(to_wrt));
        break;
    case PROC_EXIT:
        sprintf(to_wrt, "PROC_EXIT ; %d\n", inf->exit_code);
        write(file, to_wrt, strlen(to_wrt));
        break;
    case SIGNAL_RECV:
        sprintf(to_wrt, "SIGNAL_RECV ; %s\n", inf->signal_received);
        write(file, to_wrt, strlen(to_wrt));
        break;
    case SIGNAL_SENT:
        sprintf(to_wrt, "SIGNAL_SENT ; %s : %d\n", inf->sent.signal_sent, inf->sent.pid_sent);
        write(file, to_wrt, strlen(to_wrt));
        break;
    case FILE_MODF:
        sprintf(to_wrt, "FILE_MODF ; %s : %o : %o\n", inf->perm.name_file, inf->perm.old_perms, inf->perm.new_perms);
        write(file, to_wrt, strlen(to_wrt));
        break;
    default:
        break;
    }
  return 0;
}

int log_close(int file){
    int val_close = close(file);
    if(val_close!=0){ 
        fprintf(stderr, "Not closed\n");
        return -1;
    }
    return 0;
}