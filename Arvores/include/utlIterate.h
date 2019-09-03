/*
   utlIterate.h

   Iterators for binary search trees.

  Program:   weiss
  Module:    $RCSfile: utlIterate.h,v $
  Language:  C++
  Date:      $Date: 2005/10/09 22:40:44 $
  Version:   $Revision: 1.2 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 17 November 2000.
*/

#ifndef __utlIterate__
#define __utlIterate__

#include "Bst.h"
#include "utlStack.h"
#include "utlQueue.h"

//		ITERATOR BASE CLASS

/** TreeIterator class interface; maintains "current position".
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 * Etype: same restrictions as for BinaryTree. <br>
 * CONSTRUCTION: with (a) Tree to which iterator is bound. <br>
 *
 * ******************PUBLIC OPERATIONS********************** <br>
 *     First two are not virtual, last two are pure virtual <br>
 * int operator+( )      	     --> True if at valid position in tree <br>
 * const Etype& operator()( ) const  --> Return item in current position <br>
 * void First( )         	     --> Set current position to first <br>
 * void operator++( )    	     --> Advance (prefix) <br>
 * ******************ERRORS********************************* <br>
 * EXCEPTION is called for illegal access or advance. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class TreeIterator
{
  public:
  /** @name constructors  */ 
  //@{

    /// empty constructor.
    TreeIterator ( ) : T ( NULL ), Current ( NULL ) {}

    /// constructor.
    TreeIterator( const SearchTree<Etype,Cmp>* TheTree ) :
        T( TheTree ), Current( NULL ) { }

    /// copy constructor.
    TreeIterator( const TreeIterator& rhs ) {
       *this = rhs;
    }

    /// destructor.
    virtual ~TreeIterator( ) { }
  //@}

    /// sets current position to first node.
    virtual void First( ) = 0;

    /// returns whether the current position is a valid position in tree.
    int operator+ ( ) const { return this->Current != NULL; }

    /// returns the node in current position reading.
    const Etype& operator( ) ( ) const {
    	EXCEPTION( Current == NULL, "Illegal access!" );
    	return Current->GetElement();
    }

    /// returns whether the elements in this and the given iterator are the same. 
    bool operator == ( const TreeIterator& rhs) const {
        return (this->Current == rhs.Current);
    }

    /// returns whether the elements in this and the given iterator are different. 
    bool operator != ( const TreeIterator& rhs) const {
        return (this->Current != rhs.Current);
    }

    /// increment operator (prefix).
    virtual void operator++(  ) = 0;

    /// output operator.
    friend ostream& operator<<(ostream& os, TreeIterator& itr) {
        int n = 0;
	for ( itr.First(); +itr; ++itr )
            {
      	     os << itr() << " ";
             if ( !(++n%20) ) os << "\n";
            }
	os << endl;
    	return os;
    }

  protected:

    /// holds the tree to be traversed.
    const SearchTree<Etype,Cmp>* T;

    /// current node.
    const BinNode<Etype,Cmp>* Current;

    /// assignment operator.
    TreeIterator& operator = ( const TreeIterator& rhs ) { 
        T = rhs.T;
	Current = rhs.Current; 
        return *this;
    } 
};


//		PREORDER

/** PreOrder class interface; maintains "current position".
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 * Etype: same restrictions as for BinaryTree. <br>
 * CONSTRUCTION: with (a) Tree to which iterator is bound. <br>
 *
 * ******************PUBLIC OPERATIONS********************** <br>
 * int operator+( )     	    --> True if at valid position in tree <br>
 * const Etype& operator()( ) const --> Return item in current position <br>
 * void First( )        	    --> Set current position to first <br>
 * void operator++( )   	    --> Advance (prefix) <br>
 * ******************ERRORS********************************* <br>
 * EXCEPTION is called for illegal access or advance. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class PreOrder: public TreeIterator<Etype,Cmp>
{
  public:
  /** @name constructors  */ 
  //@{
    /// constructor.
    PreOrder( const SearchTree<Etype,Cmp>* TheTree ) : TreeIterator<Etype,Cmp>( TheTree ) {}

    /// copy constructor.
    PreOrder( const PreOrder& rhs ) {
       *this = rhs;
    }

    /// destructor.
    ~PreOrder( ) { }

  //@}

    /// sets current position to first node.
    virtual void First( ) {
        if ( this->T == NULL ) return;
    	while ( !S.empty() ) this->S.pop( );
    	if( this->T->GetRoot() != this->T->GetNullNode() ) {
            this->S.push( this->T->GetRoot() );
    	    operator++();
        }
    }

    /// increment operator (prefix).
    virtual void operator++( ) {
    	if( this->S.empty( ) )
    	  {
           EXCEPTION( this->Current == NULL, "Advanced past end" );
           this->Current = NULL;
           return;
          }

    	this->Current = this->S.top( );
    	S.pop( );
    	if( this->T->hasRightChild ( this->Current ) )
            this->S.push( this->Current->GetRight() );
    	if( this->T->hasLeftChild ( this->Current ) )
            this->S.push( this->Current->GetLeft() );
    }

    /// assignment operator.
    PreOrder& operator = ( const PreOrder& rhs ) { 
        TreeIterator<Etype,Cmp>::operator = ( rhs );
        S = rhs.S;
        return *this;
    } 

  protected:
    /// stack holding branching nodes (where to return).
    utlStack< const BinNode<Etype,Cmp>* > S;
};

