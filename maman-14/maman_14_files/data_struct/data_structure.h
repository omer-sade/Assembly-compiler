
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INITIAL_CAPACITY 100

/*
creating an array
*/
void initArray(Array *arr);

/*
addidng element to array.
Line num will be assigned to the symbol (element)
*/
void addArray(Array *arr, char *element, int *line_num);
/*
searching element in array
*/
int searchArray(Array *arr, char *element);


