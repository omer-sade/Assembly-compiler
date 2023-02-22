#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_assembler.h"




/*
below here is bar's code
*/



/*array of all the macros*/
struct Macro *mcr_arr = NULL;

int mcr_arr_len = 0;/*number of macros in this array*/
int mcr_arr_size = 0;/*macro array alloc size*/

int is_macro(char *str){
	int i;
    	for (i = 0; i < mcr_arr_len; i++) {
        	if (strcmp(mcr_arr[i].word, str) == 0) {
            	return i;
        	}
    	}
    	return -1;
}

void put_mcr(FILE *orig_file, FILE *output_file, int mcr_idx) {
    char ch;
    long original_position;

    original_position = ftell(orig_file);

    fseek(orig_file, mcr_arr[mcr_idx].start_bit, SEEK_SET);

    while ((ch = fgetc(orig_file)) != EOF && \
    ftell(orig_file) <= mcr_arr[mcr_idx].end_bit) {
	   fputc(ch,output_file);
    }
	/*returns the original file ptr to its previous location*/
    fseek(orig_file, original_position, SEEK_SET);
}

void add_word(char *word, int start, int end) {
	if (mcr_arr_len == mcr_arr_size) { /*expanding the alloc*/
		mcr_arr_size = (mcr_arr_size == 0) ? 1 : mcr_arr_size * 2;
		mcr_arr = (struct Macro *)realloc(mcr_arr, mcr_arr_size*sizeof(struct Macro));
	}
	strcpy(mcr_arr[mcr_arr_len].word, word);
	mcr_arr[mcr_arr_len].start_bit = start;
	mcr_arr[mcr_arr_len].end_bit = end;
	mcr_arr_len++;
}

void pre_assembler(FILE *file, FILE *output_file) {
	int bit_location = 0;/*keeps the current file ptr location*/
	char line[MAX_LINE];/*line read from the original file (buffer)*/
    	char first_word[MAX_LINE];/*keeps the first word of a line*/
    	char rest_of_line[MAX_LINE];/*keeps the line cont except the first word*/
    	int mcr_flag = 0;/*flag that keeps track if the iteration is cur in a mcr*/
    	char name[MAX_LINE];/*save the name of the cur mcr the func is checking*/
    	int start;/*saves the start location of a macro*/
    	int line_cont;/*keeps track of how many parts are in a line (empty/1word/2parts)*/
    	int macro_num;/*saves which mcr is called from the arr of mcrs*/
	
	while (fgets(line, sizeof(line), file)){
		line_cont = sscanf(line, "%s %[^\n]", first_word, rest_of_line);
		if(line_cont == 2 && strcmp(first_word, "mcr") == 0){
			/*if a start of a macro it saves the macro's name and 
			the content's start location*/
			strcpy(name, rest_of_line);
			start = bit_location + strlen(line);
			mcr_flag = 1;
		}
		else if(line_cont == 1){
			if (strcmp(first_word, "endmcr") == 0){
				/*when the end of a macro is reached add the macro
				to the list of macros*/
				add_word(name, start, bit_location);
				memset(name, 0, sizeof(name)); 
				mcr_flag = 0;
				continue;
			}
			else if((macro_num = is_macro(first_word)) != -1){
				/*if a call to a macro it replaces it with 
				the macro's content*/
				put_mcr(file, output_file, macro_num);
				continue;
			}
		}
		bit_location = ftell(file);
		if(mcr_flag == 0){
			/*if not a line of a macro it puts the line in the new file*/
			fprintf(output_file, "%s",line);
		}
	}
	fseek(output_file, 0, SEEK_SET); /*set the new file ptr to the beginning*/
	fclose(file);
}

FILE *preAssembler(FILE *fp){
   
	FILE *output_file;
	output_file = fopen("output_file.txt", "w");
    	if (output_file == NULL) {
        printf("Error opening files\n");
    	}
	pre_assembler(fp, output_file);

	free(mcr_arr);
    return output_file;
}
