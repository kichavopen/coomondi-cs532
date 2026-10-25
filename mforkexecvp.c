/* =====================================================================
 *  forkexecvp.c -- Lab: UNIX Signals (changed) 
* 
* Took this from an example that would fork and execvp() a process.
* 
* Our problem asked us to 
* "if you hit Control-C or Control-Z the child will be
* stopped or suspended and the parent will continue to
* wait for a quit signal (Control-\)" 
* 
* Here I made it so when you press
* Control-C the signal is sent to the child and it is
* stopped. Parent will continue to wait. 
* Control-Z the signal is sent to the child and it is
* suspended. Parent will continue to wait. 
* Control-\ will send the child a SIGCONT to wake it up and then
* a SIGQUIT to kill it. The Parent will no longer wait. 
* 
* By setting a handler for each signal we allow the child to receive
* these signals when they are sent to the parent. If we set 
* SIG_IGN as the handler then the child would inherit that after
* exec() was called. We wouldnt be able to interrupt or suspend
* the child. 
* 
* Compile with: gcc -Wall -o forkexecvp forkexecvp.c 
* To run: ./forkexecvp <command> [args] 
* ie ./forkexecvp sleep 1000 
 * =====================================================================
 */
#define _DEFAULT_SOURCE          /* WCONTINUED / strsignal under -std=c99 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

static pid_t                 child_pid    = -1;  /* PID of the child          */
static volatile sig_atomic_t child_alive  = 0;   /* 1 while the child exists  */
static volatile sig_atomic_t quit_received = 0;  /* set by the SIGQUIT handler*/

/* write(2) is async-signal-safe, so it is used instead of printf()
 * inside the handlers (printf is not reentrant). */
static void say(const char *msg) {
    size_t  len = strlen(msg);
    ssize_t n   = write(STDOUT_FILENO, msg, len);
    (void)n;
}

/* ---------------------------------------------------------------
 * Handler for SIGINT (Ctrl-C) and SIGTSTP (Ctrl-Z).
 * The signal is forwarded to the child and the parent just keeps
 * on waiting - it is never terminated and never stopped.
 * --------------------------------------------------------------- */
static void sig_forward(int signo) {
    if (child_alive && child_pid > 0)
        kill(child_pid, signo);              /* forward to the child */

    if (signo == SIGINT)
        say("\n[parent] SIGINT (Ctrl-C)  -> child interrupted, parent keeps waiting\n");
    else
        say("\n[parent] SIGTSTP (Ctrl-Z) -> child suspended,   parent keeps waiting\n");
}

/* ---------------------------------------------------------------
 * Handler for SIGQUIT (Ctrl-\).
 * Wake the child up (in case it is suspended) and kill it, then
 * tell the main loop to stop waiting and let the parent exit.
 * --------------------------------------------------------------- */
static void sig_quit(int signo) {
    (void)signo;
    quit_received = 1;

    say("\n[parent] SIGQUIT (Ctrl-\\) -> waking and killing child, parent will exit\n");

    if (child_alive && child_pid > 0) {
        kill(child_pid, SIGCONT);            /* resume it so the next signal is delivered */
        kill(child_pid, SIGQUIT);            /* terminate it                              */
    }
}

static void install_handlers(void) {
    if (signal(SIGINT,  sig_forward) == SIG_ERR) { perror("signal SIGINT");  exit(EXIT_FAILURE); }
    if (signal(SIGTSTP, sig_forward) == SIG_ERR) { perror("signal SIGTSTP"); exit(EXIT_FAILURE); }
    if (signal(SIGQUIT, sig_quit)    == SIG_ERR) { perror("signal SIGQUIT"); exit(EXIT_FAILURE); }
}

/* Print how the child changed state (used in the parent only). */
static void report_status(int status) {
    if (WIFEXITED(status)) {
        printf("Child exited with status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child was killed by signal %d (%s)\n",
               WTERMSIG(status), strsignal(WTERMSIG(status)));
    } else if (WIFSTOPPED(status)) {
        printf("Child was stopped by signal %d (%s) -- parent still waiting\n",
               WSTOPSIG(status), strsignal(WSTOPSIG(status)));
    } else if (WIFCONTINUED(status)) {
        printf("Child was resumed (SIGCONT)\n");
    }
}

int main(int argc, char **argv) {
    pid_t pid;
    int   status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == 0) {                    /* ---------------- child ---------------- */
        /* execvp() resets caught handlers to SIG_DFL, so the new program
         * dies on Ctrl-C and stops on Ctrl-Z exactly as usual. */
        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execvp failed ;-(\n");
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {                /* ---------------- parent --------------- */
        child_pid   = pid;
        child_alive = 1;

        install_handlers();

        printf("Parent %ld: child is %ld -- Ctrl-C interrupts, Ctrl-Z suspends, "
               "Ctrl-\\ quits\n", (long)getpid(), (long)pid);

        for (;;) {
            if (quit_received)
                break;

            pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
                if (errno == EINTR) {          /* interrupted by one of our handlers */
                    if (quit_received) break;
                    continue;
                }
                perror("waitpid");
                break;
            }

            report_status(status);

            /* The child is still alive (stopped or resumed): keep waiting. */
            if (WIFSTOPPED(status) || WIFCONTINUED(status))
                continue;

            /* The child has terminated; the parent stays alive and waits
             * for the quit signal before exiting. */
            child_alive = 0;
            while (!quit_received)
                pause();
            break;
        }

        /* Ctrl-\ arrived while the child was still alive: reap it. */
        if (child_alive) {
            child_alive = 0;
            if (waitpid(pid, &status, 0) > 0)
                report_status(status);
        }
    }
    else {                             /* ---------------- error ---------------- */
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("[%ld]: Exiting program .....\n", (long)getpid());
    return 0;
}
