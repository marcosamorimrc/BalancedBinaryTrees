/*
   Splay.h

   Splay trees.

  Program:   weiss
  Module:    $RCSfile: Splay.h,v $
  Language:  C++
  Date:      $Date: 2012-10-13 19:44:42 $
  Version:   $Revision: 1.3 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __SPLAY
#define __SPLAY

#include "Bst.h" 

/** SplayTree class interface.
 *
 *  No balance information is stored. Therefore, it is possible to create
 *  unbalanced splay trees. Nonetheless, accesses to unbalanced portions of 
 *  the tree will naturally tend to balance themselves out.
 *  The nice property is that the total time to perform any sequence
 *  of m insertion, deletion or find operations is O(m log n), where n is the
 *  maximum number of nodes in the tree.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.3 $ $Date: 2012-10-13 19:44:42 $
 *
 *  Etype: must have zero-parameter and copy constructor
 *     and must have operator<, operator!=, and operator= <br>
 *  CONSTRUCTION: with (a) no initializer. <br>
 *  Deep copy is supported.
 *
 * ******************PUBLIC OPERATIONS**************** <br>
 *  int Insert( const Etype& X)  --> Insert X <br>
 *  int Remove( const Etype& X)  --> Remove X <br>
 *  Etype Find( const Etype& X)	 --> Return item that matches X <br>
 *  ------------- Inherited ----------------------------- <br>
 *  int RemoveMin( )    	 --> Removes minimum item from tree <br>
 *  int WasFound( )     	 --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X) --> Return 1 if X would be found <br>
 *  Etype FindMin( )    	 --> Return smallest item <br>
 *  Etype FindMax( )    	 --> Return largest item <br>
 *  int IsEmpty( )      	 --> Return 1 if empty; else return 0 <br>
 *  void MakeEmpty( )   	 --> Remove all items <br>
 *  BinayNode<Etype>* GetRoot()  --> Return root node <br>
 *  void Merge()        	 --> Merge two binary trees <br>
 *  int Height( const BinaryNode* T) --> Calculates the height of a node <br>
 *  int Size( const BinaryNode* T)   --> Calculates the size of a node <br>
 * ******************ERRORS*************************** <br>
 * Predefined exception is propagated if new fails. <br>
 * Item in NullNode return for Find on empty tree. <br>
 */
template <class Etype, class Cmp=compare_to<Etype> >
class SplayTree : public SearchTree<Etype,Cmp>
{
  public:

  /// a binary node definition.
  typedef typename SearchTree<Etype,Cmp>::BinaryNode BinaryNode;

  /** @name constructors  */ 
  //@{

/// empty costructor.
    SplayTree( const Cmp& comp = Cmp() ) : SearchTree<Etype,Cmp>(comp) {
    	this->NullNode = new BinaryNode;
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->Root = this->NullNode;
    }

    /// copy constructor.
    SplayTree( const SplayTree& Rhs ) {
    	this->NullNode = new BinaryNode;
    	this->NullNode->Left = this->NullNode->Right = this->NullNode;
    	this->Root = this->NullNode;
    	*this = Rhs;
    }

/// destructor.
    virtual ~SplayTree( ) { 
	this->FreeTree( this->Root );  this->Root = NULL;
	delete this->NullNode;   this->NullNode = NULL; 
    }
  //@}

    /** adds X into the tree. If X already present, do nothing.
     *
     *  @param X item to be inserted.
     *  @return true if insertion was successful and false otherwise.
     */
    virtual int Insert( const Etype & X ) {
    	static BinaryNode* NewNode = NULL;

    	if( NewNode == NULL )
            NewNode = new BinaryNode( X );
        else
            NewNode->setElement ( X );

    	if( this->Root == this->NullNode )
    	  {
           NewNode->Left = NewNode->Right = this->NullNode;
           this->Root = NewNode;
    	  }
    	else
    	  {
           Splay( X, this->Root );
           if( this->compare (X, this->Root->Element) )
             {
              NewNode->Left = this->Root->Left;
              NewNode->Right = this->Root;
              this->Root->Left = this->NullNode;
              this->Root = NewNode;
             }
           else if( this->compare(this->Root->Element, X) )
             {
              NewNode->Right = this->Root->Right;
              NewNode->Left = this->Root;
              this->Root->Right = this->NullNode;
              this->Root = NewNode;
             }
           else    // Already in the tree
              return 0;
          }

    	NewNode = NULL;   // So next Insert will call new
    	return 1;
    }

    /** removes an item from tree. 
     *
     *  @param X item to be removed.
     *  @return 1 if X was found and 0 otherwise.
     */
    virtual int Remove( const Etype& X ) {
    	BinaryNode* NewTree;

    	if( !IsFound( X ) )
           return 0;

        // If X is found, it will be at the root
    	if( this->Root->Left == this->NullNode )
           NewTree = this->Root->Right;
    	else
    	  {    
           // Find the maximum in the left subtree
           // splay it to root; and then attach right child
           NewTree = this->Root->Left;
           Splay( X, NewTree );
           NewTree->Right = this->Root->Right;
    	  }
    	delete this->Root;
    	this->Root = NewTree;
    	return 1;
    }

