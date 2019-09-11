/*
   BinNode.h

   Nodes for binary trees.

  Program:   weiss
  Module:    $RCSfile: BinNode.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 15 November 2000.
*/
#ifndef __BinNode
#define __BinNode

#ifndef NULL
#define NULL 0L
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#include "utlSorter.h"

/// foward declaration of external SerachTree class.
template <class Etype, class Cmp> class SearchTree;
/// foward declaration of external OrderedSearchTree class.
template <class Etype, class Cmp> class OrderedSearchTree;
/// foward declaration of external AVLTree class.
template <class Etype, class Cmp> class AVLTree;
/// foward declaration of external AATree class.
template <class Etype, class Cmp> class AATree;
/// foward declaration of external RedBlackTree class.
template <class Etype, class Cmp> class RedBlackTree;
/// foward declaration of external RedBlackTree class.
template <class Etype, class Cmp> class SplayTree;

/** node class for a binary tree.
 *
 *  All fields are protected to hide them from dangerous users.
 *  All tree classes are friends, to access them directly.
 *	
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 */
template <class Etype, class Cmp=compare_to<Etype> >
class BinNode
{
 public:

  /** @name constructors  */ 
  //@{

/// empty constructor.
    BinNode( ) : Left( NULL ), Right( NULL ),
        Size( 1 ), BalancingInfo( 1 ) { }

/// contructor given data.
    BinNode( const Etype& E ) : Element( E ), Left( NULL ),
        Right( NULL ), Size( 1 ), BalancingInfo( 1 ) { }

/// contructor given data, left child and right child.
    BinNode( const Etype& E, BinNode* L, BinNode* R ) :
        Element( E ), Left( L ), Right( R ), Size( 1 ),
        BalancingInfo( 1 ) { }

/// destructor.
    ~BinNode( ) { }

  //@}

/// returns a pointer to left child.
    BinNode* GetLeft () const { return this->Left; }

/// returns a pointer to right child.
    BinNode* GetRight () const { return this->Right; }

/// returns element for reading.
    const Etype& GetElement () const { return this->Element; }

/// returns element for writing.
    Etype& GetElement () { return this->Element; }

/// returns node balance.
    int GetBalance () const { return this->BalancingInfo; }

/// returns node size.
    int GetSize () const { return this->Size; }

/// operator <.  
    int operator < ( const BinNode& n ) const {
       return this->Element < n.Element;
    }

/// operator ==.  
    int operator == ( const BinNode& n ) const {
       return this->Element == n.Element;
    }

/// operator !=.  
    int operator != ( const BinNode& n ) const {
       return !(*this == n);
    }

/// operator >.  
    int operator > ( const BinNode& n ) const {
       return this->Element > n.Element;
    }

/// SearchTree is a friend.
friend class SearchTree<Etype,Cmp>;
/// OrderedSearchTree is a friend.
friend class OrderedSearchTree<Etype,Cmp>;
/// AVLTree is a friend.
friend class AVLTree<Etype,Cmp>;
/// AATree is a friend.
friend class AATree<Etype,Cmp>;
/// RedBlackTree is a friend.
friend class RedBlackTree<Etype,Cmp>;
/// SplayTree is a friend.
friend class SplayTree<Etype,Cmp>;

/// information used for balancing.
int BalancingInfo;
    
    /// Left child.
    BinNode* Left;
    /// Right child.
    BinNode* Right;   
protected:

/// the data in the node.
    Etype Element;

/// node's size  (used for order stats).
    int Size;            
   

/// sets node fields: balance and size.
    void setData ( int Balance, int Size ) {
       this->Size = Size;
       this->BalancingInfo = Balance;
    }

/// sets node element.
    void setElement ( const Etype& e ) {
       // Element = e;
       new(&Element) Etype(e);
    }

/// copy operator. Does not copy Left and Right.
    const BinNode& operator = ( const BinNode& node ) {
       this->Element = node.Element;
       this->Size = node.Size;
       this->BalancingInfo = node.BalancingInfo;
       this->Left = this->Right = NULL;
       return *this;
    }
};
#endif
