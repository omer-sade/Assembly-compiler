#include <stdio.h>
#include <string.h>
/*
#include "second_read.h"



void symbolToBinary(Binary_table *instructions_table, Array *symbols_table){
    //test prints:
    int line_num = 0;
    char *temp_bin = (char*)calloc(15, sizeof(char));
    int external = 0; //flag to check if it is external or local symbol
    for (int i = 0; i < instructions_table->size; i++) {
        if(instructions_table->table[i].bin_str[0] == '?'){ //get the symbol: &instructions_table->table[i].bin_str[1]
            //printf("symb: %s\n", &instructions_table->table[i].bin_str[1]);
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
}*/

char* int_to_four_char_string(int input) {
    static char output[5];
    sprintf(output, "%04d", input);
    return output;
}

void convert_to_dots_slashes(char *input, char *output) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '0') {
            output[i] = '.';
        } else if (input[i] == '1') {
            output[i] = '/';
        } else {
            printf("Invalid input string!\n");
            return;
        }
    }
    output[i] = '\0';
}

void get_length_and_converted_string(int num, char *input, FILE *fp) {
    char converted[strlen(input) + 1];
    convert_to_dots_slashes(input, converted);
    fprintf(fp, "%s\t%s\n", int_to_four_char_string(num), converted);
}

int main() {
    FILE *fp = fopen("test.txt", "w");
    if (fp == NULL) {
        printf("Failed to open file for writing.\n");
        return 0;
    }
    get_length_and_converted_string(100, "00000000110100", fp);
    get_length_and_converted_string(101, "00001100000000", fp);
    get_length_and_converted_string(102, "00001000001010", fp);
    get_length_and_converted_string(103, "00111001001000", fp);
    get_length_and_converted_string(104, "00000111010010", fp);
    fclose(fp);
    return 0;
}
