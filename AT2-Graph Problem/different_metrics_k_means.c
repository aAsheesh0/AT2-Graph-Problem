#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

double euclidean_distance2(double* point1, double* point2, int dimensions) {
    double sum = 0.0;
    for (int i = 0; i < dimensions; i++) {
        sum += pow(point1[i] - point2[i], 2);
    }
    return sqrt(sum);
}
