/**
 * Note: Case does not matter
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "../../include/catalog/catalog_io.h"
#include "../../include/catalog/catalog.h"
#include "../../include/catalog/catalog_clean_up.h"
#include "../../include/catalog/catalog_structs.h"
#include "../../include/helper_module/helper_function.h"
#include "../../include/db_types.h"
#include "../../include/hash_collection/hash_collection.h"
#include "../../include/hash_collection/hash_table.h"
#include "../../include/hash_collection/si_ht.h"
#include "../../include/hash_collection/sv_ht.h"
#include "../../include/storagemanager.h"
#include "../../include/file_sys/file_sys.h"

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

    printf("catalog loc is : %s\n", catalog_loc);

    if(file_exist(catalog_loc)){
        //catalog exist
        printf("catalog exist\n");
        read_catalog(catalog_loc, &table_ht);
    }else{
        //catalog does not exist
        printf("Catalog does not exist : %s\n", catalog_loc);
        table_ht = ht_create(12, 0.75);
    }

    return 0;
}

int catalog_update_table_child(char* parent_t_name, char* child_t_name){
    char func_loc_str[] = "(catalog.c/catalog_update_table_child)";
    printf("%s \"%s\"\n", "...Updating parent table", parent_t_name);

    if(sv_ht_contains(table_ht, parent_t_name)==0){
        printf("%s \"%s\"\n", "...Cannot find parent table", parent_t_name);
        return -1;
    }

    struct catalog_table_data* t_data = sv_ht_get(table_ht, parent_t_name);

    if(t_data->num_of_childs > 0){
        if(t_data->num_of_childs == t_data->child_arr_size){
            printf("%s %s\n", func_loc_str, "...Resizing child table arr.");
            t_data->child_arr_size = t_data->child_arr_size * 2;
            t_data->childs = realloc(t_data->childs, sizeof(char*) * t_data->child_arr_size);
            printf("%s %s\n", func_loc_str, "...Resized child table arr.");
        }
        t_data->childs[t_data->num_of_childs] = strdup(child_t_name);
        t_data->num_of_childs += 1;
    }else if(t_data->num_of_childs == 0){
        t_data->child_arr_size = 6;
        t_data->childs = malloc(sizeof(char*) * t_data->child_arr_size);
        t_data->childs[0] = strdup(child_t_name);
        t_data->num_of_childs += 1;
    }else{
        return -1;
    }

    return 0;
}

int catalog_add_table(struct catalog_table_data* t_data){
    char func_loc_str[] = "(catalog.c/catalog_add_table)";
    printf("%s %s\n", func_loc_str, "Adding table meta data to catalog.c.");
    if(sv_ht_contains(table_ht, t_data->table_name)){
        printf("%s %s \"%s\" %s\n", func_loc_str, "Table", t_data->table_name, "already exist.");
        return -1;
    }

    /* Check if the table has a foreign key and update existing table as
     * necessary
     */
    printf("%s %s\n", func_loc_str, "Checking for foreign keys.");
    if(t_data->num_of_f_key > 0){
        printf("%s %s (%d).\n", func_loc_str, "Foreign key exists", t_data->num_of_f_key);
        printf("%s %s\n", func_loc_str, "Getting foreign keys");

        for(int i = 0; i < t_data->num_of_f_key; i++){
            struct foreign_key_data* f_key_data = t_data->f_keys[i];
            printf("...Obtained foreign key that refers to table \"%s\".\n", f_key_data->parent_table_name);
        }

        for(int i = 0; i < t_data->num_of_f_key; i++){
            struct foreign_key_data* f_key_data = t_data->f_keys[i];

            int update_err = catalog_update_table_child(f_key_data->parent_table_name, t_data->table_name);
            if(update_err == -1){
                printf("%s \"%s\"\n","...Cannot update parent table", f_key_data->parent_table_name);
                return -1;
            }

            printf("...Updated parent table \"%s\".\n", f_key_data->parent_table_name);
        }
    }


    printf("%s %s\n", func_loc_str, "Successfully added table meta data to catalog.c.");
    return sv_ht_add(table_ht, t_data->table_name, t_data);

}

int catalog_get_table_num(char* table_name){
    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);
    return t_data->table_num;
}

