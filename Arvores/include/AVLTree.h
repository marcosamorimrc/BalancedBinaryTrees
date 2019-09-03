/*
   AVLTree.h

   AVL trees.

  Program:   weiss
  Module:    $RCSfile: AVLTree.h,v $
  Language:  C++
  Date:      $Date: 2018/10/08 22:46:05 $
  Version:   $Revision: 1.4 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __AVLTree
#define __AVLTree

#include "Bst.h"

#define Level BalancingInfo

/** AVLTree class interface.
 *
 *  A binary search tree with the additional balance property
 *  that, for any node in the tree, the height of the left and
 *  right subtrees can differ by at most 1. As the first balanced
 *  search tree, it has historical significance, and it also 
 *  illustrates most of the ideas that are used in other schemes. 
 * 
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.4 $ $Date: 2018/10/08 22:46:05 $
 *	
 *  Etype: must have zero-parameter and copy constructor, <br>
 *     and must have operator<, operator!=, and operator= <br>
 *  CONSTRUCTION: with (a) no initializer <br>
 *  Deep copy is supported.
 * 
 *  ******************PUBLIC OPERATIONS********************* <br>  
 *  int Insert( const Etype& X )  --> Insert X <br> 
 *  int Remove( const Etype& X )  --> Remove X <br> 
 *  Etype Find( const Etype& X )  --> Return item that matches X <br> 
 *  int Height( const BinaryNode* T ) --> Calculates the height of a node <br>
 *  ------------- Inherited ----------------------------- <br>
 *  int WasFound( )               --> Return 1 if last Find succeeded <br> 
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br> 
 *  Etype FindMin( )              --> Return smallest item <br> 
 *  Etype FindMax( )              --> Return largest item <br> 
 *  int IsEmpty( )                --> Return 1 if empty; else return 0  <br>
 *  BinayNode<Etype>* GetRoot()   --> Return root node <br>
 *  void MakeEmpty( )             --> Remove all items <br>
 *  int Size ( const BinaryNode* T )  --> Calculates the size of a node <br>
 *  ******************ERRORS********************************  <br>
 *  Predefined exception is propagated if new fails. <br>
 *  Item in sentinel node returned on various degenerate conditions.
 */

template <class Etype, class Cmp=compare_to<Etype> >
class AVLTree: public SearchTree<Etype,Cmp>
{
  /// a binary node definition.
  typedef typename SearchTree<Etype,Cmp>::BinaryNode BinaryNode;

  public:

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    AVLTree( const Cmp& comp = Cmp() ) : SearchTree<Etype,Cmp> (comp) {
    	this->NullNode = new BinaryNode( );
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->NullNode->Level = -1;
    	this->Root = this->NullNode;
    }

    /// copy constructor.
    AVLTree( const AVLTree& Rsh ) {
    	this->NullNode = new BinaryNode( );
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->NullNode->Level = -1;
    	this->Root = this->NullNode;
        *this = Rsh;
    }

    /// destructor. Deletes the tree and NullNode.
    virtual ~AVLTree( ) { 
	this->FreeTree( this->Root ); this->Root = NULL;  
	delete this->NullNode; this->NullNode = NULL;
    }

  //@}

    /** adds X into the tree. If X already present, do nothing.
     *
     *  @param X item to be inserted.
     *	@return true if insertion was successful.
     */
    virtual int Insert( const Etype& X ) { 
 	BinaryNode* node = InsertAVL( X, this->Root );
        if ( node != NULL ) {
             this->Root = node;
             return 1;
        }
        return 0;
    }

    /** removes X from tree. 
     *
     *  @param X item to be removed.
     *	@return 1 if X was found, and 0 otherwise.
     */
    virtual int Remove( const Etype& X )
        { return Remove( X, this->Root ); }

    /** returns item X in tree. 
     *	Result can be checked by calling WasFound.
     *
     *  @param X item to be searched for.
     *  @return ItemNotFound, if X is not found.
     */
    virtual const Etype& Find( const Etype& X ) const {
    	this->NullNode->setElement ( X );	// avoids a test in the while below

        BinaryNode** tempCurrent = (BinaryNode**)&this->Current;
    	*tempCurrent = this->Root;

    	while ( 1 ) {
            if ( this->compare(X, this->Current->Element) )
                *tempCurrent = this->Current->Left;
            else if (this->compare (this->Current->Element, X) )
                *tempCurrent = this->Current->Right;
            else
                break;
        }

        return ( this->Current != this->NullNode ) ?
             this->Current->Element:
             this->ItemNotFound;
    }

