///**
// * Note: Case does not matter
// */
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/db_types.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/si_ht.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/storagemanager.h"
#include "hash_collection.h"




static char catalog_file_name[] = "catalog.data";
static char* catalog_loc;

static struct hashtable* table_ht;


int init_catalog(char* db_path){
    //check if catalog_exist then call init_mapping
    //catalog.data
    int length = strlen(db_path)+strlen(catalog_file_name);
    catalog_loc = malloc(sizeof(char)*length+1);
    snprintf(catalog_loc, length + 1, "%s%s", db_path, catalog_file_name);
    struct stat s;

    if(stat(catalog_loc, &s)==0){
        //catalog exist
        printf("catalog exist\n");
    }else{
        //catalog does not exist
        printf("Catalog does not exist : %s\n", catalog_loc);
        table_ht = ht_create(12, 0.75);
    }

    return 0;
}



/**
 * get the default value for the attribute and assign it to "val"
 * @param str
 * @param attr_type
 * @param val
 * @return returns the pointer to the remaining string
 */
union default_value* get_attr_constr_default_val(char** str, enum db_type attr_type){
    union default_value* val = malloc(sizeof(union default_value));
    int start_index = 0;
    int end_index = 0;
    char* ptr;


    //check if str = "constr_val" <constraints>
    if((*str)[0] != '"'){
        // this means str = constr_val <constraints>
        ptr = strchr((*str)+1, ' ');

        if(ptr == NULL){
            end_index = strlen(*str) - 1;
        }else{
            end_index = ptr - *str - 1;
            ptr = ptr + 1;
        }

        if((*str)==NULL || (*str)[0] == '\0'){
            fprintf(stderr, "(catalog.c/get_attr_constr_default_val) Expected a default val string but got none!!!\n");
            exit(0);
        }

        char* val_str = substring(*str, start_index, end_index);
        int i_val;
        double d_val;

        switch(attr_type){
            case INT:
                sscanf(val_str, "%d", &i_val);
                val->i = i_val;
                break;
            case DOUBLE:
                sscanf(val_str, "%lf", &d_val);
                val->d = d_val;
                break;
            case BOOL:
                if(strncmp(val_str, "true", 4) == 0){
                    val->b = true;
                }else{
                    val->b = false;
                }
                break;
            default:
                fprintf(stderr, "(catalog.c/get_attr_constr_default_val) Error: cannot match constraint type!!!\n");
                exit(0);
                break;
        }
        free(val_str);
    }else{

        //str = "constraint val" <constraints>
        ptr = strchr((*str) + 1, '"');     //ptr currently points to '" <constrains>'

        if(ptr == NULL){
            fprintf(stderr, "(catalog.c/get_attr_constr_default_val) Expected '\"' but could not find any!!!\n");
            fprintf(stderr, "str = >%s<\n", *str);
            exit(0);
        }

        end_index = ptr - *str - 1;
        start_index = 1;
        ptr = ptr + 2;          //ptr now currently points to "<constraints>..."

        char* val_str = substring(*str, start_index, end_index);

        if(attr_type == CHAR){
            strncpy(val->c, val_str, strlen(val_str) + 1);
        }else{
            strncpy(val->v, val_str, strlen(val_str) + 1);
        }
        free(val_str);
    }
    *str = ptr;

    return val;
}

/**
 * get the constraint type and values for the attribute
 * @param str
 * @param attr_type
 * @param constr_type
 * @param val
 * @return
 */
union default_value* get_attr_constr_type_val(char** str, char* attr_name, enum db_type attr_type, enum db_type* constr_type){
    int start_index = 0;
    int end_index = 0;
    char* ptr;
    union default_value* val;
    val = NULL;


    ptr = strchr(*str, ' ');         //i.e "notnull" -> null b/c there is no space afterward
    if(ptr == NULL){
        end_index = strlen(*str) - 1;

        if((*str)[0] == '\0' || (*str)==NULL){
            fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Expected a constraint string but got null\n");
            fprintf(stderr, "attribute is >%s<\n", attr_name);
            exit(0);
        }

        char* constr_name = substring(*str, start_index, end_index);
        enum db_type tmp_type = typeof_kw(constr_name);


        if(tmp_type == UNKNOWN || tmp_type == DEFAULT){
            fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Type of constraint name is not the following: notnull !!!\n");
            fprintf(stderr, "constraint name is >%s<\n", constr_name);
            exit(0);
        }
        free(constr_name);
        *constr_type = tmp_type;
        *str = ptr;

        return NULL;
    }

    //else there is more than one constraint i.e. "notnull default ..." -> " default ..."
    end_index = ptr - *str - 1;

    if((*str)[0] == '\0' || (*str)==NULL){
        fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Expected a constraint string but got null\n");
        exit(0);
    }

    char* constr_name = substring(*str, start_index, end_index);
    enum db_type tmp_type = typeof_kw(constr_name);
    free(constr_name);
    ptr = ptr + 1;

    if(tmp_type == UNKNOWN){
        fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Type of constraint name is unknown!!!\n");
        fprintf(stderr, "constraint name is >%s<\n", constr_name);
        exit(0);
    }

