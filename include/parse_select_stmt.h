#ifndef PARSE_SELECT_STMT_H
#define PARSE_SELECT_STMT_H

struct column_table_pair {
    char* column;
    char* table;
    int column_index;
};


struct table_columns {
    // (column_name, table) pairs
    struct column_table_pair** pairs;
    // number of pairs
    int pair_count;

};

int parse_select_stmt(char* statement);

int parse_select_ids(char** token, char** column_names, int* column_count, int* star_select);

int parse_select_tables(char** token, char** table_names, int* table_count);

int get_table_columns(char** column_names, int column_count, char** table_names, int table_count,
        struct table_columns* tc);


int parse_select_where(char** token, char** where_clause, int* order_clause_present);

int parse_select_order_by(char** token, struct table_columns* order_columns, 
            char** column_names, int column_count, char** table_names, int table_count);

int test_select_example();
void print_table_columns(struct table_columns* tc);
int column_has_dot(char* column_name);

#endif
