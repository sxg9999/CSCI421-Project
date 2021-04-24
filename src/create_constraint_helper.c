
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/create_stmt_parser.h"
#include "../include/ddl_parser.h"
#include "../include/ddl_parser_helper.h"
#include "../include/keywords.h"
#include "../include/error_printing.h"
#include "../include/catalog/catalog_structs.h"
#include "../include/catalog/catalog.h"
#include "../include/hash_collection/hash_collection.h"
#include "../include/db_types.h"


int constraint_check(char* currentAttr, char* token, char** attr_names, int* name_count, 
        int attr_num, struct catalog_table_data* new_table) {
    printf("Checking if '%s' is a valid constraint...\n", currentAttr);
    printf("TABLE NAME: '%s'\n", new_table->table_name);

    // if it is check if attribute is constraint
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    if ( is_constraint(token) == 0 && attr_num == 0) {
        fprintf(stderr, "%s: '%s'\n", 
            "INVALID: Constraint is defined before attributes", token);
        return -1;
    }
    enum db_type type = typeof_kw(token);
    printf("Constraint '%s' type: '%d'\n", token, type);
    // check if foreignkey
    if ( type == FOREIGN_KEY ) {
        printf("Attribute is a foreign constraint: '%s'\n", token);

        // foreignkey( <a 1> ... <a N> ) references <r name>( <r 1> ... <r N> ):
        // track attrs in foreignkey( <a 1> ... <a N> ) - <a N>
        char** foreign_attr;
        int fa_count = 0;
        // track attrs in <r name>( <r 1> ... <r N> ) - <r N>
        char** parent_attr;
        int pa_count = 0; 
        // initialize a new foreign_key struct
        struct foreign_key_data* new_foreign_key = (struct foreign_key_data*) malloc( 
            sizeof(struct foreign_key_data));
        new_foreign_key->f_keys = ht_create(12, 0.75);
        new_foreign_key->parent_table_name = NULL;

        // check for key attribute names
        int valid_constraint_name = foreign_contraint_name_check(attr_names, token, *name_count);
        if ( valid_constraint_name == -1 ) {
            fprintf(stderr, "Invalid constraint '%s'\n", token);
            return -1;
        }
        // check for 'references'
        token = strtok(NULL, " ");
        // printf("Token: '%s'\n", token);
        if ( strcmp(REFERENCES_CON, token) != 0) {
                fprintf(stderr, "%s: '%s'\n", 
                "Invalid constraint definition. Missing 'references' keyword", token);
                return -1;
        }

        // <relation>( check
        token = strtok(NULL, " ");
        // printf("Token: '%s'\n", token);
        if ( token[strlen(token) - 1] != '(' ) {
            fprintf(stderr, "%s: '%s'\n", 
                "Invalid constraint definition. Missing '('", token);
                return -1;
        }
        token[strlen(token) - 1] = '\0';
        // check if parent table is defined
        if (catalog_contains(token) == 0) {
            fprintf(stderr, "Invalid: table '%s' not defined\n", token);
            return -1;
        }
        printf("Table '%s' being referenced exists!", token);

        // <r_1> ... <r_n>
        while ( (token = strtok(NULL, " ")) ) {
            if (token[0] == ')') {
                break;
            }
            // printf("<r_n>: %s\n", token);
            if ( is_keyword(token) == 0) {
                fprintf(stderr, "%s: '%s'\n", 
                "Invalid constraint definition. Constraint name is a keyword", token);
                return -1;
            }
        }

        // add the new foreignkey struct to table
        if (new_foreign_key->parent_table_name == NULL) {
            fprintf(stderr, "Invalid: foreign key parent name not specified: '%s'\n", currentAttr);
            return -1;
        }
        new_table->num_of_f_key += 1;
        new_table->f_key_arr_size += 1;
        new_table->f_keys = (struct foreign_key_data**) realloc(new_table->f_keys,
                new_table->num_of_f_key * sizeof(struct foreign_key_data*));
        new_table->f_keys[new_table->num_of_f_key] = new_foreign_key;
    } 
    else { // either a primarykey, unqiue, or invalid
        printf("Attribute is a primarykey/unique constraint: '%s'\n", token);

        // check if valid constraint def
        if ( type != PRIMARY_KEY && type != UNIQUE) {
            fprintf(stderr, "Invalid constraint type: '%s'\n", token);
            return -1;
        }
        while ( (token = strtok(NULL, " ")) ) {

            if (token[0] == ')') {
                break;
            }
            for (int i = 0; token[i] != '\0'; i++) {
                if ( isalpha(token[i]) ) {
                    token[i] = tolower(token[i]);
                }
            }
            if ( is_keyword(token) == 0) {
                fprintf(stderr, "%s: '%s'\n", 
                "Invalid constraint definition. Constraint attribute is a keyword", token);
                return -1;
            }

            if ( !sv_ht_contains(new_table->attr_ht, token) ) {
                fprintf(stderr, "Attribute '%s' undefined used in constraint '%s'\n", 
                        token, currentAttr);
                return -1;
            }
            printf("Attribute '%s' valid in constraint '%s'\n", 
                    token, currentAttr);
            // add constraint to attr_struct
            struct attr_data* attribute = (struct attr_data*) sv_ht_get(new_table->attr_ht, token);
            struct attr_constraint* new_constraint = (struct attr_constraint*) malloc( 
                    sizeof(struct attr_constraint));
            new_constraint->type = type;
            attribute->constr[attribute->num_of_constr] = new_constraint;
            attribute->num_of_constr += 1;

            // if primarykey, update catalog_table_data struct
            if (type == PRIMARY_KEY) {
                new_table->p_key_len += 1;
                new_table->primary_key_attrs = (char **) realloc(
                        new_table->primary_key_attrs, (new_table->p_key_len) * sizeof(char *) ); 
                new_table->primary_key_attrs[new_table->p_key_len - 1] = (char *) malloc(
                        strlen(attribute->attr_name) ); 
                strcpy(new_table->primary_key_attrs[new_table->p_key_len - 1], attribute->attr_name);
            }

            int is_defined = 0;
            for (int i = 0; i < *name_count; i++){
                if ( strcmp(attr_names[i], token) == 0 ) {
                    is_defined = 1;
                    break;
                }
            }

            if ( is_defined == 0 ) {
                fprintf(stderr, "%s: '%s'\n", 
                "Invalid constraint parameter. Attribute name undefined", token);
                return -1;
            }
        }
    }    
    
    return 0;
}


int foreign_contraint_name_check(char** attr_names, char* constraint_name, int attr_count) {
    while ( (constraint_name = strtok(NULL, " ")) ) {
        if (constraint_name[0] == ')') {
            return 0;
        }
        if ( is_keyword(constraint_name) == 0) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint definition. Constraint name is a keyword", constraint_name);
            return -1;
        }
        // check if attribute is already defined
        int is_defined = 0;
        for (int i = 0; i < attr_count; i++){
            if ( strcmp(attr_names[i], constraint_name) == 0 ) {
                is_defined = 1;
                break;
            }
        }
        if ( is_defined == 0 ) {
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid constraint parameter. Attribute name undefined", constraint_name);
            return -1;
        }
    }
    printf("Constraint '%s' is valid!", constraint_name);
    return 0;
}

