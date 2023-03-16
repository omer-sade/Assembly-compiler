
int find_string(const char **array, const char *str);//NOT SURE IF SHOULD BE HERE



// Define the struct for each element of the table
typedef struct {
    int line_num;
    char bin_str[15]; 
} Binary_line;

// Define the struct for the array (table) itself
typedef struct {
    Binary_line *table;
    int size;
} Binary_table;

void initBinaryTable(Binary_table *tablePtr);

void addBinaryLine(Binary_table *tablePtr, char *bin_str);




char *pos_to_binary(unsigned int num, int bit_count);

char *neg_to_binary(int num, int bit_count);

char* decimalToBinary(int decimalNum, int size);

char* opecode_to_binary(const char **arr, const char *str);


void string_to_binary(char* str, Binary_table *binary_table);

void data_to_binary(char* data, Binary_table *binary_table, int *position);

char* registers_addressing(char* orig_reg, char* dest_reg);



char *get_next_word(char *string, int *position);

int isSymbol(char* str);

char *operands_params(char *orig, char *dest, const char** registers);


void other_words(char* src_operand, char* dest_operand,const char** registers, Binary_table *binary_table);

void create_binary_from_line(const char *cur_line, const char** instructions, const char** registers, Binary_table *binary_table, int *line_num);