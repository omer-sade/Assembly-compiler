
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
bool validate_operands_type(const char *line, int index);