    if(tmp_type == DEFAULT){
        *constr_type = tmp_type;
        if(ptr[0] == ' '){
            fprintf(stderr, "(catalog.c/get_attr_constr_type_val) Default value starts with a space!!!\n");
            exit(0);
        }

        val = get_attr_constr_default_val(&ptr, attr_type);
        *str = ptr;
        return val;
    }

    //else it has to be NotNULL
    *constr_type = tmp_type;
    val = NULL;
    *str = ptr;
    return val;


}



struct attr_constraint** get_attr_constraints(int* count, char* attr_name, enum db_type attr_type, char* constr_str){
//    printf("constraints are : >%s<\n", constr_str);
    struct attr_constraint** constr = malloc(sizeof(struct attr_constraint*)*4);
    *count = 0;

    int constr_index = 0;
    char* ptr = constr_str;
    enum db_type constr_type;


    while(1){

        union default_value* default_val = get_attr_constr_type_val(&ptr, attr_name, attr_type, &constr_type);

        if(constr_type == UNKNOWN){
            // just a precaution
            fprintf(stderr, "(catalog.c/get_attr_constraints) returned constr_type is unknown (this is IMPOSSIBLE)!!!\n");
            exit(0);
        }

        constr[constr_index] = malloc(sizeof(struct attr_constraint));
        constr[constr_index]->type = constr_type;
        constr[constr_index]->value = default_val;
        constr_index += 1;

        if(ptr == NULL || ptr[0] == '\0'){
            break;
        }
    }

    *count = constr_index;
    return constr;
}

/**
 * Get the attribute name and store it in attr_name
 * @param str
 * @return a ptr to the remaining part of the string
 */
char* get_attr_name(char* str, char** attr_name){
    int start_index = 0;
    int end_index = 0;
    char* ptr;

    ptr = strchr(str, ' ');

    if(ptr == NULL){
        fprintf(stderr, "(catalog.c/get_attr_name) Can't get attribute name!!!\n");
        fprintf(stderr, "Either attribute dont have a data type or there are no space!!!\n");
        exit(0);
    }

    end_index = ptr - str - 1;
    *attr_name = substring(str, start_index, end_index);

    return ptr + 1;             //returns "<data_type> ..."
}

/**
 * get the size of the attribute type
 * @param type
 * @return the size of the attribute type
 */
int get_attr_size(enum db_type type){
    if(type == UNKNOWN){
        fprintf(stderr, "(catalog.c/get_attr_size) Type is unknown when it should be either: int, double, or bool\n");
        exit(0);
    }

    int size = 0;

    switch(type){
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case BOOL:
            size = sizeof(bool);
            break;
        default:
            fprintf(stderr, "(catalog.c/get_attr_size)Got an unexpected type for attribute!!!\n");
            exit(0);
    }

    return size;

}

/**
 * get the size of a char type or a varchar type attribute
 * @param str the string that contains the size string i.e. "(23)" = size of 23
 * @return the size
 */
int get_attr_char_size(char** str){
    int size;
    int start_index = 1;
    int end_index = 1;
    char* ptr = strchr(*str, ')');      // ptr = "(23) <constraint>" -> ") <constraint>"

    end_index = ptr - *str - 1;
    char* attr_size_str = substring(*str, start_index, end_index);

    if(is_int(attr_size_str) == 0){
        //if attr_size_str is not a int in string form then give a error msg
        fprintf(stderr, "(catalog.c/get_attr_char_size)attr_size_str is not a valid int in string form!!\n");
        fprintf(stderr, "attr_size_str = >%s<\n", attr_size_str);
        exit(0);
    }
    sscanf(attr_size_str, "%d", &size);
    free(attr_size_str);

    ptr = strchr(ptr, ' ');
    if(ptr == NULL){
        //means that there are not constraints so set the ptr or in this case *str to NULL
        *str = NULL;
    }else{
        *str = ptr + 1;         //*str now points to "<constraints>"
    }

    return size;
}


char* get_attr_type_n_size(char* str, enum db_type* type, int* size){
    int start_index = 0;
    int end_index = 0;
    char* ptr;

    ptr = strchr(str, '(');     //i.e., "char(23) <constraints>"  -> "(23) <constraints>"

    if(ptr == NULL){

        ptr = strchr(str, ' '); //i.e., "int <constraints>" -> " <constraints>"

        if(ptr == NULL){
            end_index = strlen(str) - 1;
            char* attr_type_str = substring(str, start_index, end_index);
            enum db_type tmp_type = typeof_kw(attr_type_str);
            free(attr_type_str);

            *size = get_attr_size(tmp_type);
            *type = tmp_type;

            return NULL;            //No contraints to parse so return NULL
        }

        //else this means ptr currently points to " <constraints>"
        end_index = ptr - str - 1;
        char* attr_type_str = substring(str, start_index, end_index);
        enum db_type tmp_type = typeof_kw(attr_type_str);
        free(attr_type_str);

        *size = get_attr_size(tmp_type);
        *type = tmp_type;

        return ptr + 1;              //returns "<constraints>" to be parsed later (notice that there are no leading space)
    }

    //else this means ptr currently points to "(23) <constraints>"
    end_index = ptr - str - 1;
    char* attr_type_str = substring(str, start_index, end_index);
    enum db_type tmp_type = typeof_kw(attr_type_str);
    free(attr_type_str);

    *size = get_attr_char_size(&ptr);       //ptr will become either NULL or points to the head of the constraints string
    *type = tmp_type;

    if(ptr == NULL){
        //there are no constraint
        return NULL;
    }
    return ptr;

}

