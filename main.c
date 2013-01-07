#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1



int main(){

  pid_t pid;	
	int pipefd[2];
	int status;

  int fd = open("outfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  
  if (fd != -1) {
  
    dup2(fd, 1);   /* make stdout go to outfile */
    dup2(fd, 2);       
    close(fd);        
  	pipe(pipefd);
    
    if ((pid = fork()) == 0) {  /* child */
  
      close(pipefd[0]);
      dup2(pipefd[1], 1);
      dup2(pipefd[1], 2);
      close(pipefd[1]); 
      
      execlp("ls", "ls", "-1", NULL);  
    
    } else { /* parent */

      waitpid(pid,&status,0);
      
      close(pipefd[WRITE_END]);
      
      dup2(pipefd[0], 0);
      
      close(pipefd[READ_END]);
  
      execlp("sort", "sort","-r", NULL);
      
    }
    
    return 1;

  } else {  
  
    printf("outfile: Permission denied\n");
    exit(1);
  
  }
  
}