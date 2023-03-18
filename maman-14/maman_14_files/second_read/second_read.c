#include "second_read.h"

/*
function for bar
*/
void get_is_extern_and_line_num(Array *symbols_table ,char *symbol, int *line_num, int *is_extern){
    int i =0; 
    i = searchArray(symbols_table, symbol);
    if(i != -1){
        *line_num = symbols_table->symbol[i].line_num;
        if(symbols_table->symbol[i].ext)
            *is_extern = 1;
    }
    else{
        /*
        symbol not in table (you can delete if you want, added this part for clarity)
        */
    }

    /*
    EXAMPLE HOW TO USE

    char *symbol = "boo";
    int line_num = 0;
    int is_extern = 0;
    get_is_extern_and_line_num(symbols_table,symbol, &line_num, &is_extern);
    */

}

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
                symbols_table->symbol[j].ent = true;
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
        char *file_name = "entries.txt";
        create_entry_file(symbols_table, file_name);
    }
    free(entry_table.symbol);
   
}

void create_entry_file(Array *table, char *file_name){
    FILE *entry_file = fopen(file_name, "w");
    if(entry_file == NULL){
        printf("Error: Could not open entry file.\n");
        return;
    }
        
    
    int i;
    for (i = 0; i < table->size; i++){
        if(table->symbol[i].ent){
            char *temp = table->symbol[i].name;
            fprintf(entry_file, "%s\t%d\n", table->symbol[i].name, table->symbol[i].line_num);
            // fprintf(entry_file, "%s", "\t");
            // char *line_num = malloc(sizeof(char) * 10);
            // sprintf(line_num, "%d", table->symbol[i].line_num);
            // fprintf(entry_file, "%s", line_num);
            // free(line_num);
        }
        
    }
    fclose(entry_file);
}