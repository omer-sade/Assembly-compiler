#include <stdio.h>
#include <stdlib.h>
#include "first_read.h"

 bool is_valid_line_opcode(const char *line);


void reading_file_first_time(Array *symbols_table, char insturctions[][LINE_SIZE], FILE *p_outputFile){
    
    int DC = 0, IC = 0; 
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

        if(isData || isString){
            if(is_symbol_found){
                addSymbol(symbols_table, &error_counter, line, &DC);
                continue;
            }
           
        }
        bool isEntry = is_entry(line, &error_counter);
        bool isExtern = is_extern(line, &error_counter);
        if(isEntry || isExtern){
            /*
            didnt fully understand this part. this is line 9 in algorythm.
            */
           
        }
        /*
        If we got here than line doest contain data declaration nor extern / entry declaration. 
        Meaning it is instructions (with possibly a symbol).
        */
       if(is_symbol_found){
            addSymbol(symbols_table, &error_counter, line, &IC);
       }
       /*
       if there's no data, no string, no entry, no extern in line --> it has opcode
       */
        if(current_error_num == error_counter){
            if(!is_empty(line) && !isData && !isString && !isEntry && !isExtern){
                if(!valid_instruct(line, insturctions, &error_counter) ||!is_valid_line_opcode(line)){
                    printf("Invalid syntax in line: %s\n", line);
                    error_counter ++;
                }
            }
        }

        int num_binary_lines = calc_binary_lines_num(line);
        create_binary_from_line(line, num_binary_lines, p_outputFile);
        IC += num_binary_lines;
    }
    
    /*
    if errors found in file, terminate program
    */
    if(error_counter > 0){
        free(symbols_table->data);
        fclose(p_outputFile);
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

    /*
    counts how many ":" in line
    */
    int colon_counter = 0;

    /*
    checking how many ":" in line. if not 1, returns false
    */
    for( i = 0; i < strlen(line) ; i ++){
       if(line[i] == ':')
            colon_counter ++;
    }
    if(colon_counter == 0)
        return false;

    if(colon_counter > 1){
        printf("Invalid syntax, only 1 colon allowed - %s\n", line);
        *error_counter = *error_counter + 1;
        return false;
    }
    /*
    finding the colon index (":" is 58 in ascii)
    */
    int colon_index = -1;
    for( i = 0; i < strlen(line) ; i ++){
        if(line[i] == 58){
            colon_index = i;
            break;
        }
    }
    
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
  

    /*
    if error found:
    ex: line is: "gfd gf:"
    */
    if( last_space_index > first_char_index){       
        printf("Invalid symbol declaration in line: %s", line);
        *error_counter = *error_counter + 1;
        return false;
    }

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
    if we got here, there are only numbers, comas, hyphens, plus signs and white characters after ".data".
    we need to verify that everything is ok according to page 36 in pdf. 
    */

   /*
    if coma is first return false
   */
   for(i = end_index; i < strlen(line) -2; i++){
        if(isspace(line[i]))
            continue;
        /*
        if coma is first - invalid syntax
        */
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
        if(isspace(line[i])){
            continue;
        }
            
        /*
        if coma is last - invalid syntax
        */
        if(line[i] == 44 || line[i] == 45 || line[i] == 43){
            printf("Invalid syntax in line: %s\n", line);
            *error_counter = *error_counter + 1;
            return false;   
        }
        else{
            break;
        }
    }

    /* "temp" holds parts from "line", used to validate line syntax in following loop*/
    char temp[LINE_SIZE]; 
    temp[0] = '\0';
    int temp_index = 0;
    
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
    return true;
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
    checking that there's a space or tab afer ".extern"
    */
    if(line[end_index] != 32 && line[end_index] != 9){
        printf("Invalid syntax in line: %s\n",line);
        *error_counter = *error_counter + 1;
        return false;
    }
    return true;
 }

