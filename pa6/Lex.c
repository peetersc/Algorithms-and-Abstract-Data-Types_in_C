/*********************************************************************************
* Cameron Peeters, capeeter
* pa6
* Lex.c
* Lexicon sort implementation that utilizes the Dictionary ADT
*********************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"
#define MAX_LEN 160


int main(int argc, char* argv[]){
   // check command line for correct number of arguments
   if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   //File pointers
   FILE *in, *out; 
   // open files for reading  
   in  = fopen(argv[1], "r");

   //file check
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   //get number of lines
   int n = 0;
   char line[MAX_LEN];
   while( fgets(line, MAX_LEN, in) != NULL) n++;
   fclose(in);

   // open files for reading 
   in  = fopen(argv[1], "r");
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   //populates Dictionary with file contens
   Dictionary A = newDictionary(0);
   char *words[n];
   int count = 0;
   while (fgets(line, MAX_LEN, in) != NULL){ 
      words[count] = (char*)malloc(sizeof(line));
      strcpy(words[count],line);
      insert(A, words[count], count);
      count++;
   }
   fclose(in);
   
   // open files for writing
   out = fopen(argv[2], "w");
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }

   printDictionary(out,A);
   freeDictionary(&A);
   for (int i = 0; i < n; ++i){
      free(words[i]);
   }
   return(0);  

}