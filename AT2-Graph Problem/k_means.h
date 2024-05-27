#ifndef K_MEANS_H
#define K_MEANS_H
#include "neural_graph.h"

#define MAX_ITERATIONS 10

void initialize_centroids(Graph* self, int n, double** centroids);
void assign_clusters(Graph* self, int k, double** centroids, int* cluster_labels);
void update_centroids(Graph* self, int k, double** centroids, int* cluster_labels);
void k_means(Graph* self, int k, int* cluster_labels);
double degree_centrality(Graph* graph, int vertex);
void most_important_neuron(Graph* graph, int k, int* cluster_labels);

#endif