#include "second_read.h"

void reading_file_second_time(Array *symbols_table, const char **instructions, FILE *p_outputFile){
   
    char line[LINE_SIZE];
   
    
    int error_counter = 0;
    int extern_couner = 0;
    Array entry_table;
    initArray(&entry_table);

    while(fgets(line, sizeof(line), p_outputFile) != NULL){
        
        if(is_empty(line) || is_comment(line))
            continue;

        char *is_data = strstr(line, ".data");
        char *is_extern = strstr(line, ".extern");
        char *is_string = strstr(line, ".string");
        char *is_entry = strstr(line, ".entry");
       
       
        if(is_data || is_string)
            continue;
        if(is_extern){
            extern_couner ++;
            continue;
        }
        if(is_entry){
            int temp = 0;
            add_Extern_Entry_Symbol(&entry_table,&error_counter, line, &temp);        
        }
        else{
            /*
            convert line to binary
            */
        }
    }
    
    int i, j;
    bool found = false;
    for(i = 0; i < entry_table.size; i++){
        found = false;
        for(j = 0; j < symbols_table->size; j++){
            char *symbol1 = entry_table.symbol[i].name;
            char *symbol2 = symbols_table->symbol[j].name; 
            if(strcmp(symbol1, symbol2) ==0){
                found = true;
            }
        }
        if(!found){
            error_counter ++;
            printf("Error: declared entry symbol '%s' without initialzing it.\n", entry_table.symbol[i].name);
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
    if(entry_table.size >0){
        /*
        create file with extern symbols
        */
    }

   
}
