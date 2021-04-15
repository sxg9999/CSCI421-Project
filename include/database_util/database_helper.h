//
// Created by SGuan on 4/3/2021.
//

#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

struct database_params{
    char* db_loc_path;
    int page_size;
    int buffer_size;
};

/**
 * Checks if the command line args are valid and store
 * all the command line args in the database_params struct
 * if its valid.
 * @return
 */
void get_cl_args(int argc, char* argv[]);

/**
 *
 * @param stmt
 */
void process_non_statements(char* stmt);

/**
 * Initiates the database
 */

void init_db();

void print_tables();

/**
 * Terminates the database (free, save, everything included)
 */
void db_close();

#endif
