/*
 * Author: Dylan Cuprewich (dxc2961)
 * Header file for CSCI421 Group Project Storage Manager
 * Outlines Buffer Manager struct and methods.
 */

#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H 


typedef struct buffer_manager {
    int current_page_count;
    int max_page_count;
    int min_page_id;
    int min_page_use_count;
    union record_item**** pages; // SWICH TO PAGES
    int* page_arr_with_count;
}  buffer_manager;

/*
 * Constructor for buffer_manager.
 * @param max_page_count - the max number of pages the buffer can hold.
 * @return pointer to a new buffer_manager struct
 */
buffer_manager* BufferManager_new(int max_page_count);

int LRU_page_id(buffer_manager* buff_man);

/*
 * Adds a page to the buffer to enable ease of use.
 * @param buff_man - the buffer_manager that handles the pages.
 * @param page - the page to be added to the buffer
 * @return 0 if the page was added successfully, otherwise -1;
 */
int add_page(buffer_manager* buff_man, union record_item*** page);

/*
 * Gets a page from the buffer manager
 * @param buff_man - the buffer_manager that handles the pages.
 * @param page_id - the page to be added to the buffer
 * @param page - the page of interest
 * @return 0 if the page was added successfully, otherwise -1;
 */
 int get_buffer_page(buffer_manager* buff_man, int page_id, union record_item*** page);

/*
 * Removes a page from the buffer.
 * @param buff_man - the buffer manager that handles the pages.
 * @param page_index - the location of the page in the buffer
 * @return 0 if the page was successfully removed, otherwise -1;
 */
int remove_page(buffer_manager* buff_man, int page_index);

/*
 * Frees up the members of a buffer_manager struct instance.
 * @param buff_man - the buffer manager to be free'd up
 * @return
 */ 
void destroy(buffer_manager* buff_man);


#endif