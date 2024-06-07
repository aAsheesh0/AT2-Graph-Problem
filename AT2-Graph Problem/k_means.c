#include "k_means.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

//gaug the similarity and diversity of sample sets
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

//calculate the distance between a vertex and a centroid using the Jaccard similarity.
double calculate_distance(Graph* self, int vertex, double* centroid) {
    int* vertex_edges = (int*)malloc(self->vertex * sizeof(int));
    int vertex_edges_count = 0;
    EdgeNodePtr current = self->edges[vertex].head;
    while (current != NULL) {
        vertex_edges[vertex_edges_count++] = current->edge.to_vertex;
        current = current->next;
    }

    int* centroid_edges = (int*)malloc(self->vertex * sizeof(int));
    int centroid_edges_count = 0;
    for (int i = 0; i < self->vertex; i++) {
        if (centroid[i] > 0) {
            centroid_edges[centroid_edges_count++] = i;
        }
    }

    double distance = 1.0 - jaccard_similarity(vertex_edges, vertex_edges_count, centroid_edges, centroid_edges_count);

    free(vertex_edges);
    free(centroid_edges);

    return distance;
}

//initialize centeroids based on selection with the help of helper functions to calculate the centroid to initialize to
void initialize_centroids(Graph* self, int k, double** centroids) {
    //calculate the first centroid randomly
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
            distances[j] = calculate_distance(self, j, centroids[i - 1]);
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
    //Debug
    /*printf("Cluster Assignments:\n");
    for (int i = 0; i < self->vertex; i++) {
        printf("Vertex %d: Cluster %d\n", i, cluster_labels[i]);
    }*/
}

void update_centroids(Graph* self, int k, double** centroids, int* cluster_labels) {
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

    //Debug testing
    /*printf("Updated Centroids:\n");
    for (int i = 0; i < k; i++) {
        printf("Centroid %d: ", i);
        for (int j = 0; j < self->vertex; j++) {
            printf("%f ", centroids[i][j]);
        }
        printf("\n");
    }*/

    // Free memory
    free(cluster_sizes);
}

//compute K-means clustering
void k_means(Graph* self, int k, int* cluster_labels, int num_iterations) {
    double** centroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(self->vertex * sizeof(double));
    }
    //printf("Initialization started.\n");
    initialize_centroids(self, k, centroids);

    for (int iteration = 0; iteration < num_iterations; iteration++) {
        //printf("Iteration %d started.\n", iteration + 1);
        assign_clusters(self, k, centroids, cluster_labels);
        update_centroids(self, k, centroids, cluster_labels);

        //printf("Iteration %d completed.\n", iteration + 1);
    }

    for (int i = 0; i < k; i++) {
        free(centroids[i]);
    }
    free(centroids);
}

double degree_centrality(Graph* graph, int vertex) {
    int degree = 0;
    // Iterate through the edges of the graph to count the number of edges incident to the vertex
    for (int i = 0; i < graph->vertex; i++) {
        EdgeNodePtr current = graph->edges[i].head;
        while (current != NULL) {
            if (current->edge.to_vertex == vertex) {
                degree++;
            }
            current = current->next;
        }
    }
    return (double)degree;
}

//Find the most neuron for each cluster based on degree centrality. 
// NOTE- The important neurons here are shown from our util array, for this assignment we have used this approach. However, the actual neuron can also be calculated.
void most_important_neuron(Graph* graph, int k, int* cluster_labels) {
    int* most_important_nodes = (int*)malloc(k * sizeof(int));
    double* highest_centrality = (double*)malloc(k * sizeof(double));
    for (int i = 0; i < k; i++) {
        highest_centrality[i] = -1.0;
    }

    // Calculate degree centrality for each vertex and find the most important node for each cluster
    for (int i = 0; i < graph->vertex; i++) {
        double centrality = degree_centrality(graph, i);
        if (centrality > highest_centrality[cluster_labels[i]]) {
            highest_centrality[cluster_labels[i]] = centrality;
            most_important_nodes[cluster_labels[i]] = i;
        }
    }

    //print the most important node for each cluster
    for (int i = 0; i < k; i++) {
        printf("Cluster %d: Most Important Node is the one allocated in the array at %d, Degree Centrality = %f\n", i, most_important_nodes[i], highest_centrality[i]);
    }

    free(most_important_nodes);
    free(highest_centrality);
}