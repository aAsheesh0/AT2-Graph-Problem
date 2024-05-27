#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neural_graph.h"
#include "neural_graph_utils.h"
#include "k_means.h"

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

	brain_graph_initializer("reduced_brain_graph.txt", &G);

	//printf("\n-In Degree Vertices-\n");
	//print_in_degrees(&G);

	printf("\nTesting K-Means Clustering:\n");
	int k = 4;
	int* cluster_labels = (int*)malloc(G.vertex * sizeof(int));

	k_means(&G, k, cluster_labels);

	for (int i = 0; i < G.vertex; i++) {
		printf("Vertex %d: Cluster %d\n", i, cluster_labels[i]);
	}

	// Count num of vertices in each cluster
	/*int* cluster_counts = (int*)calloc(k, sizeof(int));
	for (int i = 0; i < G.vertex; i++) {
		cluster_counts[cluster_labels[i]]++;
	}

	printf("\nNumber of vertices in each cluster:\n");
	for (int i = 0; i < k; i++) {
		printf("Cluster %d: %d vertices\n", i, cluster_counts[i]);
	}*/

	//Find the most important neuron for each cluster
	most_important_neuron(&G, k, cluster_labels);

;	free(cluster_labels);

	// Free dynamically allocated memory
	free_neural_graph(&G);

	return;
}