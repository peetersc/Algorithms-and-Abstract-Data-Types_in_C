//-----------------------------------------------------------------------------
// FindComponenets.c
// Test client for the Graph ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"List.h"
#include"Graph.h"

int main(int argc, char* argv[]){
  if(argc != 3){
    printf("Usage: <input file> <output file>\n");
    exit(1);
  }

  FILE *in, *out;
  in = fopen(argv[1], "r");
  out = fopen(argv[2], "w");

  if( in == NULL){
    printf("Unable to open file %s for reading\n", argv[1]);
    exit(1);
  }
  if(out == NULL){
    printf("Unable to open file %s for writing\n",argv[2]);
    exit(1);
  }

  char line[100];

  fgets(line,100,in);

  int numVertices = atoi(line);

  Graph G = newGraph(numVertices);

  while(fgets(line,100,in) != NULL){
    char *token;
    token = strtok(line, " ");

    int a = atoi(token);

    token = strtok(NULL, " ");

    int b = atoi(token);

    if(a == 0 && b == 0)
      break;
    else addArc(G, a, b);
  }

  fprintf(out, "Adjacency list representation of G:\n");
  printGraph(out, G);

  List S = newList();

  for(int i = 1; i < getOrder(G) + 1; i++){
    append(S,i);
  }
  printList(stdout,S);
  printf("\n");
  DFS(G,S);

  printList(stdout,S);
  printf("\n");
  Graph tGraph = transpose(G);

  DFS(tGraph,S);
  printList(stdout,S);
  printf("\n");
  int numComponets = 0;

  for(int i = 1; i < getOrder(G) + 1; i++){
    if(getParent(tGraph,i) == NIL)
      numComponets++;
  }

  fprintf(out, "\nG contains %d strongly connected components:\n", numComponets);

  List tempList = newList();
  moveBack(S);

  int x = 1;
  while(index(S) != -1){
    prepend(tempList, get(S));
    if(getParent(tGraph, get(S)) == NIL){
      fprintf(out, "Component %d: ", x);
      printList(out, tempList);
      fprintf(out,"\n");
      clear(tempList);
      x++;
    }
    movePrev(S);
  }



  fclose(in);
  fclose(out);
  freeGraph(&G);
  freeGraph(&tGraph);
  freeList(&S);
  freeList(&tempList);
}