//-----------------------------------------------------------------------------
// List.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   int    data;
   struct NodeObj* next;
   struct NodeObj* prev;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj{
   Node front;
   Node back;
   Node cursor;
   int length;
} ListObj;


// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
// Private.
Node newNode(int data){
   Node N = malloc(sizeof(NodeObj));

   N->data = data;
   N->next = NULL;
   N->prev = NULL;

   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
// Private.
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

// newList()
// Returns reference to new empty List object.
List newList(void){
   List L;
   L = malloc(sizeof(ListObj));
   L->front = L->back = NULL;
   L->length = 0;
   return(L);
}


// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.S
void freeList(List* pN){
   if( pN!=NULL && *pN!=NULL ){
      while(length(*pN) != 0) { 
         deleteFront(*pN); 
      }
      free(*pN);
      *pN = NULL;
   }
}


// Access functions -----------------------------------------------------------

// isEmpty()
// Returns true (1) if Q is empty, otherwise returns false (0)
int isEmpty(List L){
   return(L->length==0);
}

// length()
// Returns the length of L.
int length(List L){
   return(L->length);
}

// index(List L)
// Returns the index of the cursor 
int index(List L)
{
   if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }

   Node N = L->front;

   for (int i = 0; i < length(L); i++)
   {
      if (N == L->cursor)
      {
         return i;
      }
      N = N->next;
   }

   return -1;

}

// front()
// Returns the value at the front of L.
// Pre: !isEmpty(L)
int front(List L){
   if( isEmpty(L) ){
      printf("List Error: calling getFront() on an empty List\n");
      exit(1);
   }
   return(L->front->data);
}

// back()
// Returns the value at the back of L.
// Pre: !isEmpty(L)
int back(List L){
   if( isEmpty(L) ){
      printf("List Error: calling getFront() on an empty List\n");
      exit(1);
   }

   return(L->back->data);
}

// get()
// Returns cursor element. Pre: length() >0, inde() >= 0
int get(List L)
{
   if(length(L) <= 0){
      printf("List Error: calling getFront() on an empty List\n");
      exit(1);
   }
   if(index(L)<0){
      printf("List Error: calling getFront() on an empty List\n");
      exit(1);
   }
   return(L->cursor->data);
}

// equals()
// returns true (1) if A is identical to B, false (0) otherwise
int equals(List A, List B){
   if( A == NULL || B == NULL ){
      printf("List Error: calling equals() on NULL List reference\n");
      exit(1);
   }
   if(length(A) != length(B)) return 0;

   int eq = 1;
   Node N = A->front;
   Node M = B->front;

   while(N!=NULL && eq){
      eq = (N->data==M->data);

      N = N->next;
      M = M->next;
   }

   return eq;
}

// Manipulation procedures ----------------------------------------------------

//clear(List)
//Resets this List to its original empty state.
void clear(List L)
{
   L->front  = NULL;
   L->back   = NULL;
   L->cursor = NULL;
   L->length = 0;

}

//moveFront()
//If List is non-empty, places the cursor under the FRONT element,
//otherwise does nothing.
void moveFront(List L)
{
      if( L==NULL ){
      printf("List Error: calling () on NULL List reference\n");
      exit(1);
   }
   if(!isEmpty(L))
   {
      L->cursor = L->front;
   }
}

//moveBack()
//If List is non-empty, places the cursor under the BACK element,
//otherwise does nothing
void moveBack(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(!isEmpty(L))
   {
      L->cursor = L->back;
   }   
}

//movePrev()
//If cursor is defined and not at front, moves cursor one step toward
//front of this List, if cursor is defined and at front, cursor becomes
//undefined, if cursor is undefined does nothing.
void movePrev(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   //cursor falls of the list
   if (L->cursor == L->front) 
   {
      L->cursor = NULL;
   }
   else if (L->cursor != NULL)
   {
      L->cursor = L->cursor->prev;
   }
}

//moveNext
//moveCursor one place over in the list
void moveNext(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   //cursor falls of the list
   if (L->cursor == L->back) 
   {
      L->cursor = NULL;
   }
   else if (L->cursor != NULL)
   {
      L->cursor = L->cursor->next;
   }
}

