#define SYMBOL_LEN 30


/*
create an entry file from array that consists only 'entry' symbols
*/
void create_entry_file(Array *entry_table, char *file_name);

/*
creates a file that holds all extern symbols and their lines. 
*/
void create_extern_file(Array *table,Binary_table* instructions_table, char *file_name);
