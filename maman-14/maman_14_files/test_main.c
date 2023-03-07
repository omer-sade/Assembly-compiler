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
    
    */
    char instructions[16][LINE_SIZE];
    add_data(instructions);
    
    /*
    opening all macros
    */
    pre_assembler(p_input_file, p_file_open_macros);


    fclose(p_file_open_macros);
    p_file_open_macros = fopen("file_open_macros.txt","r");

    /*
    reading file for the first time 
    */
    reading_file_first_time(&symbols_table, instructions, p_file_open_macros);

    


    /*
    reading file for the second time 
    */
    reading_file_second_time(&symbols_table, instructions, p_file_open_macros);


    fclose(p_file_open_macros); 
    free(symbols_table.data);
    
    
    }

    void add_data(char instructions[][LINE_SIZE]){
       
    strcpy(instructions[0], "mov");
    strcpy(instructions[1], "cmp");
    strcpy(instructions[2], "add");
    strcpy(instructions[3], "sub");
    strcpy(instructions[4], "lea");
    strcpy(instructions[5], "not");
    strcpy(instructions[6], "clr");
    strcpy(instructions[7], "inc");
    strcpy(instructions[8], "dec");
    strcpy(instructions[9], "jmp");
    strcpy(instructions[10], "bne");
    strcpy(instructions[11], "red");
    strcpy(instructions[12], "prn");
    strcpy(instructions[13], "jsr");
    strcpy(instructions[14], "rts");
    strcpy(instructions[15], "stop");

    }
