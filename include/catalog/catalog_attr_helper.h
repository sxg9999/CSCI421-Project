//
// Created by SGuan on 4/22/2021.
//

#ifndef CATALOG_ATTR_HELPER_H
#define CATALOG_ATTR_HELPER_H

#include "catalog_structs.h"

/**
 * Checks if the attribute have a notnull constraint
 * @param a_data : the attribute data
 * @return 1 for true and 0 for false
 */
int attr_has_notnull(struct attr_data* a_data);


/**
 * Checks if the attribute have a unique constraint
 * @param a_data : the attribute data
 * @return 1 for true and 0 for false
 */
int attr_has_unique(struct attr_data* a_data);


#endif
