#include <stdio.h>
#include <stdlib.h>
#include "validate_input.h"

bool is_valid_line_opcode(const char *line){
    int i;
    int start_index =0; 
    int end_index = 0; 
    /*
    validating that each opcode gets proper operand type.
    */
    bool correct_operadns_type;
    /*
    will hold opcode value found in line
    */
    char opcode[LINE_SIZE + 1]; 
    /*
    calc how many operands each opcode needs
    */
    int needed_operands_num;
    /*
    how many operands found in line
    */
    int acuall_operands_num;

    /*
    looking for illegal spaces
    */
    bool isValid;
    get_opcode_indexes(line, &start_index, &end_index);
    

    /*
     Copy substring to 'opcode' string
    */ 
    for (i = start_index; i <= end_index - 1; i++) {
        opcode[i - start_index] = line[i];
    }
    opcode[i - start_index] = '\0';
   
    needed_operands_num = calc_needed_operands_num(opcode);
    
    acuall_operands_num = calc_acuall_operands_num(line, end_index);
    if(strcmp(opcode, "jsr") !=0 && strcmp(opcode, "bne") !=0 && strcmp(opcode, "jmp") !=0){
        if(needed_operands_num != acuall_operands_num)
            return false;
    }
    
    /*
    validating that operands doesnt have spaces.
    ex: mov r 4, r 5 --> return false
    */
    isValid = false;
    if(strcmp(opcode, "jsr") ==0 || strcmp(opcode, "bne") ==0 || strcmp(opcode, "jmp") ==0){
        isValid = validate_jsr_bne_jmp(line, end_index);
    }
    else if(acuall_operands_num == 0)
        isValid = check_valid_0_operand(line, end_index);
    else if(acuall_operands_num == 1)
        isValid = check_valid_1_operand(line,opcode, end_index);
    else
        isValid = check_valid_2_operand(line, opcode, end_index);
    
    if(!isValid)
        return false;
    
    correct_operadns_type = validate_operands_type(line, opcode,  end_index);
    if(!correct_operadns_type)
        return false;
    return true;
}

int get_index_of(const char *line, char target, int start){
    int i; 
    for(i = start; i < strlen(line); i++){
        if(line[i] == target)
            return i;
    }
    return -1;
}

int count(const char *line, char target){
    int i;
    int count = 0;
    for (i = 0; i < strlen(line); i++){
        if(line[i] == target)
            count++;
    }
    return count;
}

bool validate_jsr_bne_jmp(const char *line, int index){
    bool spaces_in_symbol;
    int coma_counter = count(line, 44); 
    int i;
    
    /*
    counters
    */
    int open_parnte;
    int close_parente;

    /*
    variables to store specific char's index
    */
    int first_par_index;
    int last_par_index;
    int first_char_index;
    int coma_index;
    int last_char_index;
    
    if(coma_counter > 1)
        return false;
    
    if(coma_counter == 0){
        return check_valid_1_operand(line,"jmp", index);
    }
    else{
        open_parnte = count(line, 40);
        close_parente = count(line, 41);
        if(open_parnte != 1 && close_parente != 1)
            return false;
        first_par_index = get_index_of(line, 40, index);
        last_par_index =  get_index_of(line, 41, index);
        first_char_index = get_first_char(line, index);
        coma_index = get_index_of(line, 44, index);
        last_char_index = -1;
        for(i = first_par_index-1; i > first_char_index; i--){
            if(isspace(line[i]))
                return false;
            last_char_index = i;
            break;
        }
        spaces_in_symbol = is_contains_spaces(line, first_char_index, last_char_index);
        if(spaces_in_symbol)
            return false;
        if(!(first_par_index < last_par_index && first_par_index < coma_index && coma_index < last_par_index))
            return false;
        
       
       if(is_contains_spaces(line, first_char_index, last_par_index))
            return false;
      
    }
    return true;
}

bool is_contains_spaces(const char *line, int start, int end){
    int i;
    for(i= start; i< end; i++){
        if(isspace(line[i]))
            return true;
    }
    return false;
}