struct attr_data* get_attr(char* attr_data_str){
    struct attr_data* a_data = malloc(sizeof(struct attr_data));

    int attr_size;
    enum db_type type;
    char* attr_name;

    int num_of_constr;
    struct attr_constraint** constr;

    char* ptr;
    ptr = get_attr_name(attr_data_str, &attr_name);       //returned ptr should point to the begining of the data type
    ptr = get_attr_type_n_size(ptr, &type, &attr_size);   //can return NULL or a pointer to the head of the constraints


    if(ptr == NULL){
        //No constraints
        num_of_constr = 0;
        constr = NULL;
    }else{
        constr = get_attr_constraints(&num_of_constr, attr_name, type, ptr);
    }
    a_data->attr_size = attr_size;
    a_data->type = type;
    a_data->attr_name = attr_name;
    a_data->num_of_constr = num_of_constr;
    a_data->constr = constr;

//    printf("a_name=%s, a_type=%d, a_size=%d, a_constr_count=%d\n", a_data->attr_name,
//           (int)(a_data->type), a_data->attr_size, a_data->num_of_constr);


    return a_data;
}

int catalog_add_attributes(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size){
    struct hashtable* attr_ht = ht_create(12, 0.75);

    char* key_word;
    enum db_type type;

    for(int i = 0; i < data_str_size; i++){

        char* kw_end_ptr = strchr(data_str_arr[i], '(');

        if(kw_end_ptr != NULL){
            int kw_end_index = kw_end_ptr - data_str_arr[i] - 1;
            int kw_start_index = 0;
            key_word = substring(data_str_arr[i], kw_start_index, kw_end_index);
            /*check if the s_str(sub string) is a keyword*/
            type = typeof_kw(key_word);
            free(key_word);

            if(type == UNKNOWN) {
                //if the keyword is UNKNOWN/invalid then it has to be a attribute
                struct attr_data* a_data = get_attr(data_str_arr[i]);
                sv_ht_add(attr_ht, a_data->attr_name, a_data);
//                printf("create attr_data\n");

            }
        }else{
            //if there are no parentheses then it is an attribute
            struct attr_data* a_data = get_attr(data_str_arr[i]);
//            printf("create attr_data\n");
//            struct attr_data* a_data = get_attr(str_arr[i]);
            sv_ht_add(attr_ht, a_data->attr_name, a_data);
        }

    }

    t_data->attr_ht = attr_ht;

    return 0;
}



/**
 * get all the attributes in the data_str and
 * add it to an array that represents the primary key
 * and return it
 * @param p_key_len : length of the primary key
 * @param data_str  : the string that contains the attributes of the primary key
 * @return an array that represents the primary key
 */
char** get_primary_key_attrs(int* p_key_len, char* data_str){
    // data_str = "<attr>, <attr>, <attr>, ..."
    char** primary_key_attrs;
    *p_key_len = split(&primary_key_attrs, data_str, ' ');

    return primary_key_attrs;
}
/**
 * add the primary key to the table
 * @param t_data : a struct that stores information regarding the table
 * @param data_str : a 2d array of strings that needs to be parsed
 * @return 0 for success
 */
int catalog_add_primary_key(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size){

    char* key_word;
    char* ptr;
    enum db_type type;

    int start_index = 0;
    int end_index = 0;

    for(int i = 0; i < data_str_size; i++){
        ptr = strchr(data_str_arr[i], '(');

        if(ptr != NULL){
            //it is either a primary key, foreign key, or unique constraint
            end_index = ptr - data_str_arr[i] - 1;
            key_word = substring(data_str_arr[i], start_index, end_index);

//            printf("key word is : %s\n", key_word);
            /*check if the s_str(sub string) is a keyword*/
            type = typeof_kw(key_word);
            free(key_word);

            if(type == PRIMARY_KEY){
                // ptr current points to "( <attr> <attr> ... <attr> )" There could be space after paren or no space
                start_index = end_index + 2;
                ptr = strchr(ptr + 1, ')');
                end_index = ptr - data_str_arr[i] - 1;

                if(end_index < start_index){
                    fprintf(stderr, "(catalog.c/catalog_add_primary_key) end index is less than start index when getting primary key attributes\n");
                    exit(0);
                }

//                printf("data_str_arr[i] is : %s\n", data_str_arr[i]);
                char* p_key_attr_str = substring(data_str_arr[i], start_index, end_index);
                remove_leading_spaces(p_key_attr_str);
                remove_ending_spaces(p_key_attr_str);

                int p_key_len;
                char** primary_key_attrs = get_primary_key_attrs(&p_key_len, p_key_attr_str);
                free(p_key_attr_str);

                t_data->p_key_len = p_key_len;
                t_data->primary_key_attrs = primary_key_attrs;
                return 0;
            }
        }
    }

    return 0;
}


