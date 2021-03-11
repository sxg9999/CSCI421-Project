#include <string.h>

#include "../include/keywords.h"
#include "../include/ddl_parser_helper.h"

const char* KEYWORDS[] = {CREATE_START, DROP_START, ALTER_START,
        PRIMARY_KEY_CON, UNIQUE_CON, FOREIGN_CON, REFERENCES_CON, NOTNULL_CON,
        ALTER_DROP, ALTER_ADD, ALTER_DEFAULT,
        TABLE,
        STMT_END_STR,};
const int KEYWORD_COUNT = 13;

int is_keyword(char* word) {
        int result;
        for (int i = 0; i < KEYWORD_COUNT; i++) {
                result = strncmp(KEYWORDS[i], word, strlen(KEYWORDS[i]) );
                if (result == 0) {
                        return 1;
                }
        }
        return 0;
}
