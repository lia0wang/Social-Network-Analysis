// Girvan-Newman Algorithm for community discovery
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../analysisAlgorithms/CentralityMeasures.h"
#include "../analysisAlgorithms/GirvanNewman.h"
#include "../graphADT/Graph.h"
#include "../analysisAlgorithms/FloydWarshall.h"

// Function Declaration.
static Dendrogram createDendogramNode(int v);
static Dendrogram recursiveDendro(Graph g, Dendrogram dn, int *cmpntID, 
                                  bool *inTreeVtxArray);                     
static void DFS(Vertex v, int *visited, Graph g, int id, int *cmpntID);
static int getCmpntNum(Graph g, int *cmpntID);
static int getUniqVtx(int numVtx, int *cmpntID, 
                      int numCmpnt, int uniqVtxArray[]);                        
static bool rmInTreeVtx(int numVtx, int uniqVtxArray[], bool *inTreeVtxArray);
                                            
/**
 * Generates  a Dendrogram for the given graph g using the Girvan-Newman
 * algorithm.
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram GirvanNewman(Graph g) {
    int numVtx = GraphNumVertices(g);
    Dendrogram dn = createDendogramNode(0);
    
    int *cmpntID = calloc(sizeof(int), numVtx);
    bool *inTreeVtxArray = calloc(sizeof(bool), numVtx);
    
    recursiveDendro(g, dn, cmpntID, inTreeVtxArray);
	return dn;
}

/**
 * Frees all memory associated with the given Dendrogram  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeDendrogram(Dendrogram d) {
    if (d != NULL) return;
    freeDendrogram(d->left);
    freeDendrogram(d->right);
    free(d);
}

/**
 *  Dendrogram Recursion.
 */ 
static Dendrogram recursiveDendro(Graph g, Dendrogram dn, int *cmpntID, bool *inTreeVtxArray) {
    
    // Get num of vertex and components.
    int numVtx = GraphNumVertices(g);
    int numCmpnt = getCmpntNum(g, cmpntID);

    EdgeValues evs = edgeBetweennessCentrality(g);
    // Base Case: if num of leaves = num of vertices.
    if (numVtx == numCmpnt) {
        return dn;
    }
       
    // Find the edges with largest betweeness (E_i).
    double max = evs.values[0][0];
    for (int i = 0; i < numVtx; i++) {
        for (int j = 0; j < numVtx; j++) {
            if (evs.values[i][j] > max) {
                // Find max value 
                max = evs.values[i][j];
            }
        }
    }    

    // Remove E_i from the graph.
    for (int i = 0; i < numVtx; i++) {
        for (int j = 0; j < numVtx; j++) {
            if (evs.values[i][j] == max) {
                GraphRemoveEdge(g, i, j);
                evs.values[i][j] = -1.0;

            }
        }
    }  

    // Updated num of components.
    int newNumCmpnt = getCmpntNum(g, cmpntID);
    
    // If new component split out.
    if (newNumCmpnt > numCmpnt) {
            // Insert new nodes into tree.
            
            // Set an array to store the uniq vtx.
            int uniqVtxArray[numVtx-1];
            // Initialising the array.
            for (int i = 0; i < numVtx; i++) {
                uniqVtxArray[i] = -1;
            }

            // Find the number of vertices with a unique component ID.
            int numUniqVtx = getUniqVtx(numVtx, cmpntID, newNumCmpnt, 
                                        uniqVtxArray);
            
            // Case 1:
            // If there is only 1 unique vertex:
            if (numUniqVtx == 1) {
                // Insert that vertex into the dn->left and set dn->right = -1 
                dn->left = createDendogramNode(uniqVtxArray[0]);
                dn->right = createDendogramNode(-1);

                // Put uniqVtxArray[0] into the inTreeVtxArray array.
                // Set it as true cuz its in the tree now.
                inTreeVtxArray[uniqVtxArray[0]] = true;
                
                // recursiveDendro(g, dn->right, cmpntID).
                recursiveDendro(g, dn->right, cmpntID, inTreeVtxArray);
            } 
            
            // Case 2:
            // If there are more than 1 unique vertices:
            else {      
                // Remove vertices from unique vertices array 
                // that are already in the tree.
                rmInTreeVtx(numVtx, uniqVtxArray, inTreeVtxArray);
                
                // Insert 1st vertex in dn->left and 2nd vertex in dn->right.
                bool goLeft = true;
                for (int n = 0; n < numUniqVtx; n++) {
                    if (uniqVtxArray[n] != -1 && goLeft == true) {
                        // Put uniqVtxArray[n] into inTreeVtxArray array.
                        dn->left = createDendogramNode(uniqVtxArray[n]);
                        // Set it as true cuz its in the tree now.
                        inTreeVtxArray[uniqVtxArray[n]] = true;
                        goLeft = false;
                    } else if (uniqVtxArray[n] != -1 && goLeft == false) {
                        // Put uniqVtxArray[n] into inTreeVtxArray array.
                        dn->right = createDendogramNode(uniqVtxArray[n]);
                        // Set it as true cuz its in the tree now.
                        inTreeVtxArray[uniqVtxArray[n]] = true;
                    }
                }
            }   
    }
    
    return dn;
}