/**
 * Return 1 if the attributes are the same type and 0 if they are not
 * @param a_data_1
 * @param a_data_2
 * @return
 */
int compare_attr_data(struct attr_data* a_data_1, struct attr_data* a_data_2){
    if(a_data_1->type != a_data_2->type){
        return 0;
    }
    return 1;
}


int check_if_attr_matches(int count, struct hashtable* attr_ht, struct hashtable* ref_attr_ht,
        char** foreign_key_attrs, char** ref_table_attrs){

    for(int i = 0; i < count; i++){
        struct attr_data* a_data_1 = sv_ht_get(attr_ht, foreign_key_attrs[i]);
        struct attr_data* a_data_2 = sv_ht_get(ref_attr_ht, ref_table_attrs[i]);

        if(compare_attr_data(a_data_1, a_data_2) == 0){
            fprintf(stderr, "(catalog.c/get_foreign_key) a_data_1=%s, a_data_2=%s does not match!!!\n", a_data_1->attr_name, a_data_2->attr_name);
            return 0;
        }
    }

    return 1;
}

/**
 * Create a foreign key and return it
 * @param attr_count
 * @param ref_table_name
 * @param foreign_key_attrs
 * @param ref_attrs
 * @return
 */
struct foreign_key_data* create_foreign_key(int attr_count, char* ref_table_name, char** foreign_key_attrs, char** ref_attrs){
    struct foreign_key_data* f_key = malloc(sizeof(struct foreign_key_data));

    f_key->parent_table_name = ref_table_name;

    struct hashtable* f_key_ht = ht_create(12, 0.75);

    for(int i = 0; i < attr_count; i++){
        sv_ht_add(f_key_ht, foreign_key_attrs[i], ref_attrs[i]);
    }

    f_key->f_keys = f_key_ht;
    return f_key;
}

/**
 * Creates a foreign_key_data struct using the data_str and returns it.
 *
 * The foreign_key_data struct holds all information on the foreign key and
 * the mapping from the foreign_key to the primary_key of the referenced table
 * @param data_str : the string to be parsed
 * @return a foreign_key_data struct
 */
struct foreign_key_data* get_foreign_key(int num_of_f_key_attr, int num_of_ref_attr, char** foreign_key_attrs,
        char** ref_table_attrs, char* ref_table_name, struct hashtable* table_attr_ht, int* result){

    int table_exist = sv_ht_contains(table_ht, ref_table_name);
    if(table_exist){
        printf("Table name exist : >%s<\n", ref_table_name);
    }else{
        fprintf(stderr, "(catalog.c/get_foreign_key)Table name does not exist : >%s<\n", ref_table_name);
        *result = -1;
        return NULL;
    }

    if(num_of_ref_attr != num_of_f_key_attr){
        fprintf(stderr, "(catalog.c/get_foreign_key) len of foreign key attr does not match len of ref attrs\n");
        *result = -1;
        return NULL;
    }

    struct catalog_table_data* ref_table = sv_ht_get(table_ht, ref_table_name);
    struct hashtable* ref_table_attr_ht = ref_table->attr_ht;
//    printf("ref table name after getting the table is : %s\n", ref_table->table_name);

/* check if f_key_attr match the corresponding ref attr*/
    int attr_match = check_if_attr_matches(num_of_f_key_attr, table_attr_ht, ref_table_attr_ht,
                                           foreign_key_attrs, ref_table_attrs);

    if(attr_match == 0){
        *result = -1;
        return NULL;
    }


    /*Add foreign key*/
    *result = 1;
    struct foreign_key_data* f_key = create_foreign_key(num_of_f_key_attr, ref_table_name,
            foreign_key_attrs, ref_table_attrs);

    return f_key;

}

/**
 * parse the data_str for attributes and store it in foreign_key_attrs
 * @param data_str
 * @param foreign_key_attrs
 * @return a pointer to the remaining part of the string
 */
char* get_foreign_key_attrs(char* data_str, char*** foreign_key_attrs, int* num_of_f_key_attr){

    if(data_str == NULL || data_str[0] == '\0'){
        fprintf(stderr, "(catalog.c/get_foreign_key_attrs) data_str is invalid!!!\n");
        exit(0);
    }

    int start_index = 1;
    int end_index = 0;

    char* ptr = strchr(data_str, ')');
    end_index = ptr - data_str - 1;

    // error checking
    if(end_index < start_index){
        fprintf(stderr, "(catalog.c/get_foreign_key_attrs) end index is less than start index!!!\n");
        exit(0);
    }



    char* foreign_key_attr_str = substring(data_str, start_index, end_index);   // " <attr> <attr> ... <attr> "
    remove_leading_spaces(foreign_key_attr_str);
    remove_ending_spaces(foreign_key_attr_str);

    *num_of_f_key_attr = split(&(*foreign_key_attrs), foreign_key_attr_str, ' ');


    return ptr + 2;  //ptr + 2 points to "references <r_name>( <r_attr> <r_attr> ... <r_attr>)
}


