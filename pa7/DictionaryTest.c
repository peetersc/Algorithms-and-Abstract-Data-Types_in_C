/*********************************************************************************
* Cameron Peeters, capeeter
* PA7
* DictionaryTest.c
* Dictionary Test File for Dictionary ADT
*********************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char* argv[]){
   Dictionary A = newDictionary(0);

   //void* nil = NULL;

   /*insert(A, "a", nil);
   insert(A, "b", nil);
   insert(A, "c", nil);
   insert(A, "d", nil);
   insert(A, "e", nil);
   insert(A, "f", nil);
   insert(A, "g", nil);
   insert(A, "h", nil);
   insert(A, "i", nil);
   insert(A, "j", nil);
   insert(A, "k", nil);
   insert(A, "l", nil);
   insert(A, "m", nil);
   insert(A, "n", nil);
   insert(A, "o", nil);
   insert(A, "p", nil);
   insert(A, "q", nil);
   insert(A, "r", nil);
   insert(A, "s", nil);
   insert(A, "t", nil);
   insert(A, "u", nil);
   insert(A, "v", nil);
   insert(A, "w", nil);
   insert(A, "x", nil);
   insert(A, "y", nil);
   insert(A, "z", nil);*/

   /*insert(A, "m", nil);
   insert(A, "n", nil);
   insert(A, "o", nil);
   insert(A, "p", nil);
   insert(A, "q", nil);
   insert(A, "f", nil);
   insert(A, "e", nil);*/

   //printf("Root: %s\n", getRoot(A));
   //beginForward(A);
   //delete(A, "e");



  // next(A);
   //next(A);
    int* pInt = calloc(10, sizeof(int));
    for (int i=0; i < 10; i++) pInt[i]=i+10;
   insert(A, "m", &pInt[0]);
   insert(A, "n", &pInt[1]);
   insert(A, "o", &pInt[2]);
   insert(A, "p", &pInt[3]);
   insert(A, "q", &pInt[4]);
   insert(A, "f", &pInt[5]);
   insert(A, "e", &pInt[6]);
   beginReverse(A);
   printf("Current Key REV: %s\n", currentKey(A));
   prev(A);
   printf("Current Key: %s\n", currentKey(A));

   prev(A);
   printf("Current Key: %s\n", currentKey(A));

   if (strcmp(currentKey(A), "o") != 0 || *currentVal(A) != pInt[2]) {
      printf("fail 1\n");
      return 0;
   }
   prev(A);
   printf("Current Key: %s\n", currentKey(A));

   prev(A);
   printf("Current Key: %s\n", currentKey(A));

   if (strcmp(currentKey(A), "m") != 0 || *currentVal(A) != pInt[0]) {
      printf("fail 2\n");
      return 0;
   }
   prev(A);
   prev(A);
   prev(A);
   if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF){
        printf("fail 3\n");
        return 0;
   }
   return 0;





   printf("cur: %s\n", currentKey(A));
   beginReverse(A);
   prev(A);
   printf("cur: %s\n", currentKey(A));

   //delete(A,"c");



   printDictionary(stdout, A, "in");
   

   return(0);
}