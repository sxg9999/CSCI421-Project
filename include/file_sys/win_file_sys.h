//
// Created by SGuan on 4/3/2021.
//

/**
 * Header file for functionality that are related to file systems
 * on windows
 */

#ifndef WIN_FILE_SYS_H
#define WIN_FILE_SYS_H

/**
 * returns the os name
 */
char* win_platform();


/**
 * Checks if the file exist at the given path
 * @param path
 * @return 1 if true, 0 if false
 */
int win_file_exist(char* path);


/**
 * Check if the directory exist
 * @param path
 * @return 1 if true, 0 if false
 */
int win_dir_exist(char* path);


/**
 * checks if the path is a directory
 */
int win_is_dir(char* path);


/**
 * Make a directory using the path
 * @param path : the path to the directory (including the directory name)
 * @return 0 if successful, -1 if failed
 */
int win_mk_dir(char* path);

#endif