struct catalog_table_data* catalog_get_table_metadata(char* table_name){
    return sv_ht_get(table_ht, table_name);
}

/**
 * Remove all the foreign key attribute reference to the parent table
 * @param t_data : child table data
 * @param f_key_attr_ht : a hashtable that contains foreign key attr and primary key attr pairs.
 * @return 0 for success, -1 for failed
 */
int catalog_remove_table_attr_ref(struct catalog_table_data* t_data, struct hashtable* f_key_attr_ht){
    char func_loc_str[] = "(catalog.c/catalog_remove_table_attr_ref)";

    if(f_key_attr_ht == NULL){
        printf("%s %s\n", func_loc_str, "Error: f_key_attr_ht is NULL.");
        return -1;
    }

    sv_ht_print(t_data->attr_ht);

    printf("%s %s \"%s\"\n", func_loc_str, "Removing foreign attribute references for table", t_data->table_name);

    struct ht_node** val_nodes = f_key_attr_ht->node_list;
    for(int i = 0; i < f_key_attr_ht->size; i++){
        char* a_name = val_nodes[i]->key;
        printf("...Removing attribute \"%s\"\n", a_name);
        struct attr_data* a_data = sv_ht_remove(t_data->attr_ht, a_name);
        printf("...Removed attribute \"%s\"\n", a_data->attr_name);
    }
    printf("%s %s \"%s\"\n", func_loc_str, "Finished removing foreign attribute references for table", t_data->table_name);

    return 0;

}

/**
 * Remove all the foreign key reference to the parent table
 * @param parent_table_name
 * @param num_of_childs
 * @param childs : an array that contains the child table names
 * @return 0 for success and -1 for failure
 */
int catalog_remove_table_ref(char* parent_table_name){
    char func_loc_str[] = "(catalog.c/catalog_remove_table_ref)";

    printf("%s %s \"%s\" %s.\n", func_loc_str, "Verifying that table", parent_table_name, "exists");
    if(sv_ht_contains(table_ht, parent_table_name) == 0){
        printf("%s %s \"%s\".\n", func_loc_str, "Cannot find table", parent_table_name);
        printf("%s %s \"%s\" %s.\n", func_loc_str, "Cannot remove table", parent_table_name,
               "references");
        return -1;
    }
    printf("%s %s \"%s\" %s.\n", func_loc_str, "Verified that  table", parent_table_name, "exists");

    struct catalog_table_data* parent_table_data = sv_ht_get(table_ht, parent_table_name);

    int num_of_childs = parent_table_data->num_of_childs;
    if(num_of_childs == 0){
        printf("%s %s \"%s\" %s.\n", func_loc_str, "Table", parent_table_name, "does not have any childs");
        printf("%s %s \"%s\" %s.\n", func_loc_str, "Removal of table", parent_table_name, "references is successful");
        return 0;
    }

    char** childs = parent_table_data->childs;
    if(childs == NULL){
        printf("%s %s \"%s\".\n", func_loc_str, "Child table array is empty for table", parent_table_name);
        printf("%s %s \"%s\" %s.\n", func_loc_str, "Removal of table", parent_table_name, "references failed");
        return -1;
    }

    printf("%s %s \"%s\" %s.\n", func_loc_str, "Removing table", parent_table_name, "references");

    for(int i = 0; i < num_of_childs; i++){
        if(sv_ht_contains(table_ht, childs[i]) == 0){
            printf("%s \"%s\"\n", "Cannot find child table", parent_table_name);
            printf("%s \"%s\" %s.\n", "Removal of table", parent_table_name, "references failed");
            printf("%s\n", "Removal attempt have corrupted the database");
            printf("%s\n", "Shutting down database without saving");
            exit(0);
        }

        struct catalog_table_data* t_data = sv_ht_get(table_ht, childs[i]);

        printf("%s \"%s\"\n", "...Removing foreign key in child table", t_data->table_name);

        int f_key_count = t_data->num_of_f_key;
        if(f_key_count == 0){
            printf("%s.\n", "...Unexpect error: child table does not contain any foreign keys");
            printf("%s.\n", "...Removal attempt have corrupted the database");
            printf("%s.\n", "...Shutting down database without saving");
            exit(0);
        }

        struct foreign_key_data** f_key_arr = t_data->f_keys;
        if(f_key_arr == NULL){
            printf("%s.\n", "...Unexpected error: child table");
            printf("%s \"%s\" %s.\n", "...Removal of table", parent_table_name, "references failed");
            return -1;
        }

        if(f_key_count == 1){

            catalog_remove_table_attr_ref(t_data, f_key_arr[0]->f_keys);

            printf("...Freeing foreign key for child table\n");

            int err_code = catalog_free_f_key(f_key_arr[0]);
            if(err_code == -1){
                printf("...Cannot free foreign key for child table\n");
                return -1;
            }

            printf("...Freed foreign key for child table\n");
            f_key_arr[0] = NULL;

        }else if(f_key_count > 1){

            for(int j = 0; j < f_key_count; j++){
                if(strncmp(f_key_arr[i]->parent_table_name, parent_table_name, strlen(parent_table_name) ) == 0){
                    printf("...Freeing foreign key for child table\n");

                    int err_code = catalog_free_f_key(f_key_arr[i]);
                    if(err_code == -1){
                        printf("...Cannot free foreign key for child table\n");
                        return -1;
                    }

                    printf("...Freed foreign key for child table\n");
                    // place the last foreign_key in the removed spot
                    f_key_arr[i] = f_key_arr[f_key_count-1];
                    f_key_arr[f_key_count-1] = NULL;
                }
            }
        }
        t_data->num_of_f_key -= 1;
    }
    printf("%s %s \"%s\" %s.\n", func_loc_str, "Successfully removed table", parent_table_name, "references");

    return 0;

}

