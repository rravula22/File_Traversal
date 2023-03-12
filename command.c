#include<unistd.h>
#include <stdlib.h>

void executeCommand(char* line) {
  pid_t pid;
  int status;
  char *args[] = {"sh", "-c", line, NULL};
  pid = fork();
  
  if (pid == 0) {
    execvp(args[0], args);
    perror("exec");
    exit(-1);
  } 
  else if (pid > 0) {
    printf("\nWait for the child process to terminate");
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) { 
      printf("\nChild process exited with status = %d\t", WEXITSTATUS(status));
    } else {
      printf("\nChild process did not terminate normally!");
    }
  } 
  else {
    perror("fork"); 
    exit(EXIT_FAILURE);
  }
}