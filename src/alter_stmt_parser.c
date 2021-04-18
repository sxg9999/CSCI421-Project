/*
 * Author: Dylan Cuprewich (dxc2961), Robert Gandley ()
 * Source file for parsing Alter statements
 * Implements the public functionality for the Alter Statement Parser
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"

int parse_alter_table_stmt( char* input_statement ) {
    printf("Parsing Alter STMT: '%s'\n", input_statement);


    //get statement, skip preamble
    char* statement = (char* )malloc( strlen( input_statement ) + 1);
    strcpy(statement, input_statement); 
    statement += strlen(ALTER_START) + 1 + strlen(TABLE) + 1;

    //set up tools
    printf("Alter STMT: %s\n", input_statement);
    char *table_name;
    const char delimiter[2] = " ";

    // Get table name
    table_name = strtok(statement, delimiter);
    // Convert to lower
    char lower_char;
    for (int i = 0; table_name[i] != '\0'; i++) {
        lower_char = tolower(table_name[i]);
        table_name[i] = lower_char;
    }
    // Check if table name is not a keyword
    if (is_keyword(table_name) == -1) {
        fprintf(stderr, "%s: '%s'\n",
                "Invalid table name; table name is a keyword", table_name);
        return -1;
    }

    //Get the keyword for the operation to perform.
    char* action = strtok(NULL, delimiter);

    // Convert action to all lower for easier scan
    for (int i = 0; action[i] != '\0'; i++) {
        lower_char = tolower(action[i]);
        action[i] = lower_char;
    }

    if (strcmp(action, "add") == 0) {
        //printf("add");
        //If add, given a name and a type OR a name, type, and default phrase.

        char* attName = strtok(NULL, delimiter);
        //Check if attribute name is a keyword.
        if (is_keyword(attName) == -1) {
            fprintf(stderr, "%s: '%s'\n",
                    "Invalid attribute name; attribute name is a keyword", attName);
            return -1;
        }

        char* attType = strtok(NULL, delimiter);
        //printf("%s", attType);
        //Check if next word matches an attribute type
        if(!is_attr_type(attType)) {
            fprintf(stderr, "%s: '%s'\n",
                    "Invalid attribute type; attribute type must be integer,"
                    " double, boolean, char, or varchar", attType);
            return -1;
        }

        char* def = strtok(NULL, delimiter);

        if(def != NULL) {
            //Convert possible default keyword to lower for scan
            for (int i = 0; def[i] != '\0'; i++) {
                lower_char = tolower(def[i]);
                def[i] = lower_char;
            }
            if(strcmp(def, "default") != 0) {
                fprintf(stderr, "%s: '%s'\n",
                        "Invalid keyword; alter queries may only include"
                        " a default", def);
                return -1;
            }
            char* val = strtok(NULL, delimiter);

            //If default includes no value
            if(val == NULL) {
                fprintf(stderr, "Default keyword must have a value.\n");
            }

            //If there is more than one value after default
            if(strtok(NULL, delimiter) != NULL) {
                fprintf(stderr, "Invalid statement, altering tables may only include"
                                "a drop, add, or add default phrase.\n");
                return -1;
            }
        }

    } else if (strcmp(action, "drop") == 0) {
        //printf("drop");
        //If drop, given only attribute name.
        char* attToDrop = strtok(NULL, delimiter);

        //check if attribute name is valid?

        //send attribute to drop and table to drop from back
    } else {
        fprintf(stderr, "%s: %s\n", "Invalid operation", action);
    }

    //Given it hasn't errored out, valid statement.
    return 0;
}