char* get_ref_table_name(char* data_str, char** ref_table_name){
    //data_str should be "references <ref_table_name>( <attr> <attr> ... <attr> )"
    int start_index = 11;
    int end_index = 0;

    char* ptr = strchr(data_str, '(');
    end_index = ptr - data_str - 1;

    *ref_table_name = substring(data_str, start_index, end_index);

    str_lower(*ref_table_name, *ref_table_name, strlen(*ref_table_name));
    printf("ref table_name is : >%s<\n", *ref_table_name);

    return ptr; // ptr should point to "( <attr> <attr> ... <attr> )"
}

char* get_ref_table_attrs(char* data_str, char*** ref_table_attrs, int* num_of_ref_attr){

    if(data_str == NULL || data_str[0] == '\0'){
        fprintf(stderr, "(catalog.c/get_ref_table_attrs) data_str is invalid!!!\n");
        exit(0);
    }

    int start_index = 1;
    int end_index = 0;

    char* ptr = strchr(data_str, ')');
    end_index = ptr - data_str - 1;

    //error checking
    if(end_index < start_index){
        fprintf(stderr, "(catalog.c/get_ref_table_attrs) end index is less than start index!!!\n");
        exit(0);
    }

    char* ref_table_attr_str = substring(data_str, start_index, end_index);
    remove_leading_spaces(ref_table_attr_str);
    remove_ending_spaces(ref_table_attr_str);

    *num_of_ref_attr = split(&(*ref_table_attrs), ref_table_attr_str, ' ');

    return ptr;     // ptr should be pointing to ')';
}


int table_add_child(char* parent_name, char* child_name){
    struct catalog_table_data* parent_table = sv_ht_get(table_ht, parent_name);

    if(parent_table->num_of_childs == parent_table->child_arr_size){
        parent_table->child_arr_size = parent_table->child_arr_size * 2;
        parent_table->childs = realloc(parent_table->childs, sizeof(char*) * parent_table->child_arr_size);
    }

    int count = parent_table->num_of_childs;
    char** childs = parent_table->childs;
    for(int i = 0; i < count; i++){
        if(strncmp(childs[i], child_name, strlen(child_name)) == 0){
            return -1;
        }
    }

    int current_index = parent_table->num_of_childs;
    parent_table->childs[current_index] = malloc(strlen(child_name) + 1);
    strncpy(parent_table->childs[current_index], child_name, strlen(child_name) + 1);

    parent_table->num_of_childs += 1;

    return 0;

}
/**
 * add all the foreign keys to the table
 * @param t_data : a struct that stores information regarding the table
 * @param data_str_arr : a 2d array of strings that needs to be parsed
 * @param data_str_size : the num of strings in the 2d array
 * @return 0 for success
 */
int catalog_add_foreign_keys(struct catalog_table_data* t_data, char** data_str_arr, int data_str_size){
    char* key_word;
    char* ptr;
    enum db_type type;

    int f_keys_size = 12;
    int f_key_count = 0;
    int start_index = 0;
    int end_index = 0;

    struct foreign_key_data** f_keys = malloc(sizeof(struct foreign_key_data*) * f_keys_size);

    for(int i = 0; i < data_str_size; i++){
        ptr = strchr(data_str_arr[i], '(');

        if(ptr != NULL){
            end_index = ptr - data_str_arr[i] - 1;
            key_word = substring(data_str_arr[i], start_index, end_index);

            type = typeof_kw(key_word);
            free(key_word);

            if(type == FOREIGN_KEY){
                //ptr currently points to "( <attr> <attr> ...<attr> ) references <r_name>( <r_attr> <r_attr> ... <r_attr> )"

                int num_of_f_key_attr;
                char** foreign_key_attrs;
                ptr = get_foreign_key_attrs(ptr, &foreign_key_attrs, &num_of_f_key_attr);

                for(int j = 0; j < num_of_f_key_attr; j++){
                    printf("f_key attr : %s\n", foreign_key_attrs[j]);
                }

                char* ref_table_name;
                ptr = get_ref_table_name(ptr, &ref_table_name);
//                printf("ref table name is : >%s<\n", ref_table_name);

                int num_of_ref_attr;
                char** ref_table_attrs;         // the attrs that the user specified that should exist in the ref table
                ptr = get_ref_table_attrs(ptr, &ref_table_attrs, &num_of_ref_attr);

                for(int j = 0; j < num_of_ref_attr; j++){
                    printf("ref attr : %s\n", ref_table_attrs[j]);
                }

                int get_f_key_result;
                struct foreign_key_data* f_key = get_foreign_key(num_of_f_key_attr, num_of_ref_attr,
                        foreign_key_attrs, ref_table_attrs, ref_table_name, t_data->attr_ht, &get_f_key_result);

                printf("result of getting foreign key is: %d\n", get_f_key_result);
                if(get_f_key_result){
                    if(f_key == NULL){
                        fprintf(stderr, "(catalog.c/catalog_add_foreign_keys) f_key expected to be not null!!!\n");
                        exit(0);
                    }
                    if(f_key_count == f_keys_size){
                        f_keys_size = f_keys_size * 2;
                        f_keys = realloc(f_keys, sizeof(struct foreign_key_data*) * f_keys_size);
                    }

                    f_keys[f_key_count] = f_key;
                    f_key_count++;
                    table_add_child(ref_table_name, t_data->table_name);
                }else{
                    fprintf(stderr, "(catalog.c/catalog_add_foreign_keys) Getting foreign key failed!!!\n ");
                }


            }
        }
    }

    t_data->num_of_f_key = f_key_count;
    t_data->f_keys = f_keys;
    return 0;
}
int catalog_get_next_table_num(){
    return table_ht->size;
}


