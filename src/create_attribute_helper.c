
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



int attribute_check(char* currentAttr, char* token, char** attr_names, int* name_count, 
        struct catalog_table_data* new_table) 
{
    //printf("Checking if '%s' is a valid attribute...\n", currentAttr);
    attr_names[*name_count] = (char*)malloc( strlen( token ) + 1);
    strcpy(attr_names[*name_count], token);
    //printf("Attribute name: '%s'\n", attr_names[*name_count]);
    // attribute struct
    struct attr_data* new_attribute = 
            (struct attr_data*) malloc(sizeof(struct attr_data));
    // set attribute index
    new_attribute->index = *name_count;
    // set attribute name
    new_attribute->attr_name = attr_names[*name_count];
    // set default num of constraints on attribute to 0
    new_attribute->num_of_constr = 0;


    *name_count += 1;
    if ((*name_count)%10 == 0) { 
        // resize attr_name, add 10 slots
        attr_names = realloc(attr_names, ((*name_count)+10)* sizeof(int*));
    }
    
    token = strtok(NULL, " "); // get type of attribute
    for (int i = 0; token[i] != '\0'; i++) {
        if ( isalpha(token[i]) ) {
            token[i] = tolower(token[i]);
        }
    }
    
    // check if valid attribute type
    //printf("Checking if attribute type '%s' is valid...\n", token);
    if ( is_attr_type(token) == -1 ) {
        fprintf(stderr, "%s: '%s'\n", 
            "Invalid attribute definition. Attribute type is invalid", token);
        return -1;
    } 
    //printf("'%s' is a valid attribute type.\n", token);
    // get attribute type of attribute
//    token[4] = '\0';
    char* a_type_str = strdup(token);
    a_type_str[4] = '\0';
    enum db_type attribute_type = typeof_kw(a_type_str);
    int attribute_size = attribute_type_size(attribute_type, token, currentAttr);
    if (attribute_size == -1) {
        fprintf(stderr, "Invalid attribute size '%d' for type '%d' '%s' in '%s'\n",
                attribute_size, attribute_type, token, currentAttr);
        return -1;
    }
    
    // set type of attribute
    new_attribute->type = attribute_type;
    // set size of attribute
    new_attribute->attr_size = attribute_size;

    // check for constraints on attribute
    int contraint_check = attribute_constraint_check(token, currentAttr, 
            new_attribute, new_table);
    if (contraint_check == -1) {
        fprintf( stderr, "Constraint check on attribute '%s' failed \n", currentAttr);
        return -1;
    }
    printf("'%s' is a valid attribute\n", currentAttr);

    /*
    char* msg = "Attribute created from '%s':\n\tAttribute name: '%s'\n\tIndex: '%d'\n\tAttribute size: '%d'\n\
    \tAttribute type: '%d' \n\tConstraint count: '%d'\n";
    printf(msg, currentAttr, new_attribute->attr_name, new_attribute->index, new_attribute->attr_size,
            new_attribute->type, new_attribute->num_of_constr);
    for (int i = 0; i < new_attribute->num_of_constr; i++) {
        printf("\tConstraint type '%d': '%s'\n", 
            new_attribute->constr[i]->type,
            type_to_str(new_attribute->constr[i]->type));
    }
    */
    // add attribute to catalog table's hashtable
    int attr_add_success = sv_ht_add(new_table->attr_ht, new_attribute->attr_name, new_attribute);
    if (attr_add_success == -1) {
        fprintf(stderr, "Error adding attribute '%s' to table '%s'\n", 
            new_attribute->attr_name, new_table->table_name);
        return -1;
    }

    // success
    return 0;
}


/**
 * Checks for constraints on an attribute is valid.
 * @param token strtok pointing towards possible attribute constraint
 * @param attribute string rep of the attribute
 * @param new_attribute attribute struct
 * @return 0 if success, -1 if failure
 * 
 */
