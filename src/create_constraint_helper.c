
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
#include "../include/catalog/catalog_printer.h"


int constraint_check(char* currentAttr, char* token, char** attr_names, int* name_count, 
        int attr_num, struct catalog_table_data* new_table) {
    char func_loc_str[] = "(create_constraint_helper.c/constraint_check)";
    printf("Checking if '%s' is a valid constraint...\n", currentAttr);

    // if it is check if attribute is constraint
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    if ( is_constraint(token) == 0 && attr_num == 0) {
        fprintf(stderr, "Invalid: Constraint is defined before attributes: '%s'\n", token);
        return -1;
    }
    enum db_type type = typeof_kw(token);
    //printf("Constraint '%s' type: '%d'\n", token, type);
    // check if foreignkey
    if ( type == FOREIGN_KEY ) {
        //printf("Attribute is a foreign key constraint: '%s'\n", token);

        // foreignkey( <a 1> ... <a N> ) references <r name>( <r 1> ... <r N> ):
        // track attrs in foreignkey( <a 1> ... <a N> ) - <a N>
        char** foreign_attr = (char **) malloc( sizeof(char*));
        int fa_count = 0;
        // track attrs in <r name>( <r 1> ... <r N> ) - <r N>
        char** parent_attr = (char **) malloc( sizeof(char*));
        int pa_count = 0; 
        // initialize a new foreign_key struct
        struct foreign_key_data* new_foreign_key = (struct foreign_key_data*) malloc( 
            sizeof(struct foreign_key_data));
        new_foreign_key->f_keys = ht_create(12, 0.75);
        new_foreign_key->parent_table_name = NULL;


        // check for key attribute names
        int valid_constraint_line = foreign_contraint_name_check(attr_names, *name_count, token,
                    foreign_attr, &fa_count);
        if ( valid_constraint_line == -1 ) {
            fprintf(stderr, "Invalid constraint '%s'\n", token);
            return -1;
        }
        // check for 'references'
        token = strtok(NULL, " ");
        // printf("Token: '%s'\n", token);
        if ( strcmp(REFERENCES_CON, token) != 0) {
                fprintf(stderr, 
                    "Invalid constraint definition. Missing 'references' keyword: '%s'\n", token);
                return -1;
        }
        //printf("Valid 'reference' keyword present\n");

        // <relation>( check
        token = strtok(NULL, " ");
        // printf("Token: '%s'\n", token);
        if ( token[strlen(token) - 1] != '(' ) {
            fprintf(stderr, "Invalid constraint definition. Missing '(': '%s'\n", token);
                return -1;
        }
        token[strlen(token) - 1] = '\0';
        //printf("Parent table name: '%s'\n", token);
        // check if parent table is defined
        if (catalog_contains(token) == 0) {
            fprintf(stderr, "Invalid: table '%s' not defined\n", token);
            return -1;
        }
        new_foreign_key->parent_table_name = (char *) malloc( strlen(token) );
        strcpy(new_foreign_key->parent_table_name, token);
        //printf("Foreign table '%s' being referenced exists!\n", new_foreign_key->parent_table_name);

        // <r_1> ... <r_n>
        int valid_parent_attr = foreign_parent_attr_check(new_foreign_key->parent_table_name, token, 
                parent_attr, &pa_count);
        if (valid_parent_attr == -1) {
            fprintf(stderr, "Invalid attribute names for parent table '%s': '%s'\n", 
                    new_foreign_key->parent_table_name, currentAttr);
            return -1;
        }

        // compare <a 1> ... <a N> and <r_1> ... <r_n>
        int compare_success = compare_new_to_parent_attr_types(
                new_table, foreign_attr, fa_count, 
                new_foreign_key->parent_table_name, parent_attr, pa_count
        );
        if (compare_success == -1) {
            fprintf(stderr, "Foreignkey declaration type mismatch: '%s'\n", currentAttr);
            return -1;
        } 
        //printf("Types align in foreignkey!\n");

        // add the new foreignkey struct to table
        if (new_foreign_key->parent_table_name == NULL) {
            fprintf(stderr, "Invalid: foreign key parent name not specified: '%s'\n", currentAttr);
            return -1;
        }
        printf("Constraint '%s' is a valid  foreign key constraint!\n", currentAttr);

    } 
    else { // either a primarykey, unqiue, or invalid
        //printf("Attribute is a primarykey/unique constraint: '%s'\n", token);

        // check if valid constraint def
        if ( type != PRIMARY_KEY && type != UNIQUE) {
            fprintf(stderr, "Invalid constraint type: '%s'\n", token);
            return -1;
        }
        if (type == UNIQUE) {
            /* new code */
            int unique_group_count = new_table->unique_group_count;
            if(unique_group_count < 0){
                printf("Error: num_of_uniques < 0. %s\n", func_loc_str);
                exit(0);
            }

            if(unique_group_count == new_table->unique_group_arr_size){
                int old_size = new_table->unique_group_count;
                int new_size = old_size * 2;

                new_table->unique_group_arr = realloc(new_table->unique_group_arr, sizeof(struct unique_group*) * new_size);
                new_table->unique_group_arr_size = new_size;
            }
            new_table->unique_group_count = new_table->unique_group_count + 1;
            int unique_group_index = new_table->unique_group_count - 1;

            struct unique_group* unique_group = malloc(sizeof(unique_group));
            unique_group->size = 0;
            unique_group->arr_size = 12;
            char** unique_attr_names = malloc(sizeof(char*) * unique_group->arr_size);
            unique_group->unique_attr_names = unique_attr_names;

            new_table->unique_group_arr[unique_group_index] = unique_group;
            /* new code end */

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
                "Invalid constraint definition. Constraint attribute is a keyword\n", token);
                return -1;
            }

            if ( !sv_ht_contains(new_table->attr_ht, token) ) {
                fprintf(stderr, "Attribute '%s' undefined used in constraint '%s'\n", 
                        token, currentAttr);
                return -1;
            }
            //printf("Attribute '%s' valid in constraint '%s'\n", 
            //        token, currentAttr);
            // add constraint to attr_struct

            struct attr_data* attribute = (struct attr_data*) sv_ht_get(new_table->attr_ht, token);

            // update catalog_table_data struct for constraint primarykey/unique
            if (type == PRIMARY_KEY) {
                struct attr_constraint* new_constraint = (struct attr_constraint*) malloc(
                        sizeof(struct attr_constraint));
                new_constraint->type = type;
                attribute->constr[attribute->num_of_constr] = new_constraint;
                attribute->num_of_constr += 1;

                new_table->p_key_len += 1;
                new_table->primary_key_attrs = (char **) realloc(
                        new_table->primary_key_attrs, (new_table->p_key_len) * sizeof(char *) ); 
                new_table->primary_key_attrs[new_table->p_key_len - 1] = (char *) malloc(
                        strlen(attribute->attr_name) ); 
                strcpy(new_table->primary_key_attrs[new_table->p_key_len - 1], attribute->attr_name);
            }
            else if (type == UNIQUE) {
                // DO GROUPING OF UNIQUE: UNIQUE( a b c ) -> {..., {a,b,c}, ...}

                /* new code */
                int unique_group_index = new_table->unique_group_count - 1;
                struct unique_group* unique_group = new_table->unique_group_arr[unique_group_index];
                int unique_group_size = unique_group->size;
                int unique_arr_size = unique_group->arr_size;

                if(unique_group_size == unique_arr_size){
                    /*if the current number of attribute count is the max size then resize the group array*/
                    int old_size = unique_arr_size;
                    int new_size = old_size * 2;
                    unique_group = realloc(unique_group, sizeof(struct unique_group*) * new_size);
                    unique_group->arr_size = new_size;
                    new_table->unique_group_arr[unique_group_index] = unique_group;
                }

                char* attr_name = strdup(attribute->attr_name);
                unique_group->unique_attr_names[unique_group_size] = attr_name;
                unique_group->size = unique_group->size + 1;
                /* new code */

            }

            int is_defined = 0;
            for (int i = 0; i < *name_count; i++){
                if ( strcmp(attr_names[i], token) == 0 ) {
                    is_defined = 1;
                    break;
                }
            }

            if ( is_defined == 0 ) {
                fprintf(stderr, 
                        "Invalid constraint parameter. Attribute name undefine: '%s'\n", token);
                return -1;
            }
        }

    }    
    
    return 0;
}