int catalog_add_table_to_storage_manager(struct catalog_table_data* t_data){

    int num_of_attrs = t_data->attr_ht->size;
    int p_key_len = t_data->p_key_len;

    int* data_types = malloc(sizeof(int)*num_of_attrs);
    int* key_indices = malloc(sizeof(int)*p_key_len);

    struct hashtable* attr_ht = t_data->attr_ht;
    struct ht_node** attr_nodes = attr_ht->node_list;

    for(int i = 0; i < num_of_attrs; i++){

        struct attr_data* a_data = attr_nodes[i]->value->v_ptr;
//        printf("attr name is : >%s<\n", a_data->attr_name);
        enum db_type type = a_data->type;
        int type_int_val = (int)type;

        data_types[i] = type_int_val;

//        int num_of_constr = a_data->
    }

//    printf("primary key len is : %d\n", p_key_len);
    char** primary_key_attrs = t_data->primary_key_attrs;

    for(int i = 0; i < p_key_len; i++){
        struct attr_data* a_data = sv_ht_get(attr_ht, primary_key_attrs[i]);
        enum db_type type = a_data->type;
        int type_int_val = (int)type;

        key_indices[i] = type_int_val;
    }

    add_table(data_types, key_indices, num_of_attrs, p_key_len);
}

int catalog_add_table(int table_num, char* table_name, char* data_str){

    int table_exist = sv_ht_contains(table_ht, table_name);
    if(table_exist){
        fprintf(stderr, "(catalog.c/catalog_add_table) Table already exists !!!\n");
        return -1;
    }

    struct catalog_table_data* t_data = malloc(sizeof(struct catalog_table_data));
    t_data->table_num = table_num;

    int table_name_len = strlen(table_name);
    t_data->table_name = malloc(sizeof(table_name_len));
    strncpy(t_data->table_name, table_name, table_name_len + 1);

    t_data->num_of_childs = 0;
    t_data->child_arr_size = 12;
    t_data->childs = malloc(sizeof(char*) * t_data->child_arr_size);

    char** data_str_arr;
    int count = split(&data_str_arr, data_str, ',');

    /*Add the attributes*/
    catalog_add_attributes(t_data, data_str_arr, count);

    /*Add the primary key*/
    catalog_add_primary_key(t_data, data_str_arr, count);

    /*Add the foreign keys*/
    catalog_add_foreign_keys(t_data, data_str_arr, count);

    sv_ht_add(table_ht, table_name, t_data);

//    catalog_add_table_to_storage_manager(t_data);

    free_2d_char(data_str_arr, count);
    return 0;

}



int catalog_remove_table(char* table_name){

    int table_exist = sv_ht_get(table_ht, table_name);
    if(table_exist==0){
        fprintf(stderr, "(catalog.c/catalog_remove_table) Table does not exist or it has been removed!!!\n");
        return -1;
    }

    struct catalog_table_data* t_data = sv_ht_remove(table_ht, table_name);
    drop_table(t_data->table_num);
    //handing the removal of foreign keys will be done.
    return 0;
}

void catalog_print_foreign_key(int count, struct foreign_key_data** f_keys){

    printf("foreign Keys:\n");
    if(count == 0){
        printf("(No foreign keys)\n");
    }
    for(int i = 0; i < count; i++){
        char* referenced_table_name = f_keys[i]->parent_table_name;


        struct hashtable* f_keys_mapping = f_keys[i]->f_keys;
        int num_of_mapping = f_keys_mapping->size;
        struct ht_node** f_key_attrs = f_keys_mapping->node_list;


        printf("(foreign_key_%d) ref_table=%s", i, referenced_table_name);

        for(int j = 0; j < num_of_mapping; j++){
            printf(", %s=%s", f_key_attrs[i]->key, (char*)(f_key_attrs[i]->value->v_ptr));
        }

        printf("\n");

    }
}

void catalog_print_childs(int count, char** childs){
    printf("childs: [");
    for(int i = 0; i < count; i++){
        printf("%s, ", childs[i]);
    }
    printf("]\n");
}