int catalog_table_drop_child(char* parent_table_name, char* child_table_name){
    char func_loc_str[] = "(catalog.c/catalog_table_drop_child)";

    struct catalog_table_data* parent_table = sv_ht_get(table_ht, parent_table_name);
    if(parent_table_name == NULL){
        printf("%s %s \"%s\"\n", func_loc_str, "Unexpected Error: Cannot get parent table",
               parent_table_name);
        printf("%s %s\n", func_loc_str, "Exiting database");
        exit(0);
    }
    if(parent_table->num_of_childs == 0){
        printf("%s %s \"%s\"\n", func_loc_str, "Unexpected Error: No child exist for parent table",
               parent_table_name);
        printf("%s %s\n", func_loc_str, "Exiting database");
        exit(0);
    }else if(parent_table->num_of_childs > 0){
        if(parent_table->childs == NULL){
            printf("%s %s \"%s\" %s\n", func_loc_str,
                   "Unexpected Error: Expect parent table",
                   parent_table_name, "to have a child but the child array is NULL");
            printf("%s %s\n", func_loc_str, "Exiting database");
            exit(0);
        }
    }


    char** child_arr = parent_table->childs;

    printf("%s %s \"%s\" \n", func_loc_str, "Finding the index of the child table",
           child_table_name);

    int index_of_child = -1;
    for(int i = 0; i < parent_table->num_of_childs; i++){
        if(strncmp(child_arr[i], child_table_name, strlen(child_table_name)) == 0){
            index_of_child = i;
            break;
        }
    }

    if(index_of_child == -1){
        printf("%s %s \"%s\" \n", func_loc_str, "Index not found for the child table",
               child_table_name);
        printf("%s %s\n", func_loc_str, "Exiting database");
        exit(0);
    }

    printf("%s %s \"%s\" \n", func_loc_str, "Found the index of the child table",
           child_table_name);

    printf("%s %s \"%s\" %s \"%s\"\n", func_loc_str, "Dropping child table",
           child_table_name, "from parent table", parent_table_name);

    if( (parent_table->num_of_childs == 1) || (parent_table->num_of_childs == parent_table->child_arr_size)){
        free(child_arr[index_of_child]);
        child_arr[index_of_child] = NULL;
    }else{
        free(child_arr[index_of_child]);
        int last_child_index = parent_table->child_arr_size - 1;
        child_arr[index_of_child] = child_arr[last_child_index];
        child_arr[last_child_index] = NULL;
    }
    parent_table->num_of_childs -= 1;
    printf("%s %s \"%s\" %s \"%s\"\n", func_loc_str, "Successfully dropped child table",
           child_table_name, "from parent table", parent_table_name);

    return 0;

}

