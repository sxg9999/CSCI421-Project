# compiler flags for debugging and warnings
CC = gcc
CFLAGS = -g -Wall -Wextra -lm
SOURCE_FILES = catalog_test.c ../../src/catalog.c ../../src/helper_module/hash_function.c
HEADER_FILES = ../../include/*.h
INCLUDE_DIR = -I ../../include/helper_module -I ../../include

all: catalog_test

catalog_test: $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o catalog_test $(INCLUDE_DIR)

run: catalog_test
	./catalog_test

clean:
	rm *.exe