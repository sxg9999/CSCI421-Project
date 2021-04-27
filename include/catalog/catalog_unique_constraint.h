//
// Created by SGuan on 4/26/2021.
//

#ifndef CATALOG_UNIQUE_CONSTRAINT_H
#define CATALOG_UNIQUE_CONSTRAINT_H

/**
 * Get the indices for all unique groups of attributes and its corresponding
 * group size
 * @param table_name : name of the table
 * @param unique_group_attr_indices : 2d array that stores unique group's attribute indices
 * @param unique_group_size_arr : an array that stores each unique group's size
 * @return number of unique groups of attributes
 */
int get_unique_group_constr_indices(char* table_name, int*** unique_attr_indices_arr,
                                    int** unique_group_size_arr);


#endif
