#include <stdlib.h>
#include "../include/table_meta_data.h"
// #include "../include/page.h"


TableMeta* metatable_init(int id, char name[30], struct page **p, char **c, int num_columns, char **pk, int num_attrs_in_pk)
{
    TableMeta *mt = malloc(sizeof(TableMeta));
    mt->table_id = id;
    mt->columns = c;
    mt->primary_key = pk;
    mt->pages = p;

    return mt;
}

int save_meta_data()
{
    
}

int read_meta_data()
{
    
}
