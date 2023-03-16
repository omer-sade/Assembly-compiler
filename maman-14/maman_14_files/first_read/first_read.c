#include <stdio.h>
#include <stdlib.h>
#include "first_read.h"



void reading_file_first_time(Array *symbols_table, const char **instructions, FILE *p_outputFile, const char** registers, Binary_table *binary_table){
    
    int line_num = 100; 
    /*
    every time we find an error - increment by 1. at the end if its value isnt 0 - stop the code. 
    */
    int error_counter = 0;
    /*
    changes to true if symbol is found in line
    */
    bool is_symbol_found = false;    
    /*
    starting to read from file
    */
    char line[LINE_SIZE];
    while(fgets(line, sizeof(line), p_outputFile) != NULL){
        if(is_empty(line) || is_comment(line))
            continue;
        if(is_coma_last(line)){
            printf("Invalid syntax in line: %s\n",line);
            error_counter++;
            continue;
        }
        /*
        for tracking errors in specific line
        */
        int current_error_num = error_counter;
        
        is_symbol_found = false;
        
        if(has_symbol(line, &error_counter))
            is_symbol_found = true;
        
        bool isData = is_data(line, &error_counter);
        bool isString = is_string(line, &error_counter);

        bool isEntry = is_entry(line, &error_counter);
        bool isExtern = is_extern(line, &error_counter);
        if(isData || isString){
            if(is_symbol_found && !isExtern){
                addSymbol(symbols_table, &error_counter, line, &line_num);
            }
            else if(is_symbol_found){
                int temp = -1;
                addSymbol(symbols_table, &error_counter, line, &temp);
            }
           
        }
        else if(isEntry || isExtern){
            if(isExtern){
                int temp = -1;
                addExternSymbol(symbols_table,&error_counter, line, &temp);
                
            }
           continue;
        }
        /*
        If we got here than line doest contain data declaration nor extern / entry declaration. 
        Meaning it is instructions (with possibly a symbol).
        */
        else if(is_symbol_found){
            addSymbol(symbols_table, &error_counter, line, &line_num);
        }
       /*
       if there's no data, no string, no entry, no extern in line --> it has opcode
       */
        else if(current_error_num == error_counter){
            if(!is_empty(line) && !isData && !isString && !isEntry && !isExtern){
                if(!valid_instruct(line, instructions, &error_counter) ||!is_valid_line_opcode(line)){
                    printf("Invalid syntax in line: %s\n", line);
                    error_counter ++;
                }
            }
        }

        //int num_binary_lines = calc_binary_lines_num(line);
        if(current_error_num == error_counter){
            create_binary_from_line(line, instructions, registers, binary_table, &line_num);
        }
        //IC += num_binary_lines;
    }
    
    /*
    if errors found in file, terminate program
    */
    if(error_counter > 0){
        free(symbols_table->symbol);
        fclose(p_outputFile);
        free(binary_table->table);
        exit(1);
    }


}

bool is_coma_last(const char *line){
    int i;
    for(i = strlen(line)-2; i > 0 ; i--){
        char c = line[i];
        if(isspace(line[i]))
            continue;
        if(line[i] == 44)
            return true;
        return false;
    }
}



