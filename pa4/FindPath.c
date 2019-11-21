#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Graph.h"

#define MAX_LEN 160

int main(int argc, char* argv[]){

   int n = 0;
   int v1, v2, source, dest;
   Graph G = NULL;
   FILE *in, *out;

   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if(in == NULL){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if(out == NULL){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }
   
   fscanf(in, "%d", &n);
   G = newGraph(n);
   
   while (fgetc(in) != EOF){
      fscanf(in, "%d", &v1);
      fscanf(in, "%d", &v2);
      if (v1 == 0 && v2 == 0){
         break;
      }
      addEdge(G, v1, v2);
   }
   
   printGraph(out, G);
   fprintf(out, "\n");
   
   while (fgetc(in) != EOF){
      fscanf(in, "%d", &source);
      fscanf(in, "%d", &dest);
      if (source == 0 && dest == 0){
         break;
      }
      List L = newList();
      BFS(G, source);
      getPath(L, G, dest);
      if (getDist(G, dest) != INF){
         fprintf(out, "The distance from %d to %d is %d\n", source, dest, length(L) - 1);
         fprintf(out, "A shortest %d-%d path is: ", source, dest);
         printList(out, L);
         fprintf(out, "\n\n");
      }
      else{
         fprintf(out, "The distance from %d to %d is infinity\n", source, dest);
         fprintf(out, "No %d-%d path exists\n", source, dest);
         fprintf(out, "\n");
      }

      freeList(&L);
   }
   
   
   freeGraph(&G);
   fclose(in);
   fclose(out);
   
   return(0);
}