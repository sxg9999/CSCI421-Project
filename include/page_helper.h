/*
 * Author: Dylan Cuprewich (dxc2961)
 * Header file for CSCI421 Group Project Storage Manager
 * Outlines the public functionality for the helper functions
 * related to I/O operations on pages.
 */

#ifndef PAGE_HELPER_H
#define PAGE_HELPER_H


/*
 * Attempts to reads a page into the DB.
 * If Page is in buffer: DONE.
 * Else if space in buffer: 
 *          add page to buffer DONE
 *      if no space in buffer:
 *          write another page to hardware,
 *          remove that page from buffer
 *          add page to buffer DONE 
 * IDK BRO
 * @return 0 if sucessfully removed, -1 otherwise
 */
int read_page();

/*
 * Attempts to writes a page (as binary) out from the DB.
 * IDK BRO
 * @return 0 if sucessfully removed, -1 otherwise
 */
int write_page();

#endif