//		POSTORDER

/// holds the data necessary for stacking a node.
template <class Etype, class Cmp=compare_to<Etype> >
struct StNode
{
    /// a pointer to a node.
    const BinNode<Etype,Cmp>* Node;

    /// number of times the node has been popped out from the stack.
    int TimesPopped;

    /// constructor.
    StNode( const BinNode<Etype,Cmp>* N = 0 )
            : Node( N ), TimesPopped( 0 ) { }
};

/** PostOrder class interface; maintains "current position".
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 * Etype: same restrictions as for BinaryTree <br>
 * CONSTRUCTION: with (a) Tree to which iterator is bound <br>
 *
 * ******************PUBLIC OPERATIONS********************** <br>
 * int operator+( )     	    --> True if at valid position in tree <br>
 * const Etype& operator()( ) const --> Return item in current position <br>
 * void First( )        	    --> Set current position to first <br>
 * void operator++( )   	    --> Advance (prefix) <br>
 * ******************ERRORS********************************* <br>
 * EXCEPTION is called for illegal access or advance. <br>
 */

template <class Etype, class Cmp=compare_to<Etype> >
class PostOrder : public TreeIterator<Etype,Cmp>
{
  public:
  /** @name constructors  */ 
  //@{
    /// constructor.
    PostOrder( const SearchTree<Etype,Cmp>* TheTree ) : TreeIterator<Etype,Cmp>( TheTree ) { }

    /// copy constructor.
    PostOrder( const PostOrder& rhs ) {
       *this = rhs;
    }

    /// destructor.
    virtual ~PostOrder( ) { }
  //@}

    /// sets current position to first node.
    virtual void First( ) {
        if ( this->T == NULL ) return;
    	while ( !S.empty() ) this->S.pop( );
    	if( this->T->GetRoot() != this->T->GetNullNode() ) {
            this->S.push( StNode<Etype,Cmp>( this->T->GetRoot() ) );
    	    operator++();
        }
    }

    /// increment operator (prefix).
    virtual void operator++( ) {
    	if( this->S.empty( ) )
    	  {
           EXCEPTION( this->Current == NULL, "Advanced past end" );
           this->Current = NULL;
           return;
    	  }

    	StNode <Etype,Cmp> Cnode;

    	for( ; ; )
    	   {
            Cnode = this->S.top( );
            this->S.pop( );
    
            if( ++Cnode.TimesPopped == 3 )
              {
               this->Current = Cnode.Node;
               return;
              }
    
            this->S.push( Cnode );
            if( Cnode.TimesPopped == 1 )
              {
               if( this->T->hasLeftChild ( Cnode.Node )  )
                   this->S.push( StNode<Etype,Cmp>( Cnode.Node->GetLeft() ) );
              }
            else  // Cnode.TimesPopped == 2
              {
               if( this->T->hasRightChild ( Cnode.Node ) )
                   this->S.push( StNode<Etype,Cmp>( Cnode.Node->GetRight() ) );
              }
           }
    }

    /// assignment operator.
    PostOrder& operator = ( const PostOrder& rhs ) { 
        TreeIterator<Etype,Cmp>::operator = ( rhs );
        S = rhs.S;
        return *this;
    } 

  protected:
    /// stack holding branching nodes (where to return).
    utlStack< StNode<Etype,Cmp> > S;
};


//		INORDER

/** InOrder class interface; maintains "current position".
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 * Etype: same restrictions as for BinaryTree. <br>
 * CONSTRUCTION: with (a) Tree to which iterator is bound. <br>
 *
 * ******************PUBLIC OPERATIONS********************** <br>
 * int operator+( )     	    --> True if at valid position in tree <br>
 * const Etype& operator()( ) const --> Return item in current position <br>
 * void First( )        	    --> Set current position to first <br>
 * void operator++( )   	    --> Advance (prefix) <br>   
 * ******************ERRORS*********************************   <br>  
 * EXCEPTION is called for illegal access or advance <br> 
 */

template <class Etype, class Cmp=compare_to<Etype> >
class InOrder : public PostOrder<Etype,Cmp>
{
  // Accept PostOrder construction and default destruction.

