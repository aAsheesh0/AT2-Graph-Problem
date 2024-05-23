#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neural_graph.h"
#include "neural_graph_utils.h"

int main() {
	Graph G;
	IntArray vertices;
	init_int_array(&vertices, 1000);

	// Reading the txt file
	FILE* file = fopen("human_brain_dataset.txt", "r");
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

	init_neural_graph(&G, unique_size);
	rewind(file);

	while (fscanf(file, "%d %d", &from, &to) == 2) {
		add_edge(&G, vertex_map[from], vertex_map[to], 0);
	}

	printf("\n-In Degree Vertices-\n");
	print_in_degrees(&G);

	//Freeing Dynamically Allocated Memory
	for (int v = 0; v < G.vertex; v++) {
		EdgeNodePtr current = G.edges[v].head;
		while (current != NULL) {
			EdgeNodePtr temp = current;
			current = current->next;
			free(temp);
		}
	}

	// Free dynamically allocated memory for the array of edge lists
	free(G.edges);
	free(vertex_map);
	free_int_array(&vertices);

	fclose(file);

	return ;
}