# Makefile for Homework 1 - Process creation and management
#
# Targets:
#   make          - build everything (runcmds, hw1, input.txt)
#   make runcmds  - build the driver program only
#   make hw1      - build the sample workload program only
#   make test     - build, then run against input.txt
#   make clean    - remove build artifacts

CC      = gcc
CFLAGS  = -Wall -g
LDFLAGS =

TARGETS = runcmds hw1

# Default: build both programs and the sample input file
all: $(TARGETS) input.txt

# Driver program (the assignment solution)
runcmds: runcmds.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Sample workload program supplied with the lab
hw1: hw1.c
	$(CC) $(CFLAGS) -O -o $@ $^ $(LDFLAGS)

# Generate the sample input file
input.txt:
	printf 'uname -a\n/sbin/ifconfig\n./hw1 500\n./hw1 1000\n' > $@

# Build and run against the sample input file
test: all
	./runcmds input.txt
	@echo "---- output.log ----"
	@cat output.log

.PHONY: all clean test

clean:
	rm -f $(TARGETS) output.log input.txt
