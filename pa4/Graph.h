//------------------------------------------------------------------------------------------------------------
// Graph.h
// Header file for Graph ADT
//------------------------------------------------------------------------------------------------------------

#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_

#define INF -1
#define NIL 0

#include"List.h"

// Exported type             ----------------------------------------------------------------------------------
typedef struct GraphObj* Graph;

// Constructors-Destructors  ----------------------------------------------------------------------------------
Graph newGraph(int);
void freeGraph(Graph*);

// Access functions          ---------------------------------------------------------------------------------
int  getOrder (Graph);
int  getSize  (Graph);
int  getSource(Graph);
int  getParent(Graph, int);
int  getDist  (Graph, int);
void getPath (List, Graph, int);

// Manipulation procedures   --------------------------------------------------------------------------------
void makeNull(Graph);
void addEdge (Graph, int, int);
void addArc  (Graph, int, int);
void BFS     (Graph, int);

// Other Functions           ---------------------------------------------------------------------------------
void printGraph(FILE*, Graph);

#endif