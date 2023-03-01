#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 100

typedef struct {
    void *data;
    size_t size;
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
//EXAMPLE FOR 'MAIN' - HOW TO USE THE DATA STRUCTURE WE CREATED.

int main() {
    Array arr;
    initArray(&arr, sizeof(char[10]));
    //int a = 1, b = 5, c = 3;
    char str1[] = "abc";
    char str2[] = "rber";
    char str3[] = "refs";
    addArray(&arr, str1, sizeof(char[10]));
    addArray(&arr, str2, sizeof(char[10]));
    addArray(&arr, str3, sizeof(char[10]));

    
    
     int target = 2;
     int index = searchArray(&arr, &target, sizeof(char[10]), cmpStr);
     printf("index = %d\n", index);

      if (index != -1) {
        printf("Found at index: %d\n", index);
    } else {
        printf("Not found\n");
    }

    printf("arr[0] = %s\n", ((char*)arr.data)[0]);
    printf("arr[1] = %s\n", ((char*)arr.data)[1]);

     free(arr.data);
  
    return 0;
}
*/