void catalog_print_tables(){

    struct ht_node** node_list = table_ht->node_list;
    for(int i = 0; i < table_ht->size; i++){
        struct catalog_table_data* t_data = node_list[i]->value->v_ptr;
        printf("Table:%d - %s\n", t_data->table_num, t_data->table_name);

        catalog_print_childs(t_data->num_of_childs, t_data->childs);
        //print the attributes and its constraints
        struct ht_node** attr_node_list = t_data->attr_ht->node_list;
        int num_of_attr = t_data->attr_ht->size;

        for(int j = 0; j < num_of_attr; j++){
            struct attr_data* a_data = attr_node_list[j]->value->v_ptr;
            char* type_str = type_to_str(a_data->type);
            int constr_count = a_data->num_of_constr;

            printf("a_name=%s, a_type=%s, a_size=%d, a_constr_count=%d, constraints=[ ", a_data->attr_name,
                   type_str, a_data->attr_size, a_data->num_of_constr);


            for(int k = 0; k < constr_count; k++){
                struct attr_constraint* a_constr = a_data->constr[k];
                char* constr_type_str = type_to_str(a_constr->type);

                if(a_constr->type == DEFAULT){
                    switch(a_data->type){
                        case INT:
                            printf("%s(%d), ", constr_type_str, a_constr->value->i);
                            break;
                        case DOUBLE:
                            printf("%s(%lf), ", constr_type_str, a_constr->value->d);
                            break;
                        case BOOL:
                            if(a_constr->value->b == true){
                                printf("%s(true), ", constr_type_str);
                            }else{
                                printf("%s(false), ", constr_type_str);
                            }
                            break;
                        case CHAR:
                            printf("%s(%s), ", constr_type_str, a_constr->value->c);
                            break;
                        case VARCHAR:
                            printf("%s(%s), ", constr_type_str, a_constr->value->v);
                            break;
                        default:
                            fprintf(stderr, "(catalog.c/catalog_print_table) Got an unexpected constraint type!!!\n");
                            exit(0);
                    }
                }else{
                    printf("%s, ", constr_type_str);
                }

            }
            printf("]\n");

            /*print the primary key*/
        }

        printf("primary_key=[ ");
        for(int j = 0; j < t_data->p_key_len; j++){
            printf("%s, ", t_data->primary_key_attrs[j]);
        }
        printf("]\n");

        /*Print the foreign keys*/
        catalog_print_foreign_key(t_data->num_of_f_key, t_data->f_keys);



        printf("\n\n");
    }
}






///*
// *  Private function predefintion
// */
//int init_mapping(int capacity, bool catalog_exist);
//int init_table_names_list();
//int table_map_resize();
//int table_reorder(int removed_table_num);
//int read_catalog();
//void free_attr_ht_node(struct ht_node* n);
//void free_attr_ht(struct hashtable* ht);
//void free_non_deep_ht_node(struct ht_node* n);
//
//
//
///*
// * db_path contains a ending '/'
// */

