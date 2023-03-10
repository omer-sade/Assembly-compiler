#include <stdio.h>
#include <stdlib.h>
#include "validate_input.h"

bool is_valid_line_opcode(const char *line){

    int start_index =0; 
    int end_index = 0; 
    get_opcode_indexes(line, &start_index, &end_index);
    char opcode[LINE_SIZE + 1]; // define a character array with a fixed maximum length

    // Copy substring to 'opcode' string
    int i;
    for (i = start_index; i <= end_index - 1; i++) {
        opcode[i - start_index] = line[i];
    }
    opcode[i - start_index] = '\0';
   
    int needed_operands_num = calc_needed_operands_num(opcode);
    
    int acuall_operands_num = calc_acuall_operands_num(line, end_index);

    if(needed_operands_num != acuall_operands_num)
        return false;
    
    /*
    validating that operands doesnt have spaces.
    ex: mov r 4, r 5 --> return false
    */
    bool isValid = false;
    if(acuall_operands_num == 0)
        isValid = check_valid_0_operand(line, end_index);
    else if(acuall_operands_num == 1)
        isValid = check_valid_1_operand(line,opcode, end_index);
    else
        isValid = check_valid_2_operand(line, opcode, end_index);
    
    if(!isValid)
        return false;
    
    
    bool correct_operadns_type = validate_operands_type(line, end_index);
    return true;
}


bool validate_operands_type(const char *line, int index){

}
int calc_acuall_operands_num(const char *line, int index){
    int i = index;
    bool is_empty = true;
    for(i; i< strlen(line); i ++){
        if(!isspace(line[i])){
            is_empty = false;
            break;
        }
    }
    if(is_empty)
        return 0;
    
    int coma_counter = 0;
    i = index;
    for(i; i < strlen(line); i ++){
        char x = line[i];
        if(line[i] == 44)
            coma_counter ++;
    }
    return coma_counter + 1;
    
}

int calc_needed_operands_num(char opcode[]){
    if(strcmp("mov", opcode) ==0)
        return 2;
    if(strcmp("cmp", opcode) ==0)
        return 2;
    if(strcmp("add", opcode) ==0)
        return 2;
    if(strcmp("sub", opcode) ==0)
        return 2;
    if(strcmp("lea", opcode) ==0)
        return 2;
    if(strcmp("not", opcode) ==0)
        return 1;
    if(strcmp("clr", opcode) ==0)
        return 1;
    if(strcmp("inc", opcode) ==0)
        return 1;
    if(strcmp("dec", opcode) ==0)
        return 1;
    if(strcmp("jmp", opcode) ==0)
        return 1;
    if(strcmp("bne", opcode) ==0)
        return 1;
    if(strcmp("red", opcode) ==0)
        return 1;
    if(strcmp("prn", opcode) ==0)
        return 1;
    if(strcmp("jsr", opcode) ==0)
        return 1;
    if(strcmp("rts", opcode) ==0)
        return 0;
    if(strcmp("stop", opcode) ==0)
        return 0;
    return -1;
}

int get_colon_index(const char *line){
    int i;
    for(i=0; i < strlen(line); i ++){
        if(line[i] == 58)
        return i;
    }
    return -1;
}

bool check_valid_0_operand(const char *line, int index){
    int i;
    for(i = index; i < strlen(line); i++){
        if(!isspace(line[i]))
            return false;
    }
    return true;
}

bool check_valid_1_operand(const char *line, char operand[], int index){
    int first_op_char = -1;
    int last_op_char = -1;
    int i; 
    for(i = index; i < strlen(line); i++){
        if(isspace(line[i]))
            continue;
        first_op_char = i;
        break;
    }
    for(i = strlen(line) - 1; i> first_op_char; i--){
        if(isspace(line[i]))
            continue;
        last_op_char = i;
        break;
    }
    for(i = first_op_char; i < last_op_char; i++){
        if(isspace(line[i]))
            return false;
    }
    return true;
}


bool check_valid_2_operand(const char *line, char operand[], int index){
    int coma_index = -1;
    int first_op_char = -1;
    int last_op_char = -1;
    int i; 
    /*
    validating no space is first operand
    */
    for(i = index; i < strlen(line); i++){
        if(line[i] == 44){
            coma_index = i;
            break;
        }
    }
    for(i = index; i < coma_index; i ++){
        if(isspace(line[i]))
            continue;
        first_op_char = i;
        break;
    }
    for(i = coma_index-1; i > first_op_char; i--){
        if(isspace(line[i]))
            continue;
        last_op_char = i;
    }
    for(i = first_op_char; i < last_op_char; i ++){
        if(isspace(line[i]))
            return false;
    }
    /*
    validating no space is second operand
    */
    for(i = (coma_index+1); i < strlen(line); i ++){
        if(isspace(line[i]))
            continue;
        first_op_char = i;
        break;
    }
    for(i = (strlen(line)-2); i > first_op_char; i--){
        if(isspace(line[i]))
            continue;
        last_op_char = i;
    }
    for(i = first_op_char; i < last_op_char; i ++){
        if(isspace(line[i]))
            return false;
    }
    
    return true;
}
void get_opcode_indexes(const char *line, int *start_index, int *end_index){

    int i=0;
    int colon_index = get_colon_index(line);
    
    if(colon_index == -1){
        for(i = 0; i< strlen(line); i++){
            if(isspace(line[i])){
                continue;
            }
            *start_index = i;
            break;
        }
        if(isspace(line[*start_index + 3]))
            *end_index = *start_index + 3;
        else
            *end_index = *start_index + 4;
    }
    else{
        for(i = (colon_index +1) ; i< strlen(line); i++){
            if(isspace(line[i])){
                continue;
            }
            *start_index = i;
            break;
        }
        if(isspace(line[*start_index + 3]))
            *end_index = *start_index + 3;
        else
            *end_index = *start_index + 4;
    }
}