int catalog_rm_table_from_child_list(char* table_name){
    char func_loc_str[] = "(catalog.c/catalog_rm_table_from_child_list)";

    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);

    if(t_data->num_of_f_key == 0){
        printf("%s %s \"%s\" %s\n", func_loc_str, "Table", t_data->table_name,
               "does not have any foreign keys. No child dropped\n");
        return 0;
    }else if(t_data->num_of_f_key > 0){
        if(t_data->f_keys == NULL){
            printf("%s %s \"%s\" %s\n", func_loc_str, "Unexpected error: Table", t_data->table_name,
                   "expected to have foreign keys but the foreign key array is NULL\n");
            printf("%s %s\n", func_loc_str, "Exiting database");
            exit(0);
        }else{
            printf("%s %s \"%s\" %s\n", func_loc_str, "Dropping child table",
                   t_data->table_name, "in all its parent table");
            struct foreign_key_data** f_key_arr = t_data->f_keys;
            for(int i = 0; i < t_data->num_of_f_key; i++){
                catalog_table_drop_child(f_key_arr[i]->parent_table_name, t_data->table_name);
            }
            printf("%s %s \"%s\" %s\n", func_loc_str, "Successfully dropped child table",
                   t_data->table_name, "from all its parent table");
        }

        return 0;
    }

    return -1;


//    printf("%s ")
}

int catalog_remove_table(char* table_name){
    char func_str[] = "(catalog.c/catalog_remove_table)";

    printf("%s %s \"%s\"\n", func_str, "Looking for table", table_name);
    int table_exist = sv_ht_contains(table_ht, table_name);
    if(table_exist==0){
        printf("%s %s \"%s\" \n", func_str, "Cannot find table", table_name);
        return -1;
    }
    printf("%s %s \"%s\"\n", func_str, "Found table", table_name);

    int child_drop_err = catalog_rm_table_from_child_list(table_name);
    if(child_drop_err == -1){
        printf("%s %s \"%s\" %s\n", func_str, "Attempted to drop child table",
               table_name, "but failed");
        printf("%s %s\n", func_str, "Exiting database");
        exit(0);
    }

    int ref_removal_err = catalog_remove_table_ref(table_name);
    if(ref_removal_err == -1){
        printf("%s %s \"%s\"\n", func_str, "Cannot remove table", table_name);
        return -1;
    }


    printf("%s %s \"%s\"\n", func_str, "Removing table", table_name);
    int list_index = ht_list_index_of(table_ht, table_name);
    if(list_index == -1){
        fprintf(stderr, "%s %s \"%s\"\n", func_str, "Cannot get the list index for table:", table_name);
    }else{
        printf("%s %s \"%s\" %s %d\n", func_str, "List index for table", table_name,
               "is:", list_index);
    }


    if((table_ht->size == 1) || (list_index == (table_ht->size) - 1)){
        struct catalog_table_data* t_data = sv_ht_remove(table_ht, table_name);
    }else{
        sv_ht_print(table_ht);
        struct ht_node** val_nodes = table_ht->node_list;

        if(val_nodes == NULL){
            fprintf(stderr, "%s %s\n", func_str, "Cannot obtain node_list from table_ht.");
        }else{
            printf("%s %s\n", func_str, "Obtained node_list.");
            printf("%s %s %d\n", func_str, "Size of node_list:", table_ht->size);
        }

        struct catalog_table_data* t_data = sv_ht_remove(table_ht, table_name);
        printf("%s %s \"%s\"\n", func_str, "Removed table:", t_data->table_name);
        int removed_table_num = t_data->table_num;

        printf("%s %s %d\n", func_str, "Index of removal:", list_index);
        struct catalog_table_data* replaced_table = val_nodes[list_index]->value->v_ptr;

        if(replaced_table == NULL){
            fprintf(stderr, "%s %s\n", func_str, "Cannot obtain replacement table data from node_list.");
        }else{
            printf("%s %s \"%s\"\n", func_str, "Obtained replacement table data for table:", replaced_table->table_name);
        }


        replaced_table->table_num = removed_table_num;
        printf("%s %s \"%s\"\n", func_str, "Cleaned up removal of table:", t_data->table_name);

    }

    printf("%s %s \"%s\"\n", func_str, "Removed confirmed for table:", table_name);
    return 0;

//    struct catalog_table_data* t_data = sv_ht_remove(table_ht, table_name);
//    drop_table(t_data->table_num);
    //handing the removal of foreign keys will be done.
}

