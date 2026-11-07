/* Simple program to illustrate how to implement pipes.
 * This example reads two commands as command-line arguments, 
 * creates a pipe and then creates a child process.
 * The parent process replaces the stdout with write end of the pipe and
 * execs the first command. The child replaces the stdin with 
 * read end of the pipe and execs the second command. Thus, the stdout
 * of the parent process is sent to the stdin of the child process. 
 *
 * To Compile: gcc -Wall pipe0.c
 * To Run: ./a.out <command1> <command2> 
 *          Output of <command1> will be the input for <command2>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    pid_t pid;
    int pipefd[2]; /* fildes[0] for read, fildes[1] for write */

    if (argc != 3) {
        printf("Usage: %s <command1> <command2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == 0) { /* Open a pipe */
      pid = fork(); /* fork child process to execute command2 */
      if (pid == 0) { /* this is the child process */
        /* close write end of the pipe */
	close(pipefd[1]);

        /* replace stdin with read end of pipe */
	if (dup2(pipefd[0], 0) == -1) {
           perror("dup2");
           exit(EXIT_FAILURE);
        }

        /* execute <command2> */
        execlp(argv[2], argv[2], (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
      } else if (pid > 0) { /* this is the parent process */
        /* close read end of the pipe */
	close(pipefd[0]);

	/* replace stdout with write end of pipe */
	if (dup2(pipefd[1], 1) == -1) {
           perror("dup2");
           exit(EXIT_FAILURE);
        }

        /* execute <command1> */
        execlp(argv[1], argv[1], (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
      } else if (pid < 0) { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
      }
    } else {
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    return 0;
}
