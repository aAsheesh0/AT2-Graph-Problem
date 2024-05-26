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
    printf("Initial Centroids:\n");
    for (int i = 0; i < k; i++) {
        printf("Centroid %d: ", i);
        for (int j = 0; j < n; j++) {
            printf("%f ", centroids[i][j]);
        }
        printf("\n");
    }
}

double jaccard_similarity(int* set1, int size1, int* set2, int size2) {
    int intersection_size = 0;
    int union_size = size1 + size2;

    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (set1[i] == set2[j]) {
                intersection_size++;
                break;
            }
        }
    }

    union_size -= intersection_size;

    if (union_size == 0) {
        return 1.0;
    }

    return (double)intersection_size / union_size;
}

// assign data points to clusters
void assign_clusters(Graph* self, int k, double** centroids, int* cluster_labels) {
    for (int i = 0; i < self->vertex; i++) {
        double max_similarity = -1.0;
        int closest_centroid = -1;

        int* vertex_edges = (int*)malloc(self->vertex * sizeof(int));
        int vertex_edges_count = 0;
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            vertex_edges[vertex_edges_count++] = current->edge.to_vertex;
            current = current->next;
        }

        // Calculate distance to each centroid
        for (int j = 0; j < k; j++) {
            int* centroid_edges = (int*)malloc(self->vertex * sizeof(int));
            int centroid_edges_count = 0;
            for (int l = 0; l < self->vertex; l++) {
                if (centroids[j][l] > 0) {
                    centroid_edges[centroid_edges_count++] = l;
                }
            }
            double similarity = jaccard_similarity(vertex_edges, vertex_edges_count, centroid_edges, centroid_edges_count);

            free(centroid_edges);

            if (similarity > max_similarity) {
                max_similarity = similarity;
                closest_centroid = j;
            }
        }
        cluster_labels[i] = closest_centroid;
        free(vertex_edges);
    }
    printf("Cluster Assignments:\n");
    for (int i = 0; i < self->vertex; i++) {
        printf("Vertex %d: Cluster %d\n", i, cluster_labels[i]);
    }
}

//calculate Euclidean distance between two data points
/*double euclidean_distance(double* data_point1, double* data_point2, int n) {
    double distance = 0.0;
    for (int i = 0; i < n; i++) {
        distance += pow(data_point1[i] - data_point2[i], 2);
    }
    return sqrt(distance);
}*/

void update_centroids(Graph* self, int k, double** centroids, int* cluster_labels) {
    //Based on assigned clusters, update the centroids
    //int* cluster_sizes = (int*)calloc(k, sizeof(int));
    //double** cluster_sums = (double**)malloc(k * sizeof(double*));

    //reset
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < self->vertex; j++) {
            centroids[i][j] = 0.0;
        }
    }
    int* cluster_sizes = (int*)calloc(k, sizeof(int));

    // Compute cluster sizes and sums
    for (int i = 0; i < self->vertex; ++i) {
        int cluster = cluster_labels[i];
        cluster_sizes[cluster]++;
        EdgeNodePtr current = self->edges[i].head;

        while (current != NULL) {
            centroids[cluster][current->edge.to_vertex] += 1.0;
            current = current->next;
        }
    }

    // Update the centroids
    for (int i = 0; i < k; i++) {
        if (cluster_sizes[i] > 0) {
            for (int j = 0; j < self->vertex; j++) {
                centroids[i][j] /= cluster_sizes[i];
            }
        }
    }

    printf("Updated Centroids:\n");
    for (int i = 0; i < k; i++) {
        printf("Centroid %d: ", i);
        for (int j = 0; j < self->vertex; j++) {
            printf("%f ", centroids[i][j]);
        }
        printf("\n");
    }

    // Free memory
    free(cluster_sizes);
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