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
#include "../include/page.h"

buffer_manager* BufferManager_new(int max_page_count) {
    buffer_manager* new_buff_man = (buffer_manager*)malloc( sizeof(buffer_manager) );
    new_buff_man->current_page_count = 0;
    new_buff_man->max_page_count = max_page_count;
    new_buff_man->pages = (Page**)malloc( max_page_count *
         sizeof( Page* ));
    new_buff_man->page_arr_with_count = (int*)malloc( 
        max_page_count * sizeof(int) );
    new_buff_man->min_page_id  = -1;
    new_buff_man->min_page_use_count = 0;

    return new_buff_man;
}

int add_page(buffer_manager* buff_man, Page* page) {
    if (buff_man->current_page_count >= buff_man->max_page_count) {
        // buffer is full

        // get LRU page id
        int LRU_page_id = buff_man->min_page_id;
        // write page out
        Page_write(buff_man->pages[LRU_page_id]);
        // remove page from buffer
        remove_page(buff_man, LRU_page_id);
    }
    // add new page to buffer
    buff_man->pages[buff_man->current_page_count] = page;
    buff_man->page_arr_with_count[
        buff_man->current_page_count] = 0;
    if ( buff_man->min_page_id == -1) {
        buff_man->min_page_id = 0;
    }
    buff_man->current_page_count += 1;

    return 0;
}

int remove_page(buffer_manager* buff_man, int page_index) {
    if ( page_index > buff_man->current_page_count ) {
        // invalid page_index
        fprintf(stderr, "Error: %s Page with index %d is not in buffer. Currently %d pages in buffer", 
                "Could not remove page from buffer.\n", 
                page_index, 
                buff_man->current_page_count
        );

        return -1;
    }

    // Remove page from page array
    Page_destroy( buff_man->pages[page_index] );

    for (int i = page_index; i<buff_man->current_page_count-1; i++) {
        buff_man->pages[i] = buff_man->pages[i+1];
    }
    buff_man->pages[ buff_man->current_page_count ] = 
        (Page*)malloc(sizeof( Page ));

    // ALSO REMOVE FROM PAGE_ARR_WITH_COUNT
    for (int i = page_index; i<buff_man->current_page_count-1; i++) {
        buff_man->page_arr_with_count[i] = 
            buff_man->page_arr_with_count[i+1];
    }
    buff_man->page_arr_with_count[ 
        buff_man->current_page_count ] = 0;

    buff_man->current_page_count -= 1;
    

    return 0;
}

int get_buffer_page(buffer_manager* buff_man, int page_id, Page* npage) {
    if ( page_id > buff_man->current_page_count ) {
        // invalid page_index
        fprintf(stderr, "Error: %s Page with index %d is not in buffer. Currently %d pages in buffer", 
                "Could not get page",
                page_id,
                buff_man->current_page_count
        );

        return -1;
    }

    npage = buff_man->pages[page_id];
    buff_man->page_arr_with_count[page_id] += 1;
    set_LRU_page_id(buff_man);

    return 0;
}

int set_LRU_page_id(buffer_manager* buff_man) {
    if (buff_man->current_page_count == 0) {
        fprintf(stderr, "Error: %s Currently %d pages in buffer", 
                "Can't set Least Recently Used ID.\n",
                buff_man->current_page_count
        );

        return -1;
    }
    for (int i = 0; i< buff_man->current_page_count; i++) {
        if ( buff_man->page_arr_with_count[i] < 
            buff_man->min_page_use_count ) {
            buff_man->min_page_id = 1;
            buff_man->min_page_use_count = buff_man->page_arr_with_count[i];
        }
    }

    return 0;
}

void purge_buffer(buffer_manager* buff_man) {
    for (int i = 0; i < buff_man->current_page_count; i++) {
        // write out page
        Page_write(buff_man->pages[0]);
        // destroy page
        remove_page(buff_man, 0);
    }
}

void destroy_buffer(buffer_manager* buff_man) {
    if (buff_man) {
        // Free each page
        for (int i = 0; i<buff_man->current_page_count; i++) {
            Page_destroy(buff_man->pages[i]);
        }
        free(buff_man->page_arr_with_count);

        free(buff_man);
    }
    
}