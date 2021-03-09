# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = src/*.c src/helper_module/*.c
HEADER_FILES = include/*.h
INCLUDE_DIR = include

# first entry for make to envoke
database: database

database: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(SOURCE_FILES) -o database  $(CFLAGS) -I $(INCLUDE_DIR)

clean:
		rm *.o *.exe
