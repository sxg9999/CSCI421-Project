

# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm -std=c99
SOURCE_FILES = src/*.c src/helper_module/*.c src/hash_collection/*.c src/catalog/*.c src/file_sys/*.c
HEADER_FILES = include/*.h include/helper_module/*.h include/hash_collection/*.h include/catalog/*.h include/file_sys/*.h
INCLUDE_DIR = -I ./include -I ./include/helper_module -I ./include/hash_collection

# first entry for make to envoke
database: database

database: $(SOURCE_FILES) $(HEADER_FILES)
	$(CC) $(SOURCE_FILES) -o database  $(CFLAGS) $(INCLUDE_DIR)

clean:
		rm *.o *.exe
