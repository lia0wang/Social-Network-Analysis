// Centrality Measures ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CentralityMeasures.h"
#include "FloydWarshall.h"
#include "Graph.h"

// Funcition Declaration
static EdgeValues newEvs(Graph g);

/**
 * Finds  the  edge  betweenness  centrality  for each edge in the given
 * graph and returns the results in a  EdgeValues  structure.  The  edge
 * betweenness centrality of a non-existant edge should be set to -1.0.
 */
EdgeValues edgeBetweennessCentrality(Graph g) {
	assert(g != NULL);
	
    // Initialising the evs.
	EdgeValues evs = newEvs(g);
    
    // Find shortest paths between all pairs of nodes using task 1.
    ShortestPaths sps = FloydWarshall(g);
    
    // For each edge, calculate the number of shortest paths through edge. 
    for (Vertex v = 0; v < evs.numNodes; v++) {
        for (Vertex w = 0; w < evs.numNodes; w++) {
            // If the edge is non-existant, set it to -1.
            if (!GraphIsAdjacent(g, v, w)) evs.values[v][w] = -1;
            // If the path from v->w exists, 
            // calculate the total number of shortest paths from v->w.
            if (sps.next[v][w] != -1) {
                // Set a trackable vertex trk go through from v->w.
                Vertex trk = v;    
                while (trk != w) {
                    // Find the next vertex from v->w.
                    int next_vtx = sps.next[trk][w];
                    // Increase the edge betweenness if find a shortest path.
                    evs.values[trk][next_vtx] += 1;
                    // Update trk as the next vertex.
                    trk = next_vtx;
                }
            }
        }
    }
    
    freeShortestPaths(sps);
	return evs;
}

/**
 * Prints  the  values in the given EdgeValues structure to stdout. This
 * function is purely for debugging purposes and will NOT be marked.
 */
void showEdgeValues(EdgeValues evs) {
    int i = 0;
	int j = 0;
	for (i = 0; i < evs.numNodes; i++) {
		for (j = 0; j < evs.numNodes; j++) {
			if (evs.values[i][j] != -1.0) {
				printf("%d, %d : %lf\n", i, j, evs.values[i][j]);
			}
		}
	}
}

/**
 * Frees all memory associated with the given EdgeValues  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeEdgeValues(EdgeValues evs) {
    // Free memory for 2D array.
    for (int i = 0; i < evs.numNodes; i++) {
        free(evs.values[i]);
    }
    free(evs.values);
    return;   
}

/**
 * Edge Values Initialization.
 */
static EdgeValues newEvs(Graph g) {
    // Initialising the evs.
	EdgeValues e;
	e.numNodes = GraphNumVertices(g);
    // Allocate memory for 2D array.
	e.values = malloc(e.numNodes * sizeof(double *));
	for (int i = 0; i < e.numNodes; i++) {
        e.values[i] = malloc(e.numNodes * sizeof(double));
    }
    
    return e;
}

