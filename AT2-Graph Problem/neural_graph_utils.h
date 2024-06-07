#ifndef NEURAL_GRAPH_UTILS_H
#define NEURAL_GRAPH_UTILS_H

// Utils for structuring the graph, this way we could order the graph neurons
typedef struct {
    int* data;
    int size;
    int capacity;
} IntArray;

void init_int_array(IntArray* arr, int initial_capacity);
void append_int_array(IntArray* arr, int value);
void free_int_array(IntArray* arr);
int compare_ints(const void* a, const void* b);

#endif