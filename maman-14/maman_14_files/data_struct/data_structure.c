#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100

typedef struct {
    void *data;
    /*
     size - how many items are currently in the array
    */
    size_t size; 
    /*
    capacity - max items the array is able to hold. when size equals capacity - increase capacity
    */
    size_t capacity;
} Array;


void initArray(Array *arr, size_t data_size);
void addArray(Array *arr, void *element, size_t data_size);
int searchArray(Array *arr, void *element, size_t data_size, int (*cmp)(const void*, const void*));
int cmpInt(const void *a, const void *b);
int cmpStr(const void *a, const void *b) ;

void initArray(Array *arr, size_t data_size) {
    arr->data = malloc(INITIAL_CAPACITY * data_size);
    if(arr->data == NULL){
        printf("Failed to allocate memory. Terminating program.\n");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
}

void addArray(Array *arr, void *element, size_t data_size) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * data_size);
    }
    memcpy(arr->data + arr->size * data_size, element, data_size);
    arr->size++;
}

int searchArray(Array *arr, void *element, size_t data_size, int (*cmp)(const void*, const void*)) {
    int i;
    for (i = 0; i < arr->size; i++) {
        if (cmp(arr->data + i * data_size, element) == 0) {
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
