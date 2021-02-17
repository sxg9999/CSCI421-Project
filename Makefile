# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra
SOURCE_FILES = src/*.c
HEADER_FILES = include/*.h
INCLUDE_DIR = include

# first entry for make to envoke
default: storage

storage: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o storage $(SOURCE_FILES) $(HEADER_FILES) -I $(INCLUDE_DIR)

clean:
		rm -r storage *.o *.exe
