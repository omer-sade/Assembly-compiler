#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "binary_conversions.h"


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

int find_string(const char **array, const char *str) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], str) == 0) {
            return i;
        }
    }
    return -1; // not found
}


char* opecode_to_binary(const char **arr, const char *str){
    int num;
    num = find_string(arr,str);
    return decimalToBinary(num,4);
}

char *get_next_word(char *string) {
    static char word[80]; // static variable to store the word
    static int position = 0; // static variable to keep track of the position
    int i = position, j = 0;
    
    // skip any spaces or commas
    while (isspace(string[i]) || string[i] == ',' || \
    string[i] == '(' || string[i] == ')') {
        i++;
    }
    
    // copy the characters of the word to the word buffer
    while (string[i] != '\0' && !isspace(string[i]) && string[i] != ',' \
    && string[i] != '(' && string[i] != ')') {
        word[j] = string[i];
        i++;
        j++;
    }
    
    // terminate the word buffer with null character
    word[j] = '\0';
    
    // update the position variable to the next word
    position = i;
    
    // return the word buffer
    return word[0] != '\0' ? word : NULL;
}


int is_symbol(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == ':') {
        return 1;
    }
    return 0;
}

//creating the 4 digit bit type address of the origin,destination operands
char *operands_params(char *orig, char *dest, const char** registers){
    char *bit_str = malloc(sizeof(char) * 4);
    if (orig == NULL){
        strcpy(bit_str,"00");
        
    }
    else if(orig != NULL){
        if(find_string(registers, orig) != -1){ //Type 3
            strcpy(bit_str,"11");
        }
        else if(orig[0] == '#'){ //Type 0
            strcpy(bit_str, "00");
        }
        else{ //type 1
            strcpy(bit_str, "01");
        }
    }
    
    if(find_string(registers, dest) !=-1){ //Type 3
        strcat(bit_str,"11");
    }
    else if(dest[0] == '#'){ //Type 0
        strcat(bit_str, "00");
    }
    else{ //type 1
        strcat(bit_str, "01");
    }
    return bit_str;
}

void string_to_binary(char* str, Binary_table *binary_table){
    // Find the first occurrence of a quote
    char* start = strchr(str, '\"');

    // Loop over each character between the opening and closing quotes
    for (char* p = start + 1; *p!='"'; p++) {
        char *temp;
        temp = decimalToBinary((int)*p,14);
        addBinaryLine(binary_table, temp);
        free(temp);
    }
    addBinaryLine(binary_table, "00000000000000");
}

void data_to_binary(char* data, Binary_table *binary_table){
    char* word;
    while((word=get_next_word(data)) != NULL){
        char *temp;
        temp = decimalToBinary(atoi(word),14);
        addBinaryLine(binary_table, temp);
        free(temp);
    }
}