int get_first_char(const char *line, int start){
    int i; 
    for (i=start; i<strlen(line); i++){
        if(isspace(line[i]))
            continue;
        return i;
    }
    return -1;
}

int get_last_char(const char *line, int end){
    int i;
    for(i = end; i> 0; i--){
        if(isspace(line[i]))
            continue;
        return i;
    }
    return -1;
}

bool validate_operands_type(const char *line,char opcode[], int index){
    char arr1[][5] = {"mov", "add", "sub"};
    char arr2[][5] = {"cmp"};
    char arr3[][5] ={"lea"};
    char arr4[][5] = {"not", "clr", "inc", "dec", "red"};
    char arr5[][5] = {"bne", "jsr", "jmp"};

    int arr1_size = sizeof(arr1) / sizeof(arr1[0]);
    int arr2_size = sizeof(arr2) / sizeof(arr2[0]);
    int arr3_size = sizeof(arr3) / sizeof(arr3[0]);
    int arr4_size = sizeof(arr4) / sizeof(arr4[0]);
    int arr5_size = sizeof(arr5) / sizeof(arr5[0]);

    int coma_index = get_index_of(line, 44, index);
    int first_operand_first_char = get_first_char(line, index);
    int first_operand_last_char;
    int second_operand_first_char;
    int second_operand_last_char;
    if(coma_index != -1){
        first_operand_last_char = get_last_char(line, coma_index-1);
        second_operand_first_char = get_first_char(line, coma_index+1);
        second_operand_last_char = get_last_char(line, strlen(line)-2);
    }
    else{
        first_operand_last_char = get_last_char(line, strlen(line)-2);
    }
    
    /*
    2 operands, first is 0 1 3, second is 1 3
    */
    if(contains(arr1,arr1_size, opcode)){
        bool isNum = is_number(line, first_operand_first_char,first_operand_last_char+1);
        bool isSymbol = is_symbol(line, first_operand_first_char, first_operand_last_char+1);
        bool isRegi = is_register(line, first_operand_first_char, first_operand_last_char+1);
        if(!(isNum || isSymbol || isRegi))
            return false;
        
        isSymbol = is_symbol(line, second_operand_first_char, second_operand_last_char);
        isRegi = is_register(line, second_operand_first_char,second_operand_last_char);
        if(!(isSymbol || isRegi))
            return false;   
    }
    
    /*
    2 operands, both are 0 1 3
    */
    else if(contains(arr2,arr2_size, opcode)){
        bool isNum = is_number(line, first_operand_first_char,first_operand_last_char+1);
        bool isSymbol = is_symbol(line, first_operand_first_char, first_operand_last_char+1);
        bool isRegi = is_register(line, first_operand_first_char, first_operand_last_char+1);
        if(!(isNum || isSymbol || isRegi))
            return false;
        isNum = is_number(line, second_operand_first_char, second_operand_last_char+1);
        isSymbol = is_symbol(line, second_operand_first_char, second_operand_last_char+1);
        isRegi = is_register(line, second_operand_first_char,second_operand_last_char+1);
        if(!(isSymbol || isRegi || isNum))
            return false;   
    }
    
    /*
    2 operands, first is 1, second is 1 3
    */
    else if(contains(arr3,arr3_size, opcode)){
        bool isSymbol = is_symbol(line, first_operand_first_char, first_operand_last_char+1);
        bool isRegi = is_register(line, second_operand_first_char,second_operand_last_char+1);
        if(!isSymbol)
            return false;
        isSymbol = is_symbol(line, second_operand_first_char, second_operand_last_char+1);
        
        if(!(isSymbol || isRegi))
            return false;
    }
    
    /*
    1 operand - 1 3
    */
    else if(contains(arr4,arr4_size, opcode)){
        bool isSymbol = is_symbol(line, first_operand_first_char, first_operand_last_char+1);
        bool isRegi = is_register(line, first_operand_first_char, first_operand_last_char+1);
        if(!(isSymbol || isRegi))
            return false;
        }
    /*  
    1 operand - 1 2 3
    */
     else if(contains(arr5,arr5_size, opcode)){
        if(coma_index == -1){
            bool isSymbol = is_symbol(line, first_operand_first_char, first_operand_last_char+1);
            bool isRegi = is_register(line, first_operand_first_char, first_operand_last_char+1);
            if(!(isSymbol || isRegi))
                return false;
        }
        else{
            int open_parente_index = get_index_of(line, 40,0);
            int close_parente_index = get_index_of(line, 41, 0);
            int coma_index = get_index_of(line, 44, 0);
           
            bool isNum = is_number(line, open_parente_index +1 ,coma_index);
            bool isSymbol = is_symbol(line, open_parente_index +1 ,coma_index);
            bool isRegi = is_register(line, open_parente_index +1 ,coma_index);
            if(!(isNum || isSymbol || isRegi))
                return false;
            isSymbol = is_symbol(line, coma_index +1 ,close_parente_index);
            isRegi = is_register(line, coma_index +1 ,close_parente_index);
            if(!(isSymbol || isRegi))
                return false;
        }
    }
    return true;
}

