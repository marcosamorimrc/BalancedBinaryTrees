/*
   AATree.h

   AA trees.

  Program:   weiss
  Module:    $RCSfile: AATree.h,v $
  Language:  C++
  Date:      $Date: 2012-10-13 19:44:41 $
  Version:   $Revision: 1.3 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __AATree
#define __AATree

#include "Bst.h"

#define Level BalancingInfo

/** AATree class interface.
 *
 *  The method of choice when a balanced tree is needed, a casual
 *  implementation is acceptable, and deletions are needed.
 *  An AATree is a RedBlack tree with one extra condition:
 *  left children may not be red. This simple condition greatly
 *  simplifies the red black algorithms. First it eliminates
 *  half of the restructuring cases, and second, it simplifies the
 *  remove algorithm.
 * 
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.3 $ $Date: 2012-10-13 19:44:41 $
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
 *  ------------- Inherited ----------------------------- <br>
 *  int WasFound( )        	  --> Return 1 if last Find succeeded <br> 
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br> 
 *  Etype FindMin( )              --> Return smallest item <br> 
 *  Etype FindMax( )              --> Return largest item <br> 
 *  int IsEmpty( )                --> Return 1 if empty; else return 0  <br>
 *  void MakeEmpty( )             --> Remove all items <br>
 *  BinayNode<Etype>* GetRoot()   --> Return root node <br>
 *  int Height( const BinaryNode* T ) --> Calculates the height of a node <br>
 *  int Size ( const BinaryNode* T )  --> Calculates the size of a node <br>
 *  ******************ERRORS********************************  <br>
 *  Predefined exception is propagated if new fails. <br>
 *  Item in sentinel node returned on various degenerate conditions.
 */

template <class Etype, class Cmp=compare_to<Etype> >
class AATree: public SearchTree<Etype,Cmp>
{
  public:

  /// a binary node definition.
  typedef typename SearchTree<Etype,Cmp>::BinaryNode BinaryNode;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    AATree( const Cmp& comp = Cmp() ) : SearchTree<Etype,Cmp>(comp) {
    	this->NullNode = new BinaryNode( );
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->NullNode->Level = 0;
    	this->Root = this->NullNode;
    }

    /// copy constructor.
    AATree( const AATree& Rsh ) {
    	this->NullNode = new BinaryNode( );
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->NullNode->Level = 0;
    	this->Root = this->NullNode;
        *this = Rsh;
    }

    /// destructor. Deletes the tree and NullNode.
    virtual ~AATree( ) { 
	this->FreeTree( this->Root ); this->Root = NULL;  
	delete this->NullNode; this->NullNode = NULL;
    }

  //@}

    /** adds X into the tree. If X already present, do nothing.
     *
     *  @param X item to be inserted.
     *	@return true if insertion was successful.
     */
    virtual int Insert( const Etype& X )
        { return Insert( X, this->Root ); }

    /** removes X from tree. 
     *
     *  @param X item to be deleted.
     *	@return 1 if X was found, and 0 otherwise.
     */
    virtual int Remove( const Etype& X )
        { return Remove( X, this->Root ); }

    /** returns item X in tree. 
     *	Result can be checked by calling WasFound().
     *
     *  @param X item to be searched for.
     *  @return ItemNotFound, if X is not found.
     */
    virtual const Etype& Find ( const Etype& X ) const {
    	this->NullNode->setElement ( X );	// avoids a test in the while below

        BinaryNode** tempCurrent = (BinaryNode**)&this->Current;

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

  private:

    /// usual rotation routine with left child.
    BinaryNode* RotateWithLeftChild( BinaryNode* K2 ) {
    	BinaryNode* K1 = K2->Left;
    	K2->Left  = K1->Right;
    	K1->Right = K2;
    	return K1;
    }

    /// usual rotation routine with right child.
    BinaryNode* RotateWithRightChild( BinaryNode* K1 ) {
    	BinaryNode* K2 = K1->Right;
    	K1->Right = K2->Left;
    	K2->Left  = K1;
    	return K2;
    }

    /// skew primitive.
    void Skew ( BinaryNode*& T ) {
    	if( T->Left->Level == T->Level )
            T = RotateWithLeftChild( T );
    }

    /// split primitive.
    void Split( BinaryNode*& T ) {
    	if( T->Right->Right->Level == T->Level )
          {
           T = RotateWithRightChild( T );
           T->Level++;
          }
    }
   
    /** inserts a new node "X" into the AA tree rooted at "T".
     *
     * 	@param X new node.
     *	@param T root node.
     *	@return true unless X is a duplicate.
     */
    virtual int Insert( const Etype& X, BinaryNode*& T ) {
    	if( T == this->NullNode )
    	  {
           T = new BinaryNode( X, this->NullNode, this->NullNode );
           return 1;
    	  }
    	else if( this->compare(X, T->Element) )
           Insert( X, T->Left );
    	else if( this->compare(T->Element, X) )
           Insert( X, T->Right );
    	else
           return 0;

    	Skew( T );
    	Split( T );
    	return 1;
    }

    /** removes node "X" from the AA tree rooted at "T".
     *
     *	@param X item to be removed.
     *	@param T root node.
     *	@return true unless X was not found.
     */
    virtual int Remove( const Etype& X, BinaryNode*& T ) {
    	static int ItemFound = 0;
    	static BinaryNode* DeletePtr;
    	static BinaryNode* LastPtr;

    	if( T != this->NullNode )
          {
           // Step 1: Search down the tree and set LastPtr and DeletePtr
           LastPtr = T;
           if( this->compare(X, T->Element) )
               Remove( X, T->Left );
           else
             {
              DeletePtr = T;
              Remove( X, T->Right );
             }

           // Step 2: If at the bottom of the tree and
           //         X is present, we remove it

           if( T == LastPtr )
             {
              if( DeletePtr != this->NullNode && X == DeletePtr->Element )
                {
                 DeletePtr->setElement ( T->Element );
                 DeletePtr = this->NullNode;
                 T = T->Right;
                 delete LastPtr;
                 ItemFound = 1;
                }
              else
                 ItemFound = 0;
             }

           // Step 3: Otherwise, we are not at the bottom; rebalance
           else if( T->Left->Level  < T->Level - 1 || 
		    T->Right->Level < T->Level - 1 )
             {
              if( T->Right->Level > --T->Level )
                  T->Right->Level = T->Level;

              Skew( T );
              Skew( T->Right );
              Skew( T->Right->Right );
              Split( T );
              Split( T->Right );
             }
          }

        return ItemFound;
    }
};
#endif