  public:
  /** @name constructors  */ 
  //@{
    /// empty constructor.
    InOrder( ) : PostOrder<Etype,Cmp>( NULL ) { }

    /// constructor from a tree.
    InOrder( const SearchTree<Etype,Cmp>* TheTree ) :
        PostOrder<Etype,Cmp>( TheTree ) { }

    /// constructor from a node and a tree.
    InOrder( const BinNode<Etype,Cmp>* node, const SearchTree<Etype,Cmp>* TheTree ) :
        PostOrder<Etype,Cmp>( TheTree ) { 

        this->Current = node;
        StNode<Etype,Cmp> Cnode;
        const Etype X = node->GetElement();
        const BinNode<Etype,Cmp>* curr = this->T->GetRoot();
        Cmp compare = this->T->key_comp();
    	while( curr != node ) {
           if ( compare (X, curr->GetElement()) ) {
                Cnode = StNode<Etype,Cmp> ( curr );
                Cnode.TimesPopped = 1;
                this->S.push ( Cnode );
                curr = curr->GetLeft();
           }
           else {
                curr = curr->GetRight();
           } 
        }
        if( this->T->hasRightChild ( this->Current ) ) 
            this->S.push( StNode<Etype,Cmp>( this->Current->GetRight() ) );
    }

    /// copy constructor.
    InOrder( const InOrder& rhs ) : PostOrder<Etype,Cmp>(rhs) { }
  //@}

    /// increment operator.
    virtual void operator++( ) {
    	if( this->S.empty( ) )
    	  {
           EXCEPTION( this->Current == NULL, "Advanced past end" );
           this->Current = NULL;
           return;
          }

    	StNode<Etype,Cmp> Cnode;

    	for( ; ; )
    	   {
            Cnode = this->S.top( );
            this->S.pop( );
    
            if( ++Cnode.TimesPopped == 2 )
              {
               this->Current = Cnode.Node;
               if( this->T->hasRightChild ( Cnode.Node ) ) 
                   this->S.push( StNode<Etype,Cmp>( Cnode.Node->GetRight() ) );
               return;
              }
    
            // First time through
            this->S.push( Cnode );
            if( this->T->hasLeftChild ( Cnode.Node ) ) 
                this->S.push( StNode<Etype,Cmp>( Cnode.Node->GetLeft() ) );
           }
    }

};

//		LEVEL ORDER

/** LevelOrder class interface; maintains "current position".
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 * Etype: same restrictions as for BinaryTree. <br>
 * CONSTRUCTION: with (a) Tree to which iterator is bound. <br>
 *
 * ******************PUBLIC OPERATIONS********************** <br>
 * int operator+( )     	    --> True if at valid position in tree <br>
 * const Etype& operator()( ) const --> Return item in current position <br>
 * void First( )        	    --> Set current position to first <br>
 * void operator++( )   	    --> Advance (prefix) <br>
 * ******************ERRORS********************************* <br>
 *  EXCEPTION is called for illegal access or advance.
 */

template <class Etype, class Cmp=compare_to<Etype> >
class LevelOrder : public TreeIterator<Etype,Cmp>
{
  public:
  /** @name constructors  */ 
  //@{
    /// constructor.
    LevelOrder( const SearchTree<Etype,Cmp>* TheTree ) : TreeIterator<Etype,Cmp>( TheTree ) {}

    /// copy constructor.
    LevelOrder( const LevelOrder& rhs ) { 
        *this = rhs;
    }

    /// destructor.
    ~LevelOrder( ) { }

  //@}

    /// sets current position to first node.
    virtual void First( ) {
        if ( this->T == NULL ) return;
    	while ( !Q.empty() ) Q.pop( );
    	if( this->T->GetRoot() != this->T->GetNullNode() ) {
            Q.push( this->T->GetRoot() );
    	    operator++();
        }
    }

    /// increment operator (prefix).
    virtual void operator++( ) {
    	if( Q.empty( ) )
    	  {
           EXCEPTION( this->Current == NULL, "Advanced past end" );
           this->Current = NULL;
           return;
    	  }

    	this->Current = Q.front( );
    	Q.pop( );
    	if( this->T->hasLeftChild ( this->Current ) )
            Q.push( this->Current->GetLeft() );
    	if( this->T->hasRightChild ( this->Current ) )
            Q.push( this->Current->GetRight() );
    }

    /// assignment operator.
    LevelOrder& operator = ( const LevelOrder& rhs ) { 
        TreeIterator<Etype,Cmp>::operator = ( rhs );
        Q = rhs.Q;
        return *this;
    } 

  private:
    /// queue holding branching nodes (where to return).
    utlQueue< const BinNode<Etype,Cmp>* > Q;
};
#endif
