/*
Name: Chris
BlazerId: coomondi
Project #: 11
To compile: make
To run: ./lab11
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCMD 4096

/* Strip the trailing newline that fgets leaves behind. */
static void chomp(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

int main(void) {
    char cmd[MAXCMD];
    FILE *fp;
    char line[BUFSIZ];

    for (;;) {
        /* prompt; flush so it appears before the command runs */
        printf("Enter command: ");
        fflush(stdout);

        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            /* EOF (Ctrl-D) treated like quit */
            printf("\n");
            break;
        }

        chomp(cmd);

        /* empty line: just re-prompt */
        if (cmd[0] == '\0')
            continue;

        /* exit condition */
        if (strcmp(cmd, "quit") == 0)
            break;

        /* popen runs the string through "sh -c", so wildcards,
         * pipes and redirection all work as they would at a prompt */
        fp = popen(cmd, "r");
        if (fp == NULL) {
            perror("popen");
            continue;
        }

        while (fgets(line, sizeof(line), fp) != NULL)
            fputs(line, stdout);

        if (pclose(fp) == -1)
            perror("pclose");
    }

    printf("Exiting program...bye!\n");
    return 0;
}
