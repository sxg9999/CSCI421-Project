//
// Created by SGuan on 4/3/2021.
//


//#define WINDOWS_OS 1
//#define LINUX_OS 2
//
//#ifdef __WIN32
//#define PLATFORM 1          //1 for windows platform
//#elif __linux__
//#define PLATFORM 2          //2 for linux platform
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static struct stat s;

/**
 * print the os
 */
void print_platform(){
//    if(PLATFORM == WINDOWS_OS){
//        printf("Windows os %d\n", WINDOWS_OS);
//    }else if(PLATFORM == LINUX_OS){
//        printf("Linux os %d\n", LINUX_OS);
//    }
    printf("Nothing\n");
}


/**
 * Checks if the file exist at the given path
 * @param path
 * @return 1 if true, 0 if false
 */
int file_exist(char* path){
    if(stat(path, &s) == 0){
        return 1;
    }
    return 0;
}



/**
 * Check if the directory exist
 * @param path
 * @return 1 if true, 0 if false
 */
int dir_exist(char* path){
    if(stat(path, &s) == 0 && S_ISDIR(s.st_mode)){
        return 1;
    }
    return 0;
}


/**
 * checks if the path is a directory
 */
int make_dir(char* path){
    mkdir(path, 0777);
    return 0;
}


/**
 * Make a directory using the path
 * @param path : the path to the directory (including the directory name)
 * @return 0 if successful, -1 if failed
 */
int mk_dir(char* path);