bool contains(char array[][5], int rows, char target[]) {
    int i;
    for (i = 0; i < rows; i++) {
        if (strcmp(array[i], target) == 0) {
            return true;
        }
    }
    return false;
}

bool is_register(const char *line, int start, int end){
    /*
    reg will be the register name
    */
    char reg[LINE_SIZE + 1]; 

    /*
    Copy substring to 'reg' string
    */ 
    int i;
    for (i = start; i <= end - 1; i++) {
        reg[i - start] = line[i];
    }
    reg[i - start] = '\0';

    if(strcmp(reg, "r0") == 0)
        return true;
    if(strcmp(reg, "r1") == 0)
        return true;
    if(strcmp(reg, "r2")== 0)
        return true;
    if(strcmp(reg, "r3")== 0)
        return true;
    if(strcmp(reg, "r4")== 0)
        return true;
    if(strcmp(reg, "r5")== 0)
        return true;
    if(strcmp(reg, "r6")== 0)
        return true;
    if(strcmp(reg, "r7")== 0)
        return true;
    return false;
}

bool is_number (const char *line, int start, int end){
     /*
    num will be the number in line
    */
    char num[LINE_SIZE + 1]; 
    int i;
    bool spaces = is_contains_spaces(line, start, end);
    if(spaces)
        return false;

    /*
    Copy substring to 'num' string
    */ 
    
    for (i = start; i <= end - 1; i++) {
        num[i - start] = line[i];
    }
    num[i - start] = '\0';

    if(num[0] != 35)
        return false;
    
    if(num[1] != 43 && num[1] != 45 && !isdigit(num[1]))
        return false;
    
    if(isdigit(num[1])){
        for(i = 1; i < strlen(num) -2; i++){
            if(!isdigit(line[i]))
                return false;
        }
    }
    else{
        for(i = 2; i < strlen(num)-2; i++){
            if(!isdigit(line[i]))
                return false;
        }
    }
    return true;


}

bool is_symbol (const char *line, int start, int end){
    int i;
    bool hasSpaces;
    if(is_register(line, start,end))
        return false;
    hasSpaces = is_contains_spaces(line, start, end);
    if(hasSpaces)
        return false;
    if(!((line[start] >= 65 && line[start] <= 90) || (line[start] >= 97 && line[start] <= 122))){
        return false;
    }
   
    for( i = start; i < end; i++){
        if(!((line[i] >= 48 && line[i] <= 57) || (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122))){
            return false;
        }
    }
    return true;
}

int calc_acuall_operands_num(const char *line, int index){
    int i = index;
    bool is_empty = true;
    int coma_counter = 0;
    for(i = 0; i< strlen(line); i ++){
        if(!isspace(line[i])){
            is_empty = false;
            break;
        }
    }
    if(is_empty)
        return 0;
    
    
    i = index;
    for(i = 0; i < strlen(line); i ++){
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
        break;
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
