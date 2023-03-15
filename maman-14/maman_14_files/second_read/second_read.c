#include "second_read.h"

void reading_file_second_time(Array *symbols_table, const char **instructions, FILE *p_outputFile){
    int IC =0; 
    char line[LINE_SIZE];
    int size = symbols_table->size;
    char entry_symbols_arr[size][SYMBOL_LEN];
    int entry_arr_index = 0;
    int error_counter = 0;
    int entry_counter = 0;
    int extern_couner = 0;
    while(fgets(line, sizeof(line), p_outputFile) != NULL){
        
        if(is_empty(line) || is_comment(line))
            continue;

        char *is_data = strstr(line, ".data");
        char *is_extern = strstr(line, ".extern");
        char *is_string = strstr(line, ".string");
        char *is_entry = strstr(line, ".entry");
       
        printf("line = %s\n", line);
       
        if(is_data || is_string)
            continue;
        if(is_extern){
            extern_couner ++;
            continue;
        }
        if(is_entry){
            /*
            stages 6, 7, 8 in page 46
            */
           int start, end;
           start = get_first_char(line, 0);
           start = get_first_char(line, start + 7);
           end = get_last_char(line, strlen(line));
           char symbol[SYMBOL_LEN];
            /*
            Copy substring to 'symbol' string
            */ 
            int i;
            for (i = start; i <= end - 1; i++) {
                symbol[i - start] = line[i];
            }
            symbol[i - start] = '\0';
            strncpy(entry_symbols_arr[entry_arr_index], symbol, SYMBOL_LEN-1); 
            entry_symbols_arr[entry_arr_index][SYMBOL_LEN-1] = '\0';
            entry_arr_index++;
            continue;
        }
        else{
            /*
            convert line to binary
            */
        }
    }

    if(error_counter > 0){
        printf("Errors found in file. Terminating program.\n");
        return;
    }    
    /*
    create binary file
    */

    if(extern_couner >0){
        /*
        create file with extern symbols
        */
    }
    if(entry_counter >0){
        /*
        create file with extern symbols
        */
    }

   
}
