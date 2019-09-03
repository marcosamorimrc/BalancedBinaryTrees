/*
   Bst.h

   Search trees.

  Program:   weiss
  Module:    $RCSfile: Bst.h,v $
  Language:  C++
  Date:      $Date: 2005/10/10 01:32:51 $
  Version:   $Revision: 1.2 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __BST
#define __BST

#include "utlConfig.h"
#include "BinNode.h"
#include "AbsBst.h"
#include "Exception.h"

/** SearchTree class interface.
 *
 *  For any node in the binary search tree, all smaller-keyed
 *  nodes are in the left subtree, and all larger-keyed nodes
 *  are in the right subtree. Duplicates are not allowed.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/10 01:32:51 $
 *	
 *  Etype: must have zero-parameter and copy constructor,
 *     and must have operator< <br>
 *  CONSTRUCTION: with (a) no initializer; <br>
 *  Copy construction of SearchTree objects is DISALLOWED. <br>
 *  Deep copy is supported.
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *  int Insert( const Etype& X)  --> Insert X <br>
 *  int Remove( const Etype& X)  --> Remove X <br>
 *  Etype Find( const Etype& X)	 --> Return item that matches X <br>
 *  int RemoveMin( )    	 --> Removes minimum item from tree <br>
 *  int WasFound( )     	 --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X) --> Return 1 if X would be found <br>
 *  Etype FindMin( )    	 --> Return smallest item <br>
 *  Etype FindMax( )    	 --> Return largest item <br>
 *  int IsEmpty( )      	 --> Return 1 if empty; else return 0 <br>
 *  void MakeEmpty( )   	 --> Remove all items <br>
 *  BinayNode* GetRoot()         --> Return root node <br>
 *  void Merge()        	 --> Merge two binary trees <br>
 *  int Height( const BinaryNode* T) --> Calculates the height of a node <br>
 *  int Size( const BinaryNode* T)   --> Calculates the size of a node <br>
 *  ******************ERRORS******************************** <br>
 *  Predefined exception is propagated if new fails <br>
 *  ItemNotFound returned on various degenerate conditions <br>
 *
 *  All routines are made virtual in case classes are derived <br>
 *  This includes trivial routines because we might want to <br>
 *  implement lazy deletion as a derived class. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class SearchTree : public AbsBst<Etype,Cmp>
{
  public:

  /// a binary node definition.
  typedef BinNode<Etype,Cmp> BinaryNode;

  /** @name constructors  */ 
  //@{
    /// empty constructor.
    SearchTree( ) : Current( NULL ), Root( NULL ), NullNode ( NULL ), 
                    ItemNotFound (Etype()), compare(Cmp()) { }

    /// empty constructor.
    SearchTree( const Cmp& comp ) : 
            Current( NULL ), Root( NULL ), NullNode ( NULL ), 
            ItemNotFound (Etype()), compare(comp) { }

    /// destructor. Deletes the tree.
    virtual ~SearchTree( ) { FreeTree( Root ); Root = NullNode; }

  //@}

    /** adds X into the tree. If X already present, do nothing.
     *
     *  @param X item to be inserted.
     *	@return 1 if successful.
     */
    virtual int Insert( const Etype& X )
        { return Insert( X, Root ); }

    /** removes X from the tree. 
     *
     *  @param X item to be removed.
     *	@return true if successful.
     */
    virtual int Remove( const Etype& X )
        { return Remove( X, Root ); }

    /** removes minimum item from the tree. 
     *
     *	@return true if successful.
     */
    virtual int RemoveMin( )
        { return RemoveMin( Root ); }

    /** returns minimum item in tree. 
     *
     *	@return ItemNotFound if tree is empty. 
     */
    virtual const Etype& FindMin( ) const
        { const BinaryNode* P = FindMin( GetRoot() );
              return ( P != NullNode ? P->GetElement() : ItemNotFound ); }

    /** returns maximum item in tree. 
     *
     *	@return ItemNotFound if tree is empty. 
     */
    virtual const Etype& FindMax( ) const
        { const BinaryNode* P = FindMax( GetRoot() );
              return ( P != NullNode ? P->GetElement() : ItemNotFound ); }

    /** returns item X in tree. 
     *	Result can be checked by calling WasFound.
     *
     *  @param X item to be searched for.
     *	@return ItemNotFound if X is not found. 
     */
    virtual const Etype& Find ( const Etype& X ) const { 
        BinaryNode** tempCurrent = (BinaryNode**)&Current;
        *tempCurrent = (BinaryNode*)Find( X, Root );
        if ( Current != NullNode )
             return Current->GetElement();
        else
             return ItemNotFound; 
    }

    /** returns item X in tree. 
     *  Version specialized for pairs, since it uses operator neq.
     *	Result can be checked by calling WasFound.
     *
     *  @param X item to be searched for.
     *	@return ItemNotFound if X is not found. 
     */
    virtual const Etype& FindP ( const Etype& X ) const { 
	return Find ( X );
    }

    /// returns true if X is in tree.
    virtual int IsFound( const Etype& X ) const
        { return Find( X, Root ) != NullNode; }

    /// returns true if last call to Find() was successful.
    virtual int WasFound( ) const
        { return Current != NullNode; }

    /// returns last node found in Find().
    const BinaryNode* GetCurrent() const { return Current; }

    /// returns whether this tree is full.
    virtual int IsFull( ) const { return 0; } 

    /// makes this tree empty. 
    virtual void MakeEmpty( )
        { FreeTree( Root ); Root = NullNode; }

    /// returns root.
    virtual BinaryNode* GetRoot( ) const { return Root; }

    /// returns NullNode.
    virtual const BinaryNode* GetNullNode( ) const { return NullNode; }

    /** computes the height of a node.
     *  The height is the length of the longest path from 
     *  a node to a leaf. All leaves have height 0.
     */
    virtual int Height ( const BinaryNode* T ) const {
    	if ( T == NullNode )
             return -1;
        else
    	     return 1 + max ( Height (T->Left), Height (T->Right) );
    }

    /** computes the size of a node.
     *  The size is the number of descendents of a node
     *  (including itself).
     */
    virtual int Size ( const BinaryNode* T ) const {
    	if ( T == NullNode )
             return 0;
        else
    	     return 1 + Size (T->Left) + Size (T->Right);
    }

    /// copy operator.
    virtual const SearchTree& operator=( const SearchTree& Rhs ) {
      	if ( this != &Rhs )
           {
            MakeEmpty ( ); 
            compare = Rhs.compare;
            if ( Rhs.Root != Rhs.NullNode )
                 this->Root = Duplicate ( Rhs, Rhs.Root );
           }
     return *this;
    }

    /** merge routine for Binary Trees.
     *  Forms a new tree from RootItem, T1 and T2.
     *  Does NOT allow T1 and T2 to be the same.
     *  Does NOT keep search property.
     *  Correctly handles other aliasing conditions.
     *
     *	@param RootItem element at root.
     *  @param T1 first subtree.
     *  @param T2 second subtree.
     */
    virtual void Merge ( const Etype& RootItem, 
                         SearchTree& T1,
                         SearchTree& T2 ) {
 	EXCEPTION ( T1.Root == T2.Root && T1.Root != T1.NullNode, 
		    "Left Tree == Right Tree; Merge aborted" );	    

	// save old root
	BinaryNode* OldRoot = this->Root;

	// allocate a new node.
	this->Root = new BinaryNode ( RootItem, T1.Root, T2.Root );

	// deallocates nodes in the original tree.
  	if ( this != &T1 && this != &T2 )
             FreeTree ( OldRoot );

	// ensure that every node is in one tree
 	if ( this != &T1 )
           {
            fixNullNode ( T1.Root, T1 );
            T1.Root = T1.NullNode;
           }
        if ( this != &T2 )
           {
            fixNullNode ( T2.Root, T2 );
            T2.Root = T2.NullNode;
           }
    }

    /// returns whether this tree is empty.
    virtual int IsEmpty( ) const
        { return Root == NullNode; }

    /// returns whether the given node has a right child.
    virtual int hasRightChild ( const BinaryNode* node ) const { 
	return node->Right != NullNode; 
    }

    /// returns whether the given node has a left child.
    virtual int hasLeftChild ( const BinaryNode* node ) const { 
	return node->Left != NullNode; 
    }

    /// returns the key_compare object used by the tree. 
    Cmp key_comp() const { return compare; }

    /// swaps the contents of this and a given SearchTree.
    virtual void swap(SearchTree& s) {
      BinaryNode* temp;
      temp   = Root;
      Root   = s.Root;
      s.Root = temp;
      temp       = NullNode;
      NullNode   = s.NullNode;
      s.NullNode = temp;
      temp      = Current;
      Current   = s.Current;
      s.Current = temp;
      Cmp ctemp = compare;
      compare   = s.compare;
      s.compare = ctemp;
    }

  protected:

    /// holds last element found.
    BinaryNode* Current;

    /// tree root.
    BinaryNode* Root;

    /** a node representing a NULL node. 
     *  When a node does not have a child, its corresponding child 
     *	pointer points to the NullNode.
     */
    BinaryNode* NullNode;

    /// used for returns of not found.
    const Etype ItemNotFound;  

    /// function object for comparing two keys.
    Cmp compare;

    /// disable copy constructor.
    SearchTree( const SearchTree& Rsh ) {
      *this = Rsh;
    }

    /// deletes tree rooted at "T".
    virtual void FreeTree( BinaryNode* T ) {
    	if( T != NullNode )
    	  {
           FreeTree( T->Left );
           FreeTree( T->Right );
           delete T;
          }
    }

    /** returns a pointer to a node that is the root of
     *  a duplicate of the tree rooted at the given node.
     *
     *  @param T tree to be cloned.
     *  @param node given node.
     *  @return root node.
     */
    virtual BinaryNode* Duplicate ( const SearchTree& T,
                                    const BinaryNode* node ) const {
        BinaryNode* root = 
              new BinaryNode ( node->GetElement(), this->NullNode, this->NullNode ); 
        root->setData ( node->BalancingInfo, node->Size );

    	if ( T.hasLeftChild ( node ) )
             root->Left = Duplicate ( T, node->GetLeft() );  
    	if ( T.hasRightChild ( node ) )
             root->Right = Duplicate ( T, node->GetRight() );  
    
    	return root;
    }

    /** internal Insert routine for SearchTree.
     * 	Add X into subtree rooted at T.
     *
     *  @param X item to be inserted.
     *  @param T root node.
     *	@return true if insert succeeds and false if X is a duplicate
     */
    virtual int Insert( const Etype& X, BinaryNode*& T ) {
    	if( T == NullNode )
          {
           T = new BinaryNode( X );
           return 1;
          }
    	else if( compare(X, T->Element) )
            return Insert( X, T->Left );
    	else if( compare(T->Element, X) )
            return Insert( X, T->Right );
    	// Don't insert duplicates
    	return 0;
    }

    /** internal Remove routine for SearchTree.
     *	Remove X from tree rooted at T.
     *
     *  @param X item to be removed.
     *  @param T root node.
     *	@return false only if X was not found
     */
    virtual int Remove( const Etype& X, BinaryNode*& T ) {
    	BinaryNode* Tmp;

    	if( T == NullNode )
            return 0;
    	else if( compare(X, T->Element) )   // Go left
            return Remove( X, T->Left );
    	else if( compare(T->Element, X) )   // Go right
            return Remove( X, T->Right );
    	else if( T->Left != NullNode && T->Right != NullNode )  // Two children
           {
            // Copy from node to delete into node to keep; then delete
            Tmp = (BinaryNode *) FindMin( T->Right );
            T->setElement ( Tmp->Element );
            return RemoveMin( T->Right );         	// Remove minimum
           }
    	// One or zero children
    	Tmp = T;
    	T = ( T->Left != NullNode ) ? T->Left : T->Right;  // Reroot T
    	delete Tmp;                                    // Delete old root
    	return 1;
    }

    /** internal RemoveMin routine for SearchTree.
     *	Remove minimum item in tree rooted at T.
     *
     *  @param T root node.
     *	@return false only if tree is empty
     */
    virtual int RemoveMin( BinaryNode*& T ) {
    	if( T == NullNode )
            return 0;
    	else if( T->Left != NullNode )
            return RemoveMin( T->Left );

    	BinaryNode* Tmp = T;
    	T = T->Right;
    	delete Tmp;
    	return 1;
    }

    /** internal FindMin for SearchTree.
     *
     *  @param T root node.
     *	@return pointer to node containing minimum
     *	        or NullNode, if tree is empty.
     */
    virtual const BinaryNode* FindMin( const BinaryNode* T ) const {
    	if( T != NullNode )
            while( hasLeftChild ( T ) )
                T = T->Left;

    	return T;
    }

    /** internal FindMax for SearchTree.
     *
     *  @param T root node.
     * 	@return pointer to node containing maximum
     *	        or NullNode, if tree is empty.
     */
    virtual const BinaryNode* FindMax( const BinaryNode* T ) const {
    	if( T != NullNode )
            while( hasRightChild ( T ) )
               T = T->Right;

    	return T;
    }

    /** internal Find routine for SearchTree.
     *
     *  @param X item to be searched for.
     *  @param T root node.
     *	@return pointer to node containing X or NullNode if X is not found.
     */
    virtual const BinaryNode* Find( const Etype& X,
                                    const BinaryNode* T ) const {
    	while( T != NullNode )
            if( compare(X, T->Element) )
                T = T->Left;
            else if( compare(T->Element, X) )
                T = T->Right;
            else
                return T;   // Match

    	return NullNode;    // Not found
    }

    /** sets all nodes from a given tree pointing to its NullNode, 
     *  to point to the NullNode of this class.
     *
     *	@param ptr root node.
     *	@param T given tree.
     */
    void fixNullNode ( BinaryNode* ptr, SearchTree& T ) {
        if ( ptr->Left != T.NullNode ) 
             fixNullNode ( ptr->Left, T );
        else 
             ptr->Left = this->NullNode;
        if ( ptr->Right != T.NullNode ) 
             fixNullNode ( ptr->Right, T );
        else 
             ptr->Right = this->NullNode;
    }

};
#endif
