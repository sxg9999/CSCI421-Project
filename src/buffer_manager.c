/*
 * Author: Dylan Cuprewich (dxc2961)
 * Implements functions related to the buffer manager
 * for CSCI421 Group Project Storage Manager.
 * Implements "include/buffer_manager.h".
 * 
 * See "include/buffer_manager.h" for more documentation.
 */

#include <stdlib.h>

#include "../include/buffer_manager.h"
#include "../include/storagemanager.h"

buffer_manager* BufferManager_new(int max_page_count) {
    buffer_manager* new_buff_man = malloc( sizeof(struct buffer_manager) );
    new_buff_man->current_page_count = 0;
    new_buff_man->max_page_count = 0;
    new_buff_man->pages = malloc( max_page_count *
         sizeof( union record_item )); //TODO SWITCH TO PAGES
    new_buff_man->page_arr_with_count = malloc( max_page_count * sizeof(int) );
    new_buff_man->min_page_id  = -1;
    new_buff_man->min_page_use_count = 0;

    return new_buff_man;
}

int add_page(buffer_manager* buff_man, union record_item*** page) {
    if (buff_man->current_page_count >= buff_man->max_page_count) {
        // buffer is full

        // get LRU page id
        int LRU_page_id = buff_man->min_page_id;
        // write page out
        // write_out_page(buff_man->pages[ LRU_page_id ]);
        // remove page from buffer
        remove_page(buff_man, LRU_page_id);
    }
    // add new page to buffer
    buff_man->pages[buff_man->current_page_count] = page;
    buff_man->page_arr_with_count[
        buff_man->current_page_count] = 0;
    if ( buff_man->min_page_id = -1) {
        buff_man->min_page_id = 0;
    }
    buff_man->current_page_count += 1;

    return 0;
}

int remove_page(buffer_manager* buff_man, int page_index) {
    if ( page_index > buff_man->current_page_count ) {
        // invalid page_index
        return -1;
    }

    // Remove page from page array
    for (int i = 0; i<buff_man->current_page_count-1; i++) {
        buff_man->pages[i] = buff_man->pages[i+1];
    }
    buff_man->pages[ buff_man->current_page_count ] = 
        malloc(sizeof( union record_item ));

    // ALSO REMOVE FROM PAGE_ARR_WITH_COUNT
    for (int i = 0; i<buff_man->current_page_count-1; i++) {
        buff_man->page_arr_with_count[i] = 
            buff_man->page_arr_with_count[i+1];
    }
    buff_man->page_arr_with_count[ buff_man->current_page_count ] = 
        malloc(sizeof( int ));

    buff_man->current_page_count -= 1;
    

    return 0;
}

int get_buffer_page(buffer_manager* buff_man, int page_id, union record_item*** page) {
    page = buff_man->pages[page_id];
    buff_man->page_arr_with_count[page_id] += 1;
    set_LRU_page_id(buff_man);

    return 0;
}

int set_LRU_page_id(buffer_manager* buff_man) {
    if (buff_man->current_page_count == 0) {
        return -1;
    }
    for (int i = 0; i< buff_man->current_page_count; i++) {
        if ( buff_man->page_arr_with_count[i] < buff_man->min_page_use_count ) {
            buff_man->min_page_id = 1;
            buff_man->min_page_use_count = buff_man->page_arr_with_count[i];
        }
    }

    return 0;
}

void destroy_buffer(buffer_manager* buff_man) {
    free(buff_man->pages);
    free(buff_man->page_arr_with_count);

    free(buff_man);
}