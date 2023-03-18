#include <stdio.h>
#include <string.h>

#include "second_read.h"





// char* int_to_four_char_string(int input) {
//     static char output[5];
//     sprintf(output, "%04d", input);
//     return output;
// }

// void convert_to_dots_slashes(char *input, char *output) {
//     int i;
//     for (i = 0; i < strlen(input); i++) {
//         if (input[i] == '0') {
//             output[i] = '.';
//         } else if (input[i] == '1') {
//             output[i] = '/';
//         } else {
//             printf("Invalid input string!\n");
//             return;
//         }
//     }
//     output[i] = '\0';
// }

// void get_length_and_converted_string(int num, char *input, FILE *fp) {
//     char converted[strlen(input) + 1];
//     convert_to_dots_slashes(input, converted);
//     fprintf(fp, "%s\t%s\n", int_to_four_char_string(num), converted);
// }
/*
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
*/