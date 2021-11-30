// Floyd Warshall ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "FloydWarshall.h"
#include "Graph.h"

// Function Declaration
static ShortestPaths newShortestPaths(Graph g);

/**
 * Finds all shortest paths between all pairs of nodes.
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - distance matrix
 * - matrix of intermediates (see description above)
 */
ShortestPaths FloydWarshall(Graph g) {
	assert(g != NULL);
    
    // Initialising the sps.
    ShortestPaths sps = newShortestPaths(g);

    // Set nums from 0->numNodes as the middle number.
    for (int mid = 0; mid < sps.numNodes; mid++) {
        for (Vertex v = 0; v < sps.numNodes; v++) {
            for (Vertex w = 0; w < sps.numNodes; w++) {
                // If the path v->mid exists and the dist v->w greater than
                // the sums of dist v->mid plus dist mid->w.
                if (sps.dist[v][mid] != INFINITY
                && sps.dist[mid][w] != INFINITY
                && sps.dist[v][w] > (sps.dist[v][mid] + sps.dist[mid][w])) {
                    // Upstate the next and dist.
                    sps.next[v][w] = sps.next[v][mid];
                    sps.dist[v][w] = sps.dist[v][mid] + sps.dist[mid][w];
                }
            }
        }
    }
    
	return sps;
}

/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We will not call this function during testing, so you may  print  out
 * the  given  ShortestPaths  structure in whatever format you want. You
 * may choose not to implement this function.
 */
void showShortestPaths(ShortestPaths sps) {
    int i = 0;
	int j = 0;
	printf("Shortest path distances\n");
	for (i = 0; i < sps.numNodes; i++) {
		for (j = 0; j < sps.numNodes; j++) {
			printf("    %d, %d : ", i, j);
			if (sps.dist[i][j] == INFINITY) {
				printf("INF\n");		
			} else {
				printf("%d\n", sps.dist[i][j]);
			}
		}
	}
	
	printf("\nNext vertices\n");
	for (i = 0; i < sps.numNodes; i++) {
		for (j = 0; j < sps.numNodes; j++) {
			printf("    %d, %d : ", i, j);
			if (sps.next[i][j] == -1) {
				printf("X\n");
			} else {			
				printf("%d\n", sps.next[i][j]);
			}
		}
	}
}

/**
 * Frees  all  memory associated with the given ShortestPaths structure.
 * We will call this function during testing, so you must implement it.
 */
void freeShortestPaths(ShortestPaths sps) {
    // Free memory for 2D array.
    for (int i = 0; i < sps.numNodes; i++) {
        free(sps.dist[i]);
        free(sps.next[i]);
    }
    free(sps.dist);
    free(sps.next);
    return;
}

/**
 * Shortest Paths Initialization.
 */
static ShortestPaths newShortestPaths(Graph g) {
    // Initialising Paths ps.
    ShortestPaths ps;
    ps.numNodes = GraphNumVertices(g);
    // Allocate memory for 2D array.
    ps.dist = malloc(ps.numNodes * sizeof(int *));
    ps.next = malloc(ps.numNodes * sizeof(int *));
    for (int i = 0; i < ps.numNodes; i++) {
        ps.dist[i] = malloc(ps.numNodes * sizeof(int));
        ps.next[i] = malloc(ps.numNodes * sizeof(int));
    }

    for (Vertex v = 0; v < ps.numNodes; v++) {
        for (Vertex w = 0; w < ps.numNodes; w++) {
            // The distance from a vertex to itself is 0.
            // For all v, next[v][v] should be set to -1.
            if (v == w) {
                ps.dist[v][w] = 0;
                ps.next[v][w] = -1;
            }
            // If there is no path from v to w, the distance
            // should be set to INFINITY
            // next[v][w] should be set to -1.
            else if (!GraphIsAdjacent(g, v, w)) {
                ps.dist[v][w] = INFINITY;
                ps.next[v][w] = -1;
            } 
        }
    }

    for (Vertex v = 0; v < ps.numNodes; v++) {
        // Returns a list of edges midaining vertex, weight.
        AdjList edgeList = GraphOutIncident(g, v);
        for (AdjList current = edgeList; 
            current != NULL; 
            current = current->next) {
            Vertex w = current->v;
            // Assign the value of weight to each edge.
            // Initialise the value of next equals w if its from v->w.
            ps.dist[v][w] = current->weight;
            ps.next[v][w] = w;
        }
    }
    
    return ps;
}

