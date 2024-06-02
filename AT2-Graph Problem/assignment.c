#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neural_graph.h"
#include "neural_graph_utils.h"
#include "k_means.h"
#include "different_metrics_k_means.h"

//display the menu
int display_menu() {
	int choice;
	printf("\nMenu:\n");
	printf("1. Run K-Means Clustering with Euclidean Distance\n");
	printf("2. Run K-Means Clustering with Jaccard Similarity\n");
	printf("3. Run K-Means Clustering with Cosine Distance\n");
	printf("4. Exit\n");
	printf("Enter your choice: ");
	scanf("%d", &choice);
	return choice;
}

//prompt user to ask if they want to see the most important neuron in each of the lobes used by cerebral cortex
int prompt_view_important_neurons() {
	int choice;
	printf("\nDo you want to see the most important neuron for each of the 4 clusters (Lobes)? (1 for Yes): ");
	scanf("%d", &choice);
	return choice;
}

//ask user for number of iterations
int prompt_iterations() {
	int iterations;
	printf("\nEnter the number of iterations for K-Means Clustering: ");
	scanf("%d", &iterations);
	return iterations;
}

//initialise the brain graph, ordering in an array before adding the edge for better understanding of graph
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

	for (int i = 0; i < unique_size; i++) {
		if (graph->edges[i].head == NULL) {
			graph->edges[i].head = NULL;
		}
	}

	// Free dynamically allocated memory
	free_int_array(&vertices);
	free(vertex_map);
	fclose(file);
}

int main() {
	Graph G;
	brain_graph_initializer("brain_dataset_reduced.txt", &G);

	while (1) {
		int choice = display_menu();
		if (choice == 4) {
			printf("Exiting the program.\n");
			break;
		}
		else if (choice >= 1 && choice <= 3) {
			int iterations = prompt_iterations();
			int k = 4;
			int* cluster_labels = NULL;

			switch (choice) {
			case 1:
				cluster_labels = (int*)malloc(G.vertex * sizeof(int));
				if (cluster_labels == NULL) {
					fprintf(stderr, "Memory allocation failed for cluster_labels\n");
					exit(EXIT_FAILURE);
				}
				printf("\nRunning K-Means Clustering with Euclidean Distance for %d iterations:\n", iterations);
				k_means_metric(&G, k, cluster_labels, iterations, euclidean_distance);
				break;
			case 2:
				cluster_labels = (int*)malloc(G.vertex * sizeof(int));
				if (cluster_labels == NULL) {
					fprintf(stderr, "Memory allocation failed for cluster_labels\n");
					exit(EXIT_FAILURE);
				}
				printf("\nRunning K-Means Clustering with Jaccard Similarity for %d iterations:\n", iterations);
				k_means(&G, k, cluster_labels, iterations);
				break;
			case 3:
				cluster_labels = (int*)malloc(G.vertex * sizeof(int));
				if (cluster_labels == NULL) {
					fprintf(stderr, "Memory allocation failed for cluster_labels\n");
					exit(EXIT_FAILURE);
				}
				printf("\nRunning K-Means Clustering with Cosine Distance for %d iterations:\n", iterations);
				k_means_metric(&G, k, cluster_labels, iterations, cosine_distance);
				break;
			default:
				printf("Invalid choice.\n");
				continue;
			}

			// Count and display num of neurons in each cluster
			int* cluster_counts = (int*)calloc(k, sizeof(int));
			for (int i = 0; i < G.vertex; i++) {
				cluster_counts[cluster_labels[i]]++;
			}

			printf("\nNumber of vertices in each cluster:\n");
			for (int i = 0; i < k; i++) {
				printf("Cluster %d: %d vertices\n", i, cluster_counts[i]);
			}

			int view_choice = prompt_view_important_neurons();
			if (view_choice == 1) {
				//Find the most important neuron for each cluster
				most_important_neuron(&G, k, cluster_labels);
			}
			free(cluster_labels);
			//free(cluster_counts);
		}
		else {
			printf("Invalid choice. Please try again.\n");
		}
	}
	// Free dynamically allocated memory
	free_neural_graph(&G);
	return;
}