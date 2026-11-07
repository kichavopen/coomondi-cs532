/* Simple example to illustrate the use of pipes.
 * This example first opens a pipe and then forks a child process to create 
 * a copy of itself to communicate with the child via the pipe. Both the
 * parent and child processes have access to the pipe as the child inherits 
 * all the open pipe file descriptors belonging to the parent. After fork,
 * the parent closes the read file descriptor of the pipe and writes the 
 * string passed as the command-line argument to the pipe. The child closes 
 * the write file descriptor of the pipe and read the string from the pipe
 * one character at a time, converts it to uppercase, and writes it to 
 * standard out.
 * 
 * To Compile: gcc -Wall pipe1.c
 * To Run: ./a.out <string>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
  pid_t pid;
  int status;
  int pipefd[2]; /* pipefd[0] for read, pipefd[1] for write */
  char c;

  if (argc != 2) {
     printf("Usage: %s <string>\n", argv[0]);
     exit(-1);
  }

  if (pipe(pipefd) == 0) { /* Open a pipe */
    if ((pid = fork()) == 0) { /* I am the child process */
      close(pipefd[1]);    /* close write end */

      while (read(pipefd[0], &c, 1) > 0) {
          c = toupper(c);
          write(1, &c, 1);
      }
      write(1, "\n", 1);
      close(pipefd[0]);

      exit(EXIT_SUCCESS);
    } else if (pid > 0) { /* I am the parent process */
      close(pipefd[0]);   /* close read end */

      write(pipefd[1], argv[1], strlen(argv[1]));
      close(pipefd[1]);

      wait(&status);      /* wait for child to terminate */
      if (WIFEXITED(status))
         printf("Child process exited with status = %d\n", WEXITSTATUS(status));
      else
         printf("Child process did not terminate normally!\n");
    } else { /* we have an error in fork */
      perror("fork");
      exit(EXIT_FAILURE);
    }
  } else {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

                             

