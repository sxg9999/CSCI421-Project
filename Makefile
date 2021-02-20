# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = src/*.c
HEADER_FILES = include/*.h
INCLUDE_DIR = include

# first entry for make to envoke
default: storage

storage: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(SOURCE_FILES) $(HEADER_FILES) -o storage $(CFLAGS) -I $(INCLUDE_DIR)

run: storage
	./storage

clean:
		rm *.o *.exe storage
