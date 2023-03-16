

/*function that takes an array of strings and a string (str)
and checks if str is found in the array and returns its location
if found or -1 if not*/
int find_string(const char **array, const char *str);

/*Define the struct for each element of the table*/
typedef struct {
    int line_num;
    char bin_str[31]; 
} Binary_line;

/* Define the struct for the array (table) itself*/
typedef struct {
    Binary_line *table;
    int size;
} Binary_table;

/* Function to initialize a new binary_table*/
void initBinaryTable(Binary_table *tablePtr);

/* Function to add a new binary_line element to the dynamic array (binary_table),
given the binary table, string, and line number*/
void addBinaryLine(Binary_table *tablePtr, char *bin_str, int *line_num);



/*function that converts positive integer to binary with 
bit_count number of digits*/
char *pos_to_binary(unsigned int num, int bit_count);

/*function that converts negative integer to binary with 
bit_count number of digits*/
char *neg_to_binary(int num, int bit_count);

/*function that returns an integer (positive or negative) and returns
its binary value with size number of digits*/
char* decimalToBinary(int decimalNum, int size);

/*creating the 4 digit bit type address of the origin,destination operands*/
char* opecode_to_binary(const char **arr, const char *str);

/*function that builds the binary lines of the data line, one line for each number
and puts them in the given data table.*/
void string_to_binary(char* str, Binary_table *data_table, int *line_num);

/*function that builds the binary lines of the data line, one line for each number
and puts them in the given data table.*/
void data_to_binary(char* data, Binary_table *data_table, int *position, int *line_num);

/*function that gets two strings of register names and builds a
binary line from them. Could be that one string is NULL meaning
that only the register of the other string exists. 
For example: register(NULL, r4) means that only the 
destination register exists*/
char* registers_addressing(char* orig_reg, char* dest_reg);


/*function that gets a string and returns the the next word from the string.
words are seperated by commas, parenthesis, and white chars*/
char *get_next_word(char *string, int *position);

/*function that checks if a string is of symbol format (symbol:)*/
int isSymbol(char* str);

char *operands_params(char *orig, char *dest, const char** registers);

/*function that builds the other words that come out of an instruction line*/
void other_words(char* src_operand, char* dest_operand,const char** registers, Binary_table *binary_table, int *line_num);

/*function that builds the binary tables for the instruction lines and data lines.
the function is given the line to read, the list of opecodes (instructions), list of registers, 
instruction table to build on to, data table to build on to, and line number*/
void create_binary_from_line(const char *cur_line, const char** instructions, const char** registers, Binary_table *instructions_table, Binary_table *data_table, int *IC, int *DC);