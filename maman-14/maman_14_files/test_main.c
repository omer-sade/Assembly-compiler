#include "test_main.h"


int main(){
    /*
    opening origin file
    */
    FILE *pfile = fopen("test_file.txt", "r");
    
    
    if (pfile == NULL) {
        printf("Could not open file. Terminating program\n");
        return 1;
    }

    /*
    the file we eventually return. Contains binary numbers. 
    */

    FILE *p_outputFile = fopen("output_file.txt","w");
    if (p_outputFile == NULL) {
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
    add_data(instructions);
    
    
    
    
    /*
    opening all macros
    */
    pfile = preAssembler(pfile);

    /*
    reading file for the first time 
    */
    reading_file_first_time(pfile, &symbols_table, &instructions, p_outputFile);

    /*
    reading file for the second time 
    */
    reading_file_second_time(pfile, &symbols_table, &instructions, p_outputFile);


    fclose(p_outputFile);
    fclose(pfile);

    free(symbols_table.data);
    free(instructions.data);
    
    }

    void add_data(Array instructions){
        int size = sizeof(char[10]);

        addArray(&instructions, "mov",  size);
        addArray(&instructions, "cmp",  size);
        addArray(&instructions, "add",  size);
        addArray(&instructions, "sub",  size);
        addArray(&instructions, "lea",  size);
        addArray(&instructions, "not",  size);
        addArray(&instructions, "clr",  size);
        addArray(&instructions, "inc",  size);
        addArray(&instructions, "dec",  size);
        addArray(&instructions, "jmp",  size);
        addArray(&instructions, "bne",  size);
        addArray(&instructions, "red",  size);
        addArray(&instructions, "prn",  size);
        addArray(&instructions, "jsr",  size);
        addArray(&instructions, "rts",  size);
        addArray(&instructions, "stop",  size);
    }
