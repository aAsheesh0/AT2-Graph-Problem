#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "different_metrics_k_means.h"

double euclidean_distance(double* point1, double* point2, int dimensions) {
    double sum = 0.0;
    for (int i = 0; i < dimensions; i++) {
        sum += pow(point1[i] - point2[i], 2);
    }
    return sqrt(sum);
}

double calculate_distance_metric(Graph* self, int vertex, double* centroid, double (*distance_func)(double*, double*, int)) {
    double* vertex_vector = (double*)calloc(self->vertex, sizeof(double));
    double* centroid_vector = (double*)calloc(self->vertex, sizeof(double));

    EdgeNodePtr current = self->edges[vertex].head;
    while (current != NULL) {
        vertex_vector[current->edge.to_vertex] = 1.0;
        current = current->next;
    }

    for (int i = 0; i < self->vertex; i++) {
        centroid_vector[i] = centroid[i];
    }

    double distance = distance_func(vertex_vector, centroid_vector, self->vertex);

    free(vertex_vector);
    free(centroid_vector);

    return distance;
}

void initialize_centroids_for_metric(Graph* self, int k, double** centroids) {
    // Choose the first centroid randomly
    int first_centroid = rand() % self->vertex;
    for (int j = 0; j < self->vertex; j++) {
        centroids[0][j] = 0.0;
    }
    EdgeNodePtr current = self->edges[first_centroid].head;
    while (current != NULL) {
        centroids[0][current->edge.to_vertex] = 1.0;
        current = current->next;
    }

    double* distances = (double*)malloc(self->vertex * sizeof(double));
    double* min_distances = (double*)malloc(self->vertex * sizeof(double));

    for (int i = 1; i < k; i++) {
        double total_distance = 0.0;
        for (int j = 0; j < self->vertex; j++) {
            distances[j] = calculate_distance_metric(self, j, centroids[i - 1], euclidean_distance);
            if (i == 1) {
                min_distances[j] = distances[j];
            }
            else {
                if (distances[j] < min_distances[j]) {
                    min_distances[j] = distances[j];
                }
            }
            total_distance += min_distances[j];
        }

        double random_distance = ((double)rand() / RAND_MAX) * total_distance;
        int next_centroid = -1;
        for (int j = 0; j < self->vertex; j++) {
            random_distance -= min_distances[j];
            if (random_distance <= 0) {
                next_centroid = j;
                break;
            }
        }

        for (int j = 0; j < self->vertex; j++) {
            centroids[i][j] = 0.0;
        }
        current = self->edges[next_centroid].head;
        while (current != NULL) {
            centroids[i][current->edge.to_vertex] = 1.0;
            current = current->next;
        }
    }

    free(distances);
    free(min_distances);
}

void assign_clusters_for_metric(Graph* self, int k, double** centroids, int* cluster_labels, double (*distance_func)(double*, double*, int)) {
    for (int i = 0; i < self->vertex; i++) {
        double min_distance = DBL_MAX;
        int closest_centroid = -1;

        for (int j = 0; j < k; j++) {
            double distance = calculate_distance_metric(self, i, centroids[j], distance_func);
            if (distance < min_distance) {
                min_distance = distance;
                closest_centroid = j;
            }
        }
        cluster_labels[i] = closest_centroid;
    }
}

// K-means clustering algorithm using different distance metrics
void k_means_metric(Graph* self, int k, int* cluster_labels, int num_iterations, double (*distance_func)(double*, double*, int)) {
    double** centroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(self->vertex * sizeof(double));
    }

    initialize_centroids_for_metric(self, k, centroids);

    for (int iteration = 0; iteration < num_iterations; iteration++) {
        assign_clusters_for_metric(self, k, centroids, cluster_labels, distance_func);
        update_centroids(self, k, centroids, cluster_labels);
    }

    for (int i = 0; i < k; i++) {
        free(centroids[i]);
    }
    free(centroids);
}