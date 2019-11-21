//-----------------------------------------------------------------------------
// Graph.c
// Implementation file for Graph ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>

#include"Graph.h"

#define WHITE 0
#define BLACK 1
#define GRAY  2


typedef struct GraphObj{
   List *L; 
   int *color; 
   int *parent; 
   int *dist; 
   int order;
   int size;  
   int source; 
} GraphObj;

Graph newGraph(int n){
   Graph G   = malloc(sizeof(struct GraphObj));
   G->L      = calloc(n + 1, sizeof(List));
   G->color  = calloc(n + 1, sizeof(int));
   G->parent = calloc(n + 1, sizeof(int));
   G->dist   = calloc(n + 1, sizeof(int));

   for (int i = 1; i <= n; i++)
   {
      G->L[i]      = newList();
      G->color[i]  = WHITE;
      G->parent[i] = NIL;
      G->dist[i]   = INF;
   }

   G->order = n;
   G->size = 0;
   G->source = NIL;
   return G;
}

void freeGraph(Graph* pG){
   Graph toFree = *pG;
   for (int i = 1; i <= getOrder(toFree); i++){
      freeList(&(toFree->L[i]));
   }
   free(toFree->L);
   free(toFree->color);
   free(toFree->parent);
   free(toFree->dist);
   free(*pG);
   *pG = NULL;
}

int getOrder(Graph G){
   return G->order;
}

int getSize(Graph G){
   return G->size;
}

int getSource(Graph G){
   return G->source;
}

int getParent(Graph G, int u){
   if (!(0 <= u && u <= getOrder(G))){
      printf("Graph Error: getParent() called on an invalid vertex number\n");
      exit(1);
   }
   return G->parent[u];
}

int getDist(Graph G, int u){
   if (!(0 <= u && u <= getOrder(G))){
      printf("Graph Error: getDist() called on an invalid vertex number\n");
      exit(1);
   }
   if (getSource(G) == NIL){
      return INF;
   }
   return G->dist[u];
}

void getPath(List L, Graph G, int u){
   int source = getSource(G);
   int p = G->parent[u];
   if (!(0 <= u && u <= getOrder(G))){
      printf("Graph Error: getPath() called on an invalid vertex number\n");
      exit(1);
   }
   if (source == NIL){
      printf("Graph Error: getPath() called on a Graph with an invalid source\n");
      exit(1);
   }
   if (u == source){
      append(L, source);
   }
   else if (p == NIL){
      append(L, NIL);
   }
   else{
      getPath(L, G, p);
      append(L, u);
   }
}

void makeNull(Graph G){
   for (int i = 0; i <= getOrder(G); i++){
      clear(G->L[i]);
   }
   G->size = 0;
}

void addEdge(Graph G, int u, int v){
    if(G == NULL)
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addEdge(Graph G, int u, int v)");
        exit(1);
    }
    if(u < 0 || u > getOrder(G))
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addEdge(Graph G, int u, int v), out of bounds");
        exit(1);
    }
    if(v < 0 || v > getOrder(G))
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addEdge(Graph G, int u, int v), out of bounds");
        exit(1);
    }
    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;
}

void addArc(Graph G, int u, int v){
    if(G == NULL)
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addArc(Graph G, int u, int v)");
        exit(1);
    }
    if(u < 0 || u > getOrder(G))
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addArc(Graph G, int u, int v), out of bounds");
        exit(1);
    }
    if(v < 0 || v > getOrder(G))
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addArc(Graph G, int u, int v), out of bounds");
        exit(1);
    }

    moveFront(G->L[u]);
    while(index(G->L[u]) > -1 && v > get(G->L[u]))
    {
        moveNext(G->L[u]);
    }
    if(index(G->L[u]) ==  -1)
    {
        append(G->L[u], v);
    }
    else
        insertBefore(G->L[u], v);

    G->size++;
}

void BFS(Graph G, int s){
   for (int x = 0; x <= getOrder(G); x++){
      G->color[x] = WHITE;
      G->dist[x] = INF;
      G->parent[x] = NIL;
   }

   G->source = s;
   G->color[s] = GRAY;
   G->dist[s] = 0;
   G->parent[s] = NIL;
   List list = newList();
   append(list, s);

   while (length(list) > 0)
   {
      int x = front(list);
      deleteFront(list);
      List adj = G->L[x];
      for (moveFront(adj); index(adj) != -1; moveNext(adj))
      {
         int y = get(adj);
         if (G->color[y] == WHITE){
            G->color[y] = GRAY;
            G->dist[y] = G->dist[x] + 1;
            G->parent[y] = x;
            append(list, y);
         }
      }
      G->color[x] = BLACK;
   }
   freeList(&list);
}

void printGraph(FILE* out, Graph G){
   for (int i = 1; i < getOrder(G); i++){
      List list = G->L[i];
      fprintf(out, "%d:", i);
      for (moveFront(list); index(list) != -1; moveNext(list)){
         fprintf(out, " %d", get(list));
      }
      fprintf(out, "\n");
   }
}