#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "data_struct/data_structure.c"
#include "first_read/binary_conversions.c"
#include "first_read/first_read.c"
#include "first_read/validate_input.c"
#include "second_read/second_read.c"
#include "pre_assembler/pre_assembler.c"



/*
Reads origing file and "opens" all macros. Prepares the code for it to be readable by "reading
file first time" function (located below) 
*/
void pre_assembler(FILE *fp, FILE *p_file_open_macros);

/*
Function that analyzes origin file for first time. Translates lines to binary if possible, else it'll
save symbols (that will be translated to binary in the second read).
Reads from "pfile", writes binary language to "p_outputFile"
*/
void reading_file_first_time(Array *symbols_table, const char **instructions, FILE *p_outputFile, const char** registers, Binary_table *instructions_table, Binary_table *data_table);

/*
Converts ALL remaining lines to binary. 
Reads from "pfile", writes binary language to "p_outputFile"
*/
void reading_file_second_time(Array *symbols_table, Binary_table *instructions_table, const char **instructions, FILE *p_outputFile, char *filename);

/*
adds all the 16 types of instructions to the instructions array (mov, not, jmp, etc..)
*/
void add_data(char instructions[][LINE_SIZE]);

/*
converts integers to strings with four digits
*/
char* int_to_four_char_string(int input);

/*
converts binary code to slashes and dots
*/
void convert_to_dots_slashes(char *input, char *output);

/*
adds binary data line (slashes and dots) to file
*/
void get_length_and_converted_string(int num, char *input, FILE *fp);

/*
Builds the object file from the instructions table and data table values
*/
void build_object_file(Binary_table *instructions_table, Binary_table *data_table, FILE *object_file);