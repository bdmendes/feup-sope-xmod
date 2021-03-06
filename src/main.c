#include <stdio.h>
#include <stdlib.h>


//static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

typedef struct{ //opinião da modelação
  char* cmd;
  int exit_code;
  char* signal;   //mudar isto para o "tipo" de sinal
  int process_target;  
  char* direct;
  int old_perms;
  int new_perms;
} info;

int log_file(char* action, info inf){   //vamos querer tipo evento ou string para action ou definido com macro
  char* dir = getenv("LOG_FILENAME");   //vamos querer só nome na variável ou o caminho
  if(dir == NULL){
    printf("ENVP not found\n");
    return 1;
    }
  printf("%s", dir);
  return 0;
}

int main(int argc, char **argv) {
  for (int i = 0; i < argc; ++i) {
    printf("Argument %d: %s\n", i, argv[i]);
  }

  info hello;
  log_file("PROC_CREAT", hello);

  return 0;
}
