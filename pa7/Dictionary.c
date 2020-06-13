/*********************************************************************************
* Cameron Peeters, capeeter
* PA7
* Dictionary.c
* Implementation file for Dictionary ADT
*********************************************************************************/

#include"Dictionary.h"

// Red Black Tree Properties --------------------------------------------------

#define RED 1
#define BLACK 0
#define COLOR_TYPE int

// Private Types and Functions ------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   VAL_TYPE   value;
   KEY_TYPE   key;
   COLOR_TYPE color;
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
   N->color  = BLACK;

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
   Node nil;
   int size;
   int isUnique;
} DictionaryObj;

Dictionary newDictionary(int unique){
   Dictionary D = malloc(sizeof(DictionaryObj));
   D->nil       = newNode("nil", 0);
   D->root      = D->nil;
   D->current   = D->nil;
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

// Helper Prototypes -----------------------------------------------------------

static Node findKey(Dictionary, Node, KEY_TYPE);
static Node treeMin(Dictionary, Node);
static Node treeMax(Dictionary, Node);
static Node treeSuccessor(Dictionary, Node);
static Node treePredecessor(Dictionary, Node);
static void transplant(Dictionary, Node, Node);
static void deleteAll(Node);
static void Inorder_Walk(FILE*, Node);
static void Preorder_Walk(FILE*, Node);
static void Postorder_Walk(FILE*, Node);

// Access functions -----------------------------------------------------------

//isEmpty()
// Returns true if the size is zero and false otherwise
static inline int isEmpty(Dictionary D)  {
   return (D->size == 0);
}

// size()
// Returns the number of (key, value) pairs in Dictionary D.
inline int size(Dictionary D){
   return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
inline int getUnique(Dictionary D){
   return D->isUnique;
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){
   if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Node N = findKey(D, D->root, k);

   return (N == D->nil || N == NULL) ? VAL_UNDEF:N->value;
}


// Manipulation procedures ----------------------------------------------------
static void leftRotate(Dictionary T, Node x){
   Node y = x->right;
   x->right = y->left;

   if (y->left != T->nil)
      y->left->parent = x;
   
   y->parent = x->parent;
   if (x->parent == T->nil){
      T->root = y;
   }
   else if (x == x->parent->left){
      x->parent->left = y;
   }
   else{
      x->parent->right = y;
   }

   y->left = x;
   x->parent = y;
}

static void rightRotate(Dictionary T, Node x){
   Node y = x->left;
   x->left = y->right;

   if (y->right != T->nil)
      y->right->parent = x;
   
   y->parent = x->parent;
   if (x->parent == T->nil){
      T->root = y;
   }
   else if (x == x->parent->right){
      x->parent->right = y;
   }
   else{
      x->parent->left = y;
   }

   y->right = x;
   x->parent = y;
}

static void insertFixUp(Dictionary T, Node z)
{
   while (z != T->root && z->parent->color == RED){
      Node y;
      if (z->parent == z->parent->parent->left){
         y = z->parent->parent->right;

         if (y->color == RED){
            z->parent->color = BLACK;              // case 1
            y->color = BLACK;                      // case 1
            z->parent->parent->color = RED;        // case 1
            z = z->parent->parent;                 // case 1
         }
         else{ 
            if (z == z->parent->right){
               z = z->parent;                      // case 2
               leftRotate(T, z);                   // case 2
            }
            z->parent->color = BLACK;              // case 3
            z->parent->parent->color = RED;        // case 3
            rightRotate(T, z->parent->parent);     // case 3
         }
      }
      else{ 
         y = z->parent->parent->left;

         if (y->color == RED){
            z->parent->color = BLACK;              // case 4
            y->color = BLACK;                      // case 4
            z->parent->parent->color = RED;        // case 4
            z = z->parent->parent;                 // case 4
         }
         else{
            if (z == z->parent->left){
               z = z->parent;                      // case 5
               rightRotate(T, z);                  // case 5
            }
            z->parent->color = BLACK;              // case 6
            z->parent->parent->color = RED;        // case 6
            leftRotate(T, z->parent->parent);      // case 6
         }
      }
   }
   T->root->color = BLACK;
}

static void deleteFixUp(Dictionary T, Node x)
{
   while (x != T->root && x->color == BLACK){
      Node w;
      if (x == x->parent->left){
         w = x->parent->right;
         if (w->color == RED){
            w->color = BLACK;                        // case 1
            x->parent->color = RED;                  // case 1
            leftRotate(T, x->parent);                // case 1
            w = x->parent->right;                    // case 1
         }
         if (w->left->color == BLACK && w->right->color == BLACK){
            w->color = RED;                          // case 2
            x = x->parent;                           // case 2
         }
         else{
            if (w->right->color == BLACK){
               w->left->color = BLACK;               // case 3
               w->color = RED;                       // case 3
               rightRotate(T, w);                    // case 3
               w = x->parent->right;                 // case 3
            }
            w->color = x->parent->color;             // case 4
            x->parent->color = BLACK;                // case 4
            w->right->color = BLACK;                 // case 4
            leftRotate(T, x->parent);                // case 4
            x = T->root;                             // case 4
         }
      }
      else{
         w = x->parent->left;
         if (w->color == RED){
            w->color = BLACK;                        // case 5
            x->parent->color = RED;                  // case 5
            rightRotate(T, x->parent);               // case 5
            w = x->parent->left;                     // case 5
         }
         if (w->right->color == BLACK && w->left->color == BLACK){
            w->color = RED;                          // case 6
            x = x->parent;                           // case 6
         }
         else{ 
            if (w->left->color == BLACK){
               w->right->color = BLACK;              // case 7
               w->color = RED;                       // case 7
               leftRotate(T, w);                     // case 7
               w = x->parent->left;                  // case 7
            }
            w->color = x->parent->color;             // case 8
            x->parent->color = BLACK;                // case 8
            w->left->color = BLACK;                  // case 8
            rightRotate(T, x->parent);               // case 8
            x = T->root;                             // case 8
         }
      }
   }
   x->color = BLACK;
}

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
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

   Node y = D->nil;
   Node x = D->root;
   Node N = newNode(k,v);
   D->size++;

   while(x != D->nil){
      y = x;
      if (KEY_CMP(k,x->key) < 0)
         x = x->left;
      else
         x = x->right;
   }

   N->parent = y;
   if (y == D->nil)
      D->root = N;
   else if(KEY_CMP(k,y->key) < 0){
      y->left = N;
      y->left->parent = y;
   }
   else{
      y->right = N;
      y->right->parent = y;
   }

   N->left = D->nil;
   N->right = D->nil;
   N->color = RED;

   // call insertFixUp to fix reb-black tree's property if it
   // is voilated due to insertion.
   insertFixUp(D,N);
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary T, KEY_TYPE k){

  if (findKey(T, T->root, k) == T->nil){
     
      fprintf(stderr,
         "Dictionary Error: calling delete() on undefined Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   Node z = findKey(T, T->root, k);
   Node y = z;
   Node x = T->nil;
   (T->size)--;

   COLOR_TYPE y_original_color = y->color;
   if (z->left == T->nil){
      x = z->right;
      transplant(T, z, z->right);
   }
   else if (z->right == T->nil){
      x = z->left;
      transplant(T, z, z->left);
   }
   else{ 
      y = treeMin(T, z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z)
         x->parent = y;
      else{
         transplant(T, y, y->right);
         y->right = z->right;
         y->right->parent = y;
      }
      transplant(T, z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
   }

   if (y_original_color == BLACK)
      deleteFixUp(T, x);

   if (T->current == z){
      T->current = T->nil;
   }
   freeNode(&z);
}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D){
   if (isEmpty(D)) return;

   deleteAll(D->root->parent);
   D->root     = NULL;
   D->current  = NULL;
   D->nil      = NULL;
   D->size     = 0;
   D->isUnique = 0;
}

// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D){
   if (D == NULL || isEmpty(D))
      return VAL_UNDEF;

   D->current = treeMin(D,D->root);
   return  (D->current == D->nil || D->current == NULL) ? VAL_UNDEF:D->current->value;
}

// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D){
   if (D == NULL || isEmpty(D))
      return VAL_UNDEF;

   D->current = treeMax(D,D->root);
   return  (D->current == D->nil  || D->current == NULL) ? VAL_UNDEF:D->current->value;
}

// currentKey()  
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D){
   if (D==NULL || isEmpty(D))
      return KEY_UNDEF;
   return (D->current == D->nil  || D->current == NULL) ? KEY_UNDEF:D->current->key;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D){
   if (D==NULL || isEmpty(D))
      return VAL_UNDEF;
   return (D->current == D->nil  || D->current == NULL) ? VAL_UNDEF:D->current->value;
}

// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D){
   if (D==NULL || isEmpty(D) || D->current == D->nil){
      return VAL_UNDEF;
   }
   if (D->current == treeMax(D,D->root)){
      D->current = D->nil;
      return VAL_UNDEF;
   }
   D->current = treeSuccessor(D, D->current);
   return (D->current == D->nil) ? VAL_UNDEF:D->current->value;
}


// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D){
   if (D==NULL || isEmpty(D) || D->current == D->nil){
      return VAL_UNDEF;
   }
   if (D->current == treeMin(D,D->root)){
      D->current = D->nil;
      return VAL_UNDEF;
   }
   D->current = treePredecessor(D, D->current);
   return (D->current == D->nil) ? VAL_UNDEF:D->current->value;
}


// Other operations -----------------------------------------------------------

// printDictionary()
// Prints the keys (only) of D in an order determined by the parameter ord.
// If ord is "pre", "in" or "post", executes the corresponding tree traversal
// on the underlying RBT, printing keys only (no values).  If ord is some
// other string, prints nothing.
void printDictionary(FILE* out, Dictionary D, const char* ord){
   Node N = D->root;
   if (KEY_CMP(ord,"pre")==0)
      Preorder_Walk(out, N);

   else if (KEY_CMP(ord,"in")==0)
      Inorder_Walk(out, N);
      
   else if (KEY_CMP(ord,"post")==0)
      Postorder_Walk(out, N);
}