int attribute_constraint_check(char* token, char* attribute, 
            struct attr_data* new_attribute, struct catalog_table_data* new_table) {
    printf("Checking for constraints on '%s'\n", attribute);
    int MAX_CONSTRAINT_COUNT = 3;
    char* constraints_used[3];
    enum db_type constraint_types[3];
    int constraints_count = 0;
    // allot space for 3 constraints
    new_attribute->constr = (struct attr_constraint**)malloc(
                sizeof(struct attr_constraint*) * MAX_CONSTRAINT_COUNT);

    while ( (token = strtok(NULL, " ")) ) {
        if (token[0] == ')') { // end of create statement
            break;
        }
        printf("Checking if '%s' is a valid constraint on an attribute...\n", token);
        for (int i = 0; token[i] != '\0'; i++) {
            if ( isalpha(token[i]) ) {
                token[i] = tolower(token[i]);
            }
        }
        if ( is_attr_con(token) == -1 ) { // check is it is an attribute constraint
            fprintf(stderr, "%s: '%s'\n", 
            "Invalid attribute definition. Attribute constraint is invalid", token);
            return -1;
        }
        for (int i = 0; i < constraints_count; i++ ) { // check if constraint was already used
            if (strcmp(token, constraints_used[i]) == 0) {
                fprintf(stderr, "%s: '%s'\n", 
                "Invalid attribute definition. Attribute constraint already used", token);
                return -1;
            }
        }
        // add constraint sring to used constraints
        constraints_used[constraints_count] = (char*)malloc( strlen( token ) + 1 );
        strcpy(constraints_used[constraints_count], token);
        // add constraint type
        constraint_types[constraints_count] = typeof_kw(constraints_used[constraints_count]);
        //printf("Constraint on attribute found: '%s' '%d'\n", 
        //        constraints_used[constraints_count], constraint_types[constraints_count]);
        constraints_count += 1;
        


    }
    // set constraint count
    new_attribute->num_of_constr = constraints_count;
    // add attribute constraints to attribute struct
    add_attr_constraints(constraints_used, constraint_types, new_attribute, new_table);

    return 0;
}


/**
 * Validate the attribute constraint and add it to attribute struct
 * @param constraints the array of constraints used
 * @param constraint_types the enum types of the constraints on the attribute
 * @param attribute the attribute struct
 * @param table the current table catalog struct being created
 * @return 0 if all constraints were validated and added successfully, else -1 for failure
 */
int add_attr_constraints(char** constraints, enum db_type* constraint_types, 
            struct attr_data* attribute, struct catalog_table_data* table) {
    printf("Adding constraints to attribute '%s'...\n", attribute->attr_name);
    for (int i = 0; i < attribute->num_of_constr; i++) {
        //printf("Adding constraint '%s' '%d'...\n", constraints[i], constraint_types[i]);

        if (constraint_types[i] == NOT_NULL) { // not null
            // create attr_constraint struct
            struct attr_constraint* not_null_constraint = (struct attr_constraint*)malloc(
                sizeof(struct attr_constraint)
            );
            not_null_constraint->type = NOT_NULL;
            // add attr_constraint to attribute struct
            attribute->constr[i] = not_null_constraint;

            // update table
            table->num_of_notnull += 1;
            // allot space for notnull attribute string
            table->notnull_attrs= (char **) realloc(
                    table->notnull_attrs, (table->num_of_notnull) * sizeof(char *) ); 
            table->notnull_attrs[table->num_of_notnull - 1] = (char*) malloc(
                strlen(attribute->attr_name));

            strcpy(table->notnull_attrs[table->num_of_notnull - 1], attribute->attr_name);
            printf("Added notnull constraint '%s' to table '%s' \n", 
                    table->notnull_attrs[table->num_of_notnull - 1], table->table_name);
        }
        else if (constraint_types[i] == PRIMARY_KEY) { // primarykey
            // check if primarykey constraint was already used
            // create attr_constraint struct
            struct attr_constraint* primary_key_constraint = (struct attr_constraint*)malloc(
                sizeof(struct attr_constraint)
            );
            primary_key_constraint->type = PRIMARY_KEY;
            // add attr_constraint to attribute struct
            attribute->constr[i] = primary_key_constraint;

            // update table
            table->p_key_len += 1;
            // allot space for primarykey string
            table->primary_key_attrs = (char **) realloc(
                    table->primary_key_attrs, (table->p_key_len) * sizeof(char *) ); 
            table->primary_key_attrs[table->p_key_len - 1] = (char*) malloc(
                strlen(attribute->attr_name));

            strcpy(table->primary_key_attrs[table->p_key_len - 1], attribute->attr_name);
            printf("Added primary key '%s' to table '%s' \n", 
                    table->primary_key_attrs[table->p_key_len - 1], table->table_name);
        }
        else if (constraint_types[i] == UNIQUE) { // unique
            // create attr_constraint struct
            struct attr_constraint* unique_constraint = (struct attr_constraint*)malloc(
                sizeof(struct attr_constraint)
            );
            unique_constraint->type = UNIQUE;
            // add attr_constraint to attribute struct
            attribute->constr[i] = unique_constraint;

            // add a new unqiue grouping
            table->num_of_unique += 1;
            table->unique_group_sizes = (int*) realloc(table->unique_group_sizes, 
                        table->num_of_unique * sizeof(int));
            table->unique_group_sizes[table->num_of_unique - 1] = 1;
            // allot space for unique attribute string
            table->unique_attrs = (char ***) realloc(
                    table->unique_attrs, (table->num_of_unique) * sizeof(char **) ); 
            table->unique_attrs[table->num_of_unique - 1] = (char**) malloc(
                    table->unique_group_sizes[table->num_of_unique - 1] * sizeof(char*));
            table->unique_attrs[table->num_of_unique - 1][0] = (char*) malloc( 
                    strlen(attribute->attr_name) );

            strcpy(table->unique_attrs[table->num_of_unique - 1][0], attribute->attr_name);
            printf("Added unique constraint '%s' to table '%s' \n", 
                    table->unique_attrs[table->num_of_unique - 1][0], table->table_name);
        }
        else {
            fprintf( stderr, "Invalid constraint type '%d': '%s'\n", 
                    constraint_types[i], constraints[i]);
            return -1;
        }
    }
    return 0;
}


