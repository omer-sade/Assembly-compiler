#include <stdio.h>
#include <stdlib.h>
#include "first_read.h"


void reading_file_first_time(FILE *pfile, Array *symbols_table, Array *insturctions, FILE *p_outputFile){
    int DC = 0, IC = 0;
    /*
    changes to true if symbol is found in line
    */
    bool is_symbol_found = false;
    
    
    /*
    creating an array of strings to contain all the errors we will find in the file. 
    */
    Array errors_str;
    initArray(&errors_str, sizeof(char[error_message_len]));
    
    
    
    /*
    starting to read from file
    */
    char line[LINE_SIZE];
    while(fgets(line, LINE_SIZE, pfile) != NULL){
        if(has_symbol(line))
            is_symbol_found = true;
        
        if(is_data(line) || is_string(line)){
            if(is_symbol_found){
                addSymbol(symbols_table, &errors_str, line, &DC);
            
            }
            break;
        }
        if(is_entry(line) || is_extern(line)){
            /*
            didnt fully understand this part. this is line 9 in algorythm.
            */
           break;
        }
        /*
        If we got here than line doest contain data declaration nor extern / entry declaration. 
        Meaning it is instructions (with possibly a symbol).
        */
       if(is_symbol_found){
            addSymbol(symbols_table, &errors_str, line, &IC);
       }
       if(!valid_instruct(line, insturctions)){
            char *msg = "Invalid instruction in line: %s\n", line;
            addArray(insturctions, msg, sizeof(char[error_message_len]));
       }

        int num_binary_lines = calc_binary_lines_num(line);
        create_binary_from_line(line, num_binary_lines, p_outputFile);
        IC += num_binary_lines;
    }
    
    /*
    if errors found in file, print errors and terminate program
    */
    if(errors_str.size > 0){
        int i;
        for(i = 0; i < errors_str.size; i++){
            printf("%s\n", *((char *)errors_str.data + i));
        }
        free(symbols_table->data);
        free(insturctions->data);
        fclose(p_outputFile);
        fclose(pfile);
        exit(1);
    
    
    }


}

bool has_symbol(const char *line){
    return true;
}

bool is_data(const char *line)
{
    return false;
}


bool is_string(const char *line)
{
    return false;
}

bool is_entry(const char *line){
    return true;
}



 bool is_extern(const char *line){
    return true;
 }



void addSymbol(Array *symbols_table, Array *errors_str, const char *line, int *DC){

}

bool valid_instruct(const char *line,Array *insturctions){
    return true;
}

int calc_binary_lines_num(const char *line){
    return -1;
}

void create_binary_from_line(const char *line, int num_binary_lines, FILE *p_outputFile){
    
}