    /** returns whether a given item is into the tree. 
     *  If X is found, it will be at the root.
     *  Thus it returns success or failure.
     *  GetRoot() can be used to see what is there.
     *
     *  @param X element to be searched for.
     *  @return true if found, and false otherwise.
     */
    virtual int IsFound( const Etype& X ) const { 
        BinaryNode** tempRoot = (BinaryNode**)& this->Root;
	Splay( X, *tempRoot );
        return (this->Root != this->NullNode && this->Root->Element == X); 
    }

    /** returns item X in tree. 
     *	Result can be checked by calling WasFound().
     *
     *  @param X item to be searched for.
     *  @return ItemNotFound, if X is not found.
     */
    virtual const Etype& Find( const Etype& X ) const {
        BinaryNode** tempCurrent = (BinaryNode**)& this->Current;
    	*tempCurrent = (IsFound(X) ? this->Root : this->NullNode); // PRC - 01/02/01
        return ( this->Current != this->NullNode ) ?
             this->Current->Element:
             this->ItemNotFound;
    }

    /** returns item X in tree. 
     *  Version specialized for pairs, since it uses operator neq.
     *	Result can be checked by calling WasFound().
     *
     *  @param X item to be searched for.
     *  @return ItemNotFound, if X is not found.
     */
    virtual const Etype& FindP( const Etype& X ) const {
        BinaryNode** tempCurrent = (BinaryNode**)& this->Current;
    	*tempCurrent = (IsFoundKey(X) ? this->Root : this->NullNode); // PRC - 01/02/01
        return ( this->Current!= this->NullNode ) ?
             this->Current->Element: 
             this->ItemNotFound;
    }
  private:

    /// usual rotate routine with left child.
    BinaryNode*
    RotateWithLeftChild( BinaryNode* K2 ) const {
    	BinaryNode* K1 = K2->Left;
    	K2->Left = K1->Right;
    	K1->Right = K2;
    	return K1;
    }

    /// usual rotate routine with right child.
    BinaryNode*
    RotateWithRightChild( BinaryNode* K1 ) const {
    	BinaryNode* K2 = K1->Right;
    	K1->Right = K2->Left;
    	K2->Left = K1;
    	return K2;
    }

    /** Top-down splay routine.
     *  Last accessed node becomes the new root.
     */
    void Splay( const Etype& X, BinaryNode*& T ) const {
    	BinaryNode* LeftTreeMax; 
        BinaryNode* RightTreeMin, Header;

    	Header.Left = Header.Right = this->NullNode;
    	LeftTreeMax = RightTreeMin = &Header;

    	// Copy X to NullNode to guarantee match
    	this->NullNode->setElement ( X );

    	for ( ; ; ) {
          if( this->compare (X, T->Element) )
            {
             if( this->compare (X, T->Left->Element) ) 
                T = RotateWithLeftChild( T );
             if( T->Left == this->NullNode )
                break;
             // Link Right
             RightTreeMin->Left = T;
             RightTreeMin = T;
             T = T->Left;
            }
          else if( this->compare(T->Element, X) )
            {
             if( this->compare(T->Right->Element, X) ) 
                T = RotateWithRightChild( T );
             if( T->Right == this->NullNode )
                break;
             // Link Left
             LeftTreeMax->Right = T;
             LeftTreeMax = T;
             T = T->Right;
            }
          else break;
        }
    	LeftTreeMax->Right = T->Left;
    	RightTreeMin->Left = T->Right;
    	T->Left = Header.Right;
    	T->Right = Header.Left;
    }

    /** Top-down splay routine.
     *  Last accessed node becomes the new root.
     *  Version for comparing pairs by using operator!=().
     */
    void SplayF( const Etype& X, BinaryNode*& T ) const {
    	BinaryNode* LeftTreeMax; 
        BinaryNode* RightTreeMin, Header;

    	Header.Left = Header.Right = this->NullNode;
    	LeftTreeMax = RightTreeMin = &Header;

    	// Copy X to NullNode to guarantee match
    	this->NullNode->setElement ( X );

    	while ( this->compare.neq(X, T->Element) ) {
          if( this->compare(X, T->Element) )
            {
             if( this->compare(X, T->Left->Element) && this->compare.neq(X, T->Left->Element) ) 
                T = RotateWithLeftChild( T );
             if( T->Left == this->NullNode )
                break;
             // Link Right
             RightTreeMin->Left = T;
             RightTreeMin = T;
             T = T->Left;
            }
          else // if( T->Element < X )
            {
             if( this->compare(T->Right->Element, X) && this->compare.neq(X, T->Right->Element) ) 
                T = RotateWithRightChild( T );
             if( T->Right == this->NullNode )
                break;
             // Link Left
             LeftTreeMax->Right = T;
             LeftTreeMax = T;
             T = T->Right;
            }
       // else break;
        }
    	LeftTreeMax->Right = T->Left;
    	RightTreeMin->Left = T->Right;
    	T->Left = Header.Right;
    	T->Right = Header.Left;
    }

    /** returns whether a given item is into the tree. 
     *  If X is found, it will be at the root.
     *  Thus it returns success or failure.
     *  GetRoot() can be used to see what is there.
     *  Version for comparing pairs by using operator!=().
     *
     *  @param X element to be searched for.
     *  @return true if found, and false otherwise.
     */
    int IsFoundKey( const Etype& X ) const { 
        BinaryNode** tempRoot = (BinaryNode**)& this->Root;
	SplayF( X, *tempRoot );
        return !(this->Root == this->NullNode || this->compare.neq(this->Root->Element, X)); 
    }

};
#endif
