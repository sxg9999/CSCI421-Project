#include <string.h>

#include "../include/keywords.h"
#include "../include/ddl_parser_helper.h"

const char* KEYWORDS[] = {CREATE_START, DROP_START, ALTER_START,
        PRIMARY_KEY_CON, UNIQUE_CON, FOREIGN_CON, REFERENCES_CON, NOTNULL_CON,
        ALTER_DROP, ALTER_ADD, ALTER_DEFAULT,
        TABLE,
        INTEGER_TYPE, DOUBLE_TYPE, BOOLEAN_TYPE, CHAR_TYPE, VARCHAR_TYPE,
        STMT_END_STR,};
const int KEYWORD_COUNT = 18;

const char* CONSTRAINTS[] = {PRIMARY_KEY_CON, FOREIGN_CON, UNIQUE_CON};
const int CONSTRAINT_COUNT = 3;

const char* ATTR_CON[] = {NOTNULL_CON, PRIMARY_KEY_CON, UNIQUE_CON};
const int ATTR_CON_COUNT = 3;

const char* ATTR_TYPES[] = {
                INTEGER_TYPE, DOUBLE_TYPE, BOOLEAN_TYPE, CHAR_TYPE, VARCHAR_TYPE};
const int ATTR_TYPE_COUNT = 5;

int is_keyword(char* word) {
        int result;
        for (int i = 0; i < KEYWORD_COUNT; i++) {
                result = strncmp(KEYWORDS[i], word, strlen(KEYWORDS[i]) );
                if (result == 0) {
                        return 0;
                }
        }
        return -1;
}

int is_constraint(char* word) {
        int result;
        for (int i = 0; i < CONSTRAINT_COUNT; i++) {
                result = strncmp(CONSTRAINTS[i], word, strlen(CONSTRAINTS[i]));
                if (result == 0) {
                        return 0;
                }
        }
        return -1;
}

int is_attr_con(char* word) {
        int result;
        for (int i = 0; i < ATTR_CON_COUNT; i++) {
                result = strncmp(ATTR_CON[i], word, strlen(ATTR_CON[i]));
                if (result == 0) {
                        return 0;
                }
        }
        return -1;
}

int is_attr_type(char* word) {
        int result;
        for (int i = 0; i < ATTR_TYPE_COUNT; i++) {
                result = strncmp(ATTR_TYPES[i], word, 
                        strlen(ATTR_TYPES[i]));
                if (result == 0) {
                        return 0;
                }
        }
        return -1;
}

