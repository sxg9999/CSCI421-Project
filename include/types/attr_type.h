//
// Created by SGuan on 3/19/2021.
//

#ifndef ATTR_TYPE_H
#define ATTR_TYPE_H

enum attr_type {NO_ATTR_TYPE = 0, INT = 1,
            DOUBLE = 2, BOOL = 3, CHAR = 4,
            VARCHAR = 5};

/**
 * Initiates the attr_types
 * @return 0 if success and -1 if failed
 */
int init_attr_types();

/**
 * Gets the attribute type of the given key_word
 * @param key_word
 * @return the attribute type
 */
enum attr_type typeof_a(char* key_word);

void attr_type_print();

void attr_type_close();


#endif