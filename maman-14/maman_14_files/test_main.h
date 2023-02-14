#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_struct/data_structure.c"
#include "first_read/api/first_read.c"
#include "second_read/second_read.c"
#include "pre_assembler/pre_assembler.c"

/*
Reads origing file and "opens" all macros. Prepares the code for it to be readable by "reading
file first time" function (located below) 
*/
FILE *preAssembler(FILE *fp);

/*
Function that analyzes origin file for first time. Translates lines to binary if possible, else it'll
save symbols (that will be translated to binary in the second read).
Reads from "pfile", writes binary language to "p_outputFile"
*/
void reading_file_first_time(FILE *pfile, Array *symbols_table, Array *insturctions, FILE *p_outputFile);

/*
Converts ALL remaining lines to binary. 
Reads from "pfile", writes binary language to "p_outputFile"
*/
void reading_file_second_time(FILE *pfile, Array *symbols_table, Array *insturctions, FILE *p_outputFile);