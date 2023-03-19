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

void reading_file_second_time(Array *symbols_table, Binary_table *instructions_table, const char **instructions, FILE *p_outputFile, char *filename){
    
    /*
    varialbes to check if line has entry / extern / data in it.
    */
    char *is_entry;
    char *is_extern;
    char *is_data;
    char *is_string;
    char line[LINE_SIZE];
    
    /*
    name to be given to output file
    */
    char file_name[100];
    int i, j;
    /*
    counters for errors in file and extern symbols in file
    */
    int error_counter = 0;
    int extern_counter = 0;
    
    /*
    used for validating that all entry symbols are 
    initialized in file
    */
    bool found;
    
    /*
    saves all entry symbols found in file
    */
    Array entry_table;
    initArray(&entry_table);
  
    while(fgets(line, sizeof(line), p_outputFile) != NULL){
        
        if(is_empty(line) || is_comment(line))
            continue;
   
        is_data = strstr(line, ".data");
        is_extern = strstr(line, ".extern");
        is_string = strstr(line, ".string");
        is_entry = strstr(line, ".entry");  
       
        if(is_data || is_string)
            continue;
        if(is_extern){
            extern_counter++;
            continue;
        }
        if(is_entry){
            int temp = 0;
            add_Extern_Entry_Symbol(&entry_table,&error_counter, line, &temp);        
        }
    }
    
    
    /*
    making sure that all symbols that are declared 'entry'
    are acually initialized in file. 
    */
    found = false;
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
    
    if(extern_counter >0){
        sprintf(file_name, "%s.ext", filename);
        create_extern_file(symbols_table, instructions_table, file_name);
    }
    if(entry_table.size >0){
        sprintf(file_name, "%s.ent", filename);
        create_entry_file(symbols_table, file_name);
    }

    symbolToBinary(instructions_table, symbols_table);
    free(entry_table.symbol);
   
}

void create_extern_file(Array *table,Binary_table* instructions_table, char *file_name){
    FILE *extern_file = fopen(file_name, "w");
    int i;
    if(extern_file == NULL){
        printf("Error: Could not open entry file.\n");
        return;
    }
    
   
    for(i = 0; i< instructions_table->size; i++){
        int line_num = 0;
        int is_extern = 0;
        char *symbol = instructions_table->table[i].bin_str;
        if(symbol[0] =='?'){
            char *new_symbol = (symbol +1);
            get_is_extern_and_line_num(table, new_symbol, &line_num, &is_extern);
            if(is_extern){
                fprintf(extern_file,"%s\t%d\n", new_symbol, (i+100));
            }
            
        }
        
    }
    fclose(extern_file);
}



void create_entry_file(Array *table, char *file_name){
    FILE *entry_file = fopen(file_name, "w");
    int i;
    if(entry_file == NULL){
        printf("Error: Could not open entry file.\n");
        return;
    }
        
   
    for (i = 0; i < table->size; i++){
        if(table->symbol[i].ent){
            fprintf(entry_file, "%s\t%d\n", table->symbol[i].name, table->symbol[i].line_num);
        }
        
    }
    fclose(entry_file);
}

void symbolToBinary(Binary_table *instructions_table, Array *symbols_table){
    int i;
    int line_num = 0;
    char *temp_bin = (char*)calloc(15, sizeof(char));
    int external = 0; /*flag to check if it is external or local symbol*/
    for ( i = 0; i < instructions_table->size; i++) {
        if(instructions_table->table[i].bin_str[0] == '?'){ /*get the symbol: &instructions_table->table[i].bin_str[1]*/
            /*printf("symb: %s\n", &instructions_table->table[i].bin_str[1]);*/
            get_is_extern_and_line_num(symbols_table, &instructions_table->table[i].bin_str[1], &line_num, &external);
            temp_bin = decimalToBinary(line_num, 12);
            if(external==1){
                strcat(temp_bin, "01");
            }
            else if(external==0){
                strcat(temp_bin, "10");
            }
            strcpy(instructions_table->table[i].bin_str,temp_bin);
        }
    }
}