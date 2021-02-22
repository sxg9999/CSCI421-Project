# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = ../src/*.c src/*.c
HEADER_FILES = ../include/*.h
TEST_HEADER_FILES = include/*.h
MAIN_INCLUDE_DIR = ../include
TEST_INCLUDE_DIR = include


default: test


test: test.c ../include/hashtable.c 
	$(CC) $(CFLAGS) test.c ../include/hashtable.c  -o test 


run: test
	./test

clean:
	rm test.exe
