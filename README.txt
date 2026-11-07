Lab 11 - Interactive command runner

Name: Chris
BlazerId: coomondi
Project #: 11

COMPILE
  make

RUN
  ./lab11

DESCRIPTION
  Prompts repeatedly for a UNIX command, executes each one with popen in
  read mode, and copies the command's standard output to the terminal.
  Because popen hands the string to "sh -c", wildcards, pipes, and
  redirections behave exactly as they would at a shell prompt. Entering
  "quit" (or Ctrl-D) ends the program.

  Output is read with fgets and echoed with fputs, so a command producing
  no output simply prints nothing and returns to the prompt.

TESTING

  1. Interactive run

     make
     ./lab11

     Then try each of these, matching the sample session:

       ls *.c
       ls -l pipe[1-3].c
       ls pipe*.c | sort
       ls pipe?.c | sort > files.out
       cat files.out
       quit

     Expected: each command's output appears, the redirection writes to
     files.out rather than the terminal, cat shows its contents, and quit
     prints "Exiting program...bye!" and returns to the shell.

  2. Automated run

     make test

     Runs the same sequence non-interactively and cleans up files.out.

  3. Empty input

     Press Enter at the prompt a few times.

     Expected: prompt reappears each time, no error, no crash.

  4. Bad command

     nosuchcommand

     Expected: "sh: 1: nosuchcommand: not found" from the shell, then the
     prompt returns. The program keeps running.

  5. Long output

     ls -l /etc

     Expected: full listing, no truncation or hang.

  6. Ctrl-D

     Press Ctrl-D at the prompt.

     Expected: newline, then "Exiting program...bye!".

EDGE CASES HANDLED
  - empty line -> re-prompt
  - Ctrl-D / EOF -> clean exit
  - popen failure -> perror, continue prompting
  - pclose failure -> perror
  - commands with no output -> nothing printed, prompt returns
  - input longer than the buffer is split across prompts

KNOWN ISSUES
  - "quit" must be typed exactly, with no leading or trailing spaces.
  - Commands are limited to 4095 characters per line.
  - Interactive pagers (more, less) are not driven; their output is just
    copied through.

NOTES
  - popen is used in "r" mode only, so the parent reads the child's stdout.
    The alternative "w" mode would require the parent to generate the input
    instead of consuming output.
