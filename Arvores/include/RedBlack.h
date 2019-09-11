/*
   RedBlack.h

   Red-Black trees.

  Program:   weiss
  Module:    $RCSfile: RedBlack.h,v $
  Language:  C++
  Date:      $Date: 2012-10-13 19:44:42 $
  Version:   $Revision: 1.3 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __RedBlack
#define __RedBlack

#include "Bst.h"
#include "AVLTree.h"

#define Color BalancingInfo
#define Header Root

/** RedBlackTree class interface.
 *
 *  A good alternative to the AVL tree because a single top-down pass
 *  can be used during the insertion and deletion routines.
 *  The coding details tend to give a faster implementation.
 *
 *  A Red_Black tree is a binary tree with the following ordered properties: <br>
 *    1) Every node is colored either red or black. <br>
 *    2) The root is black. <br>
 *    3) If a node is red, its children must be black. <br>
 *    4) Every path from a node to an external node (NULL pointer)
 *       must contain the same number of black nodes. <br>
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.3 $ $Date: 2012-10-13 19:44:42 $
 *	
 *  Etype: must have zero-parameter and copy constructor,
 *     and must have operator<, operator!=, and operator= <br>
 *  CONSTRUCTION: with (a) Negative infinity sentinel. <br>
 *  Deep copy is supported.
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *  int Insert( const Etype& X )  --> Insert X <br>
 *  Etype Find( const Etype& X )  --> Return item that matches X <br>
 *  int IsEmpty( )         	  --> Return 1 if empty; else return 0 <br>
 *  void MakeEmpty( )     	  --> Remove all items <br>
 *  BinayNode<Etype>* GetRoot()   --> Return root node <br>
 *  ------------- Inherited ----------------------------- <br>
 *  int WasFound( )        	  --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br>
 *  Etype FindMin( )       	  --> Return smallest item <br>
 *  Etype FindMax( )       	  --> Return largest item <br>
 *  int Height( const BinaryNode* T ) --> Calculates the height of a node <br>
 *  int Size ( const BinaryNode* T )  --> Calculates the size of a node <br>
 *  ******************ERRORS******************************** <br>
 *  Predefined exception is propagated if new fails. <br>
 *  Item in NullNode returned on various degenerate conditions. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class RedBlackTree : public SearchTree<Etype, Cmp>
{
  /// a binary node definition.
  typedef typename SearchTree<Etype,Cmp>::BinaryNode BinaryNode;

  public:
  /** @name constructors  */ 
  //@{
    /// constructor from a negative sentinel.
    RedBlackTree( const Etype& NegInf, const Cmp& comp = Cmp() ) : 
                                                 SearchTree<Etype,Cmp>(comp) {
        initTree ( NegInf );
    }

    /// constructor from a comparison function object.
    RedBlackTree( const Cmp& comp = Cmp() ) : SearchTree<Etype,Cmp>(comp) {
        initTree ( Etype() );
    }

    /// constructor from an AVLTree.
    RedBlackTree ( const AVLTree<Etype,Cmp>& avl, const Etype& NegInf = Etype() ) {
        initTree ( NegInf );
        this->Header->Right = this->Duplicate(avl, avl.GetRoot());
        Graduate ( this->Header->Right );
        this->Header->Right->Color = Black;
        SetColor ( this->Header->Right );
    }

    /// copy constructor.
    RedBlackTree( const RedBlackTree& Rsh ) {
        initTree (  );
        *this = Rsh;
    }

    /// destructor.
    virtual ~RedBlackTree( ) {
	 this->FreeTree( this->Header ); this->Header = NULL;
	 delete this->NullNode; this->NullNode = NULL;
    }
  //@}

    /** adds X into the tree, and if X already present, do nothing.
     *  Top-down approach. The idea is to make the node 
     *  (previously a leaf) above the new node Black, 
     *  so the new node can be Red. If
     *  necessary apply exactly one rotation (possibly double). 
     *
     *  @return true if insertion was successful.
     */
    virtual int Insert( const Etype& X ) {
    	this->Current = Parent = Grand = this->Header;
    	this->NullNode->setElement ( X );
    	while ( 1 ) 
    	  {
           if ( this->compare (X, this->Current->Element) ) {
                Great = Grand; Grand = Parent; Parent = this->Current;
                this->Current = this->Current->Left;
           } 
           else if ( this->compare (this->Current->Element, X) ) {
                Great = Grand; Grand = Parent; Parent = this->Current;
                this->Current = this->Current->Right;
           }
           else break;

           if( this->Current->Left->Color == Red && this->Current->Right->Color == Red )
                HandleReorient( X );
          }

        // Do not add a duplicate
    	if( this->Current != this->NullNode )
            return 0;
    	this->Current = new BinaryNode( X, this->NullNode, this->NullNode );

        // Attach to parent
    	if( this->compare (X, Parent->Element) )
            Parent->Left = this->Current;
    	else
            Parent->Right = this->Current;
    	HandleReorient( X );
    	return 1;
    }

    /// removes X from the tree. 
    virtual int Remove( const Etype& X ) 
        { return Remove( X, this->Root ); }

    /** returns element that matches X in a RedBlackTree.
     *  WasFound() should test if Current is the NullNode sentinel.
     *
     *  @return item X in tree or item in NullNode, if X is not found.  
     */
    virtual const Etype& Find( const Etype& X ) const {
    	this->NullNode->setElement ( X );	// avoids a test in the while below

        BinaryNode** tempCurrent = (BinaryNode**)& this->Current;
    	*tempCurrent = this->Header->Right;

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

        BinaryNode** tempCurrent = (BinaryNode**)& this->Current;
    	*tempCurrent = this->Header->Right;

    	while ( this->compare.neq (X, this->Current->Element) ) {
            *tempCurrent = ( this->compare(X, this->Current->Element) ?
                this->Current->Left : this->Current->Right );
        }

        return ( this->Current != this->NullNode ) ?
            this->Current->Element:
            this->ItemNotFound;
    }

    /// makes this tree empty.
    virtual void MakeEmpty( )
        { this->FreeTree( this->Header->Right ); this->Header->Right = this->NullNode; }

    /// returns whether the tree is empty.
    virtual int IsEmpty( ) const
        { return this->Header->Right == this->NullNode; }

    /// returns root.
    virtual BinaryNode* GetRoot( ) const { return this->Header->Right; }

  private:

