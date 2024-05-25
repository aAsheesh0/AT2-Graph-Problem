#ifndef SPECTRAL_CLUSTERING_H
#define SPECTRAL_CLUSTERING_H

#include "neural_graph.h"

void compute_degrees(Graph* self, int* degrees);
void compute_laplacian(Graph* self, int* degrees, double** laplacian);

#endif