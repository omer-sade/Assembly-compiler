
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INITIAL_CAPACITY 100


void initArray(Array *arr, size_t data_size);
void addArray(Array *arr, void *element, size_t data_size);
int searchArray(Array *arr, void *element, size_t data_size, int (*cmp)(const void*, const void*));
int cmpInt(const void *a, const void *b);
int cmpStr(const void *a, const void *b) ;
