//
// Created by SGuan on 4/24/2021.
//

#ifndef STRINGIFY_RECORD_H
#define STRINGIFY_RECORD_H

#include "storagemanager.h"
#include "catalog/catalog_structs.h"
#include "catalog/catalog.h"

/**
 * Convert the record to a string
 * @param attr_data_arr : array of attribute meta data
 * @param record : the record to be converted
 * @param num_attribute : the number of attributes
 * @return a record string
 */
char* record_to_str(struct attr_data** attr_data_arr, union record_item* record, int num_attribute);

/**
 * Convert a array of records to a array of record string
 * @param attr_data_arr : array of attribute meta data
 * @param records : the record to be converted
 * @param num_attribute : the number of attributes
 * @param num_of_records : the number of records
 * @return a array of record strings
 */
char** record_to_str_n(struct attr_data** attr_data_arr, union record_item** records,
        int num_attribute, int num_of_records);

#endif
