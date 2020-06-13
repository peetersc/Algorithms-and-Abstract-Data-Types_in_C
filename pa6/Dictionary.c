/*********************************************************************************
* Cameron Peeters, capeeter
* PA6
* Dictionary.c
* Implementation file for Dictionary ADT
*********************************************************************************/

#include"Dictionary.h"

// Private Types and Functions ------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   VAL_TYPE value;
   KEY_TYPE key;
   struct NodeObj* left;
   struct NodeObj* right;
   struct NodeObj* parent;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
// Private.
Node newNode(char* key, VAL_TYPE value){
   Node N = malloc(sizeof(NodeObj));

   //quantifiable fields 
   N->key    = key;
   N->value  = value;

   //positional fields
   N->left   = NULL;
   N->right  = NULL;
   N->parent = NULL;
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

// private DictionaryObj type
typedef struct DictionaryObj{
   Node root;
   Node current;
   int size;
   int isUnique;
} DictionaryObj;

Dictionary newDictionary(int unique){
   Dictionary D = malloc(sizeof(DictionaryObj));
   D->root      = NULL;
   D->current   = NULL;
   D->size      = 0;
   D->isUnique  = (unique > 0) ? 1:0;
   return(D);
}

void freeDictionary(Dictionary* pD){
   if( pD!=NULL && *pD!=NULL ){
      makeEmpty(*pD);
      free(*pD);
      *pD = NULL;
   }
}

//Helper function prototypes for Binary Search Tree Data Structure ----------------------------------------------------
static Node findKey(Node, KEY_TYPE);
static Node treeMin(Node);
static Node treeMax(Node);
static void Inorder_Walk(FILE*, Node);
//static void Preorder_Walk(FILE*, Node);
//static void Postorder_Walk(FILE*, Node);
static void transplant(Dictionary, Node, Node);
static void deleteAll(Node);

//Inline getter functions For Dictionary ADT ----------------------------------------------------
inline int size(Dictionary D)            {return(D->size);}
inline int getUnique(Dictionary D)       {return(D->isUnique);}
static inline int isEmpty(Dictionary D)  {return (D->size == 0);}

//Core Dictionary ADT Functions ----------------------------------------------------
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){
   if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Node N = findKey(D->root, k);
   return (N==NULL ? VAL_UNDEF : N->value);
}

