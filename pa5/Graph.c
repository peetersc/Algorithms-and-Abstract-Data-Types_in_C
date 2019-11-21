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
   int *discover; 
   int *finish;
   int order;
   int size;  
   int time;
} GraphObj;

Graph newGraph(int n){
   Graph G     = malloc(sizeof(struct GraphObj));
   G->L        = calloc(n + 1, sizeof(List));
   G->color    = calloc(n + 1, sizeof(int));
   G->parent   = calloc(n + 1, sizeof(int));
   G->discover = calloc(n + 1, sizeof(int));
   G->finish   = calloc(n + 1, sizeof(int));
   G->order    = n;
   G->size     = 0;
   G->time     = 0;

   for (int i = 0; i <= n; i++)
   {
      G->L[i]        = newList();
      G->color[i]    = WHITE;
      G->parent[i]   = NIL;
      G->discover[i] = UNDEF;
      G->finish[i]   = UNDEF;

   }

   return G;
}

void freeGraph(Graph* pG){
   Graph toFree = *pG;
   for (int i = 0; i <= getOrder(toFree); i++){
      freeList(&(toFree->L[i]));
   }
   free(toFree->L);
   free(toFree->color);
   free(toFree->parent);
   free(toFree->discover);
   free(toFree->finish);
   free(*pG);
   *pG = NULL;
}

int getOrder(Graph G){
    if(G == NULL)
    {
        fprintf(stderr, "Invalid: cannot call getOrder()");
        exit(1);
    }
    return G->order;
}

int getSize(Graph G){
      if(G == NULL)
    {
        fprintf(stderr, "Invalid: cannot call getSize()");
        exit(1);
    }
    return G->size;
}

//part of DFS pa4
/*int getSource(Graph G){
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
}*/

int getParent(Graph G, int u)  /* Pre: 1<=u<=n=getOrder(G) */ 
{
  if (!(1 <= u || u <= getOrder(G)))
  {
      printf("Graph Error: getParent() preconditions not met\n");
      exit(1);
  }

  return G->parent[u];
}

int getDiscover(Graph G, int u) /* Pre: 1<=u<=n=getOrder(G) */ 
{
  if (!(1 <= u || u <= getOrder(G)))
  {
      printf("Graph Error: getParent() preconditions not met\n");
      exit(1);
  }
  
  return G->discover[u];
}

int getFinish(Graph G, int u)  /* Pre: 1<=u<=n=getOrder(G) */
{
  if (!(1 <= u || u <= getOrder(G)))
  {
      printf("Graph Error: getParent() preconditions not met\n");
      exit(1);
  }
  
  return G->finish[u];
}

void addEdge(Graph G, int u, int v){
    if(G == NULL)
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addEdge(Graph G, int u, int v)");
        exit(1);
    }
    if(u < 1 || u > getOrder(G))
    {
        fprintf(stderr, "Invalid(Graph Usage): cannot call addEdge(Graph G, int u, int v), out of bounds");
        exit(1);
    }
    if(v < 1 || v > getOrder(G))
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
        fprintf(stderr, "Invalid: cannot call addArc(Graph G, int u, int v)");
        exit(1);
    }
    if(u < 1 || u > getOrder(G))
    {
        fprintf(stderr, "Invalid: cannot call addArc(Graph G, int u, int v), out of bounds");
        exit(1);
    }
    if(v < 1 || v > getOrder(G))
    {
        fprintf(stderr, "Invalid: cannot call addArc(Graph G, int u, int v), out of bounds");
        exit(1);
    }

    List Q = G->L[u];

    if(isEmpty(Q))
    {
        append(Q, v);
        G->size++;
        return;
    }
    moveFront(Q);
    while(index(Q) != -1)
    {
        if(v == get(Q))
        {
            return;
        }
        if(v < get(Q))
        {
            insertBefore(Q, v);
            G->size++;
            return;
        }
        moveNext(Q);
    }
    append(Q, v);
    G->size++;

}
/*
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
}*/

void visit(Graph G, List S, int u, int *time) 
{
  G->color[u] = GRAY;
  G->discover[u] = ++*(time);
  moveFront(G->L[u]);
  for(int i = 1; i < length(G->L[u]) + 1; i++){
    int y = get(G->L[u]);
    if(G->color[y] == WHITE){
      G->parent[y] = u;
      visit(G, S, y, time);
    }
    moveNext(G->L[u]);
  }
  G->finish[u] = ++*(time);
  prepend(S, u);
  G->color[u] = BLACK;
}

void DFS(Graph G, List S) /* Pre: length(S)==getOrder(G) */
{
  for(int i = 1; i < getOrder(G) + 1; i++){
    G->color[i]    = WHITE;
    G->parent[i]   = NIL;
    G->discover[i] = UNDEF;
    G->finish[i]   = UNDEF;
  }
  int time = 0;
  moveFront(S);
  for(int i = 1; i < getOrder(G) + 1; i++){
    int u = get(S);
    moveNext(S);
    if(G->color[u] == WHITE){
      visit(G, S, u, &time);
    }
  }

  for(int i = 1; i < getOrder(G) + 1; i++ ){
    deleteBack(S);
  }
}

/* Other Functions */
Graph transpose(Graph G)
{
    if(G == NULL)
    {
        fprintf(stderr, "Invalid: cannot call transpose(Graph G)");
        exit(1);
    }
    Graph Transpose = newGraph(getOrder(G));

    for(int i = 1; i <= getOrder(G); i++)
    {
        moveFront(G->L[i]);
        while(index(G->L[i])>= 0)
            {
                addArc(Transpose, get(G->L[i]), i);
                moveNext(G->L[i]);
            }
    }
    return Transpose;
}

Graph copyGraph(Graph G)
{
     if(G == NULL)
    {
        fprintf(stderr, "Invalid: cannot call copyGraph(Graph G)");
        exit(1);
    }

    Graph Copy = newGraph(getOrder(G));
    for(int i = 1; i <= getOrder(G); i++)
    {
        moveFront(G->L[i]);

        while(index(G->L[i]) >= 0)
        {
            addArc(Copy, i, get(G->L[i]));
            moveNext(G->L[i]);
        }
    }
    return Copy;
}

void printGraph(FILE* out, Graph G){
   if( G==NULL )
   {
      printf("Graph Error: calling printGraph() on NULL Graph reference\n");
      exit(1);
   }
  for(int i = 1; i<=getOrder(G); i++)
  {
    fprintf(out,"%d: ",i);
    printList(out, G->L[i]);
    fprintf(out, "\n");
  }
}