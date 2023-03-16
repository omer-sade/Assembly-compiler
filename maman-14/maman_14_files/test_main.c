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
   

    /*
    randomly chose 100. probably will need to change
    (see page 42 in instructions file)
    */
    Array symbols_table;
    initArray(&symbols_table);

    /*
    contains all valid instructions: mov, sub, inc.. 
    16 total. 
    
    */
    
    /*
    opening all macros
    */
    pre_assembler(p_input_file, p_file_open_macros);


    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");

    const char* instructions[] = {"mov", "cmp", "add", "sub","not","clr","lea",
    "inc", "dec", "jmp", "bne", "red","prn","jsr","rts","stop", NULL};
    const char* registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7", NULL};

    Binary_table binaryTable; // Declare a new binary_table
    initBinaryTable(&binaryTable); // Initialize the binary_table

    /*
    reading file for the first time 
    */
    reading_file_first_time(&symbols_table, instructions, p_file_open_macros, registers, &binaryTable);

    for (int i = 0; i < binaryTable.size; i++) {
        printf("line_num: %d, bin_str: %s\n", binaryTable.table[i].line_num, binaryTable.table[i].bin_str);
    }

    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");
    /*
    reading file for the second time 
    */
    reading_file_second_time(&symbols_table, instructions, p_file_open_macros);


    fclose(p_file_open_macros); 
    free(symbols_table.symbol);
    free(binaryTable.table);
    
}
