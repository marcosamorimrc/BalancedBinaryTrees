/*
   AbsQueue.h

   Abstract class for queues.

  Program:   weiss
  Module:    $RCSfile: AbsQueue.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 17 November 2000.
*/
#ifndef __AbsQueue
#define __AbsQueue

/** Queue abstract class interface.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 * Etype: must have zero-parameter constructor; <br>
 *     implementation will require either. <br>
 *     operator= or copy constructor, perhaps both. <br>
 * CONSTRUCTION: with (a) no initializer; <br>
 *     copy construction of Queue objects is DISALLOWED <br>
 *
 * ******************PUBLIC OPERATIONS********************* <br>
 *     All of the following are pure virtual functions <br>
 * void Enqueue( const Etype& X )--> Insert X <br>
 * void Dequeue( )        	 --> Remove least recently inserted item <br>
 * Etype GetFront( )         	 --> Return least recently inserted item <br>
 * int IsEmpty( )         	 --> Return 1 if empty; else return 0 <br>
 * int IsFull( )          	 --> Return 1 if full; else return 0 <br>
 * void MakeEmpty( )      	 --> Remove all items <br>
 * ******************ERRORS******************************** <br>
 * Front or Dequeue on empty queue <br>
 */

template <class Etype>
class AbsQueue
{
  public:
  /** @name constructors  */ 
  //@{

/// Default constructor.
    AbsQueue( ) { }               
/// Destructor.
    virtual ~AbsQueue( ) { }      
  //@}

/// Insert.
    virtual void Enqueue( const Etype& X ) = 0;    
/// Remove.
    virtual void Dequeue( ) = 0;                    
/// Find.
    virtual const Etype& GetFront( ) const = 0;    
/// returns whether this queue is empty.
    virtual int IsEmpty( ) const = 0;
/// returns whether this queue is full.
    virtual int IsFull( ) const = 0;
/// makes this queue empty.
    virtual void MakeEmpty( ) = 0;

  private:
/// Disable copy constructor.
    AbsQueue( const AbsQueue& ) { }
};
#endif
