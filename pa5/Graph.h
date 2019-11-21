//------------------------------------------------------------------------------------------------------------
// Graph.h
// Header file for Graph ADT
//------------------------------------------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include"List.h"

#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_

#define INF   -2
#define NIL    0
#define UNDEF -1

// Exported type             ----------------------------------------------------------------------------------
typedef struct GraphObj* Graph;

// Constructors-Destructors  ----------------------------------------------------------------------------------
Graph newGraph (int);
void  freeGraph(Graph*);

// Access functions          ---------------------------------------------------------------------------------
int  getOrder (Graph);
int  getSize  (Graph);
//int  getSource(Graph);
//int  getDist  (Graph, int);
int getParent  (Graph, int);   /* Pre: 1<=u<=n=getOrder(G) */ 
int getDiscover(Graph, int); /* Pre: 1<=u<=n=getOrder(G) */ 
int getFinish  (Graph, int);   /* Pre: 1<=u<=n=getOrder(G) */

// Manipulation procedures   --------------------------------------------------------------------------------
//void makeNull(Graph);
void addArc (Graph, int, int); /* Pre: 1<=u<=n, 1<=v<=n */ 
void addEdge(Graph, int, int); /* Pre: 1<=u<=n, 1<=v<=n */ 
void DFS    (Graph, List);     /* Pre: length(S)==getOrder(G) */
void visit  (Graph G, List S, int u, int *time);

//void BFS     (Graph, int);

// Other Functions           ---------------------------------------------------------------------------------
Graph transpose (Graph);
Graph copyGraph (Graph);
void  printGraph(FILE*, Graph);

#endif