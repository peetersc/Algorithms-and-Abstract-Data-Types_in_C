//------------------------------------------------------------------------------------------------------------
// List.h
// Header file for List ADT
//------------------------------------------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>

#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_

// Exported type            ----------------------------------------------------------------------------------
typedef struct ListObj* List;

// Constructors-Destructors ----------------------------------------------------------------------------------
List newList(void);           // Returns reference to new empty List object. 
void freeList(List*);         // Frees all heap memory associated with List *pL, and sets *pL to NULL.

// Access functions          ---------------------------------------------------------------------------------
int isEmpty (List);           // Returns true (1) if L is empty, otherwise returns false (0)
int length  (List);           // Returns the length of Q.
int index   (List);           // Returns the index of the cursor
int front   (List);           // Returns the index of the cursor Pre: !isEmpty(Q)
int back    (List);           // Returns the back element of the list
int get     (List);           // Returns the current position of the cursor
int equals  (List, List);     // Returns true (1) if A is identical to B, false (0) otherwise

// Manipulation procedures   --------------------------------------------------------------------------------
void clear       (List);      // Resets this List to its original empty state.
void moveFront   (List);      // If List is non-empty, places the cursor under the FRONT element.
void moveBack    (List);      // If List is non-empty, places the cursor under the BACK  element.
void movePrev    (List);      // If cursor is defined and not at front, moves cursor one step forward.
void moveNext    (List);      // MoveCursor one place over in the list
void prepend     (List, int); // Places new data element at the end of L
void append      (List, int); // Places new data element at the end of L
void insertBefore(List, int); // Insert data before the cursor of list L
void insertAfter (List, int); // Insert data after  the cursor of list L
void deleteFront (List);      // Deletes element at front of L Pre: !isEmpty(L)
void deleteBack  (List);      // Deletes element at front of L Pre: !isEmpty(L)
void delete      (List);      // Deletes cursor element, making cursor undefined.

// Other Functions           ---------------------------------------------------------------------------------
void printList(FILE*, List);  // Prints data elements in Q on a single line to stdout.
List copyList (List);         // Creates a new list that is a copy of list L

#endif
