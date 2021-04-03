//
// Created by SGuan on 4/3/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


//macros that determines which file_sys header file to use (for each platform)
#define WINDOWS_OS 1
#define LINUX_OS 2

#ifdef __WIN32
#define PLATFORM 1          //1 for windows platform
#include "../../include/file_sys/win_file_sys.h"
#elif __linux__
#define PLATFORM 2          //2 for linux platform
#include "../../include/file_sys/linux_file_sys.h"
#endif

/**
 * print the os
 */
void print_platform(){
    if(PLATFORM == WINDOWS_OS){
        printf("Windows os %d\n", WINDOWS_OS);
    }else if(PLATFORM == LINUX_OS){
        printf("Linux os %d\n", LINUX_OS);
    }
}


/**
 * Checks if the file exist at the given path
 * @param path
 * @return 1 if true, 0 if false
 */
int file_exist(char* path);


/**
 * Check if the directory exist
 * @param path
 * @return 1 if true, 0 if false
 */
int dir_exist(char* path);


/**
 * checks if the path is a directory
 */
int is_dir(char* path);


/**
 * Make a directory using the path
 * @param path : the path to the directory (including the directory name)
 * @return 0 if successful, -1 if failed
 */
int mk_dir(char* path);