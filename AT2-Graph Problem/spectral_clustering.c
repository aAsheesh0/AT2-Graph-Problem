#include <stdlib.h>
#include <stdio.h>
#include "spectral_clustering.h"

// Computing the degree of each neuron, directly from the adjacency list
void compute_degrees(Graph* self, int* degrees) {
    for (int i = 0; i < self->vertex; i++) {
        degrees[i] = 0;
        EdgeNodePtr current = self->edges[i].head;
        while (current != NULL) {
            degrees[i]++;
            current = current->next;
        }
    }
}

// Laplacian matrix to compute the difference between the degree matrix and adjacency matrix
// The eigenvalues and eigenvectors of this matrix will be used to partition the 
// lobes of our cerebral cortex in the brain dataset
void compute_laplacian(Graph* self, int* degrees, double** laplacian) {
    for (int i = 0; i < self->vertex; i++) {
        for (int j = 0; j < self->vertex; j++) {
            if (i == j) {
                laplacian[i][j] = degrees[i];
            }
            else {
                EdgeNodePtr current = self->edges[i].head;
                int found = 0;
                while (current != NULL) {
                    if (current->edge.to_vertex == j) {
                        found = 1;
                        break;
                    }
                    current = current->next;
                }
                laplacian[i][j] = found ? -1 : 0;
            }
        }
    }
}