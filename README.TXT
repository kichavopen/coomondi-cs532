===================================================================== 
Lab: UNIX Signal Handling 
File(s) submitted: forkexecvp.c, README.txt 
===================================================================== 

1. WHAT THE PROGRAM DOES 
--------------------------------------------------------------------- 
Same thing as the original forkexecvp.c but we added signal handling
to it. It forks off a child which execvp()s whatever you give it on
the command line and the parent waits on the child. Also: 

Ctrl-C (SIGINT) -> child gets interrupted and parent continues
to wait. 
Ctrl-Z (SIGTSTP) -> child gets stopped and parent continues to
wait. 
Ctrl-\ (SIGQUIT) -> child gets continued and killed and parent
stops waiting. 

So the parent will not die on Ctrl-C or Ctrl-Z which was what was
asked for. 

2. HOW TO COMPILE AND RUN 
--------------------------------------------------------------------- 
$ gcc -Wall -o forkexecvp forkexecvp.c 
$ ./forkexecvp <command> [args] 

You can run things like 

$ ./forkexecvp sleep 1000 
$ ./forkexecvp cat 
$ ./forkexecvp yes 

I have not submitted any executable/object files just the .c file and
this readme. 

3. DESIGN / IMPLEMENTATION 
--------------------------------------------------------------------- 
a) We set up signal() handlers for SIGINT SIGTSTP and SIGQUIT in the
parent before it begins to wait. SIGINT and SIGTSTP both go to 
the same handler (sig_forward) which kills the child with that
signal and prints something out. SIGQUIT goes to sig_quit. 

b) We used handlers rather than SIG_IGN because if you set a signal
to be ignored it is inherited by the child after an exec(). Which 
would mean our child would also ignore SIGINT and SIGTSTP and not
get interrupted/stopped. If you catch a signal it will be reset
to default after an exec() so the child will act normal and the
parent will stay alive. 

c) Since Ctrl-C and Ctrl-Z are sent to all processes in the foreground
process group the child should also receive these signals. We 
chose to forward the signals to the child just to be safe and make
sure it worked if the child was put in a different process group.

d) We used waitpid with the WUNTRACED and WCONTINUED flags so the
parent knows when the child is stopped or continued. If the child 
is stopped the parent will just continue to wait. Once the child 
dies we want the parent to continue to wait using pause() until
SIGQUIT is received as stated in the question.

e) When we receive SIGQUIT we set a variable and send the child
SIGCONT and then SIGQUIT. We need to send SIGCONT because if the
child is stopped it will not respond to the SIGQUIT until it is
continued. Then the parent can kill the child and exit.

f) We only used safe functions in our signal handlers such as write
and kill/pause. Anything that is not safe (i.e printf) we did not
use in our signal handlers. 

4. SAMPLE SESSION 
--------------------------------------------------------------------- 
$ gcc -Wall -o forkexecvp forkexecvp.c 
$ ./forkexecvp sleep 1000 
Parent 23117: child is 23118 -- Ctrl-C interrupts, Ctrl-Z suspends, Ctrl-\ quits
^Z 
[parent] SIGTSTP (Ctrl-Z) -> child suspended, parent keeps waiting
Child was stopped by signal 20 (Stopped (signal)) -- parent still waiting
^C 
[parent] SIGINT (Ctrl-C) -> child interrupted, parent keeps waiting
Child was killed by signal 2 (Interrupt) 
^\ 
[parent] SIGQUIT (Ctrl-\) -> waking and killing child, parent will exit
[23117]: Exiting program ..... 
$ 

5. NOTES / CLEANUP 
--------------------------------------------------------------------- 
- Since we are catching SIGTSTP the shell will not return a prompt
when you press Ctrl-Z. You will have to use Ctrl-\ to quit the
program or type kill -9 <pid> in another terminal.
- You will also need to press Ctrl-\ after pressing Ctrl-C to quit
the program since we are catching that signal as well.
- SIGKILL and SIGSTOP cannot be caught so you can always use kill -9
to kill the program if necessary. 
