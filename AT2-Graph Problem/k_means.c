#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>

//initialize centeroids randomly
void initialize_centroids(int k, int n, double** centroids) {
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < n; ++j) {
            centroids[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

// assign data points to clusters
void assign_clusters(Graph* self, int k, double** centroids, int* cluster_labels) {
    for (int i = 0; i < self->vertex; ++i) {
        double min_distance = INT_MAX;
        int closest_centroid = -1;

        // Calculate distance to each centroid
        for (int j = 0; j < k; ++j) {
            double distance = euclidean_distance(centroids[j], self->edges[i]);
            if (distance < min_distance) {
                min_distance = distance;
                closest_centroid = j;
            }
        }
        cluster_labels[i] = closest_centroid;
    }
}

void update_centroids(Graph* self, int k, double** centroids, int* cluster_labels) {
    //Based on assigned clusters, update the centroids
    int* cluster_sizes = (int*)calloc(k, sizeof(int));
    double** cluster_sums = (double**)malloc(k * sizeof(double*));

    for (int i = 0; i < k; ++i) {
        cluster_sums[i] = (double*)calloc(self->vertex, sizeof(double));
    }
}