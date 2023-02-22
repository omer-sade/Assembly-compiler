#define MAX_LINE 80

/*function that checks if a given string is a 
name of a macro already in the system*/
int is_macro(char *str);

/*function that is given a idx of a macro and puts the content of the
macro in the output file*/
void put_mcr(FILE *orig_file, FILE *output_file, int mcr_idx);

/*func that adds a creates a new macro and adds it to the arr of macros*/
void add_word(char *word, int start, int end);

/*the main function that runs the preassembler proccess*/
void pre_assembler(FILE *file, FILE *output_file);

struct Macro {
	char word[MAX_LINE];
	int start_bit;
	int end_bit;
};