int compare_new_to_parent_attr_types(
    struct catalog_table_data* new_table, char** new_attributes, int n_count,
    char* parent_table_name, char** parent_attributes, int p_count) {
    if (n_count != p_count) {
        fprintf(stderr, "Invalid: imbalance of attributes in foreignkey constraint.\n\
        Count of foreign keys in table being created: '%d'\n\
        Count of keys being referenced in parent table '%s': '%d'\n", 
                n_count, parent_table_name, p_count);
        return -1;
    }
    struct catalog_table_data* parent_table = sv_ht_get(catalog_get_ht(), parent_table_name);
    
    // create a new foreign key
    struct foreign_key_data* new_f_key = (struct foreign_key_data*) malloc(
            sizeof(struct foreign_key_data));
    new_f_key->parent_table_name = (char*) malloc( strlen(parent_table_name));
    new_f_key->parent_table_name = parent_table_name;
    new_f_key->f_keys = (struct hashtable*) malloc( sizeof(struct hashtable));
    new_f_key->f_keys = ht_create(12, 0.75);

    for( int i = 0; i < n_count; i++ ) {
        char* n_attr = new_attributes[0];
        char* p_attr = parent_attributes[0];
        /*printf("Checking if attribute '%s' in table '%s' is same type as \
attribute '%s' in table '%s'\n", 
            n_attr, new_table->table_name, p_attr, parent_table_name);*/
        enum db_type n_type = ((struct attr_data*)sv_ht_get(new_table->attr_ht, n_attr))->type;
        enum db_type p_type = ((struct attr_data*)sv_ht_get(parent_table->attr_ht, p_attr))->type;
        if( n_type != p_type ) {
            fprintf(stderr, "Attribute mismatch types:\
             '%s' in table '%s' is type '%d'. '%s' in table '%s' is type '%d'\n", 
            n_attr, new_table->table_name, n_type,
            p_attr, parent_table_name, p_type);
            return -1;
        }
        //printf("Adding foreign key pairing...\n");
        
        // add pair to f key hashtable
        sv_ht_add(new_f_key->f_keys, n_attr, p_attr);

        
    }
    // add new f key to new table
    new_table->num_of_f_key += 1;
    new_table->f_key_arr_size += 1;
    new_table->f_keys = (struct foreign_key_data**) realloc(
        new_table->f_keys, new_table->num_of_f_key * sizeof(struct foreign_key_data*)
    );
    new_table->f_keys[new_table->num_of_f_key - 1] = (struct foreign_key_data*) malloc(
        sizeof(struct foreign_key_data)
    );
    new_table->f_keys[new_table->num_of_f_key - 1] = new_f_key;

    return 0;
}


