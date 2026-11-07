/* Simple program to illustrate how to implement pipes.
 * This example creates two pipes and three processes. The first pipe is
 * used to setup communication between the first two child processes and the
 * second pipe is used to setup communication between the second and third
 * process. 
 *
 * To Compile: gcc -Wall pipe3.c
 * To Run: ./a.out <command1> <command2> <command3>
 *          Output of <command1> will be the input for <command2> and
 *          output of <command2> will be the input for <command3>.
 *          Example: ./a.out ps sort wc
 *                   ./a.out ls sort wc
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    pid_t pid1, pid2, pid3;
    int pipefd1[2]; /* pipefd1[0] for read, pipefd1[1] for write */
    int pipefd2[2]; /* pipefd2[0] for read, pipefd2[1] for write */
    int status1, status2, status3;

    if (argc != 4) {
        printf("Usage: %s <command1> <command2> <command3>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd1) != 0) { /* Open pipefd1 */
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    if (pipe(pipefd2) != 0) { /* Open pipefd2 */
      perror("pipe");
      exit(EXIT_FAILURE);
    }

    pid1 = fork(); /* fork first process to execute command1 */
    if (pid1 == 0) { /* this is the child process */
      /* close read end of the pipefd1 */
      close(pipefd1[0]);

      /* close both ends of pipefd2 */
      close(pipefd2[0]);
      close(pipefd2[1]);

      /* replace stdout with write end of pipefd1 */
      if (dup2(pipefd1[1], 1) == -1) {
	perror("dup2");
	exit(EXIT_FAILURE);
      }
      
      /* execute <command1> */
      execlp(argv[1], argv[1], (char *)NULL);
      perror("execlp");
      exit(EXIT_FAILURE);
      
    } else if (pid1 < 0) { /* we have an error */
      perror("fork"); /* use perror to print the system error message */
      exit(EXIT_FAILURE);
    }
    
    pid2 = fork(); /* fork second process to execute command2 */
    if (pid2 == 0) { /* this is child process */
      /* close write end of the pipefd1 */
      close(pipefd1[1]);
      
      /* replace stdin with read end of pipefd2 */
      if (dup2(pipefd1[0], 0) == -1) {
	perror("dup2");
	exit(EXIT_FAILURE);
      }

      /* close read end of pipefd2 */
      close(pipefd2[0]);

      /* replace stdout with write end of pipefd2 */
      if (dup2(pipefd2[1], 1) == -1) {
	perror("dup2");
	exit(EXIT_FAILURE);
      }
      
      /* execute <command2> */
      execlp(argv[2], argv[2], (char *)NULL);
      perror("execlp");
      exit(EXIT_FAILURE);
      
    } else if (pid2 < 0) { /* we have an error */
      perror("fork"); /* use perror to print the system error message */
      exit(EXIT_FAILURE);
    }

    pid3 = fork(); /* fork third process to execute command3 */
    if (pid3 == 0) { /* this is child process */
      /* close both ends of the pipefd1 */
      close(pipefd1[0]);
      close(pipefd1[1]);
      
      /* close write end of pipefd2 */
      close(pipefd2[1]);

      /* replace stdin with read end of pipefd2 */
      if (dup2(pipefd2[0], 0) == -1) {
	perror("dup2");
	exit(EXIT_FAILURE);
      }
      
      /* execute <command3> */
      execlp(argv[3], argv[3], (char *)NULL);
      perror("execlp");
      exit(EXIT_FAILURE);
      
    } else if (pid3 < 0) { /* we have an error */
      perror("fork"); /* use perror to print the system error message */
      exit(EXIT_FAILURE);
    }
    
    /* close the pipes in the parent */
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);
    
    /* wait for both child processes to terminate */
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    waitpid(pid3, &status3, 0);

    return 0;
}