/// color of a node.
    enum { 
        /// red node.
	Red = 0, 
        /// black node.
	Black = 1 
    };  

    /// initializes NulNode and Header.
    void initTree ( const Etype& NegInf = Etype() ) {
    	this->NullNode = new BinaryNode;
    	this->Header = new BinaryNode( NegInf ); 

    	this->NullNode->Left = this->NullNode->Right =
            this->Header->Left = this->Header->Right = this->NullNode;
    }

 // Globals used to traverse down the tree in an insert
    /// parent of a node.
    BinaryNode* Parent;
    /// grand parent of a node.
    BinaryNode* Grand;
    /// great parent of a node.
    BinaryNode* Great;

    /// usual rotate with left child.
    BinaryNode*
    RotateWithLeftChild( BinaryNode* K2 ) {
    	BinaryNode* K1 = K2->Left;
    	K2->Left = K1->Right;
    	K1->Right = K2;
    	return K1;
    }

    /// usual rotate with right child.
    BinaryNode*
    RotateWithRightChild( BinaryNode* K1 ) {
    	BinaryNode* K2 = K1->Right;
    	K1->Right = K2->Left;
    	K2->Left = K1;
    	return K2;
   }

    /** double rotate binary tree node.
     *  First left child with its right child; then node "K3" with
     *  new left child. For RedBlack trees, this is a double rotation for case 3.
     */
    BinaryNode* DoubleRotateWithLeftChild( BinaryNode* K3 ) {
    	K3->Left = RotateWithRightChild ( K3->Left );
    	return RotateWithLeftChild ( K3 );
    }

    /** double rotate binary tree node.
     *  First right child with its left child; then node "K1" with
     *  new right child. For RedBlack trees, this is a double rotation for case 3.
     */
    BinaryNode* DoubleRotateWithRightChild( BinaryNode* K1 ) {
    	K1->Right = RotateWithLeftChild ( K1->Right );
    	return RotateWithRightChild ( K1 );
    }

   /** routine to choose and perform 1 of 4 types of rotations.
    *  Sorry about that mess in the test -- I needed to compress it.
    */
   BinaryNode*
   Rotate( const Etype& Item, BinaryNode* Parent ) {
    	if( this->compare (Item, Parent->Element) )
          {
           if( this->compare (Item, Parent->Left->Element) )
               return Parent->Left = RotateWithLeftChild( Parent->Left );
           else
               return Parent->Left = RotateWithRightChild( Parent->Left );
          }
        else
          {
           if( this->compare (Item, Parent->Right->Element) )
               return Parent->Right = RotateWithLeftChild( Parent->Right );
           else
               return Parent->Right = RotateWithRightChild( Parent->Right );
          }
    }

    /** private routine that is called during an insertion
     *  if a node has two red children.
     *
     *  @param Item node with two red children.
     */
    void HandleReorient( const Etype& Item ) {
        // Do the color flip
    	this->Current->Color = Red;
    	this->Current->Left->Color = Black;
    	this->Current->Right->Color = Black;

    	if( Parent->Color == Red )     // Have to rotate
          {
           Grand->Color = Red;
           if( this->compare (Item, Grand->Element) != this->compare (Item, Parent->Element) )
               Parent = Rotate( Item, Grand );  // Start double rotation
           this->Current = Rotate( Item, Great );
           this->Current->Color = Black;
          }
    	this->Header->Right->Color = Black;  // Make root black
    }

    /** sets the rank of a node. For a graduate treee: <br>
     *  The rank of an external node is 0. <br>
     *  The rank of a parent of an external node is 1. For all other nodes: <br>
     *  --> rank(T) <= rank(Parent(T)) <= rank(T)+1. <br>
     *  --> rank(T) < rank(GrandParent(T)). <br>
     *  A tree is graduate if and only if it is Red Black.
     *
     *  @param T given node.
     *  @return node rank.
     */
    int Graduate ( BinaryNode* T ) const {
    	if ( T == this->NullNode )
             return 0;
        else {
    	     T->Size = 1 + min ( Graduate (T->Left), Graduate (T->Right) );
             if ( T->Left != this->NullNode ) {
                  if ( T->Left->Left != this->NullNode )
                       if ( T->Left->Left->Size >= T->Size )
                            --(T->Left->Left->Size);
                  
                  if ( T->Left->Right != this->NullNode )
                       if ( T->Left->Right->Size >= T->Size )
                            --(T->Left->Right->Size);
                  
             }
             if ( T->Right != this->NullNode ) {
                  if ( T->Right->Left != this->NullNode )
                       if ( T->Right->Left->Size >= T->Size )
                            --(T->Right->Left->Size);
                  
                  if ( T->Right->Right != this->NullNode )
                       if ( T->Right->Right->Size >= T->Size )
                            --(T->Right->Right->Size);
                  
             }
             return T->Size;
        }
     return 0; 
    }

    /** sets the color of a node based on its parent rank.
     *  The root is Black.
     *  If the node has the same rank of its parent, it is Red.
     *  Otherwise, it is Black.
     *
     *  @param T given node.
     */
    void SetColor ( BinaryNode* T ) const {
    	if ( T->Left != this->NullNode )
           {
             T->Left->Color = ( T->Left->Size == T->Size ) ? Red : Black;
             SetColor ( T->Left );
           }
    	if ( T->Right != this->NullNode )
           {
             T->Right->Color = ( T->Right->Size == T->Size ) ? Red : Black;
             SetColor ( T->Right );
           }
    }

    /** rebalances the tree for a removal operation.
     *  Top-down approach. The idea is to make the 
     *  node to be deleted Red.
     *
     *  @param X contents of the node to be deleted.
     */
     void Rebalance( const Etype& X ) {
        this->Header->Color = Red;
    	Parent = this->Header;       			
        this->Current = this->Header; 
        BinaryNode* Sibling, *Tmp; 	
        int side;
        while ( 1 ) { // Current->Element != X ) 
           if ( this->compare (X, this->Current->Element) ) {
               Grand   = Parent; 
               Parent  = this->Current;
               this->Current = Parent->Left;
               Sibling = Parent->Right;
               side = 1;
           }
           else if ( this->compare (this->Current->Element, X) ) {
               Grand   = Parent; 
               Parent  = this->Current;
               this->Current = Parent->Right;
               Sibling = Parent->Left;
               side = 0;
           }
           else break;
// Invariant: at this point P is Red, Current and Sibling are Black.
Start:
           if ( this->Current->Left->Color == Black &&		// Current has two black children 
                this->Current->Right->Color == Black ) {
                if( Sibling->Left->Color == Black &&    	// Sibling has two black children
                    Sibling->Right->Color == Black ) {		// flip color
                    Parent->Color = Black;
                    this->Current->Color = Red; 
                    if ( Sibling != this->NullNode )
                         Sibling->Color = Red;
                }
                else if ( side ) {				// goto left side
                    if ( Sibling->Right->Color == Red ) {  	// outer Red child
                         Tmp = RotateWithRightChild ( Parent ); 
                         Parent->Color = Black;
                         this->Current->Color = Red;
                         Sibling->Color = Red;
                         Sibling->Right->Color = Black; 
                         if ( Grand->Right == Parent ) 
                              Grand->Right = Tmp;
                         else
                              Grand->Left = Tmp;
                    }
                    else if ( Sibling->Left->Color == Red ) { 	// inner Red child
                         Tmp = DoubleRotateWithRightChild ( Parent );
                         Parent->Color = Black;
                         this->Current->Color = Red;
                         Tmp->Color = Red;
                         if ( Grand->Right == Parent ) 
                              Grand->Right = Tmp;
                         else
                              Grand->Left = Tmp;
                    }
                }
                else {						// simetric cases (right side)
                    if ( Sibling->Left->Color == Red ) {
                         Tmp = RotateWithLeftChild ( Parent );  // outer Red child
                         Parent->Color = Black;
                         this->Current->Color = Red;
                         Sibling->Color = Red;
                         Sibling->Left->Color = Black; 
                         if ( Grand->Right == Parent ) 
                              Grand->Right = Tmp;
                         else
                              Grand->Left = Tmp;
                    }
                    else if ( Sibling->Right->Color == Red ) {	// inner Red child
                         Tmp = DoubleRotateWithLeftChild ( Parent );
                         Parent->Color = Black;
                         this->Current->Color = Red;
                         Tmp->Color = Red;
                         if ( Grand->Right == Parent ) 
                              Grand->Right = Tmp;
                         else
                              Grand->Left = Tmp;
                    }
                }  
           }
           else {						// Current has one Red child
                 Grand = Parent;				// go one level deeper
                 Parent = this->Current;				// Black	
                 if ( this->compare (X, this->Current->Element) ) {
                      this->Current = Parent->Left;  
                      if ( this->Current->Color == Black ) { 		// Current Red: nothing to do
                           Sibling = Parent->Right; 		// Red
                           Tmp = RotateWithRightChild ( Parent );             
                           Parent->Color = Red;
                           Sibling->Color = Black;
                           if ( Grand->Right == Parent ) 
                                Grand->Right = Tmp;
                           else
                                Grand->Left = Tmp;
                           Grand = Tmp;
                           Sibling = Parent->Right;
                           side = 1;
                           goto Start;				// back to the starting point
                      }						// but one level below
                 } 
                 else {
                      this->Current = Parent->Right;  
                      if ( this->Current->Color == Black ) { 
                           Sibling = Parent->Left; 		// Red
                           Tmp = RotateWithLeftChild ( Parent );             
                           Parent->Color = Red;
                           Sibling->Color = Black;
                           if ( Grand->Right == Parent ) 
                                Grand->Right = Tmp;
                           else
                                Grand->Left = Tmp;
                           Grand = Tmp;
                           Sibling = Parent->Left;
                           side = 0;
                           goto Start;
                      }
                 }
           }	// endif 
        } 	// end while
     this->Header->Right->Color = Black;
    }

    /** removes node "X" from the RedBlack tree rooted at "T".
     *
     *	@param X item to be removed.
     *	@param T root node.
     *	@return true unless X was not found.
     */
    virtual int Remove( const Etype& X, BinaryNode*& T ) {
    	BinaryNode* tmp;
        BinaryNode* cmp;

    	if( T == this->NullNode )					// not found
            return 0;
    	else if( this->compare (X, T->Element) ) {  // Go left
           return Remove( X, T->Left );
        }
    	else if( this->compare (T->Element, X) ) {  // Go right
           return Remove( X, T->Right );
        }
    	else if( T->Left != this->NullNode && T->Right != this->NullNode )  // Two children
           {
            // Copy from node to delete into node to keep; then delete
            tmp = (BinaryNode*) this->FindMin( T->Right );
            cmp = T;
            if ( tmp->Right->Right == this->NullNode && 
                 tmp->Right->Left == this->NullNode ) 
                 if ( tmp->Right->Color == Red && 
                      tmp->Color == Black )	
                    {					// only one Red Child
                     tmp->Color = Red;
                     tmp->Right->Color = Black;
                    }
            if ( tmp->Color == Black ) {
                 Rebalance ( tmp->Element ); 
#if 1
                cmp->setElement ( tmp->Element );
                if ( Parent->Left == tmp )
    	             Parent->Left = tmp->Right;
                else
                     Parent->Right = tmp->Right;
    	        delete tmp;
#endif
    	        return 1;
               }
            else {
                T->setElement ( tmp->Element );
                return this->RemoveMin( T->Right ); 			// Remove minimum
            } 
           }
    	// One or zero children
    	tmp = T;
    	cmp = ( tmp->Left != this->NullNode ) ? tmp->Left : tmp->Right; 
        if ( tmp->Color == Black ) {
           if ( cmp->Color == Black )
              Rebalance ( tmp->Element ); 
           else							// cmp color is Red
              cmp->Color = Black;
        }         
    	T = cmp; 						// Reroot T
    	delete tmp;                                    		// Delete old root
    	return 1;
    }
};
#endif