    /** returns item X in tree. 
     *  Version specialized for pairs, since it uses operator neq.
     *	Result can be checked by calling WasFound.
     *
     *  @param X item to be searched for.
     *  @return ItemNotFound, if X is not found.
     */
    virtual const Etype& FindP( const Etype& X ) const {
    	this->NullNode->setElement ( X );	// avoids a test in the while below

        BinaryNode** tempCurrent = (BinaryNode**)&this->Current;
    	*tempCurrent = this->Root;

    	while ( this->compare.neq (X, this->Current->Element) ) {
            *tempCurrent = ( this->compare(X, this->Current->Element) ?
                this->Current->Left : this->Current->Right );
        }

        return ( this->Current != this->NullNode ) ?
             this->Current->Element:
             this->ItemNotFound;
    }

    /** computes the height of a node.
     *  The height is the length of the longest path from 
     *  a node to a leaf. All leaves have height 0.
     */
    virtual int Height ( const BinaryNode* T ) const {
    	if ( T == this->NullNode )
             return -1;
        else
    	     return T->Level;
    }

  private:

    // usual rotation routines 

    /** rotate binary tree node with left child.
     *  For AVL trees, this is a single rotation for case 1.
     */
    BinaryNode* RotateWithLeftChild( BinaryNode* K2 ) {
    	BinaryNode* K1 = K2->Left;
    	K2->Left  = K1->Right;
    	K1->Right = K2;

        K2->Level = max ( Height(K2->Left), Height (K2->Right) ) + 1;        
        K1->Level = max ( Height(K1->Left), Height (K1->Right) ) + 1;        

    	return K1;
    }

    /** rotate binary tree node with right child.
     *  For AVL trees, this is a single rotation for case 4.
     */
    BinaryNode* RotateWithRightChild( BinaryNode* K1 ) {
    	BinaryNode* K2 = K1->Right;
    	K1->Right = K2->Left;
    	K2->Left  = K1;

        K1->Level = max ( Height(K1->Left), Height (K1->Right) ) + 1;        
        K2->Level = max ( Height(K2->Left), Height (K2->Right) ) + 1;        

    	return K2;
    }

    /** double rotate binary tree node.
     *  First left child with its right child; then node "K3" with
     *  new left child. For AVL trees, this is a double rotation for case 2.
     */
    BinaryNode* DoubleRotateWithLeftChild( BinaryNode* K3 ) {
    	K3->Left = RotateWithRightChild ( K3->Left );
    	return RotateWithLeftChild ( K3 );
    }

    /** double rotate binary tree node.
     *  First right child with its left child; then node "K1" with
     *  new right child. For AVL trees, this is a double rotation for case 3.
     */
    BinaryNode* DoubleRotateWithRightChild( BinaryNode* K1 ) {
    	K1->Right = RotateWithLeftChild ( K1->Right );
    	return RotateWithRightChild ( K1 );
    }

    /** inserts a new node "X" into the AVL tree rooted at "T".
     *  At most one rotation (single or double) rebalances the tree. 
     *  The overall operation is O(log n).
     *
     * 	@param X new node.
     *	@param T root node.
     *	@return root node or NULL, if X is a duplicate.
     */
    BinaryNode* InsertAVL( const Etype& X, BinaryNode* T ) {
    	if ( T == this->NullNode )				// empty tree: create new node
    	  {
           T = new BinaryNode( X, this->NullNode, this->NullNode );
           T->Level = 0;
    	  }
    	else if ( this->compare (X, T->Element) ) 
          {
           BinaryNode* ptr = InsertAVL( X, T->Left );	// insert recursively on left
           if ( ptr == NULL ) return NULL;  		// already present (duplicate)
           T->Left = ptr; 
           if ( (Height(T->Left) - Height(T->Right)) == 2 ) // check height condition
              {
               if ( this->compare (X, T->Left->Element) )	// rotate up the left side
                  T = RotateWithLeftChild ( T );	// left-left insertion
               else
                  T = DoubleRotateWithLeftChild ( T );  // left-right insertion
              }
           else						// balance OK: update height
               T->Level = 1 + max (Height(T->Left), Height(T->Right));
          }
    	else if ( this->compare (T->Element, X) ) 
          {
           BinaryNode* ptr = InsertAVL( X, T->Right );	// insert recursively on right
           if ( ptr == NULL ) return NULL;  		// already present (duplicate)
           T->Right = ptr;
           if ( (Height(T->Right) - Height(T->Left)) == 2 ) // check height condition
              {
               if ( this->compare ( T->Right->Element, X ) )	// rotate up the right side
                  T = RotateWithRightChild ( T );	// right-right insertion
               else
                  T = DoubleRotateWithRightChild ( T ); // right-left insertion
              }
           else						// balance OK: update height
               T->Level = 1 + max (Height(T->Left), Height(T->Right));
          }
    	else
           return NULL;					// already present (duplicate)

    	return T;
    }


