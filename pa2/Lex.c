/****************************************************************************************
*  Lex.c
*  sorts the list into lexicon order
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"

#define MAX_LEN 160

int main(int argc, char* argv[])
{

   int n = 0, count = 0;
   FILE *in, *out; 
   char line[MAX_LEN];


   // check command line for correct number of arguments
   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   // open files for reading  
   in  = fopen(argv[1], "r");

   //file check
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   //get number of lines
   while( fgets(line, MAX_LEN, in) != NULL)
   {
      n++;
   }
   fclose(in);

   //words
   char *A[n];

   // open files for reading 
   in  = fopen(argv[1], "r");

   //file check
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   //populates array with file contens
   while (fgets(line, MAX_LEN, in) != NULL){ 
      A[count] = (char*)malloc(sizeof(line));
      strcpy(A[count],line);
      count++;
   }
   fclose(in);

   //list of indices to be sorted
   List list = newList();
   prepend(list, 0);

   // from insertion sort
   int j;
   for (int i = 1; i < n; i++)
   {
      moveBack(list);
      j = get(list);

      while(strcmp(A[i], A[j]) <= 0)
      {
         movePrev(list);
         if (indexx(list) == -1)
         {
            prepend(list, i);
            break;
         }

         j = get(list);
      }

      if (indexx(list) >= 0)
      {
         insertAfter(list, i);
      }
   }
   
   // open files for writing
   out = fopen(argv[2], "w");

   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }

   //print out to file
   moveFront(list);
   for (int i = 0; i < n; i++)
   {
      fprintf(out, "%s", A[get(list)]);
      moveNext(list);
   }
   /* close files */
   fclose(out);

   //de alloocate mem
   for (int i = 0; i < n; i++)
   {
      free(A[i]);
   }
   freeList(&list);

   return(0);  
}
