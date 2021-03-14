# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = ./src/*.c ./src/helper_module/*.c
HEADER_FILES = include/*.h include/helper_module/*.h
INCLUDE_DIR = -I ./include -I ./include/helper_module

# first entry for make to envoke
database: database

database: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(SOURCE_FILES) -o database.exe  $(CFLAGS) $(INCLUDE_DIR)

clean:
		rm *.o *.exe
