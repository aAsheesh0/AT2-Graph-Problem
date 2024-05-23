#include <stdio.h>
#include <stdlib.h>
#include "neural_graph.h"

// Initializer for the Neural Graph
void init_neural_graph(Graph* self, int vertex) {
    self->vertex = vertex;
    self->edges = (EdgeList*)malloc(vertex * sizeof(EdgeList));

    for (int i = 0; i < vertex; i++) {
        init_edge_list(&self->edges[i]);
    }
}

// Initializer for a new Edge List
void init_edge_list(EdgeList* list) {
    list->head = NULL;
}

// Function to implement addition of the edge from "from" vertex (node) to "to" vertex (node)
void add_edge(Graph* self, int from, int to, int weight) {
    insert_edge_at_front(&self->edges[from], to, weight);
}

// Insert the edge node to the front of the list
void insert_edge_at_front(EdgeList* list, int to_vertex, int weight) {
    EdgeNodePtr new_node = (EdgeNodePtr)malloc(sizeof(struct edgeNode));
    new_node->edge.to_vertex = to_vertex;
    new_node->edge.weight = weight;

    new_node->next = list->head;
    list->head = new_node;
}

void print_in_degrees(Graph* self) {
    int* in_degrees = (int*)malloc(self->vertex * sizeof(int));

    // Initialize all in-degrees to zero
    for (int i = 0; i < self->vertex; i++) {
        in_degrees[i] = 0;
    }

    // Traverse each vertex in the graph
    for (int i = 0; i < self->vertex; i++) {
        EdgeNodePtr current = self->edges[i].head;

        //Traverse each edge in the vertex's list
        while (current != NULL) {
            // Increment the in-degree of the destination vertex of the edge
            in_degrees[current->edge.to_vertex]++;
            current = current->next;
        }
    }

    // Print in degree of each vertex
    for (int j = 0; j < self->vertex; j++) {
        printf("Vertex %d - %d\n", j, in_degrees[j]);
    }

    //Freeing the dyanmically allocated memory for in-degress array
    free(in_degrees);
}