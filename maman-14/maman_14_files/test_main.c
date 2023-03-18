#include "test_main.h"


int main(){
   
    /*
    opening origin file
    */
    FILE *p_input_file;
    p_input_file = fopen("test_file.txt", "r");
    
    
    if (p_input_file == NULL) {
        printf("Could not open file. Terminating program\n");
        return 1;
    }

    /*
    the file with all macros expanded. 
    */
    FILE *p_file_open_macros;
    p_file_open_macros = fopen("file_open_macros.txt","w");
    if (p_file_open_macros == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }
   

    Array symbols_table;
    initArray(&symbols_table);
    
    /*
    opening all macros
    */
    pre_assembler(p_input_file, p_file_open_macros);


    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");

    const char* instructions[] = {"mov", "cmp", "add", "sub","not","clr","lea",
    "inc", "dec", "jmp", "bne", "red","prn","jsr","rts","stop", NULL};
    const char* registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7", NULL};

    Binary_table instructions_table; // Declare a new binary_table
    initBinaryTable(&instructions_table); // Initialize the binary_table

    Binary_table data_table; // Declare a new binary_table
    initBinaryTable(&data_table); // Initialize the binary_table

    /*
    reading file for the first time 
    */
    reading_file_first_time(&symbols_table, instructions, p_file_open_macros, registers, &instructions_table, &data_table);
    

   

    int i;
    for(i=0; i < symbols_table.size; i++){
        printf("symbol: %s, extern = %d, entry = %d, line num = %d\n", symbols_table.symbol[i].name, symbols_table.symbol[i].ext, symbols_table.symbol[i].ent, symbols_table.symbol[i].line_num);
    }

    

    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");
    /*
    reading file for the second time 
    */
    reading_file_second_time(&symbols_table, &instructions_table ,instructions, p_file_open_macros);

    FILE *binary_code_file = fopen("binary_code_file.txt", "w");
    if(binary_code_file == NULL){
        printf("Error: could not open file.\n");
        return 1;
    }

    //test prints:
    for (int i = 0; i < instructions_table.size; i++) {
        get_length_and_converted_string(instructions_table.table[i].line_num, instructions_table.table[i].bin_str, binary_code_file);
    }

    // Print the size of the array
    printf("Size of the array: %d\n", instructions_table.size);

    for (int i = 0; i < data_table.size; i++) {
        get_length_and_converted_string(data_table.table[i].line_num, data_table.table[i].bin_str, binary_code_file);
    }

    // Print the size of the array
    printf("Size of the array: %d\n", data_table.size);


    fclose(p_file_open_macros); 
    free(symbols_table.symbol);
    free(instructions_table.table);
    free(data_table.table);
    
}

char* int_to_four_char_string(int input) {
    static char output[5];
    sprintf(output, "%04d", input);
    return output;
}

void convert_to_dots_slashes(char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '0') {
            output[i] = '.';
        } else if (input[i] == '1') {
            output[i] = '/';
        } else {
            printf("Invalid input string!\n");
            return;
        }
    }
    output[i] = '\0';
}

void get_length_and_converted_string(int num, char *input, FILE *fp) {
    char converted[strlen(input) + 1];
    convert_to_dots_slashes(input, converted);
    fprintf(fp, "%s\t%s\n", int_to_four_char_string(num), converted);
}