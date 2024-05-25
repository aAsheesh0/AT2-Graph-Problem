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