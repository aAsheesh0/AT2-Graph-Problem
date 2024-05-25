#ifndef K_MEANS_H
#define K_MEANS_H
#include "neural_graph.h"

#define MAX_ITERATIONS 100
#define EPSILON 1e-4

void initialize_centroids(int k, int n, double** centroids);
void assign_clusters(Graph* graph, int k, double** centroids, int* cluster_labels);
void k_means(Graph* graph, int k, int* cluster_labels);

#endif