#include "second_read.h"

void symbolToBinary(Binary_table *instructions_table){
    //test prints:
    int line_num;
    char *temp_bin = (char*)calloc(15, sizeof(char));
    int external; //flag to check if it is external or local symbol
    for (int i = 0; i < instructions_table->size; i++) {
        if(instructions_table->table[i].bin_str[0] == '?'){ //get the symbol: &instructions_table->table[i].bin_str[1]
            //printf("symb: %s\n", &instructions_table->table[i].bin_str[1]);
            line_num = 0; //change it to the right number
            external = 0; //change it to the right condition
            temp_bin = decimalToBinary(line_num, 12);
            if(external){
                strcat(temp_bin, "01");
            }
            else if(!external){
                strcat(temp_bin, "10");
            }
            strcpy(instructions_table->table[i].bin_str,temp_bin);
        }
            instructions_table->table[i].line_num
    }
}


void convert_to_dots_slashes(char *input) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '0') {
            input[i] = '.';
        } else if (input[i] == '1') {
            input[i] = '/';
        } else {
            printf("The string should only contain 0s and 1s!\n");
            return;
        }
    }
    printf("%s\n", input);
}