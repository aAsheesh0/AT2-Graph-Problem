#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neural_graph.h"
#include "neural_graph_utils.h"

void brain_graph_initializer(const char* filename, Graph* graph) {
	IntArray vertices;
	init_int_array(&vertices, 1000);

	// Reading the txt file
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error opening file\n");
		perror("fopen");
		return 1;
	}

	int from, to;
	while (fscanf(file, "%d %d", &from, &to) == 2) {
		append_int_array(&vertices, from);
		append_int_array(&vertices, to);
	}

	// Remove duplicates from the vertex list
	qsort(vertices.data, vertices.size, sizeof(int), compare_ints);
	int unique_size = 0;
	for (int i = 0; i < vertices.size; i++) {
		if (i == 0 || vertices.data[i] != vertices.data[i - 1]) {
			vertices.data[unique_size++] = vertices.data[i];
		}
	}
	vertices.size = unique_size;
	printf("Number of Unique Neurons: %d\n", unique_size);

	// Map original vertex IDs to new compact IDs
	int* vertex_map = (int*)malloc((vertices.data[unique_size - 1] + 1) * sizeof(int));
	for (int i = 0; i < unique_size; i++) {
		vertex_map[vertices.data[i]] = i;
	}

	init_neural_graph(graph, unique_size);
	rewind(file);

	while (fscanf(file, "%d %d", &from, &to) == 2) {
		add_edge(graph, vertex_map[from], vertex_map[to], 0);
	}

	// Free dynamically allocated memory
	free_int_array(&vertices);
	free(vertex_map);
	fclose(file);
}

int main() {
	Graph G;

	brain_graph_initializer("human_brain_dataset.txt", &G);

	printf("\n-In Degree Vertices-\n");
	print_in_degrees(&G);

	// Free dynamically allocated memory
	free_neural_graph(&G);

	return;
}