/**
 *  Create a new node.
 */
static Dendrogram createDendogramNode(int data) {
    Dendrogram dn = malloc(sizeof(DNode));
    assert(dn != NULL);
    dn->vertex = data;
    dn->left = NULL;
    dn->right = NULL;
    return dn;
}

// cmpntID[] = [0] = 1 [1] = 1 [2] = 1 [3] = 2
/**
 *  DFS Processing.
 */
static void DFS(Vertex v, int *visited, Graph g, int id, int *cmpntID) {
    visited[v] = 1;
    cmpntID[v] = id;
    int numVtx = GraphNumVertices(g);
    for (Vertex w = 0; w < numVtx; w++) {
        if ((GraphIsAdjacent(g, v, w) || GraphIsAdjacent(g, w, v)) 
            && visited[w] == -1) {
            DFS(w, visited, g, id, cmpntID);
        }
    }

    return;
}

/**
 *  Get number of components by using DFS.
 */
static int getCmpntNum(Graph g, int *cmpntID) {
    int numVtx = GraphNumVertices(g);
    int *visited = malloc(sizeof(int *));
    
    // Mark all vertices as not visited.
    for (Vertex v = 0; v < numVtx; v++) {
        visited[v] = -1;
    }
    
    // Variable to store number of components.
    int cmpntCounter = 0; 
    for (Vertex v = 0; v < numVtx; v++) {
        if (visited[v] == -1) {
            // Mark all the nodes in the component that 
            // vertex v is in as visited.
            DFS(v, visited, g, cmpntCounter, cmpntID);
            cmpntCounter++;
        }
    }
    
    free(visited);
    return cmpntCounter;
}

/**
 *  Put unique vertices into the uniqVtxArray[].
 *  Find the number of vertices with a unique component ID.
 */
static int getUniqVtx(int numVtx, int *cmpntID, 
                      int numCmpnt, int uniqVtxArray[]) {
    
    int cmpntIDCounter[numCmpnt-1];
    for (int i = 0; i < numCmpnt; i++) {
        cmpntIDCounter[i] = 0;
    }
    
    // For every component ID in the array cmpntIDCounter:
    for (int i = 0; i < numCmpnt; i++) {
        // cmpntIDCounter[0] = 2
        // cmpntIDCounter[1] = 1
        // For every vertex in the graph:
        for (int j = 0; j < numVtx; j++) {
            // If the component id of vertex j is equal to the component id 
            // in the cmpntIDCounter:
            if (cmpntID[j] == i) {
                // The number of vertices with cmpntID i increases by 1.
                cmpntIDCounter[i]++;
            }
        }
    }
    
    int counter = 0;
    int uniqCmpntID = -1;
    for (int i = 0; i< numCmpnt; i++) {
        // If the component id has only 1 vertex:
        if (cmpntIDCounter[i] == 1) {
            // Find the uniq component w the componentID.
            uniqCmpntID = i;
            // Add the vertex with that component id to the 
            // unique vertices array.
            for (int v = 0; v < numVtx; v++) {
                if (cmpntID[v] == uniqCmpntID) {
                    // Add unique vertex to uniqVtxArray array.
                    uniqVtxArray[counter] = v;
                    counter++;
                }
            }
        }
    }
    
    return counter;
}

/**
 *  Remove Vtx that are already in the tree(Dendrogram).
 */
static bool rmInTreeVtx(int numVtx, int uniqVtxArray[], bool *inTreeVtxArray) {
    for (int i = 0; i < numVtx; i++) {
        // If the vertex is already in the tree, remove it(Set it as -1).
        if (uniqVtxArray[i] != -1 && inTreeVtxArray[i] == true) {
            uniqVtxArray[i] = -1;
        
        }
    }
    
    return false;
}