    /** internal RemoveMinAVL routine for AVLTree.
     *	Remove minimum item in tree rooted at T.
     *
     *  @param T root node.
     *  @param LastPtr parent of minimum node.
     *	@return false only if tree is empty
     */
    int RemoveMinAVL( BinaryNode*& T, BinaryNode*& LastPtr ) {
    	if( T == this->NullNode )
            return 0;
    	else if( T->Left != this->NullNode ) {
            LastPtr = T;
            return RemoveMinAVL( T->Left, LastPtr );
        }
    	BinaryNode* Tmp = T;
    	T = T->Right;
    	delete Tmp;
    	return 1;
    }

    /** rebalances the tree after a remove operation.
     *
     *  @param X contents of the parent of the deleted node.
     *	@param T root.
     */
    void Rebalance( const Etype& X, BinaryNode*& T ) {
    	if( T == this->NullNode )
            return;		     
    	else if( this->compare (X, T->Element) ) {  // Go left
           Rebalance( X, T->Left );
           if ( (Height(T->Right) - Height(T->Left)) == 2 ) 
              {
               if ( Height (T->Right->Right) < Height (T->Right->Left) )
                  T = DoubleRotateWithRightChild ( T );
               else
                  T = RotateWithRightChild ( T );
              }
           else
               T->Level = 1 + max (Height(T->Left), Height(T->Right));
          }
    	else if( this->compare (T->Element, X) ) {  // Go right
           Rebalance( X, T->Right );
           if ( (Height(T->Left) - Height(T->Right)) == 2 ) 
              {
               if ( Height (T->Left->Left) < Height (T->Left->Right) )
                  T = DoubleRotateWithLeftChild ( T );
               else
                  T = RotateWithLeftChild ( T );
              }
           else
               T->Level = 1 + max (Height(T->Left), Height(T->Right));
          }
        else {
           int hr = Height(T->Right);
           int hl = Height(T->Left); 
           if ( hr < hl ) {
              if ( (hl - hr) == 2 ) 
                 {
                  if ( Height (T->Left->Left) < Height (T->Left->Right) )
                     T = DoubleRotateWithLeftChild ( T );
                  else
                     T = RotateWithLeftChild ( T );
                 }
              else
                  T->Level = 1 + max (hl, hr);
           }
           else if ( hr > hl ) { 
              if ( (hr - hl) == 2 ) 
                 {
                  if ( Height (T->Right->Right) < Height (T->Right->Left) )
                     T = DoubleRotateWithRightChild ( T );
                  else
                     T = RotateWithRightChild ( T );
                 }
              else
                  T->Level = 1 + max (hl, hr);
           }
           else T->Level = 1 + hr;
        }
    }

    /** removes node "X" from the AVL tree rooted at "T".
     *  A total of O(log n) rotations may be performed. 
     *  The overall operation is O(log n).
     *
     *	@param X item to be removed.
     *	@param T root node.
     *	@return true unless X was not found.
     */
    virtual int Remove( const Etype& X, BinaryNode*& T ) {
    	BinaryNode* Tmp;
    	static BinaryNode *LastPtr = this->Root;

    	if( T == this->NullNode )					// not found
            return 0;
    	else if( this->compare (X, T->Element) ) {  // Go left
           LastPtr = T;
           return Remove( X, T->Left );
        }
    	else if( this->compare (T->Element, X) ) {  // Go right
           LastPtr = T;
           return Remove( X, T->Right );
        }
    	else if( T->Left != this->NullNode && T->Right != this->NullNode )  // Two children
           {
            // Copy from node to delete into node to keep; then delete
            Tmp = (BinaryNode*) this->FindMin( T->Right );
            T->setElement (Tmp->Element);
            LastPtr = T;
            int resp = RemoveMinAVL( T->Right, LastPtr ); 		// Remove minimum
            Rebalance ( LastPtr->Element, this->Root ); 
            return resp;
           }
    	// One or zero children
    	Tmp = T;
    	T = ( T->Left != this->NullNode ) ? T->Left : T->Right;  	// Reroot T
    	delete Tmp;                                    		// Delete old root
        Rebalance ( LastPtr->Element, this->Root ); 
    	return 1;
    }
};
#endif
