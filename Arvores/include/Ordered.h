/*
   Oredered.h

   Ordered search trees.

  Program:   weiss
  Module:    $RCSfile: Ordered.h,v $
  Language:  C++
  Date:      $Date: 2012-10-13 19:44:42 $
  Version:   $Revision: 1.3 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 19 November 2000.
*/
#ifndef __Ordered
#define __Ordered

#include "Bst.h"

/** OrderedSearchTree class interface.
 *
 *  Sometimes it is important to be able to access the Kth smallest 
 *  element, for an arbitrary K provided as a parameter. It turns out that
 *  this is easy to do if we keep track of the size of each node in the tree.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.3 $ $Date: 2012-10-13 19:44:42 $
 *	
 *  Etype: must have zero-parameter and copy constructor,
 *     and must have operator. <br>
 *  CONSTRUCTION: with (a) no initializer; <br>
 *  Copy construction of OrderedSearchTree objects is DISALLOWED. <br>
 *  Deep copy is supported.
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *  int Insert( const Etype& X )  --> Insert X <br>
 *  int Remove( const Etype& X )  --> Remove X <br>
 *  Etype FindKth( int K ) 	  --> Return Kth smallest item <br>
 *  int Size( const BinaryNode* T) --> Calculates the size of a node <br>
 *  int TreeSize( ) 	 	  --> Return size of tree <br>
 *  ------------- Inherited ----------------------------- <br>
 *  Etype Find( const Etype& X )  --> Return item that matches X <br>
 *  int WasFound( )        	  --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br>
 *  Etype FindMin( )       	  --> Return smallest item <br>
 *  Etype FindMax( )       	  --> Return largest item <br>
 *  int IsEmpty( )         	  --> Return 1 if empty; else return 0 <br>
 *  void MakeEmpty( )      	  --> Remove all items <br>
 *  BinayNode* GetRoot()          --> Return root node <br>
 *  void Merge()        	  --> Merge two binary trees <br>
 *  int Height( const BinaryNode* T) --> Calculates the height of a node <br>
 *  ******************ERRORS******************************** <br>
 *  Predefined exception is propagated if new fails. <br>
 *  ItemNotFound returned on various degenerate conditions. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class OrderedSearchTree : public SearchTree<Etype,Cmp>
{
  public:

  /// a binary node definition.
  typedef typename SearchTree<Etype,Cmp>::BinaryNode BinaryNode;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    OrderedSearchTree( ) { }

    /// destructor.
    virtual ~OrderedSearchTree () { }
  //@}

    /** returns kth smallest item in tree.
     *  Result can be checked by calling WasFound.
     *
     *  @return ItemNotFound, if X is not found.
     */
    const Etype& FindKth( int K ) {
      	return ( this->Root == this->NullNode ) ?
             this->ItemNotFound:
             FindKth( K, this->Root )->Element; 
    }

    /** adds X into the tree. If X already present, do nothing.
     *  If we attempt to inherit the public Insert from SearchTree,
     *  the hidden Insert is scoped to SearchTree rather than 
     *  OrderedSearchTree.
     *
     * 	@return true if insertion was successful.
     */
    virtual int Insert( const Etype& X )
        { return Insert( X, this->Root ); }

    /** removes X from the tree. 
     *  If we attempt to inherit the public Remove from SearchTree,
     *  the hidden Remove is scoped to SearchTree rather than 
     *  OrderedSearchTree.
     *
     *	@return true if successful.
     */
    virtual int Remove( const Etype& X )
        { return Remove( X, this->Root ); }

    /** removes minimum item from the tree. 
     *  If we attempt to inherit the public RemoveMin from SearchTree,
     *  the hidden RemoveMin is scoped to SearchTree rather than 
     *  OrderedSearchTree.
     *
     *  @return true if successful.
     */
    virtual int RemoveMin( )
        { return RemoveMin( this->Root ); }

    /** computes the size of a node.
     *  The size is the number of descendents of a node
     *  (including itself).
     */
    virtual int Size ( const BinaryNode* T ) const {
    	if ( T == this->NullNode )
             return 0;
        else
    	     return T->Size;
    }

    /// returns tree size.
    int TreeSize( ) const { return (this->Root != this->NullNode ? this->Root->Size : 0); }

  private:
    
    /** Internal Insert routine for OrderedSearchTree.
     *  Add X into tree rooted at T.
     *
     *  @return true unless X is a duplicate.
     */
    virtual int Insert( const Etype& X, BinaryNode*& T ) {
    	if( T == this->NullNode )
            return ( T = new BinaryNode( X ) ) != this->NullNode;
    	else if( this->compare (X, T->Element) )
            return Insert( X, T->Left ) ? ++T->Size : 0;
    	else if( this->compare (T->Element, X) )
            return Insert( X, T->Right ) ? ++T->Size : 0;
    	else  // Duplicate -- do nothing
            return 0;
    }

    /** Internal Remove for OrderedSearchTree.
     *  Remove X from tree rooted at T. 
     *
     *  @return true if X was found and false otherwise.
     */
    virtual int Remove( const Etype& X, BinaryNode*& T ) {
    	BinaryNode* Tmp = this->NullNode;

    	if ( T == this->NullNode )
           return 0;
    	else if( this->compare (X, T->Element) )   // Go left
           return Remove( X, T->Left ) ? T->Size-- : 0;
    	else if( this->compare (T->Element, X) )   // Go right
           return Remove( X, T->Right ) ? T->Size-- : 0;
    	else if( T->Left != this->NullNode && T->Right != this->NullNode )  // Two children
    	   {
            Tmp = (BinaryNode*) this->FindMin( T->Right );
            T->setElement ( Tmp->Element );
            T->Size--;
            return RemoveMin( T->Right );       // Remove minimum
    	   }

        // One or zero children
    	Tmp = T;
    	T = ( T->Left != this->NullNode ) ? T->Left : T->Right;
    	delete Tmp;
    	return 1;
    }

    /** Internal RemoveMin for OrderedSearchTree.
     *  Remove minimum item from tree rooted at T.
     *
     *  @return true if tree was not empty prior to removal.
     */
    virtual int RemoveMin( BinaryNode*& T ) {
    	if( T == this->NullNode )
            return 0;
    	else if( T->Left != NULL )
            return RemoveMin( T->Left ) ? T->Size-- : 0;

    	BinaryNode* Tmp = T;
    	T = T->Right;
    	delete Tmp;
    	return 1;
    }

    /** Internal FindKth routine for OrderedSearchTree.
     *
     *  @return pointer to node containing Kth smallest item
     *          in tree rooted at T or NULL if K is out of range.
     */
    const BinaryNode* FindKth( int K, const BinaryNode* T ) const {
    	if( T == this->NullNode )
            return this->NullNode;

    	int LeftSize = ( T->Left != this->NullNode ? T->Left->Size : 0 );

    	if( K <= LeftSize )
            return FindKth( K, T->Left );
    	if( K == LeftSize + 1 )
            return T;
    	return FindKth(  K - LeftSize - 1, T->Right );
    }
};

#endif
