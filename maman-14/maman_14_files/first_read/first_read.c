#include <stdio.h>
#include <stdlib.h>
#include "first_read.h"


void reading_file_first_time(Array *symbols_table, Array *insturctions, FILE *p_outputFile){
    
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
       /*
       printf("line: %s\n", line);
       */
       
        if(has_symbol(line, &error_counter))
            is_symbol_found = true;
        
        if(is_data(line, &error_counter) || is_string(line, &error_counter)){
            if(is_symbol_found){
                addSymbol(symbols_table, &error_counter, line, &DC);
            
            }
           
        }
        if(is_entry(line) || is_extern(line)){
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
       if(!valid_instruct(line, insturctions)){
            char *msg = "Invalid instruction in line:\n";
            printf("%s\n", msg);
            printf("%s\n",line);
            error_counter ++;
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
        free(insturctions->data);
        fclose(p_outputFile);
        exit(1);
    
    
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
        
        char *message = "Invalid symbol declaration in line: ";
       
        printf("%s", message);
        printf(" %s\n", line);
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
    bool is_contains_number(char str[]){
        int i;
        
        if(str[0] == '\0')
            return false;

        for(i = 0; i < strlen(str); i ++){
            
            if(isspace(str[i]))
                continue;
            if(isdigit(str[i]))
                return true;
        }
        return false;
    }


bool is_data(const char *line, int *error_counter)
{
    

   /*
   checking if the word ".data" is in the current line. if it is --> strstr() returns a pointer to starting index
   ex: line = "my name is john"
       word = "name"
       result = "name is john"

   */
    char *word = ".data";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }


    /*
    if we got here - ".data" is in line. now we need to look for syntax errors. 
    */

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
            bool has_number = is_contains_number(temp);
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
    
   
    

    


bool is_string(const char *line, int *error_counter)
{
    char *word = ".string";
    char *result = strstr(line, word);
    if( result == NULL){
        return false;
    }


    /*
    if we got here - ".string" is in line. now we need to look for syntax errors. 
    */

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
        /*
        if quotation mark isnt first - invalid syntax
        */
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
            
        /*
        if quotation mark isnt last - invalid syntax
        */
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

bool is_entry(const char *line){
    return true;
}



 bool is_extern(const char *line){
    return true;
 }



void addSymbol(Array *symbols_table, int *error_counter, const char *line, int *DC){
    int i; 
    /*
    1. find symbol in line and save it as a variable
    2. search that symbol in symbol table
    3. if found - print error message
    4. else - add symbol
    */
}

bool valid_instruct(const char *line,Array *insturctions){
    
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
