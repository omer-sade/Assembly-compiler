#define MAX_LINE 80

/*function that checks if a given string is a 
name of a macro already in the system*/
int is_macro(char *str);

/*func that adds a creates a new macro and adds it to the arr of macros*/
void add_word(char *word, char *cont);

/*function that adds string to another string (dynamically).*/
void append_string(char **str, const char *suffix);

/*function used to free the content fields of the macros*/
void free_mcr_cont();

/*the main function that runs the preassembler process*/
void pre_assembler(FILE *file, FILE *output_file);

/*The function creates the output file, 
runs the calls the preassembler to run and free all allocated memory*/
FILE *preAssembler(FILE *fp);

struct Macro {
	char word[MAX_LINE];
	char *content;
};