//
//
//
//int init_mapping(int capacity, bool catalog_exist){
//
//    if(catalog_exist==true){
//        read_catalog();
//    }else{
//        table_ht = ht_create(capacity, 0.75);
//
//    }
//    return 0;
//}
//
//
///**
// * returns the table num
// * @param table_name
// * @return returns table num if the table exist and returns -1 if the table doesn't exist
// */
//int catalog_get_table_num(char* table_name){
//
//    struct table_data* t_data = sk_ht_func->get(table_ht, table_name);
//
//    if(t_data != NULL){
//        return t_data->table_num;
//    }
//
//    return -1;
//}
//
///*
// *Checks to see if table_name is already in the table_map
// *
// * returns: 1 if exist and 0 if it does not exist;
// */
//
//int catalog_table_mapping_contains(char* table_name){
//
//    int result = sk_ht_func->contains(table_ht, table_name);
//    return result;
//}
//
//
//int catalog_table_mapping_add(char* table_name, int table_num, char** attr_names, int** attr_int_vals,
//                              char** foreign_keys){
//
//    struct table_data* t_data = (struct table_data*)malloc(sizeof(struct table_data));
//    t_data->table_num = table_num;
//    t_data->attr_ht = ht_create(10, 0.75);
//    t_data->foreign_key_ht = ht_create(10,0.75);
//    t_data->primary_keys_ht = ht_create(10, 0.75);
//
//    sk_ht_func->add(table_ht, table_name, t_data);
//    return 0;
//
//}
//
//int catalog_table_mapping_remove(char* table_name){
//
//     struct table_data* t_data = sk_ht_func->remove(table_ht, table_name);
//
//    //clean up by freeing up the memory
//    free_attr_ht(t_data->attr_ht);
//    sk_ht_func->close(t_data->foreign_key_ht);
//    ik_ht_func->close(t_data->primary_keys_ht);
//
//    free(t_data);
//
//    return 0;
//}
//
//
//int read_catalog(){
//    //do reading
////    int map_size;
////    int capacity;
////    int base_multiplier;
////    int size;
////
////    FILE* catalog_file = fopen(catalog_loc, "rb");
////
////    fread(&capacity, sizeof(int), 1, catalog_file);
////    fread(&base_multiplier, sizeof(int), 1, catalog_file);
////    fread(&map_size, sizeof(int), 1, catalog_file);
////
////    //initialize the table_map
////    ht = ht_create(capacity, 0.75);
//////    table_map = (struct i_node**)malloc(sizeof(struct i_node*)*table_map_capacity);
////
////    //initialize the table_name arr
//////    init_table_names_list();
////
//////    for(int i = 0; i <table_map_capacity; i++){
//////        table_map[i] = NULL;
//////    }
////
////    //read all entries
////    //format: <len of table name> <table_name> <table_num>
////    char buffer[255];
////    buffer[0] = 0;
//////    printf("size of map is : %d\n", map_size);
////    for(int i = 0; i < map_size; i++){
////        int table_name_len;
////        int table_num;
////        fread(&table_name_len, sizeof(int), 1, catalog_file);
////        fread(buffer, sizeof(char), table_name_len, catalog_file);
////        buffer[table_name_len]=0;
////        fread(&table_num, sizeof(int), 1, catalog_file);
////
////        catalog_table_mapping_add(buffer,table_num);
//////        printf("name is: %s\n", buffer);
////        clear_buffer(buffer, table_name_len+2);
////    }
////
////    fclose(catalog_file);
//    return 0;
//}
//
//int write_catalog(){
////    //do writing
////    FILE* catalog_file = fopen(catalog_loc, "wb");
////
////    //write table_map_capacity
////    fwrite(&table_map_capacity, sizeof(int), 1, catalog_file);
////    //write table_map_multiplier
////    fwrite(&table_map_multiplier, sizeof(int), 1, catalog_file);
////    //write table_map_size
////    fwrite(&table_map_size, sizeof(int), 1, catalog_file);
////
////    //write all the mapping pairs to the file
////    //format: <len of table name> <table_name> <table_num>
////    for(int i = 0; i < table_map_capacity; i++){
////        if(table_map[i]!=NULL){
////            struct i_node* curr = table_map[i];
////            while(curr!=NULL){
////                int table_name_length = strlen(curr->key);
////                int table_num = curr->value;
////                fwrite(&table_name_length, sizeof(int), 1, catalog_file);
////                fwrite(curr->key, sizeof(char), table_name_length, catalog_file);
////                fwrite(&table_num, sizeof(int), 1, catalog_file);
////                curr = curr->next_node;
////            }
////        }
////    }
////    fclose(catalog_file);
//    return 0;
//}
//
//void catalog_recur_free_i_node(struct i_node* n){
//    if(n != NULL){
//        catalog_recur_free_i_node(n->next_node);
//        free(n->key);
//        free(n->hash_code_str);
//        free(n);
//        n=NULL;
//    }
//}
//
//
//
//void free_attr_ht_node(struct ht_node* n){
//    if(n!=NULL){
//        free_attr_ht_node(n->next_node);        //recursively free
//        free(n->key);
//        free(n->hash_code);
//        struct attr_data* val = n->value->v_ptr;
//        free(val->constraints);
//        free(val);
//        free(n->value);
//        free(n);
//        n = NULL;
//    }
//}
//
//void free_attr_ht(struct hashtable* ht){
//    if(ht){
//        for(int i = 0; i < ht->capacity; i++){
//            struct ht_node* curr = ht->table[i];
//            if(curr!=NULL){
//                free_attr_ht_node(curr);
//            }
//        }
//        free(ht->table);
//        free(ht->keys);
//        free(ht);
//    }
//}
//
//
//void free_non_deep_ht_node(struct ht_node* n){
//    if(n!=NULL){
//        free_non_deep_ht_node(n->next_node);        //recursively free
//        free(n->key);
//        free(n->hash_code);
//        free(n->value);
//        free(n);
//    }
//}
//
///**
// * frees hashtable that don't have a value to be freed
// * @param ht
// * @return
// */
//int free_non_deep_ht(struct hashtable* ht){
//    if(ht){
//        for(int i = 0; i < ht->capacity; i++){
//            struct ht_node* curr = ht->table[i];
//            if(curr!=NULL){
//                free_non_deep_ht_node(curr);
//            }
//        }
//        free(ht->table);
//        free(ht->keys);
//        free(ht);
//    }
//}
//
//int catalog_close(){
//    //write everything to storage and then free everything
////    write_catalog();
////    catalog_free_table_map();
////    free(table_names);
////    free(catalog_loc);
//
////    printf("catalog closed...\n");
//
//    return 0;
//}
//
//
///*
// * Below are functions used to help with debugging catalog
// */
//
//void catalog_print_table_map_content(){
//
////    if(table_map_size == 0){
////        printf("Table_Map's Content: {}\n");
////        return;
////    }
////    printf("Table_Map's Content: {\n");
////    for(int i = 0; i < table_map_capacity; i++){
////        if(table_map[i]!=NULL){
////            struct i_node* curr = table_map[i];
////            while(curr!=NULL){
////                printf("\tindex : %d, Key : %s, Value: %d\n", i, curr->key, curr->value);
////                curr = curr->next_node;
////            }
////        }
////    }
//    printf("}\n");
//}
//
//void catalog_print_table_map_info(){
////    printf("size : %d, capacity : %d, multiplier : %d,  load_factor : %lf, threshold : %lf\n",
////           table_map_size, table_map_capacity, table_map_multiplier, load_factor, ceil(table_map_capacity*load_factor));
//
//}
//
//void catalog_print_table_names(){
////    if(table_map_size == 0){
////        printf("Table_name's content = {}\n");
////    }
////    printf("Table_names's Content: {\n");
////    for(int i = 0; i < table_map_size; i++){
////        printf("table_num: %d, table_name: %s\n", i+1, table_names[i]);
////    }
////    printf("}\n");
//
//
//}
//
//void catalog_test_print(){
//    printf("test");
//}


