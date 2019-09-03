/*
   StackLi.h

   Stack based on linked list.

  Program:   weiss
  Module:    $RCSfile: StackLi.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 17 November 2000.
*/
#ifndef __StackLi
#define __StackLi

#include <stdlib.h>
#include "AbsStack.h"
#include "Exception.h"

/** Stack class interface: linked list implementation.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 * Etype: must have zero-parameter and copy constructor. <br>
 * CONSTRUCTION: with (a) no initializer; <br>
 *     copy construction of Stack objects is DISALLOWED. <br> 
 * Deep copy is supported. <br>
 *
 * ******************PUBLIC OPERATIONS********************* <br>
 * void Push( const Etype& X )   --> Insert X <br>
 * void Pop( )            	 --> Remove most recently inserted item <br>
 * Etype Top( )           	 --> Return most recently inserted item <br>
 * int IsEmpty( )         	 --> Return 1 if empty; else return 0 <br>
 * int IsFull( )          	 --> Return 1 if full; else return 0 <br>
 * void MakeEmpty( )      	 --> Remove all items <br>
 * ******************ERRORS******************************** <br>
 * Predefined exception is propagated if new fails. <br>
 * EXCEPTION is called for Top or Pop on empty stack. <br>
 */

// List-based stack
template <class Etype>
class Stack : public AbsStack<Etype>
{
  public:

  /** @name constructors  */ 
  //@{
    /// empty constructor.
    Stack( ) : TopOfStack( NULL ) { }
    /// destructor.
    virtual ~Stack( ) { MakeEmpty( ); }
  //@}

    /// copy operator (deep).
    const Stack & operator= ( const Stack & Rhs ) {
        // Check for aliasing
    	if( this == &Rhs )
            return *this;

    	MakeEmpty( );
    	if( Rhs.IsEmpty( ) )
            return *this;

    	StackNode *Ptr = new StackNode( Rhs.TopOfStack->Element );
    	StackNode *RhsPtr = Rhs.TopOfStack->Next;

    	TopOfStack = Ptr;
    	while( RhsPtr != NULL )
    	  {
           Ptr->Next = new StackNode( RhsPtr->Element );
           RhsPtr = RhsPtr->Next;
           Ptr = Ptr->Next;
          }

    	return *this;
    }

    /// pushes an element onto the stack.
    virtual void Push( const Etype & X ) {
    	TopOfStack = new StackNode( X, TopOfStack );
    }

    /// pops an element form the stack.
    virtual void Pop( ) {
    	EXCEPTION( IsEmpty( ), "Can't Pop an empty stack" );

    	StackNode *Old = TopOfStack;
    	TopOfStack = TopOfStack->Next;
    	delete Old;
    }

    /// returns element on top of the stack.
    virtual const Etype & Top( ) const {
    	EXCEPTION( IsEmpty( ), "Can't Top an empty stack" );
    	return TopOfStack->Element;
    }

    /// returns whether this stack is empty.
    virtual int IsEmpty( ) const   { return TopOfStack == NULL; }

    /// returns whether this stack is full.
    virtual int IsFull( ) const    { return 0; }

    /// makes this stack empty.
    virtual void MakeEmpty( ) {
    	while( !IsEmpty( ) )
           Pop( );
    }

  private:
    // Copy constructor remains disabled by inheritance

    /// hold the structure of a node into the stack.
    struct StackNode
    {
        /// data.
        Etype Element;
        /// next element.
        StackNode *Next;
   
        /// empty constructor. 
        StackNode( ) : Next( NULL ) { }
        /// constructor from element and next.
        StackNode( const Etype& E, StackNode* N = NULL ) :
                    Element( E ), Next( N ) { }
    };

    /// top of the stack.
    StackNode* TopOfStack;
};
#endif
