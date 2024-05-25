#ifndef NEURAL_GRAPH_H
#define NEURAL_GRAPH_H

/**
*  Graph Data Structure for Neural Network Graph
*/

typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} *EdgeNodePtr;

typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

typedef struct graph {
	int vertex;
	EdgeList* edges;
} Graph;

// Prototypes for Neural Network Graph Data structure
void init_neural_graph(Graph* self, int vertex);
void add_edge(Graph* self, int from, int to, int weight);
void init_edge_list(EdgeList* list);
void insert_edge_at_front(EdgeList* list, int to_vertex, int weight);
void print_in_degrees(Graph* self); // Tester function
void free_neural_graph(Graph* self);

#endif