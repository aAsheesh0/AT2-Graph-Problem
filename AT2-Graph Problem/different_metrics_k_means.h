#ifndef DIFFERENT_METRIC_K_MEANS_H
#define DIFFERENT_METRIC_K_MEANS_H

#include "neural_graph.h"

void initialize_centroids_for_metric(Graph* self, int k, double** centroids);
void assign_clusters_for_metric(Graph* self, int k, double** centroids, int* cluster_labels, double (*distance_func)(double*, double*, int));
double euclidean_distance(double* point1, double* point2, int dimensions);

#endif