#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100

typedef struct {
    char name[30]; 
    bool ent;
    bool ext;
    int line_num;
} Symbol;

typedef struct {
    Symbol *symbol;
    int size;
} Array;




void initArray(Array *arr) {
    arr->symbol = (Symbol *)malloc(INITIAL_CAPACITY * sizeof(Symbol));
    if(arr->symbol == NULL){
        printf("Failed to allocate memory. Terminating program.\n");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
}

void addArray(Array *arr, char *element, int *line_number) {
    if (arr->size % 10 == 0) {
        arr->symbol = (Symbol *) realloc(arr->symbol, (arr->size + 10) * sizeof(Symbol));
    }
    Symbol s = {"", false, false, -1};
    strcpy(s.name, element);
    s.line_num = *line_number;
    arr->symbol[arr->size] = s;
    /*
    if not extern
    */
    if(*line_number != -1){
        arr->symbol[arr->size].line_num = *line_number;
    }
    else{
        arr->symbol[arr->size].ext = true;
    }
    
    (arr->size) ++;
}

int cmp_symbol(Symbol sy1, Symbol sy2){
    return strcmp(sy1.name, sy2.name);
}

int searchArray(Array *arr, char *element) {
    int i;
    for (i = 0; i < arr->size; i++) {
        if (strcmp(arr->symbol[i].name, element) == 0) {
            return i;
        }
    }
    return -1;
}


/*
methods to compare to types of data. 
being used in method "searchArray" as last parameter.
*/
int cmpInt(const void *a, const void *b) {
    int x = *(int*)a;
    int y = *(int*)b;
    return (x > y) - (x < y);
}

int cmpStr(const void *a, const void *b) {
    char *x = (char*)a;
    char *y = (char*)b;
    return strcmp(x, y);
}


/*

EXAMPLE FOR 'MAIN' - HOW TO USE THE DATA STRUCTURE WE CREATED.


int main() {
    Array arr;
    initArray(&arr, sizeof(int));
    int a = 1, b = 10, c = 30;
    
    addArray(&arr, &a, sizeof(int));
    addArray(&arr, &b, sizeof(int));
    addArray(&arr, &c, sizeof(int));

    
    
     int target = 2;
     int index = searchArray(&arr, &target, sizeof(int), cmpInt);
     printf("index = %d\n", index);

      if (index != -1) {
        printf("Found at index: %d\n", index);
    } else {
        printf("Not found\n");
    }

    printf("arr[0] = %d\n", *(int*)arr.data);
    printf("arr[1] = %d\n", *((int*)arr.data + 1));

     free(arr.data);
  
    return 0;
}

*/
