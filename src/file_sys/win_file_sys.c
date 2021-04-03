//
// Created by SGuan on 4/3/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../../include/file_sys/win_file_sys.h"

static char os_name[] = "windows";

/**
 * print the os
 */
char* win_platform(){
    return os_name;
}


/**
 * Checks if the file exist at the given path
 * @param path
 * @return 1 if true, 0 if false
 */
int win_file_exist(char* path){
    return 0;
}


/**
 * Check if the directory exist
 * @param path
 * @return 1 if true, 0 if false
 */
int win_dir_exist(char* path){
    return 0;
}


/**
 * checks if the path is a directory
 */
int win_is_dir(char* path){
    return 0;
}


/**
 * Make a directory using the path
 * @param path : the path to the directory (including the directory name)
 * @return 0 if successful, -1 if failed
 */
int win_mk_dir(char* path){
    return 0;
}