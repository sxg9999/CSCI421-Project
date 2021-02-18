/*
 * Author: Steven Guan
 * Implementation of functions related to the Page class
 */

#include "page.h"


//constructor
void Page_init(Page* self, union record_item** records){
    //to do
}

Page* Page_create(union record_item** records){
    Page* page = (Page*)malloc(sizeof(Page));
    Page_init(page, records);
    return page;
}