void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){
   if (getUnique(D)){
      if (lookup(D, k) != VAL_UNDEF){
         fprintf(stderr,
            "Dictionary Error: calling insert() on dublicate Dictionary reference\n");
         exit(EXIT_FAILURE);
      }
   }

   if( D==NULL ){
      fprintf(stderr,
         "Dictionary Error: calling insert() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   Node y = NULL;
   Node x = D->root;
   Node N = newNode(k,v);
   D->size++;

   while(x != NULL){
      y = x;
      if (KEY_CMP(k,x->key) < 0)
         x = x->left;
      else
         x = x->right;
   }

   if (y == NULL)
      D->root = N;
   else if(KEY_CMP(k,y->key) < 0){
      y->left = N;
      y->left->parent = y;
   }
   else{
      y->right = N;
      y->right->parent = y;
   }
   freeNode(&x);
}

void delete(Dictionary D, KEY_TYPE k){
  if (lookup(D,k)==VAL_UNDEF){
      fprintf(stderr,
         "Dictionary Error: calling delete() on undefined Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Node Z = findKey(D->root, k);
   
   if (D->current == Z)
      D->current = NULL;

   if (Z->left == NULL)
      transplant(D,Z,Z->right);
   else if (Z->right == NULL)
      transplant(D,Z,Z->left);
   else{
      Node y = treeMin(Z->right);
      if (y->parent != Z){
         transplant(D,y,y->right);
         y->right = Z->right;
         y->right->parent = y;
      }
      transplant(D,Z,y);
      y->left = Z->left;
      y->left->parent = y;
   }
   (D->size)--;
   freeNode(&Z);
}

void makeEmpty(Dictionary D){
   deleteAll(D->root);
   D->root      = NULL;
   D->current   = NULL;
   D->size      = 0;
}

VAL_TYPE beginForward(Dictionary D){
   if (D == NULL || isEmpty(D))
      return VAL_UNDEF;

   D->current = treeMin(D->root);
   return  (D->current == NULL) ? VAL_UNDEF:D->current->value;
}

VAL_TYPE beginReverse(Dictionary D){
   if (D == NULL || isEmpty(D)){
      return VAL_UNDEF;
   }
   D->current = treeMax(D->root);
   return  (D->current == NULL) ? VAL_UNDEF:D->current->value;
}

KEY_TYPE currentKey(Dictionary D){
   if (D==NULL || isEmpty(D)){
      return KEY_UNDEF;
   }
   return (D->current == NULL) ? KEY_UNDEF:D->current->key;
}

VAL_TYPE currentVal(Dictionary D){
   if (D==NULL || isEmpty(D)){
      return VAL_UNDEF;
   }
   return (D->current == NULL) ? VAL_UNDEF:D->current->value;
}

VAL_TYPE next(Dictionary D){
   if (D==NULL || isEmpty(D)){
      return VAL_UNDEF;
   }
   else if (D->current == treeMax(D->root)){
      D->current = NULL;
      return VAL_UNDEF;
   }
   else if (D->current->left != NULL && D->current->right == NULL){
      D->current = D->current->left;
      return D->current->value;
   }
   else if (D->current->left == NULL && D->current->right != NULL){
      D->current = D->current->right;
      return D->current->value;
   }
   else if (KEY_CMP(D->current->left->key, D->current->right->key) < 0){
      D->current = D->current->left;
   }
   else{
      D->current = D->current->right;
   }
   return (D->current == NULL) ? VAL_UNDEF:D->current->value;
}

VAL_TYPE prev(Dictionary D){
   if (D==NULL || isEmpty(D)){
      return VAL_UNDEF;
   }
   else if (D->current == treeMin(D->root)){
      D->current = NULL;
      return VAL_UNDEF;
   }
   else{
      D->current = treeMin(D->current->parent);
   }
   return (D->current == NULL) ? VAL_UNDEF:D->current->value;
}

void printDictionary(FILE* out, Dictionary D){
   Node N = D->root;
   Inorder_Walk(out,N);
   freeNode(&N);
}

//Helper functions for Binary Search Tree ----------------------------------------------------

// findKey()
// Returns the Node containing key k in the subtree rooted at R, or returns 
// NULL if no such Node exists.
static Node findKey(Node R, KEY_TYPE k){
   if(R==NULL || strcmp(k, R->key)==0) 
      return R;
   if( strcmp(k, R->key)<0 ) 
      return findKey(R->left, k);
   else 
      return findKey(R->right, k);
}

static Node treeMin(Node N){
   while (N->left != NULL)
      N = N->left;
   return N;
}

static Node treeMax(Node N){
   while (N->right != NULL)
      N = N->right;
   return N;
}

static void Inorder_Walk(FILE* out, Node N){
   if (N != NULL){
      Inorder_Walk(out, N->left);
      fprintf(out, KEY_FORMAT, N->key);
      Inorder_Walk(out, N->right);
   }
}

/*static void Preorder_Walk(FILE* out, Node N){
   if (N != NULL){
      fprintf(out, "%s %d\n", N->key,N->value);
      Preorder_Walk(out, N->left);
      //if (N->parent != NULL)
         //fprintf(out, "Parent: %s", N->parent->key);
      Preorder_Walk(out, N->right);
   }
}

static void Postorder_Walk(FILE* out, Node N){
   if (N != NULL){
      Postorder_Walk(out, N->left);
      //if (N->parent != NULL)
         //fprintf(out, "Parent: %s", N->parent->key);
      Postorder_Walk(out, N->right);
      fprintf(out, "%s %d\n", N->key,N->value);
   }
}*/

static void transplant(Dictionary T,Node u, Node v){
   if (u->parent == NULL)
      T->root = v;
   else if (u == u->parent->left)
      u->parent->left = v;
   else
      u->parent->right= v;

   if (v != NULL)
      v->parent = u->parent;
}

// deleteAll()
// deletes all Nodes in the subtree rooted at R.
static void deleteAll(Node R){
   if( R!=NULL ){
      deleteAll(R->left);
      deleteAll(R->right);
      freeNode(&R);
   }
}