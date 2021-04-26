//
// Created by SGuan on 4/15/2021.
//

#ifndef STORAGE_MEDIATOR_PRINTER_H
#define STORAGE_MEDIATOR_PRINTER_H

void sm_print_all_table_meta_datas();

/**
 * Prints all the records for the table with the given table name
 * @param table_name : the name of the table
 * @return
 */
void sm_print_table_records(char* table_name);

#endif
