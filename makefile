# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = search
SOURCE = search.c

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean up
clean:
	rm -f $(TARGET) *.o

# Install target (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
