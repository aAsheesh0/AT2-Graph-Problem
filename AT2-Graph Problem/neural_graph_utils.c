#include <stdio.h>
#include <stdlib.h>
#include "neural_graph_utils.h"

void init_int_array(IntArray* arr, int initial_capacity) {
    arr->data = (int*)malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
}

void append_int_array(IntArray* arr, int value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (int*)realloc(arr->data, arr->capacity * sizeof(int));
        if (arr->data == NULL) {
            perror("Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }
    arr->data[arr->size++] = value;
}

void free_int_array(IntArray* arr) {
    free(arr->data);
}

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}