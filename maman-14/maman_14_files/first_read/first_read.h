
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
returns true if first word in line is symbol.
ex1: "MAIN: mov r3 ,LENGTH" --> true
ex2: "prn #-5"              --> false
*/
bool has_symbol(const char *line);


/*
returns true if data is declared in line
ex1: "x: .data 23" --> true
ex2: "MAIN: mov r3 ,LENGTH" --> false
*/
bool is_data(const char *line);


/*
returns true if string is declared in line
ex1: "STR: .string "abcdef"" --> true
ex2: "MAIN: mov r3 ,LENGTH" --> false
*/
bool is_string(const char *line);

/*
returns true if "entry" is in line
*/
bool is_entry(const char *line);


/*
returns true if "extern" is in line
*/
 bool is_extern(const char *line);


/*
addidng symbol to symbols table. if symbol already there - doesnt add it to table + addding error message
to errors_str.
updating DC. 
*/
void addSymbol(Array *symbols_table, Array *errors_str, const char *line, int *DC);


/*
returns true if insruction in line is valid.
ex1: "mov r3, r4" --> true
ex2: "sdhf r3" --> false
*/
bool valid_instruct(const char *line,Array *insturctions);


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
void create_binary_from_line(const char *line, int num_binary_lines, FILE *p_outputFile, Array *insturctions);



char *pos_to_binary(unsigned int num, int bit_count);

char *neg_to_binary(int num, int bit_count);

char* decimalToBinary(int decimalNum, int size);


/*function that gets a opecode and returns its binary value*/
void opecode_to_binary(Array *arr, void *element);