int catalog_contains(char* table_name){
    int exist = sv_ht_contains(table_ht, table_name);
    return exist;
}


int catalog_get_data_types(struct catalog_table_data* t_data, int** data_types){
    int num_of_attrs = t_data->attr_ht->size;
    *data_types = malloc(sizeof(int) * num_of_attrs);

    struct ht_node** val_nodes = t_data->attr_ht->node_list;
    for(int i = 0; i < num_of_attrs; i++){
        struct attr_data* a_data = val_nodes[i]->value->v_ptr;
        enum db_type a_type = a_data->type;
        int type_int_val = (int)a_type;

        (*data_types)[i] = type_int_val;
    }
    return num_of_attrs;
}



int catalog_get_p_key_indices(struct catalog_table_data* t_data, int** p_key_indices){
    char func_str[] = "(catalog.c/catalog_get_p_key_indices)";
    int p_key_len = t_data->p_key_len;
    *p_key_indices = malloc(sizeof(int) * p_key_len);

    struct hashtable* attr_ht = t_data->attr_ht;
    char** p_key_attrs = t_data->primary_key_attrs;

    printf("%s %s\n", func_str, "Printing p_key_attrs");
    for(int i = 0; i < p_key_len; i++){
        printf("%s %d %s\"%s\"\n", "...p_key_attr", i, " = ", p_key_attrs[i]);
    }

    if(attr_ht == NULL){
        fprintf(stderr, "%s %s\n", "(catalog.c/catalog_get_p_key_indices)",
                "attr_ht is NULL.");
        return -1;
    }else{
        sv_ht_print(attr_ht);
    }


    for(int i = 0; i < p_key_len; i++){
        struct attr_data* a_data = sv_ht_get(attr_ht, p_key_attrs[i]);

        if(a_data == NULL){
            printf("%s %s\n", "(catalog.c/catalog_get_p_key_indices)",
                    "Cannot get attribute from attr_ht.");
            return -1;
        }


        (*p_key_indices)[i] = a_data->index;
    }
    return p_key_len;
}

int catalog_get_foreign_key_indices(char* table_name, int*** foreign_key_indices, int** foreign_key_lens){
    char func_loc_str[] = "(catalog.c/catalog_get_foreign_key_indices)";
    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);
    struct hashtable* attr_ht = t_data->attr_ht;

    int foreign_key_count = t_data->num_of_f_key;
    if(foreign_key_count == 0){
        return 0;
    }

    struct foreign_key_data** f_keys = t_data->f_keys;
    if(f_keys == NULL){
        printf("Error: Expected atleast one foreign key but foreign key arr is NULL. %s\n",
               func_loc_str);
        exit(0);
    }

    *foreign_key_indices = malloc(sizeof(int*) * foreign_key_count);
    *foreign_key_lens = malloc(sizeof(int) * foreign_key_count);

    for(int i = 0; i < foreign_key_count; i++){
        struct foreign_key_data* foreign_key_data = f_keys[i];
        struct hashtable* foreign_key_ht = foreign_key_data->f_keys;
        struct ht_node** val_nodes = foreign_key_ht->node_list;

        int foreign_key_len = foreign_key_ht->size;
        (*foreign_key_lens)[i] = foreign_key_len;
        (*foreign_key_indices)[i] = malloc(sizeof(int) * foreign_key_len);

        for(int j = 0; j < foreign_key_ht->size; j++){
            char* foreign_attr_name = val_nodes[i]->key;
            struct attr_data* a_data = sv_ht_get(attr_ht, foreign_attr_name);

            int attr_index = a_data->index;
            (*foreign_key_indices)[i][j] = attr_index;
        }

    }

    return foreign_key_count;
}

