#ifndef DIFFERENT_METRIC_K_MEANS_H
#define DIFFERENT_METRIC_K_MEANS_H

#include "neural_graph.h"
#include "k_means.h"

void initialize_centroids_for_metric(Graph* self, int k, double** centroids);
void assign_clusters_for_metric(Graph* self, int k, double** centroids, int* cluster_labels, double (*distance_func)(double*, double*, int));
double euclidean_distance(double* point1, double* point2, int dimensions);
double calculate_distance_metric(Graph* self, int vertex, double* centroid, double (*distance_func)(double*, double*, int));
void k_means_metric(Graph* self, int k, int* cluster_labels, int num_iterations, double (*distance_func)(double*, double*, int));

#endif