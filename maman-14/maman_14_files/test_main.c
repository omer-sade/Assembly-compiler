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
    initArray(&symbols_table, sizeof(char[100]));

    /*
    contains all valid instructions: mov, sub, inc.. 
    16 total. 
    havent created yet.
    */
    Array instructions;
    initArray(&instructions, sizeof(char[10]));
    add_data(&instructions);
    
    /*
    opening all macros
    */
    pre_assembler(p_input_file, p_file_open_macros);


    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");

    /*
    reading file for the first time 
    */
    reading_file_first_time(&symbols_table, &instructions, p_file_open_macros);

    


    /*
    reading file for the second time 
    */
    reading_file_second_time(&symbols_table, &instructions, p_file_open_macros);


    fclose(p_file_open_macros); 
    free(symbols_table.data);
    free(instructions.data);
    
    }

    void add_data(Array *instructions){
        int size = sizeof(char[10]);

        addArray(instructions, "mov",  size);
        addArray(instructions, "cmp",  size);
        addArray(instructions, "add",  size);
        addArray(instructions, "sub",  size);
        addArray(instructions, "lea",  size);
        addArray(instructions, "not",  size);
        addArray(instructions, "clr",  size);
        addArray(instructions, "inc",  size);
        addArray(instructions, "dec",  size);
        addArray(instructions, "jmp",  size);
        addArray(instructions, "bne",  size);
        addArray(instructions, "red",  size);
        addArray(instructions, "prn",  size);
        addArray(instructions, "jsr",  size);
        addArray(instructions, "rts",  size);
        addArray(instructions, "stop",  size);
    }

