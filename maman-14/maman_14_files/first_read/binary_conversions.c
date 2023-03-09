#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//global arrays:
    const char* instructions[] = {"mov", "cmp", "add", "sub","not","clr","lea",
    "inc", "dec", "jmp", "bne", "red","prn","jsr","rts","stop", NULL};
    const char* registers[] = {"r0","r1","r2","r3","r4","r5","r6","r7", NULL};

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
char *operands_params(char *orig, char *dest){
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

void string_to_binary(char* str){
    // Find the first occurrence of a quote
    char* start = strchr(str, '\"');

    // Loop over each character between the opening and closing quotes
    for (char* p = start + 1; *p!='"'; p++) {
        printf("%s\n", decimalToBinary((int)*p,14));//remember to free
    }
}

void data_to_binary(char* data){
    char* word;
    while((word=get_next_word(data)) != NULL)
        printf("%s\n",decimalToBinary(atoi(word),14));//remember to free
}


int main() {
    
    //local stuff:
    const char* first_type[] = {"mov", "cmp", "add", "sub","lea", NULL}; //2 operands
    const char* second_type[] = {"not","clr","inc", "dec", "jmp", "bne", "red","prn","jsr", NULL}; //1 operand
    const char* third_type[] = {"rts","stop", NULL};// 0 operands
    const char* jump_addressing[] = {"jmp","bne","jsr", NULL};
    char *final_line = malloc(sizeof(char) * 14);
    
    char opecode[30];
    char symbol[30];
    char src_operand[30];
    char dest_operand[30];
    char *ope_bin;
    char *address_type;
    char *params;
    
    //lines for testing:
    char line1[] = "MAIN: mov r3,LENGTH";
    char line2[] = "sub r1, r4";
    char line3[] = "LOOP: jmp L1(#-1,r6)";
    char line4[] = "END: stop";
    char line5[] = "STR: .string \"abcdef\"";
    char line6[] = "LENGTH: .data 6,-9,15,22,0";
    char line[80];
    strcpy(line, line6);
    
    //checks if symbol to skip it
    strcpy(opecode,get_next_word(line));
    if(is_symbol(opecode)){
        strcpy(opecode,get_next_word(line));
    }

    if(find_string(third_type, opecode) != -1){
        //printf("third type\n");
        strcat(final_line, "0000");//bits 13-10
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(final_line, ope_bin);//bits 9-6
        free(ope_bin);
        strcat(final_line, "0000");//bits 5-2
        strcat(final_line, "00");//bits 1-0
    }
    else if(find_string(second_type, opecode) != -1){
        //printf("second type\n");
        if(find_string(jump_addressing, opecode) != -1){
            //printf("jumping\n");
            strcpy(symbol,get_next_word(line));
            strcpy(src_operand,get_next_word(line));
            strcpy(dest_operand,get_next_word(line));
            
            params = operands_params(src_operand,dest_operand);
            strcat(final_line,params);//bits 13-10
            free(params);
            
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(final_line, ope_bin);//bits 9-6
            free(ope_bin);
            strcat(final_line,"0010");//bits 5-2
            strcat(final_line, "00");//bits 1-0
        }
        else{
            strcpy(dest_operand,get_next_word(line));
            strcat(final_line, "0000");//bits 13-10
            ope_bin = opecode_to_binary(instructions,opecode);
            strcat(final_line, ope_bin);//bits 9-6
            free(ope_bin);
            address_type = operands_params(NULL,dest_operand);
            strcat(final_line,address_type);//bits 5-2
            free(address_type);
            strcat(final_line, "00");//bits 1-0
        }
    }
    else if(find_string(first_type, opecode) != -1){
        //printf("first type\n");
        strcpy(src_operand,get_next_word(line));
        strcpy(dest_operand,get_next_word(line));
        strcat(final_line, "0000");//bits 13-10
        ope_bin = opecode_to_binary(instructions,opecode);
        strcat(final_line, ope_bin);//bits 9-6
        free(ope_bin);
        address_type = operands_params(src_operand,dest_operand);
        strcat(final_line,address_type);//bits 5-2
        free(address_type);
        strcat(final_line, "00");//bits 1-0
    }
    else if(strcmp(opecode,".string") == 0){
        string_to_binary(line);
    }
    else if(strcmp(opecode,".data") == 0){
        data_to_binary(line);
    }
    
    
    printf("line: %s\n", final_line);
    return 0;
}

