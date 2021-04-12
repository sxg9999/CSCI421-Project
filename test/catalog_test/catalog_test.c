#include <stdio.h>
#include "catalog.h"

int main(){
    printf("Catalog Test\n");
    catalog.test_print();
    catalog.print_table_info();

    char db_loc[] = "./db/";
    init_catalog(db_loc);

    catalog.print_table_map();

    printf("\n\n");
    printf("Adding...\n");
    catalog.add_table("apples", 1);
    catalog.add_table("trees", 2);
    catalog.add_table("pear", 3);
    catalog.add_table("apples1", 1);

    printf("Adding up to here without problem...\n");
    // catalog.add_table("trees2", 2);
    catalog.add_table("bob", 2);
    catalog.add_table("sam", 3);

    // catalog.print_table_info();

    
    // catalog.add_table("pear3", 3);
    // catalog.add_table("apples4", 1);
    // catalog.add_table("trees5", 2);
    // catalog.add_table("pear6", 3);
    // catalog.add_table("apples7", 1);
    // catalog.add_table("trees8", 2);
    // catalog.add_table("pear9", 3);
    catalog.print_table_map();
    catalog.print_table_info();

}