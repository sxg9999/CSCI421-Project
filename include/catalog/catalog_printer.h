//
// Created by SGuan on 4/14/2021.
//

/**
 * Header file for declaring print functions for printing
 * table meta data in catalog
 */

#ifndef CATALOG_PRINTER_H
#define CATALOG_PRINTER_H

#include "catalog_structs.h"

void catalog_print_tables();
void catalog_print_childs(int count, char** childs);
void catalog_print_foreign_key(int count, struct foreign_key_data** f_keys);



#endif //CSCI421_PROJECT_CATALOG_PRINTER_H
