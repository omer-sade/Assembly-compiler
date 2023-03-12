
/*
gets a line with op code, validate its syntax
*/
bool is_valid_line_opcode(const char *line);


void get_opcode_indexes(const char *line, int *start_index, int *end_index);

/*
in a line that has valid opcode, validate operands
*/
bool is_valid_line_opcode(const char *line);

/*
returns the number of operands of given opcode
*/
int calc_needed_operands_num(char opcode[]);

/*
returns the acuall number of operands in line
*/
int calc_acuall_operands_num(const char *line, int index);

/*
validating that operand that should get 0 operands acually gets 0 operands
ex: "stop" --> true
    "stop r3" --> false
*/
bool check_valid_0_operand(const char *line, int index);

bool check_valid_1_operand(const char *line, char operand[], int index);

bool check_valid_2_operand(const char *line, char operand[],  int index);


/*
validating that opcode recieves corrent operands type. 
mov 3, 4 --> false
mov r2,r4 --> true
*/
bool validate_operands_type(const char *line,char opcode[], int index);

/*
validating syntax for opcodes jsr, bne, jmp
option 1: jmp SYMBOL_NAME
option 2: jmp SYMBOL_NAME(attribute1, attribute 2)
*/
bool validate_jsr_bne_jmp(const char *line, int index);

/*
looking for target's index in line.
search starts at index 'start'
*/
int get_index_of(const char *line, char target, int start);

/*
returns index of first char that isnt white char.
search start at index 'start'
*/
int get_first_char(const char *line, int start);

/*
returns true if a given line contains white chars in specific indexes
*/
bool is_contains_spaces(const char *line, int start, int end);

/*
returns true if in string 'line' in indexes start to end
there's a register name
*/
bool is_register(const char *line, int start, int end);

/*
returns true if in string 'line' in indexes start to end
there's a number
*/
bool is_number (const char *line, int start, int end);

/*
returns true if in string 'line' in indexes start to end
there's a symbol name
*/
bool is_symbol (const char *line, int start, int end);

/*
checking if 'target' is in 'array'
*/
bool contains(char array[][5], int rows, char target[]);

/*
get last char that isnt white char. 
starting from 'end' (including) and looping backwards
*/
int get_last_char(const char *line, int end);

 
