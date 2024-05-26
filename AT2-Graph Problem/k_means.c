#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

//initialize centeroids randomly
void initialize_centroids(int k, int n, double** centroids) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            centroids[i][j] = (double)rand() / RAND_MAX;
        }
    }
}

// assign data points to clusters
void assign_clusters(Graph* self, int k, double** centroids, int* cluster_labels) {
    for (int i = 0; i < self->vertex; i++) {
        double min_distance = INT_MAX;
        int closest_centroid = -1;

        // Calculate distance to each centroid
        for (int j = 0; j < k; j++) {
            EdgeNodePtr current = self->edges[i].head;
            double distance = 0.0;
            while (current != NULL) {
                distance += euclidean_distance(centroids[j], (double[]) { current->edge.to_vertex }, 1);
                current = current->next;
            }

            if (distance < min_distance) {
                min_distance = distance;
                closest_centroid = j;
            }
        }
        cluster_labels[i] = closest_centroid;
    }
}

//calculate Euclidean distance between two data points
double euclidean_distance(double* data_point1, double* data_point2, int n) {
    double distance = 0.0;
    for (int i = 0; i < n; i++) {
        distance += pow(data_point1[i] - data_point2[i], 2);
    }
    return sqrt(distance);
}

void update_centroids(Graph* self, int k, double** centroids, int* cluster_labels) {
    //Based on assigned clusters, update the centroids
    int* cluster_sizes = (int*)calloc(k, sizeof(int));
    double** cluster_sums = (double**)malloc(k * sizeof(double*));

    for (int i = 0; i < k; ++i) {
        cluster_sums[i] = (double*)calloc(self->vertex, sizeof(double));
    }

    // Compute cluster sizes and sums
    for (int i = 0; i < self->vertex; ++i) {
        int cluster = cluster_labels[i];
        cluster_sizes[cluster]++;
        EdgeNodePtr current = self->edges[i].head;

        while (current != NULL) {
            cluster_sums[cluster][current->edge.to_vertex] += 1.0; // Count the edge
            current = current->next;
        }
    }

    // Update the centroids
    for (int i = 0; i < k; i++) {
        if (cluster_sizes[i] > 0) {
            for (int j = 0; j < self->vertex; j++) {
                centroids[i][j] = cluster_sums[i][j] / cluster_sizes[i];
            }
        }
    }

    // Free memory
    free(cluster_sizes);
    for (int i = 0; i < k; i++) {
        free(cluster_sums[i]);
    }
    free(cluster_sums);
}

//compute K-means clustering
void k_means(Graph* self, int k, int* cluster_labels) {
    double** centroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(self->vertex * sizeof(double));
    }
    initialize_centroids(k, self->vertex, centroids);

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        assign_clusters(self, k, centroids, cluster_labels);
        update_centroids(self, k, centroids, cluster_labels);
    }

    for (int i = 0; i < k; i++) {
        free(centroids[i]);
    }
    free(centroids);
}