char* registers_addressing(char* orig_reg, char* dest_reg) {
    char *final_line = malloc(sizeof(char) * 14);
    final_line[0] = '\0';
    char* reg_part1;
    char* reg_part2;
    if(orig_reg == NULL) {
        /*only destination register exists*/
        reg_part1 = decimalToBinary(atoi(&dest_reg[1]),6);
        
        strcat(final_line,"000000"); /* no orig reg */
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else if(dest_reg == NULL) {
        /*only origin register exists*/
        reg_part2 = decimalToBinary(atoi(&orig_reg[1]),6);
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"000000"); /* no dest reg */
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
    else {
        /*both registers exists*/
        reg_part1 = decimalToBinary(atoi(&orig_reg[1]),6);
        reg_part2 = decimalToBinary(atoi(&dest_reg[1]),6);
        strcat(final_line,reg_part1); /*adds the dest reg binary value*/
        strcat(final_line,reg_part2); /*adds the orig reg binary value*/
        strcat(final_line,"00"); /*A,R,E value*/
        return final_line;
    }
}

void other_words(char* src_operand, char* dest_operand, const char** registers, Binary_table *binary_table){
    if(src_operand == NULL){
        char *temp;
        if(find_string(registers,dest_operand)!=-1){//if dest a register
            temp = registers_addressing(NULL, dest_operand);
            addBinaryLine(binary_table, temp);
            free(temp);
        }
        else if(dest_operand[0]=='#'){//if dest a number
            temp = decimalToBinary(atoi(dest_operand+1),12);
            strcat(temp, "00");
            addBinaryLine(binary_table, temp);
            free(temp);
        }
        else{//if dest a symbol
            addBinaryLine(binary_table, "?");
        }
        return;
    }
    else if((find_string(registers,src_operand) != -1) && (find_string(registers,dest_operand) != -1)){//if both registers
        char *temp1;
        temp1 = registers_addressing(src_operand, dest_operand);
        addBinaryLine(binary_table, temp1);
        free(temp1);
        return;
    }
    else if(src_operand != NULL){
        char *temp2;
        if(find_string(registers,src_operand)!=-1){//if source a register
            temp2 = registers_addressing(src_operand, NULL);
            addBinaryLine(binary_table,temp2);
            free(temp2);
        }
        else if(src_operand[0]=='#'){//if source a number
            temp2 = decimalToBinary(atoi(src_operand+1),12);
            strcat(temp2, "00");
            addBinaryLine(binary_table,temp2);
            free(temp2);
        }
        else{//if source a symbol
            addBinaryLine(binary_table,"?");
        }
    }
    char *temp3;
    if(find_string(registers,dest_operand)!=-1){//if dest a register
        temp3 = registers_addressing(NULL, dest_operand);
        addBinaryLine(binary_table,temp3);
        free(temp3);
    }
    else if(dest_operand[0]=='#'){//if dest a number
        temp3 = decimalToBinary(atoi(dest_operand+1),12);
        strcat(temp3, "00");
        addBinaryLine(binary_table, temp3);
        free(temp3);
    }
    else{//if dest a symbol
        addBinaryLine(binary_table, "?");
    }
}

void create_binary_from_line(const char *cur_line, const char** instructions, const char** registers, Binary_table *binary_table){
    const char* first_type[] = {"mov", "cmp", "add", "sub","lea", NULL}; //2 operands
    const char* second_type[] = {"not","clr","inc", "dec", "jmp", "bne", "red","prn","jsr", NULL}; //1 operand
    const char* third_type[] = {"rts","stop", NULL};// 0 operands
    const char* jump_addressing[] = {"jmp","bne","jsr", NULL};
    char *first_word = malloc(sizeof(char) * 14);
    char opecode[80], src_operand[80], dest_operand[80], line[80];
    char *ope_bin, *address_type, *params;
    strcpy(line, cur_line);

    //checks if symbol to skip it
    strcpy(opecode,get_next_word(line));
    if(is_symbol(opecode)){
        strcpy(opecode,get_next_word(line));
    }

    if(find_string(third_type, opecode) != -1){
        strcat(first_word, "0000");//bits 13-10
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(first_word, ope_bin);//bits 9-6
        free(ope_bin);
        strcat(first_word, "0000");//bits 5-2
        strcat(first_word, "00");//bits 1-0
        addBinaryLine(binary_table,first_word);
        free(first_word);
    }
    else if(find_string(second_type, opecode) != -1){
        char *check_word;
        char first_operand[80];
        strcpy(first_operand,get_next_word(line));
        if((check_word = get_next_word(line)) != NULL){
            strcpy(src_operand,check_word);
            strcpy(dest_operand,get_next_word(line));
            
            params = operands_params(src_operand,dest_operand, registers);
            strcat(first_word,params);//bits 13-10
            free(params);
            
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(first_word, ope_bin);//bits 9-6
            free(ope_bin);
            strcat(first_word,"0010");//bits 5-2
            strcat(first_word, "00");//bits 1-0
            addBinaryLine(binary_table, first_word);
            free(first_word);
            addBinaryLine(binary_table, "?");
            other_words(src_operand, dest_operand, registers, binary_table);
        }
        else{
            strcpy(dest_operand,first_operand);
            strcat(first_word, "0000");//bits 13-10
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(first_word, ope_bin);//bits 9-6
            free(ope_bin);
            address_type = operands_params(NULL,dest_operand, registers);
            strcat(first_word,address_type);//bits 5-2
            free(address_type);
            strcat(first_word, "00");//bits 1-0
            addBinaryLine(binary_table, first_word);
            free(first_word);
            other_words(NULL, dest_operand, registers, binary_table);
        }
    }
    else if(find_string(first_type, opecode) != -1){
        strcpy(src_operand,get_next_word(line));
        strcpy(dest_operand,get_next_word(line));
        strcat(first_word, "0000");//bits 13-10
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(first_word, ope_bin);//bits 9-6
        free(ope_bin);
        address_type = operands_params(src_operand,dest_operand, registers);
        strcat(first_word,address_type);//bits 5-2
        free(address_type);
        strcat(first_word, "00");//bits 1-0
        addBinaryLine(binary_table,first_word);
        other_words(src_operand, dest_operand, registers, binary_table);
    }
    else if(strcmp(opecode,".string") == 0){
        string_to_binary(line, binary_table);
    }
    else if(strcmp(opecode,".data") == 0){
        data_to_binary(line, binary_table);
    }
}

// Function to initialize a new binary_table
void initBinaryTable(Binary_table *tablePtr) {
    tablePtr->table = (Binary_line *) malloc(10 * sizeof(Binary_line)); // Allocate memory for 10 binary_line elements
    tablePtr->size = 0; // Initialize the size of the array to 0
}

// Function to add a new binary_line element to the dynamic array
void addBinaryLine(Binary_table *tablePtr, char *bin_str) {
    // Double the size of the array if necessary
    if (tablePtr->size % 10 == 0) {
        tablePtr->table = (Binary_line *) realloc(tablePtr->table, (tablePtr->size + 10) * sizeof(Binary_line));
    }

    // Initialize the new binary_line element and add it to the array
    Binary_line newLine = {tablePtr->size+100, ""};
    strncpy(newLine.bin_str, bin_str, 14); // Copy the first 14 characters of the string
    tablePtr->table[tablePtr->size] = newLine;

    // Update the size of the array
    (tablePtr->size)++;
}

int main() {
    //from outside
    const char* instructions[] = {"mov", "cmp", "add", "sub","not","clr","lea",
    "inc", "dec", "jmp", "bne", "red","prn","jsr","rts","stop", NULL};
    const char* registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7", NULL};

    //lines for testing:
    char line1[] = "MAIN: mov r3,LENGTH";
    char line2[] = "sub r1, r4";
    char line3[] = "LOOP: jmp L1(#-1,r6)";//issue with last one
    char line4[] = "END: stop";
    char line5[] = "STR: .string \"abcdef\"";
    char line6[] = "LENGTH: .data 6,-9,15";
    char line7[] = "LOOP: bne LOOP(r4,r5)";//issue 
    char line8[] = "bne END";
    char line9[] = "bne LOOP(K,STR)";
    

    Binary_table binaryTable; // Declare a new binary_table
    initBinaryTable(&binaryTable); // Initialize the binary_table

    create_binary_from_line(line3, instructions, registers, &binaryTable);
    
    // Print the elements of the array
    for (int i = 0; i < binaryTable.size; i++) {
        printf("line_num: %d, bin_str: %s\n", binaryTable.table[i].line_num, binaryTable.table[i].bin_str);
    }

    // Print the size of the array
    printf("Size of the array: %d\n", binaryTable.size);

    // Free the memory allocated for the array
    free(binaryTable.table);
    
    
    return 0;
}

