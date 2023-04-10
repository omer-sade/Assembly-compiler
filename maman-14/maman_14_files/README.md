Welcome to my project!

A quick intro is found below.

Summary: The code gets 1 or more input files written in assembley, and translate them into binary. 

How to use: after typing 'make' in the terminal, run the command ./app (filename1) (filename2) ...

main programs that are responsible to run the entire code:

1. app.c: the main file that is responsible to run the entire program.
2. pre_assembler.c: Runs on one input file and expands all of its macros.
3. first_read.c: responsible to read the input files for the first time, converts lines to binary and saves all symbols.
4. second_read.c: reads file for the second time. Completes binary translation of all unknown symbols from the first read.

Helper functions:
1. validate_input.c: helps the 'first_read' file to validate that the input files are written in proper assembley.
2. data_structure.c: Has two very important structs. The first is 'Array' that saves all symbols, the second is 'Binary_table' that is used to save all the translated lines and their numbers.
3. 'binary_conversions': Helps first and second read to translate a given line to binary.
 
Additional general info:
1. All the header files are saved in the file 'headers.h'
2. Further explanation on specific functions is found in relevant header file.
3. Three test files added: test_file, test_file1, test_file2