//Helper functions for Red-Black Tree ----------------------------------------------------

// findKey()
// Returns the Node containing key k in the subtree rooted at R, or returns 
// NULL if no such Node exists.
static Node findKey(Dictionary D, Node R, KEY_TYPE k){
   if( R==D->nil || KEY_CMP(k, R->key)==0) 
      return R;
   if( KEY_CMP(k, R->key) < 0 ) 
      return findKey(D, R->left, k);
   else 
      return findKey(D, R->right, k);
}

static Node treeMin(Dictionary D, Node N){
   while (N->left != D->nil)
      N = N->left;
   return N;
}

static Node treeMax(Dictionary D, Node N){
   while (N->right != D->nil)
      N = N->right;
   return N;
}

static Node treeSuccessor(Dictionary D, Node N){
   if (N->right != D->nil)
      return treeMin(D, N->right);
   Node y = N->parent;
   while (y != D->nil && N == y->right){
      N = y;
      y = y->parent;
   }
   return y;
}

static Node treePredecessor(Dictionary D, Node N){
   if (N->left != D->nil)
      return treeMax(D, N->left);
   Node y = N->parent;
   while (y != D->nil && N == y->left){
      N = y;
      y = y->parent;
   }
   return y;
}

static void transplant(Dictionary T,Node u, Node v){
   if (u->parent == T->nil)
      T->root = v;
   else if (u == u->parent->left)
      u->parent->left = v;
   else
      u->parent->right= v;

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

static void Inorder_Walk(FILE* out, Node N){
   if (N != NULL){
      Inorder_Walk(out, N->left);
      if (KEY_CMP("nil", N->key) != 0)
         fprintf(out, KEY_FORMAT, N->key);
      Inorder_Walk(out, N->right);
   }
}

static void Preorder_Walk(FILE* out, Node N){
   if (N != NULL){
      if (KEY_CMP("nil", N->key) != 0)
         fprintf(out, KEY_FORMAT, N->key);
      Preorder_Walk(out, N->left);
      Preorder_Walk(out, N->right);
   }
}

static void Postorder_Walk(FILE* out, Node N){
   if (N != NULL){
      Postorder_Walk(out, N->left);
      Postorder_Walk(out, N->right);
      if (KEY_CMP("nil", N->key) != 0)
         fprintf(out, KEY_FORMAT, N->key);
   }
}
