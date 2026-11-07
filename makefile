# Makefile for Lab 11 - command runner using popen/pclose
#
# Targets:
#   make        - build lab11
#   make test   - build, then run a scripted session
#   make clean  - remove build artifacts

CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = lab11

all: $(TARGET)

$(TARGET): lab11.c
	$(CC) $(CFLAGS) -o $@ $^

# Scripted session exercising wildcards, pipes, and redirection
test: $(TARGET)
	@printf 'ls *.c\nls -l pipe[1-3].c\nls pipe*.c | sort\nls pipe?.c | sort > files.out\ncat files.out\nquit\n' | ./$(TARGET)
	@rm -f files.out

.PHONY: all test clean

clean:
	rm -f $(TARGET) files.out
