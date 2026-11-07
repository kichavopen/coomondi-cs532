/* Simple program to illustrate how to use popen and pclose system calls.
 *
 * The program reads a command as command-line argument, 
 * uses popen to create a pipe in "read" mode, 
 * reads the output of the pipe, and writes it to stdout.
 *
 * To Compile: gcc -Wall popen.c
 * To Run: ./a.out <command>
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *fp;
    char line[BUFSIZ];

    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((fp = popen(argv[1], "r")) == NULL) {
	perror("popen");
	exit(EXIT_FAILURE);
    }

    while (fgets(line, BUFSIZ, fp) != NULL) {
	  fputs(line, stdout); 
    }

    if (pclose(fp) == -1) {
	perror("pclose");
	exit(EXIT_FAILURE);
    }

    return 0;
}
