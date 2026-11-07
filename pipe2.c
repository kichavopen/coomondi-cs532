/* Simple program to illustrate how to implement pipes.
 * This example creates a pipe and then creates two child processes.
 * The first child replaces the stdout with write end of the pipe and
 * execs the first command. The second child replaces the stdin with 
 * read end of the pipe and execs the second command. Thus, the stdout
 * of the first child is sent to the stdin of the second child. The parent
 * process closes both ends of the pipe and waits for the child process
 * to terminate.
 *
 * To Compile: gcc -Wall pipe2.c
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
    pid_t pid1, pid2;
    int pipefd[2]; /* fildes[0] for read, fildes[1] for write */
    int status1, status2;

    if (argc != 3) {
        printf("Usage: %s <command1> <command2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == 0) { /* Open a pipe */

      pid1 = fork(); /* fork first process to execute command1 */
      if (pid1 == 0) { /* this is the child process */
        /* close read end of the pipe */
	close(pipefd[0]);

	/* replace stdout with write end of pipe */
	if (dup2(pipefd[1], 1) == -1) {
           perror("dup2");
           exit(EXIT_FAILURE);
        }

        /* execute <command1> */
        execlp(argv[1], argv[1], (char *)NULL);
        printf("If you see this statement then exec failed ;-(\n");
        perror("execlp");
        exit(EXIT_FAILURE);

      } else if (pid1 < 0) { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
      }

      pid2 = fork(); /* fork second process to execute command2 */
      if (pid2 == 0) { /* this is child process */
        /* close write end of the pipe */
	close(pipefd[1]);

        /* replace stdin with read end of pipe */
	if (dup2(pipefd[0], 0) == -1) {
           perror("dup2");
           exit(EXIT_FAILURE);
        }

        /* execute <command2> */
        execlp(argv[2], argv[2], (char *)NULL);
        printf("If you see this statement then exec failed ;-(\n");
        perror("execlp");
        exit(EXIT_FAILURE);

      } else if (pid2 < 0) { /* we have an error */
        perror("fork"); /* use perror to print the system error message */
        exit(EXIT_FAILURE);
      }

      /* close the pipe in the parent */
      close(pipefd[0]);
      close(pipefd[1]);

      /* wait for both child processes to terminate */
      waitpid(pid1, &status1, 0);
      waitpid(pid2, &status2, 0);
    } else {
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    return 0;
}
