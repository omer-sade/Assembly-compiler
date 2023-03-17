#include <stdbool.h>
/*
max line size in origin file
*/
#define LINE_SIZE 82

/*
max length of each error message.
ex: "you created two macros with same name" -- total 37 chars
*/
#define error_message_len 100

/*
returns true if last char is coma
(excluding white chars)
*/
bool is_coma_last(const char *line);


/*
returns true if first word in line is symbol.
ex1: "MAIN: mov r3 ,LENGTH" --> true
ex2: "prn #-5"              --> false
*/
bool has_symbol(const char *line, int *error_counter);


/*
returns true if data is declared in line
ex1: "x: .data 23" --> true
ex2: "MAIN: mov r3 ,LENGTH" --> false
*/
bool is_data(const char *line, int *error_counter);


/*
returns true if string is declared in line
ex1: "STR: .string "abcdef"" --> true
ex2: "MAIN: mov r3 ,LENGTH" --> false
*/
bool is_string(const char *line, int *error_counter);

/*
returns true if "entry" is in line
*/
bool is_entry(const char *line, int *error_counter);


/*
returns true if "extern" is in line
*/
 bool is_extern(const char *line, int *error_counter);


/*
addidng symbol to symbols table. if symbol already there - doesnt add it to table + addding error message
to errors_str.
updating DC. 
*/
void addSymbol(Array *symbols_table, int *error_counter, const char *line, int *line_num);


/*
returns true if insruction in line is valid.
ex1: "mov r3, r4" --> true
ex2: "sdhf r3" --> false
*/
bool valid_instruct(const char *line, const char **instructions, int *error_counter);
/*
returns the number of binary lines needed to represent current line.
ex: "prn #-5" is | 00001100000000 | so output is 2. 
                 | 11111111101100 |
*/
int calc_binary_lines_num(const char *line);


/*
writes binary numbers to output file based on current line. 
ex: "prn #-5": writes | 00001100000000 |  in output file.
                      | 11111111101100 |
*/
//void create_binary_from_line(const char *line, int num_binary_lines, FILE *p_outputFile);


/*
returns true if there are only white charachters in line
*/
bool is_empty(const char *line);

/*
returns true if this line is a comment
(comment line starts with ";")
*/
bool is_comment(const char *line);

/*
help function for 'has symbol'. 
validates that symbol's name isnt an opcode / register name
*/
bool is_valid_symbol_name(const char *line, int start, int end);

/*
returns the index of char 'target' in 'line'.
search starts at index 'start'
*/
int get_index_of(const char *line, char target, int start);

/*
help function for 'is_data'
validates that the syntax is valid
*/
bool is_valid_syntax(const char *line, int start_index, int end_index, int *error_counter);

bool is_valid_line_opcode(const char *line);

/*
counts how many times 'target' appears in 'line'
*/
int count(const char *line, char target);

/*
if line has 'extry' or 'enxtern' - returns true if there's a valid symbol
*/
bool has_symbol_entry_extern(const char *line,int *error_counter);

/*
returns true if a theres spaces in line between given indexes
*/
bool is_spaces(const char *line, int start, int end);

/*
get last char that isnt white char. 
starting from 'end' (including) and looping backwards
*/
int get_last_char(const char *line, int end);

/*
returns index of first char that isnt white char.
search start at index 'start'
*/
int get_first_char(const char *line, int start);

/*
assuming there is a valid external symbol declarition in line,
return the symbol's indexes
*/
void find_external_symbol_indexes(const char *line, int *start, int *end);

int get_last_char(const char *line, int end);

/*
being used only if there's 'extern' in 'line'
addidng symbol to symbols table
*/
void addExternSymbol(Array *symbols_table, int *error_counter, const char *line, int *line_num);

/*
looking for all uncheckd extreme cases
*/
bool find_all_errors(const char *line);

/*
validating that only allowed chars in line
*/
bool validate_ascii(const char *line);