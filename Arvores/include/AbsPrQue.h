/*
   AbsPrQue.h

   Abstract class for priority queues.

  Program:   weiss
  Module:    $RCSfile: AbsPrQue.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 29 November 2000.
*/
#ifndef __AbsPrQue
#define __AbsPrQue

/**
 *  Abstract class for priority queues.
 *
 *  Etype: must have zero-parameter constructor and operator=; <br>
 *     must have operator< <br>
 *  CONSTRUCTION: with (a) Etype representing negative infinity <br>
 *  Copy construction of AbsPrQue objects is DISALLOWED. <br>
 *  Deep copy is supported. <br>
 *
 *  ******************PUBLIC OPERATIONS****************** <br>
 *  void Insert( const Etype& X ) --> Insert X <br>
 *  const Etype& FindMin( ) const --> Return smallest item <br>
 *  void DeleteMin( )             --> Remove smallest item <br>
 *  void DeleteMin( Etype& X )    --> Same, but put it in X <br>
 *  int IsEmpty( )                --> Return 1 if empty; else return 0 <br>
 *  int IsFull( )                 --> Return 1 if full; else return 0 <br>
 *  void MakeEmpty( )             --> Remove all items <br>
 *  ******************ERRORS***************************** <br>
 *  Predefined exception is propagated if new fails. <br>
 *  EXCEPTION is called for FindMin or DeleteMin when empty. <br>
 */
template <class Etype>
class AbsPrQueue
{
  public:

  /** @name constructors  */ 
  //@{
/// empty constructor.
    AbsPrQueue( ) { }

/// destructor.
    virtual ~AbsPrQueue( ) { }
  //@}

/// adds an item into priority queue.
    virtual void Insert( const Etype& X ) = 0;

/// deletes minimum item in priority queue.
    virtual void DeleteMin( ) = 0;

/// deletes minimum item in priority queue, which is returned in X.
    virtual void DeleteMin( Etype& X ) = 0;

/// returns minimum item in priority queue.
    virtual const Etype& FindMin( ) = 0;

/// returns whether this priority queue is empty.
    virtual int IsEmpty( ) const = 0;

/// returns whether this priority queue is full.
    virtual int IsFull( ) const = 0;

/// makes this priority queue empty.
    virtual void MakeEmpty( ) = 0;

  private:

/// disable copy constructor.
    AbsPrQueue( const AbsPrQueue & );
};
#endif