bool has_symbol(const char *line, int *error_counter){
    int i = 0;
    /*
    if this is a comment line
    */
    if (line[0] == ';')
        return false;
    
    int colon_counter = count(line, ':');
    /*
    checking how many ":" in line. if not 1, returns false
    */
    char *isExten = strstr(line, ".extern");
    char *isEntry = strstr(line, ".entry");
    if(colon_counter == 0 && isEntry == NULL & isExten == NULL)
        return false;
    if(isExten || isEntry){
        return false;
    }
    if(colon_counter > 1){
        printf("Invalid syntax, only 1 colon allowed - %s\n", line);
        *error_counter = *error_counter + 1;
        return false;
    }
    /*
    finding the colon index (":" is 58 in ascii)
    */
    int colon_index = get_index_of(line, 58, 0);

    if(colon_index == 0){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
        
    /*
    checking if there is space before the colon using ascii
    */
    if(line[colon_index-1] == 32){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    /*
    analyzing the line from index "0" to index "colon index"
    */
   int last_space_index = -1;
   int first_char_index = 0;
   bool found_first_char = false;

   for(i = 0; i < colon_index; i++){
        if(line[i] == 32)
            last_space_index = i;
        if((line[i] >= 97 && line[i] <= 122) || (line[i] >= 65 && line[i] <= 90) || isdigit(line[i])){
            if(!found_first_char){
                first_char_index = i;
                found_first_char = true;
            }   
        }
   }
   bool error_found = false;
   int index = first_char_index;
    if(!((line[index] >= 65 && line[index] <= 90) || (line[index] >= 97 && line[index] <= 122)))
        error_found = true;

    if(colon_index -  first_char_index > 30)
        error_found = true;
  
    if( last_space_index > first_char_index)   
        error_found = true;

    bool isValidSymbolName = is_valid_symbol_name(line, first_char_index, colon_index);
    if(!isValidSymbolName)
        error_found = true;
    
    if(error_found){
        printf("Invalid symbol declaration in line: %s", line);
        *error_counter = *error_counter + 1;
        return false;
    }
    return true;
    }

bool is_valid_symbol_name(const char *line, int start, int end){
    char symbol[LINE_SIZE + 1];
    /*
    Copy substring to 'symbol' string
    */ 
    int i;
    for (i = start; i <= end - 1; i++) {
        symbol[i - start] = line[i];
    }
    symbol[i - start] = '\0';

    if(strcmp(symbol, "mov") == 0)
        return false;
    if(strcmp(symbol, "cmp") == 0)
        return false;
    if(strcmp(symbol, "add") == 0)
        return false;
    if(strcmp(symbol, "sub") == 0)
        return false;
    if(strcmp(symbol, "lea") == 0)
        return false;
    if(strcmp(symbol, "not") == 0)
        return false;
    if(strcmp(symbol, "clr") == 0)
        return false;
    if(strcmp(symbol, "inc") == 0)
        return false;
    if(strcmp(symbol, "dec") == 0)
        return false;
    if(strcmp(symbol, "jmp") == 0)
        return false;
    if(strcmp(symbol, "bne") == 0)
        return false;
    if(strcmp(symbol, "red") == 0)
        return false;
    if(strcmp(symbol, "prn") == 0)
        return false;
    if(strcmp(symbol, "jsr") == 0)
        return false;
    if(strcmp(symbol, "rts") == 0)
        return false;
    if(strcmp(symbol, "stop") == 0)
        return false;
    if(strcmp(symbol, "r0") == 0)
        return false;
    if(strcmp(symbol, "r1") == 0)
        return false;
    if(strcmp(symbol, "r2") == 0)
        return false;
    if(strcmp(symbol, "r3") == 0)
        return false;
    if(strcmp(symbol, "r4") == 0)
        return false;
    if(strcmp(symbol, "r5") == 0)
        return false;
    if(strcmp(symbol, "r6") == 0)
        return false;
    if(strcmp(symbol, "r7") == 0)
        return false;
    return true;
}

/*
help function for "is_data". checking if a string contains numbers
*/  
bool is_number_in(char temp[]){
    int i;
    for(i = 0; i < strlen(temp); i ++){
        if(isdigit(temp[i]))
            return true;
    }
    return false;
}

/*
help function for "is_data". checking if a string contains numbers
*/
bool is_only_white_chars(char temp[]){
    int i;
    for(i = 0; i < strlen(temp) -2; i ++){
        if(!isspace(temp[i]))
            return false;
    }
    return true;
}

/*
help function for "is data". Checks if there's a number in string
*/
bool is_contains_number(char str[], int index){
    int i;
    
    if(str[0] == '\0')
        return false;

    for(i = 0; i < index; i ++){
        
        if(isspace(str[i]))
            continue;
        if(isdigit(str[i]))
            return true;
    }
    return false;
}


bool is_data(const char *line, int *error_counter)
{
    char *word = ".data";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }
   /*
   start_index = starting index of ".data"
   end_index = end index of ".data"
   */
    int start_index = result - line;
    int end_index = start_index + 5;
    
    if(line[end_index] != 32 && line[end_index] != 9){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    

    /*
    looping from start of line until the first index of ".data". 
    looking for any syntax errors. we might have symbol before, so only allowed 
    characters are letters and numbers, white characters, and 1 colon
    */
    int i;
    if(start_index != 0){
        for(i = 0; i < start_index; i++){
                if((line[i] >= 97 && line[i] <= 122) && (line[i] >= 65 && line[i] <= 90) && isdigit(line[i]) && line[i] != 58){
                    printf("Invalid syntax in line: %s\n",line);
                    *error_counter = *error_counter + 1;
                    return false;
                }
            }
    }

    /*
    looping from end of ".data" to end of "line".
    if there's any thing that isnt a space, number, coma, hyphen, plus sign --> return false
    */
    for(i = end_index; i < strlen(line)-2; i++){  
        if (line[i] == 32 || line[i] == 9 || line[i] == 10 || line[i] == '\0'){ 
            continue;
        }
        if((line[i] < 48 || line[i] > 57) && line[i] != 44 && line[i] != 45 && line[i] != 43){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
    }
   /*
    if coma is first return false
   */
   for(i = end_index; i < strlen(line) -2; i++){
        if(isspace(line[i]))
            continue;
        if(line[i] == 44){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
        else{
            break;
        }
    }

    /*
    if coma is last return false
    */
    for(i = strlen(line) -2; i > end_index ; i--){
        if(isspace(line[i]))
            continue;
        if(line[i] == 44 || line[i] == 45 || line[i] == 43){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
        else
            break;
    }

    bool isValidSyntax = is_valid_syntax(line, start_index, end_index, error_counter);
    if(isValidSyntax)
        return true;
    return false;
    }
    
bool is_valid_syntax(const char *line, int start_index, int end_index, int *error_counter){
    /* "temp" holds parts from "line", used to validate line syntax in following loop*/
    char temp[LINE_SIZE]; 
    temp[0] = '\0';
    int temp_index = 0;
    int i;
    /*
    looping through "line" and looking for errors
    */
    for(i = end_index; i < strlen(line) - 2; i++){
    
        /*
        if current char is number
        */
       if(isdigit(line[i])){
            bool has_number = is_contains_number(temp, temp_index);
            if(temp[0] != '\0' && has_number && !isdigit(temp[temp_index-1])){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;
            }
            else{
                temp[temp_index] = line[i];
                temp_index++;
            }
       }
        
        
        /*
        if current char is space
        */
       else if( line[i] == 32 || line[i] == 9){
            if(temp[0] == '\0'){
            temp[temp_index] = line[i];
            temp_index++;
        }
            else if (temp[0] != '\0' && (isdigit(temp[temp_index-1]) || temp[temp_index-1] == 32 || temp[temp_index-1] == 9)  ){
            temp[temp_index] = line[i];
            temp_index++;
        }
            else{
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;
        }
       }
        
        /*
        if current char is "+" or "-"
        */
       else if(line[i] == 43 || line[i] == 45){
            if(temp[0] == '\0'){
                temp[temp_index] = line[i];
                temp_index++;
            }
            else if(temp[0] != '\0' && is_only_white_chars(temp)){
                temp[temp_index] = line[i];
                temp_index++;
            }
            else{
                printf("Invalid syntax in line: %s\n", line);
                *error_counter = *error_counter + 1;
                return false;
            }
       }
       
    
        /*
        if current char is ","
        */
       else{
            if(temp[0] == '\0'){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;
            }
            else if(temp[0] != '\0' && !is_number_in(temp)){
                printf("Invalid syntax in line: %s\n", line);
                *error_counter = *error_counter + 1;
                return false;
            }
            else{
                int j;
                for(j =0; j<= temp_index ; j ++){
                    temp[j] = '\0';
                }
                temp_index = 0;
            }
       }      
    }
        /*
        reset "temp"
        */
        int k;
        for(k =0; k<= temp_index ; k ++){
            temp[k] = '\0';
        }
        return true;
}
    


bool is_empty(const char *line){
    int i; 
    for(i = 0; i < strlen(line); i++){
        if(!isspace(line[i])){
            return false;
        }
    }
    return true;
}
    
bool is_comment(const char *line){
    int i; 
    for(i = 0; i < strlen(line); i++){
        if(isspace(line[i])){
            continue;
        }
        if(line[i] == 59)
            return true;
        else    
            return false;
    }
}


bool is_string(const char *line, int *error_counter)
{
    char *word = ".string";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }

   /*
   start_index = starting index of ".string"
   end_index = end index of ".string"
   */
    int start_index = result - line;
    int end_index = start_index + 7;
    /*
    checking that there's a space or tab afer ".string"
    */
    if(line[end_index] != 32 && line[end_index] != 9){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    
    /*
    checking that there are only 2 quotation marks (")
    */
    int i; 
    int qm_counter = 0;
    for(i = end_index; i< strlen(line) ; i++){
        if(line[i] == 34)
            qm_counter ++;
    }
    
    if(qm_counter != 2){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }


    /*
    now we know there are exactly 2 quotation marks. 
    Lets validate that they are first and last.
    */

   /*
    if quotation mark isnt first return false
   */
   for(i = end_index; i < strlen(line) -2; i++){
        if(isspace(line[i]))
            continue;
    
        if(line[i] != 34){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
        else{
            break;
        }
    }

    /*
    if quotation mark isnt last return false
    */
    for(i = strlen(line) -2; i > end_index ; i--){
        if(isspace(line[i])){
            continue;
        }
  
        if(line[i] != 34){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
        else{
            break;
        }
    }
    return true;
}

bool is_entry(const char *line, int *error_counter){
    
    char *word = ".entry";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }

   /*
   start_index = starting index of ".entry"
   end_index = end index of ".entry"
   */
    int start_index = result - line;
    int end_index = start_index + 6;
    
   
    /*
    checking that there's a space or tab afer ".entry"
    */
    if(line[end_index] != 32 && line[end_index] != 9){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    bool error_found = false;
    int last_char_index = get_last_char(line, strlen(line) -2);
    int first_char_index = get_first_char(line, 6);
    int index = first_char_index;

    if(!((line[index] >= 65 && line[index] <= 90) || (line[index] >= 97 && line[index] <= 122)))
        error_found = true;

    if(last_char_index -  first_char_index > 30)
        error_found = true;
  
    
    if(is_spaces(line, first_char_index, last_char_index))   
        error_found = true;

    bool isValidSymbolName = is_valid_symbol_name(line, first_char_index, last_char_index);
    if(!isValidSymbolName)
        error_found = true;
    
    if(error_found){
        printf("Invalid syntax in line: %s", line);
        *error_counter = *error_counter + 1;
        return false;
    }
    return true;
}

bool is_spaces(const char *line, int start, int end){
    int i;
    for(i= start; i< end; i++){
        if(isspace(line[i]))
            return true;
    }
    return false;
}

 bool is_extern(const char *line, int *error_counter){
    
    char *word = ".extern";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }

   /*
   start_index = starting index of ".extern"
   end_index = end index of ".extern"
   */
    int start_index = result - line;
    int end_index = start_index + 7;
    /*
    checking that there's a space or tab afer ".entry"
    */
    if(line[end_index] != 32 && line[end_index] != 9){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    bool error_found = false;
    int last_char_index = get_last_char(line, strlen(line) -2);
    int first_char_index = get_first_char(line, 7);
    int index = first_char_index;

    if(!((line[index] >= 65 && line[index] <= 90) || (line[index] >= 97 && line[index] <= 122)))
        error_found = true;

    if(last_char_index -  first_char_index > 30)
        error_found = true;
  
    
    if(is_spaces(line, first_char_index, last_char_index))   
        error_found = true;

    bool isValidSymbolName = is_valid_symbol_name(line, first_char_index, last_char_index);
    if(!isValidSymbolName)
        error_found = true;
    
    if(error_found){
        printf("Invalid syntax in line: %s", line);
        *error_counter = *error_counter + 1;
        return false;
    }
    return true;
 }

void find_symbol_indexes(const char *line, int  *start_index, int *end_index){
    int i=0;
    for(i ; i < strlen(line); i++){
        if(isspace(line[i]))
            continue;
        break;
    }
    *start_index = i;

    for(i = *start_index +1 ; i < strlen(line); i++){
        if(isspace(line[i]))
            break;
    }
    *end_index = i;
}

void find_external_symbol_indexes(const char *line, int *start, int *end){
    int len = strlen(line);
    int first_char = get_first_char(line, 0);
    first_char += 7;
    first_char = get_first_char(line, first_char);
    int last_char = get_last_char(line, strlen(line)-1);
    *start = first_char;
    *end = last_char +1;
} 


void addExternSymbol(Array *symbols_table, int *error_counter, const char *line, int *line_num){
    int start_index;
    int end_index;
    find_external_symbol_indexes(line, &start_index, &end_index);
    
     char symbol[LINE_SIZE+1]; // define a character array with a fixed maximum length

    // Copy substring to 'symbol' string
    int i;
    for (i = start_index; i <= end_index - 1; i++) {
        symbol[i - start_index] = line[i];
    }
    symbol[i - start_index] = '\0';

    int index = searchArray(symbols_table, symbol);

    if (index == -1){
        addArray(symbols_table, symbol, line_num);
    }
        
    else {
        printf("Error: multiple definitions of symbol '%s'\n", symbol);
        *error_counter = *error_counter + 1;
    }
}

void addSymbol(Array *symbols_table, int *error_counter, const char *line, int *line_num){
    
   /*
    1. find symbol in line and save it as a variable
    2. search that symbol in symbol table
    3. if found - print error message
    4. else - add symbol
    */

    int start_index = 0;
    int end_index = 0;
    char *isExtern = strstr(line, ".extern");
    if(isExtern != NULL){
        addExternSymbol(symbols_table, error_counter, line, line_num);
        return;
    }
    
    find_symbol_indexes(line, &start_index, &end_index);

    char symbol[LINE_SIZE+1]; // define a character array with a fixed maximum length

    // Copy substring to 'symbol' string
    int i;
    for (i = start_index; i <= end_index - 1; i++) {
        symbol[i - start_index] = line[i];
    }
    symbol[i - start_index] = '\0';

    int index = searchArray(symbols_table, symbol);

    if (index == -1){
        addArray(symbols_table, symbol, line_num);
    }
        
    else {
        printf("Error: multiple definitions of symbol '%s'\n", symbol);
        *error_counter = *error_counter + 1;
    }
}


bool valid_instruct(const char *line, const char **instructions, int *error_counter){
    
    /*
    1. check if there's a symbol in line (if there's a ":")
    2. if there is: find its index and loop from one index after it until end of instruction
    3. now we have start index and end index of instruction. we skip white chars before the instruction
    4. save it as a string of its own(?) and check if it is in instructions array
    */
   
   
   /*
   checking if there's a colon in line (if there is --> we have a symbol in line)
   */
   int colon_index = -1;
   int i;
   for (i = 0; i < strlen(line); i++){
        if(line[i] == 58){
            colon_index = i;
            break;
        }
   }
   /*
   if we didnt find a colon (symbol): start searching at index 0
   */
   bool found_char = false;
   int start_index = -1;
    if(colon_index == -1){
        for(i = 0; i < strlen(line); i++){
            if(isspace(line[i]) && !found_char)
                continue;
            if(!found_char)
                start_index = i;
            found_char = true;

            if(isspace(line[i]) && found_char){
                break;
            }
            if(!(line[i] >=65 && line[i] <= 90) && !(line[i] >= 97 && line[i] <= 122)){
                return false;
            }
        }
    }
    /*
    found a colon --> start search at index ("colon_index" + 1)
    */
    else{
        for(i = (colon_index+1); i < strlen(line); i++){
            if(isspace(line[i]) && !found_char)
                continue;
            if(!found_char)
                start_index = i;
            found_char = true;

            if(isspace(line[i]) && found_char){
                break;
            }
            if(!(line[i] >=65 && line[i] <= 90) && !(line[i] >= 97 && line[i] <= 122)){
                return false;
            }
        }
    }
    int end_index = i;
    
    /*
    now we have start and end index of op code in line. start index is first char, end index is space/tab after op code
    now we need to copy substring to new variable and search it in symbols table
    */
    
    char *opcode = malloc(end_index - start_index + 2);

    /*
    copy substring to copy string
    */ 
    for (int i = start_index; i <= end_index -1; i++) {
        opcode[i - start_index] = line[i];
    }
    opcode[end_index - start_index] = '\0'; 
    /*
    now we need just to check if opcode is in symbols table
    */
    int index = -1;
    for (i = 0; instructions[i] != NULL; i++){
         if (strcmp(instructions[i], opcode) == 0) {
            index = i;
            break;
        }
    }
    
    if(index == -1){
        return false;
    }
    free(opcode); 
    return true;
}

int calc_binary_lines_num(const char *line){
    return -1;
}

// void create_binary_from_line(const char *line, int num_binary_lines, FILE *p_outputFile){
    
// }