void find_symbol_indexes(const char *line, int  *start_index, int *end_index){
    int i;
    for(i = 0; i < strlen(line); i++){
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

void addSymbol(Array *symbols_table, int *error_counter, const char *line, int *DC){
    
   /*
    1. find symbol in line and save it as a variable
    2. search that symbol in symbol table
    3. if found - print error message
    4. else - add symbol
    */

    int start_index = 0;
    int end_index = 0;
    find_symbol_indexes(line, &start_index, &end_index);

    char symbol[LINE_SIZE + 1]; // define a character array with a fixed maximum length

    // Copy substring to 'symbol' string
    int i;
    for (i = start_index; i <= end_index - 1; i++) {
        symbol[i - start_index] = line[i];
    }
    symbol[i - start_index] = '\0';

    int index = searchArray(symbols_table, &symbol, sizeof(char[10]), cmpStr);

    if (index == -1)
        addArray(symbols_table, &symbol, sizeof(char[10]));
    else {
        printf("Error: multiple definitions of symbol '%s'\n", symbol);
        *error_counter = *error_counter + 1;
    }
}


bool valid_instruct(const char *line, char instructions[][LINE_SIZE], int *error_counter){
    
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
    for (i = 0; i < 16; i++){
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

char *pos_to_binary(unsigned int num, int bit_count) {
    // Allocate memory for the binary string
    char *binary = (char*)malloc(bit_count + 1);
    if (binary == NULL) {
        printf("Error: Failed to allocate memory.\n");
        return NULL;
    }
    // Convert the number to binary
    int i;
    for (i = 0; i < bit_count; i++) {
        binary[i] = '0';
    }
    int pos = bit_count - 1;
    while (num != 0 && pos >= 0) {
        if (num & 1) {
            binary[pos] = '1';
        }
        num = num >> 1;
        pos--;
    }
    // Return the binary string
    binary[bit_count] = '\0';
    return binary;
}

char *neg_to_binary(int num, int bit_count) {
    // Allocate memory for the binary string
    char *binary = (char*)malloc(bit_count + 1);
    if (binary == NULL) {
        printf("Error: Failed to allocate memory.\n");
        return NULL;
    }
    // Convert the number to binary using 2's complement
    int i;
    for (i = 0; i < bit_count; i++) {
        binary[i] = '0';
    }
    int pos = bit_count - 1;
    while (num != 0 && pos >= 0) {
        if (num & 1) {
            binary[pos] = '1';
        }
        num = num >> 1;
        pos--;
    }
    if (pos >= 0) {
        binary[pos] = '1';
        pos--;
        while (pos >= 0) {
            binary[pos] = '0';
            pos--;
        }
    }
    // Return the binary string
    binary[bit_count] = '\0';
    return binary;
}


char* decimalToBinary(int decimalNum, int size) {
    if (decimalNum >= 0) {
        return pos_to_binary(decimalNum, size);
    } else {
        return neg_to_binary(decimalNum, size);
    }
}

void opecode_to_binary(Array *arr, void *element){
    int num;
    num = searchArray(arr,element,sizeof(char[50]), cmpStr);
    printf("num = %d\n", num);
}

char* registers_addressing(char* orig_reg, char* dest_reg) {
    char *final_line = malloc(sizeof(char) * 14);
    char* reg_part1;
    char* reg_part2;
    if(orig_reg == NULL) {
        /*only destination register exists*/
        reg_part1 = decimalToBinary(atoi(&dest_reg[1]),6);
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,"000000"); /* no orig reg */
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else if(dest_reg == NULL) {
        /*only origin register exists*/
        reg_part2 = decimalToBinary(atoi(&orig_reg[1]),6);
        strcat(final_line,"000000"); /* no dest reg */
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else {
        /*both registers exists*/
        reg_part1 = decimalToBinary(atoi(&dest_reg[1]),6);
        reg_part2 = decimalToBinary(atoi(&orig_reg[1]),6);
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
}

void create_binary_from_line(const char *line, int num_binary_lines, FILE *p_outputFile){
    
}
