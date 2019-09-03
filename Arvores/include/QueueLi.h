/*
   QueueLi.h

   Queue based on linked list.

  Program:   weiss
  Module:    $RCSfile: QueueLi.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 17 November 2000.
*/
#ifndef __QueueLi
#define __QueueLi

#include <stdlib.h>
#include "AbsQueue.h"
#include "Exception.h"

/** Queue class interface: linked list implementation.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 * Etype: must have zero-parameter and constructor. <br>
 * CONSTRUCTION: with (a) no initializer; <br>
 *     copy construction of Queue objects is DISALLOWED <br>
 * Deep copy is supported. <br>
 *
 * ******************PUBLIC OPERATIONS********************* <br>
 * void Enqueue( const Etype& X ) --> Insert X <br>
 * void Dequeue( )        	  --> Remove least recently inserted item <br>
 * Etype GetFront( )         	  --> Return least recently inserted item <br>
 * int IsEmpty( )         	  --> Return 1 if empty; else return 0 <br>
 * int IsFull( )          	  --> Return 1 if full; else return 0 <br>
 * void MakeEmpty( )      	  --> Remove all items <br>
 * ******************ERRORS******************************** <br>
 * Predefined exception is propogated if new fails. <br>
 * EXCEPTION is called for Front or Dequeue on empty queue. <br>
 */

// Array-based queue
template <class Etype>
class Queue : public AbsQueue<Etype>
{
  public:
  /** @name constructors  */ 
  //@{

    /// empty constructor.
    Queue( ) : Front( NULL ), Back( NULL ) { }
    /// destructor.
    virtual ~Queue( ) { MakeEmpty( ); }

  //@}

    /// copy operator (deep).
    const Queue & operator=( const Queue & Rhs ) {
    	if( this == &Rhs )
            return Rhs;

    	MakeEmpty( );
    	if( Rhs.IsEmpty( ) )
            return *this;

    	QueueNode *Ptr = new QueueNode( Rhs.Front->Element );
    	QueueNode *RhsPtr = Rhs.Front->Next;

    	Front = Ptr;
    	while( RhsPtr != NULL )
    	  {
           Ptr->Next = new QueueNode( RhsPtr->Element );
           RhsPtr = RhsPtr->Next;
           Ptr = Ptr->Next;
    	  }

    	Ptr->Next = NULL;   // This line is unnecssary, but is in for clarity
    	Back = Ptr;

    	return *this;
    }

    /// insert "X" into queue.
    virtual void Enqueue( const Etype & X ) {
    	if( IsEmpty( ) )
            Back = Front = new QueueNode( X );
    	else
            Back = Back->Next = new QueueNode( X );
    }

    /// removes an element from the queue.
    virtual void Dequeue( ) {
    	EXCEPTION( IsEmpty( ), "Queue is empty" );

    	QueueNode* Old = Front;
    	Front = Front->Next;
    	delete Old;
    }

    /// returns element at front of the queue.
    virtual const Etype & GetFront( ) const {
    	EXCEPTION( IsEmpty( ), "Queue is empty" );
    	return Front->Element;
    }

    /// returns whether the queue is empty.
    virtual int IsEmpty( ) const { return Front == NULL; }

    /// returns whether the queue is full.
    virtual int IsFull( ) const  { return 0; }

    /// make this queue empty.
    virtual void MakeEmpty( ) {
    	while( !IsEmpty( ) )
            Dequeue( );
    }

  private:
    // Copy constructor remains disabled by inheritance

    /// structure of a node of the queue.
    struct QueueNode
    {
	/// data.
        Etype Element;
        /// pointer to next node.
        QueueNode* Next;
   
        /// empty constructor. 
        QueueNode( ) : Next( NULL ) { }
        /// constructor given element and next.
        QueueNode( const Etype& E, QueueNode* N = NULL ) :
                    Element( E ), Next( N ) { }
    };

    /// front of the queue.
    QueueNode* Front;

    /// back of the queue.
    QueueNode* Back;
};
#endif