// prepend()
// Places new data element at the end of L
void prepend(List L, int data)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   Node N = newNode(data);

   if( isEmpty(L) ) 
   { 
      L->front  = L->back = N; 
      L->cursor = L->front;
   }else
   { 
        L->front->prev = N;
        N->next = L->front;
        L->front = N;
   }
   L->length++;
}

// append()
// Places new data element at the end of L
void append(List L, int data)
{
   if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   
   Node N = newNode(data);

   N->prev = L->back;
   N->next = NULL;

   if (L->back != NULL) {
      L->back->next = N;
   }
   else
   {
      L->front = N;
   }

   L->back = N;
   L->length++;

}

//insertBefore(List, int)
//inserts data before the cursor of list L
void insertBefore(List L, int data)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(length(L) <= 0 || index(L) < 0)
    {
      printf("List Error: calling insertBefore() on NULL List reference\n");
      exit(1);
   }

   Node N = newNode(data);

   N->prev = L->cursor->prev;
   N->next = L->cursor;

   if(L->cursor->prev != NULL)
   {
         L->cursor->prev->next = N;
   } 
   else
   {
      L->front = N;
   }

   L->cursor->prev = N;
   L->length++;

}

//insertAfter(List, int)
//inserts data after the cursor of list L
void insertAfter(List L, int data)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(length(L) <= 0 || index(L) < 0)
    {
      printf("List Error: calling insertBefore() on NULL List reference\n");
      exit(1);
   }

   Node N = newNode(data);

   N->next = L->cursor->next;
   N->prev = L->cursor;

   if(L->cursor->next != NULL)
   {
         L->cursor->next->prev = N;
   } 
   else
   {
      L->back = N;
   }

   L->cursor->next = N;
   L->length++;
}

// deleteFront()
// Deletes element at front of L
// Pre: !isEmpty(L)
void deleteFront(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(isEmpty(L)){
      printf("List Error: calling DeList on an empty List\n");
      exit(1);
   }

   Node N = NULL;
   N = L->front;

   if( length(L)>1 ) 
   { 
      L->front = L->front->next; 
   }
   else
   { 
      L->front = L->back = NULL; 
   }

   L->length--;
   freeNode(&N);
}

// deleteBack()
// Deletes element at front of L
// Pre: !isEmpty(L)
void deleteBack(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(isEmpty(L)){
      printf("List Error: calling DeList on an empty List\n");
      exit(1);
   }

   Node N = NULL;
   N = L->back;

   if( length(L)>1 ) 
   { 
      L->back = L->back->prev; 
   }
   else
   { 
      L->front = L->back = NULL; 
   }
   L->length--;
   freeNode(&N);
}

void delete(List L)
{
      if( L==NULL ){
      printf("List Error: calling getLength() on NULL List reference\n");
      exit(1);
   }
   if(length(L) <= 0 || index(L) < 0){
      printf("List Error: calling delete on an empty List\n");
      exit(1);
   }

   Node N = NULL;
   N = L->cursor;

   if (N == L->front)
   {
      deleteFront(L);
   }
   else if(N == L->back)
   {
      deleteBack(L);
   }
   else
   {
      L->cursor->prev->next = L->cursor->next;
      L->cursor->next->prev = L->cursor->prev;
      L->cursor = NULL;
      L->length--;
      freeNode(&N);
   }
}

// Other Functions ------------------------------------------------------------

// printList()
// Prints data elements in L on a single line to stdout.
void printList(FILE *out, List L){
  moveFront(L);
  while(L->cursor != NULL){
    fprintf(out, "%d ",get(L));
    moveNext(L);
  }
}

//copy()
//Returns a new List representing the same integer sequence as this
//List. The cursor in the new list is undefined, regardless of the
//state of the cursor in this List. This List is unchanged.
List copyList(List L)
{
   List Q = newList(); 
   Node N = L->front;

   while (N != NULL)
   {
      append(Q, N->data); 
      N = N->next;
   }

   return Q;
}
