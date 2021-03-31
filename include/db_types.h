#ifndef DB_TYPES_H
#define DB_TYPES_H

/**
 * A enum that holds the reserved key words as types
 */
enum db_type {UNKNOWN = 0,
        NOT_NULL = 1, UNIQUE = 2, PRIMARY_KEY = 3, FOREIGN_KEY = 4, DEFAULT = 5,                      //constraint
        INT = 10, DOUBLE = 11, BOOL = 12, CHAR = 13, VARCHAR = 14,  //data types
        CREATE = 20, DROP = 21, ADD = 22, ALTER = 23,                          //ddl
        INSERT = 30, UPDATE = 31, DELETE = 32, SELECT = 40,         //dml and query
        DDL = 50, DML = 51, QUERY = 52, NON_QUERY = 53};

/**
 * Initiates the db types
 * @return 0 if successful and -1 if failed
 */
int init_db_type();

/**
 * Get the type of the key_word(i.e., int, double,...create,..primary ..., etc
 * @return  a enum that represents the type
 */
enum db_type typeof_kw(char* key_word);


/**
 * Get the type of the statement (DDL or DML)
 * @param statement
 * @return the statement type
 */
enum db_type typeof_s(char* statement);

/**
 * Get the type of query (QUERY or NON_QUERY
 * @param statement
 * @return the type of query
 */
enum db_type typeof_q(char* statement);


/**
 * Check if the key_word is a attribute type
 * @param key_word
 * @return 1 if true, 0 if false
 */
int is_attr(char* key_word);

/**
 * Check if the key_word is a constraint type
 * @param key_word
 * @return
 */
int is_constraint(char* key_word);




/**
 * free everything that was allocated in db_type
 * @return
 */
int db_type_close();



/**
 * Checks if the type is a attribute type
 * @param t
 * @return 1 if true, 0 if false
 */
int type_is_attr(enum db_type t);

/**
 * Checks if the type is a constraint
 * @param t
 * @return 1 if true, 0 if false
 */
int type_is_constr(enum db_type t);


char* type_to_str(enum db_type type);

void db_type_print_all();
/**
 * A struct that holds function pointers
 */
struct type_func{
        int (*is_attr)(enum db_type);
        int (*is_constr)(enum db_type);
};


static struct type_func Type = {
        .is_attr = type_is_attr,
        .is_constr = type_is_constr,
};


#endif