int catalog_get_attr_types(char* table_name, enum db_type** attr_types){
    char func_loc_str[] = "(catalog.c/catalog_get_attr_types)";

    /* Verify that the table exist */
    if(catalog_contains(table_name) == 0){
        printf("%s Error: Table \"%s\" does not exist\n", func_loc_str, table_name);
        return -1;
    }

    /* Get the table meta data */
    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);
    if(t_data == NULL){
        printf("%s Unexpected Error: Cannot get table \"%s\"\n", func_loc_str, table_name);
        return -1;
    }

    /* Get the hashtable that stores the attribute */
    struct hashtable* attr_ht = t_data->attr_ht;
    if(attr_ht == NULL){
        printf("%s Unexpected Error: Cannot get attr_ht (a hashtable that stores attribute) for table \"%s\"\n", func_loc_str, table_name);
        return -1;
    }

    int arr_size = attr_ht->size;
    if(arr_size <= 0){
        printf("%s Unexpected Error: There exist no attributes for table \"%s\"\n", func_loc_str, table_name);
        return -1;
    }

    /* Allocate memory for the array that is storing the attribute types */
    *attr_types = malloc(sizeof(enum db_type) * arr_size);

    /* Get all the attribute value nodes from the attribute hashtable */
    struct ht_node** val_nodes = attr_ht->node_list;
    if(val_nodes == NULL){
        printf("%s Unexpected Error: attr_ht->node_list is NULL for table \"%s\"\n", func_loc_str, table_name);
        return -1;
    }


    /* Add all the attribute types to the attr_types array in order of insertion */
    for(int i = 0; i < arr_size; i++){
        struct attr_data* a_data = val_nodes[i]->value->v_ptr;
        if(a_data == NULL){
            printf("%s Unexpected Error: Cannot retrieve attribute data from attribute node for table \"%s\"\n", func_loc_str, table_name);

            /* Free the array that was allocated */
            free(*attr_types);
            return -1;
        }
        (*attr_types)[i] = a_data->type;
    }

    return arr_size;
}

int catalog_get_attr_data(char* table_name, struct attr_data*** attr_data_arr){
    char func_loc_str[] = "(catalog.c/catalog_get_attr_data)";
    if(catalog_contains(table_name) != 1){
        printf("Error: Table \"%s\" does not exist. %s\n", table_name, func_loc_str);
        return -1;
    }
    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);
    int num_of_attribute = sv_ht_values(t_data->attr_ht, attr_data_arr);
    return num_of_attribute;

}



char* catalog_get_table_header(char* table_name){
    char func_loc_str[] = "(catalog.c/catalog_get_table_header)";
    if(table_name == NULL || table_name[0] == '\0'){
        printf("Error: table_name is NULL\n");
        return NULL;
    }

    if(catalog_contains(table_name)!=1){
        printf("Error: Table does not exist\n");
        return NULL;
    }

    struct catalog_table_data* t_data = sv_ht_get(table_ht, table_name);
    struct hashtable* attr_ht = t_data->attr_ht;
    struct ht_node** val_nodes = attr_ht->node_list;

    int char_count = 0;
    for(int i = 0; i < attr_ht->size; i++){
        struct attr_data* a_data = val_nodes[i]->value->v_ptr;
        enum db_type a_type = a_data->type;
        char* a_type_str = type_to_str(a_type);
        int a_type_str_len = strlen(a_type_str);
        int a_name_len = strlen(a_data->attr_name);

        char_count = char_count + a_name_len + a_type_str_len;
    }

    /*Add more char_count to account for spaces, comma, colon: sign, parenthesis*/
    int num_of_angular_bracket = 2;  //per attribute
    int total_num_of_angular_bracket = 2 * attr_ht->size;
    int total_num_of_spaces = ((attr_ht->size) * 3) + 5;    //add 5 just in case
    int total_num_of_colon = attr_ht->size;
    int total_num_of_comma = (attr_ht->size) - 1;
    int total_num_of_parenthesis = 2;
    int len_of_table_name = strlen(table_name);

    char_count = char_count + total_num_of_angular_bracket +
            total_num_of_spaces + total_num_of_colon +
            total_num_of_comma + total_num_of_parenthesis +
            (len_of_table_name * 2) + 50;                   // add 50 to account for the heading intro

    char* table_header_str = malloc(char_count);
    table_header_str[0] = 0;


    if(attr_ht->size == 1){
        struct attr_data* a_data = val_nodes[0]->value->v_ptr;
        enum db_type a_type = a_data->type;
        char* a_type_str = type_to_str(a_type);
        sprintf(table_header_str, "Table %d : %s ( <%s : %s> )", t_data->table_num,
                t_data->table_name, a_data->attr_name, a_type_str);
        return table_header_str;
    }else if(attr_ht->size > 1){
        sprintf(table_header_str, "Table %d : %s (", t_data->table_num, t_data->table_name);
        char* ptr = table_header_str + strlen(table_header_str);

        for(int i = 0; i < (attr_ht->size) - 1; i++){
            struct attr_data* a_data = val_nodes[i]->value->v_ptr;
            enum db_type a_type = a_data->type;
            char* a_type_str = type_to_str(a_type);

            sprintf(ptr, " <%s : %s>,", a_data->attr_name, a_type_str);
            ptr = ptr + strlen(ptr);
        }
        struct attr_data* a_data = val_nodes[(attr_ht->size)-1]->value->v_ptr;
        enum db_type a_type = a_data->type;
        char* a_type_str = type_to_str(a_type);
        sprintf(ptr, " <%s : %s> )", a_data->attr_name, a_type_str);

        return table_header_str;
    }

    return NULL;
}

