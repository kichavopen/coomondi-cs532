/******************************************************************************
 * Homework 1 - Process creation and management
 *
 * Reads a file of commands (one per line), executes each one in a child
 * process created with fork()+execvp(), waits for it to finish, and logs
 * the command, its start time, and its end time to output.log.
 *
 * To Compile: gcc -Wall -o runcmds runcmds.c
 * To Run:     ./runcmds <inputfile>
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARGS 64

/* ctime() returns a string ending in '\n'; copy it and strip that newline
 * so the log entry stays on a single, tab-separated line. */
static void copytime(char *dst, size_t n, time_t *t) {
    strncpy(dst, ctime(t), n - 1);
    dst[n - 1] = '\0';
    dst[strcspn(dst, "\n")] = '\0';
}

int main(int argc, char **argv) {
    FILE *in, *log;
    char line[MAXLINE];
    char *cmdline;
    char *args[MAXARGS];
    char startbuf[64], endbuf[64];
    int nargs;
    pid_t pid;
    int status;
    time_t start, end;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("fopen input file");
        exit(EXIT_FAILURE);
    }

    log = fopen("output.log", "w");
    if (log == NULL) {
        perror("fopen output.log");
        fclose(in);
        exit(EXIT_FAILURE);
    }

    /* Step 2: read the file one line at a time */
    while (fgets(line, sizeof(line), in) != NULL) {
        line[strcspn(line, "\n")] = '\0';   /* drop trailing newline */
        if (line[0] == '\0')                /* skip blank lines */
            continue;

        /* Duplicate the line so we can tokenize it without destroying
         * the original text used for logging. */
        cmdline = strdup(line);
        if (cmdline == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }

        /* Build the argv vector: program + its arguments */
        nargs = 0;
        char *tok = strtok(cmdline, " \t");
        while (tok != NULL && nargs < MAXARGS - 1) {
            args[nargs++] = tok;
            tok = strtok(NULL, " \t");
        }
        args[nargs] = NULL;

        if (nargs == 0) {
            free(cmdline);
            continue;
        }

        /* Step 4: note the start time just before forking */
        start = time(NULL);

        /* Step 3: fork-exec the program */
        pid = fork();
        if (pid == 0) {
            /* child: replace the process image with the target program.
             * execvp searches PATH when the name has no '/', and handles
             * absolute paths directly. */
            execvp(args[0], args);
            /* only reached if exec fails */
            perror("execvp");
            _exit(EXIT_FAILURE);
        } else if (pid > 0) {
            /* parent: wait for the child to terminate */
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                free(cmdline);
                continue;
            }

            /* Step 4: capture the completion time once wait returns */
            end = time(NULL);

            if (WIFEXITED(status)) {
                copytime(startbuf, sizeof(startbuf), &start);
                copytime(endbuf, sizeof(endbuf), &end);

                /* Step 5: log command, start time, end time (tab-separated) */
                fprintf(log, "%s\t%s\t%s\n", line, startbuf, endbuf);
                fflush(log);
            } else {
                fprintf(stderr,
                        "Child %ld did not terminate normally (status=%d)\n",
                        (long)pid, status);
            }
        } else {
            perror("fork");
        }

        free(cmdline);
        /* Step 6: loop back to read the next command */
    }

    fclose(in);
    fclose(log);
    return 0;
}