int foreign_contraint_name_check(char** attr_names, int attr_count, char* constraint_line, 
            char** foreign_attrs, int* fa_count) {
    while ( (constraint_line = strtok(NULL, " ")) ) {
        if (constraint_line[0] == ')') {
            break;
        }
        printf("Checking if attribute '%s' is defined...\n", constraint_line);

        if ( is_keyword(constraint_line) == 0) {
            fprintf(stderr, "Invalid attribute '%s' not defined in table.\n", constraint_line);
            return -1;
        }
        // check if attribute is already defined
        int is_defined = 0;
        for (int i = 0; i < attr_count; i++){
            if ( strcmp(attr_names[i], constraint_line) == 0 ) {
                is_defined = 1;
                break;
            }
        }
        if ( is_defined == 0 ) {
            fprintf(stderr, 
                    "Invalid constraint parameter. Attribute name undefined: '%s'\n", constraint_line);
            return -1;
        }
        printf("Foreignkey attr '%s' is defined!\n", constraint_line);
        *fa_count += 1;
        foreign_attrs = (char**) realloc(foreign_attrs, *fa_count * sizeof(char *));
        foreign_attrs[ *fa_count - 1] = (char*) malloc( strlen(constraint_line) );
        strcpy(foreign_attrs[ *fa_count - 1], constraint_line);

    }

    return 0;
}

int foreign_parent_attr_check(char* parent_name, char* constraint_line, 
            char** parent_attrs, int* pa_count) {
    while ( (constraint_line = strtok(NULL, " ")) ) {
        if (constraint_line[0] == ')') {
            break;
        }
        printf("Checking if attribute '%s' is defined in parent...\n", constraint_line);

        if ( is_keyword(constraint_line) == 0) {
            fprintf(stderr, "Invalid attribute name: '%s', is a keyword\n", constraint_line);
            return -1;
        }

        // check if attribute is defined in parent
        // check if parent table exists
        if (catalog_contains(parent_name) != 1) {
            fprintf(stderr, "Catalog does not contain table '%s'\n", parent_name);
            return -1;
        }
        // get parent table
        struct catalog_table_data* parent_table = sv_ht_get( catalog_get_ht(), parent_name);
        // check for attr in parent table hashtable
        int parent_contains_attr = sv_ht_contains(parent_table->attr_ht, constraint_line);
        // check error code
        if ( parent_contains_attr != 1 ) {
            fprintf(stderr,  "Invalid constraint parameter. Attribute '%s' not defined in parent '%s'\n", 
                    constraint_line, parent_name);
            return -1;
        }
        printf("Foreignkey attr '%s' is defined in parent '%s'!\n", constraint_line, parent_name);
        *pa_count += 1;
        parent_attrs = (char**) realloc(parent_attrs, *pa_count * sizeof(char *));
        parent_attrs[ *pa_count - 1] = (char*) malloc( strlen(constraint_line) );
        strcpy(parent_attrs[ *pa_count - 1], constraint_line);

    }

    return 0;
}