int catalog_table_data_is_valid(struct catalog_table_data* t_data){
    if(t_data == NULL){
        printf("Error: t_data is NULL\n");
        return 0;
    }
    int num_of_tables = storage_get_num_of_tables();
    if(t_data->table_num < 0){
        printf("Error: Invalid table number: %d\n", t_data->table_num);
        return 0;
    }

    if(t_data->table_name[0] == '\0' || t_data->table_name == NULL){
        printf("Error: Invalid table name: %s. It is NULL\n", t_data->table_name);
        return 0;
    }

    if(catalog_contains(t_data->table_name)){
        printf("Error: Invalid table name: %s. Table already exist\n", t_data->table_name);
        return 0;
    }

    /*Check if the attribute hashtable is valid*/
    struct hashtable* attr_ht = t_data->attr_ht;
    if(attr_ht == NULL || attr_ht->size <= 0){
        printf("Error: Invalid attribute hashtable\n");
        return 0;
    }

    if(t_data->p_key_len < 1){
        printf("Error: Invalid amount of primary key attributes : %d\n", t_data->p_key_len);
        return 0;
    }

    /*Check if primary_key_attrs is valid*/
    if(t_data->primary_key_attrs == NULL){
        printf("Error: Primary key attribute array is NULL\n");
        return 0;
    }

    char** p_key_attrs_arr = t_data->primary_key_attrs;
    for(int i = 0; i < t_data->p_key_len; i++){
        if(p_key_attrs_arr[i] == NULL){
            printf("Error: p_key_attr[%d] is NULL\n", i);
            return 0;
        }
    }

    if(t_data->num_of_f_key < 0){
        printf("Error: Number of foreign keys cannot be less than zero\n");
        return 0;
    }

    if(t_data->num_of_f_key > 0){
        if(t_data->num_of_f_key > t_data->f_key_arr_size){
            printf("Error: Number of foreign keys cannot be greater than its array size\n");
            return 0;
        }

        if(t_data->f_keys == NULL){
            printf("Error: Expected at least one foreign key but the foreign key array is NULL\n");
            return 0;
        }

        for(int i = 0; i < t_data->num_of_f_key; i++){
            if(t_data->f_keys[i] == NULL){
                printf("Error: f_keys[%d] is NULL\n", i);
                return 0;
            }
        }
    }

    if(t_data->num_of_childs < 0){
        printf("Error: Number of child cannot be less than 0\n");
        return 0;
    }

    if(t_data->num_of_childs > 0){
        if(t_data->num_of_childs > t_data->child_arr_size){
            printf("Error: Number of child cannot be greater than the array that stores it\n");
            return 0;
        }

        if(t_data->childs == NULL){
            printf("Error: Expected at least one child but array is NULL\n");
            return 0;
        }

        for(int i = 0; i < t_data->num_of_childs; i++){
            if(t_data->childs[i] == NULL){
                printf("Error: childs[%d] is NULL\n", i);
                return 0;
            }
        }
    }

    return 1;

}

struct hashtable* catalog_get_ht(){
    return table_ht;
}

void catalog_close(){
    write_catalog(catalog_loc, table_ht);

}












