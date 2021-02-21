# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = src/*.c
HEADER_FILES = include/*.h
# first entry for make to envoke
default: storage

storage: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o storage $(SOURCE_FILES)

clean:
		rm storage *.o
