# Homework 1 — Process Creation and Management 

C program that will take commands from a file. Run them using fork() and execvp(). Wait on them to complete. Write out commands as well as start time and end time to a log file.

## Files 

| File | Description | 
|--------------|----------------------------------------------------------| 
| `runcmds.c` | Solution for this assignment. | 
| `hw1.c` | Will be used to consume cpu time. | 
| `Makefile` | Used to compile the files. | 
| `input.txt` | An input file. Created when running make test. | 
| `output.log` | Will hold commands as well as times. Created at runtime. | 

## Build 

```bash 
make 


## The input.txt file can contain the following commands

uname -a
/sbin/ifconfig
./hw1 500
./hw1 1000
