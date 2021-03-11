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

//static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file
static clock_t time_init;

FILE* setup_event_logging(){
    time_init = clock();
    char* dir = getenv("LOG_FILENAME");
    if(dir == NULL){
        fprintf(stderr, "ENVP not found\n");
        return NULL;
    }
    FILE* val_open = fopen(dir, "w");
    if(val_open == NULL){
        fprintf(stderr, "Not open\n");
        return NULL;
    }
    return val_open;
}

int log_file(FILE* file, enum XMOD_EVENT event, const union EventLog *inf){   
  //time  
  clock_t time = clock();
  double mili_secs = (double) (time - time_init) / CLOCKS_PER_SEC *1000.0;
  fprintf(file, "%f ; ", mili_secs);
  
  //pid
  pid_t pid = getpid();
  fprintf(file, "%d ; ", pid);
  //event and info
  switch(event){
    case PROC_CREAT:
        fprintf(file, "PROC_CREAT ;");
        for(int i= 0; i<inf->arg.argc_info; i++){
            fprintf(file, " %s", inf->arg.argv_info[i]);
        }
        fprintf(file, "\n");
        break;
    case PROC_EXIT:
        fprintf(file, "PROC_EXIT ; %d\n", inf->exit_code);
        break;
    case SIGNAL_RECV:
        fprintf(file, "SIGNAL_RECV ; %s\n", inf->signal_received);
        break;
    case SIGNAL_SENT:
        fprintf(file, "SIGNAL_SENT ; %s : %d\n", inf->sent.signal_sent, inf->sent.pid_sent);
        break;
    case FILE_MODF:
        fprintf(file, "FILE_MODF ; %s : %o : %o\n", inf->perm.name_file, inf->perm.old_perms, inf->perm.new_perms);
        break;
    default:
        break;
    }
  return 0;
}

int log_close(FILE* file){
    int val_close = fclose(file);
    if(val_close!=0){ 
        fprintf(stderr, "Not closed\n");
        return -1;
    }
    return 0;
}