/**
 * Gets the attribute size based on the attribute type
 * @param attribute_type the attribute type for token
 * @param token the attribute type string
 * @param currentAttr the current attribute
 * @return the size of the attribute if successful, else -1 for failure
 */
int attribute_type_size(enum db_type attribute_type, char* token, char* currentAttr) {
    int attribute_size = -1;
    if (attribute_type == INT) 
    {
        attribute_size = sizeof(int);
    }
    else if (attribute_type == DOUBLE)
    {
        attribute_size = sizeof(double);
    }
    else if (attribute_type == BOOL)
    {
        attribute_size = sizeof(bool);
    }
    else if (attribute_type == CHAR || attribute_type == VARCHAR) 
    {
        int char_length = char_type_check(token, attribute_type);
        if (char_length == -1) {
            fprintf( stderr, "vchar/char length check failed for '%s' in '%s'\n", token, currentAttr);
            return -1;
        } 
        attribute_size = sizeof(char) * char_length;
    }
    else {
        fprintf( stderr, "Invalid attribute type: '%d' from '%s'! \n", attribute_type, token);
        return -1;
    }

    return attribute_size;
}


/**
 * Check for the specified size of vchar/char attribute.
 * 'varchar(X)' or 'char(X)'.
 * @param token the varchar/char token to get the length from
 * @param new_attribute the attribute struct currently being constructed
 * @param attribute_type the attribute type -- varchar/char
 * @return varchar/char length if successful check, -1 if check failed
 */
int char_type_check(char* token, enum db_type attribute_type) 
{
    char* char_attr = (char *)malloc( strlen(token) + 1 );
    strcpy(char_attr, token);
    int length;
    
    if (attribute_type == VARCHAR) {
        char_attr += strlen(VARCHAR_TYPE);
    }
    else if (attribute_type == CHAR) {
        char_attr += strlen(CHAR_TYPE);
    }
    else {
        fprintf( stderr, "Invalid attribute type '%d' for '%s'\n", attribute_type, token );
        return -1;
    }
    // skip '('
    char_attr += 1;

    //printf("Attribute '%s' is a vchar/char! Getting length of vchar/char...\n", token);
    length = 0;
    char* text_part;
    length = strtol(char_attr, &text_part, 10);

    if ( length < 1) {
        fprintf( stderr, "Invalid varchar/char length speicifed: '%d' in '%s'. Length must be >= 1\n",
                length, token );
        return -1;
    }
    //printf("Length of '%s' found: '%d'.\n", token, length);

    return length;
}
