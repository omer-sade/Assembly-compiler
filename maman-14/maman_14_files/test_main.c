#include "test_main.h"


int main(){
    /*
    opening origin file
    */
    FILE *pfile = fopen("test_file.txt", "w");
    
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
    being used as a hashMap. even indexes are keys, odd indexes that follows are values
    */
    Array symbols_table;
    initArray(&symbols_table, sizeof(char[100]));

    /*
    contains all valid instructions: mov, sub, inc.. 
    16 total. 
    havent created yet.
    */
    Array insturctions;
    initArray(&insturctions, sizeof(char[10]));



    
    /*
    opening all macros
    */
    pfile = preAssembler(pfile);
    printf("line 64 in test_main\n");

    /*
    reading file for the first time 
    */
    reading_file_first_time(pfile, &symbols_table, &insturctions, p_outputFile);
    printf("line 70 in test_main\n");

    /*
    reading file for the second time 
    */
    reading_file_second_time(pfile, &symbols_table, &insturctions, p_outputFile);


    fclose(p_outputFile);
    fclose(pfile);

    free(symbols_table.data);
    free(insturctions.data);
    }