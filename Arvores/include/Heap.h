/*
   Heap.h

   Binary heaps.

  Program:   weiss
  Module:    $RCSfile: Heap.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 29 November 2000.
*/
#ifndef __BinaryHeap
#define __BinaryHeap

#include "Exception.h"
#include "AbsPrQue.h"

/** BinaryHeap class interface.
 *
 *  The priority queue is a fundamental data structure that
 *  allows access to only the minimum item. The binary heap
 *  supports insertion of new items and deletion of the minimum 
 *  item in logarithmic worst-case time.
 *  In a heap, for every node X with parent P, the key in P
 *  is smaller than or equal to the key in X.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *  Etype: must have zero-parameter constructor and operator=; <br>
 *     must have operator< <br>
 *  CONSTRUCTION: with (a) Etype representing negative infinity <br>
 *  Copy construction of BinaryHeap objects is DISALLOWED. <br>
 *  Deep copy is supported. <br>
 *
 *  ******************PUBLIC OPERATIONS****************** <br>
 *  void Insert( const Etype& X ) --> Insert X <br>
 *  void Toss( const Etype& X )   --> Insert X (lazily) <br>
 *  const Etype& FindMin( )       --> Return smallest item <br>
 *  void DeleteMin( )             --> Remove smallest item <br>
 *  void DeleteMin( Etype& X )    --> Same, but put it in X <br>
 *  int IsEmpty( )                --> Return 1 if empty; else return 0 <br>
 *  int IsFull( )                 --> Return 1 if full; else return 0 <br>
 *  void MakeEmpty( )             --> Remove all items <br>
 *  void FixHeap( )               --> Reestablish heap order property <br>
 *  ******************ERRORS***************************** <br>
 *  Predefined exception is propagated if new fails. <br>
 *  EXCEPTION is called for FindMin or DeleteMin when empty. <br>
 */

template <class Etype>
class BinaryHeap : public AbsPrQueue<Etype>
{
  public:

  /** @name constructors  */ 
  //@{
/// constructor from a negative sentinel.
    BinaryHeap( const Etype& MinVal ):
        MaxSize( DefaultSize ), CurrentSize( 0 ), OrderOK( 1 ) {
    	GetArray( MaxSize );
    	Array[ 0 ] = MinVal;
    }

/// destructor. 
    virtual ~BinaryHeap( ) { delete [ ] Array; }
  //@}

/// copy assignment.
    const BinaryHeap& operator=( const BinaryHeap& Rhs ) {
   	if ( this != &Rhs ) {
            this->MakeEmpty();
            for ( int i = 0; i < Rhs.CurrentSize; ++i )
                  this->Toss ( Rhs.Array[i] );
        }
        return *this;
    }

/// adds an item maintaining heap order.
    virtual void Insert( const Etype& X ) {
    	CheckSize( );
    	Array[ ++CurrentSize ] = X;
    	if( X < Array[ CurrentSize / 2 ] )
           OrderOK = 0;
    }

/// adds an item but does not maintain heap order.
    void Toss( const Etype& X ) {
    	CheckSize( );
    	Array[ ++CurrentSize ] = X;
    	if( X < Array[ CurrentSize / 2 ] )
            OrderOK = 0;
    }

/** returns minimum item in the heap.
 *  Calls FixHeap() first if necessary.
 */
    virtual const Etype& FindMin( ) {
    	EXCEPTION( IsEmpty( ), "Binary heap is empty" );

    	if( OrderOK == 0 )
            FixHeap( );
    	return Array[ 1 ];
    }

/// deletes minimum item in heap.
    virtual void DeleteMin( ) {
    	Etype X;
    	DeleteMin( X );
    }

/** deletes the minimum item; throw it away.
 *  NOTE: It would be better to write an additional
 *  private member to consolidate the common work between
 *  the two forms of DeleteMin.
 */
    virtual void DeleteMin( Etype& X ) {
    	X = FindMin( );
    	Array[ 1 ] = Array[ CurrentSize-- ];
    	PercolateDown( 1 );
    }

/// reestablishes heap order (linear-time function).
    void FixHeap( ) {
    	for( int i = CurrentSize / 2; i > 0; i-- )
             PercolateDown( i );
    	OrderOK = 1;
    }

/// returns whether this heap is empty.
    virtual int IsEmpty( ) const { return CurrentSize == 0; }

/// returns whether this heap is full.
    virtual int IsFull( ) const  { return 0; }

/// makes this heap empty.
    virtual void MakeEmpty( )    { CurrentSize = 0; }

/// output operator.
    friend ostream& operator<<(ostream& os, const BinaryHeap<Etype>& hp) {
        int n = 0;
	for ( int i = 1; i <= hp.CurrentSize; ++i )
            {
      	     os << hp.Array[i] << " ";
             if ( !(++n%20) ) os << "\n";
            }
	os << endl;
    	return os;
    }
  private:

/// default array size.
    enum { DefaultSize = 10 };

/// number of elements that can be stored.
    int MaxSize;         
/// number of elements currently stored.
    int CurrentSize;     
/// zero if heap order is not guaranteed.
    int OrderOK;         
/// dynamically allocated array.
    Etype* Array;        

/// disable copy constructor.
    BinaryHeap( const BinaryHeap & ); 

// Internal routines

/// private member to percolate down in the heap.
    void PercolateDown( int Hole ) {
    	int Child;
    	Etype Tmp = Array[ Hole ];

    	for( ; Hole * 2 <= CurrentSize; Hole = Child )
           {
            Child = Hole * 2;
            if( Child != CurrentSize &&
                Array[ Child + 1 ] < Array[ Child ] )
            Child++;
            if( Array[ Child ] < Tmp )
                Array[ Hole ] = Array[ Child ];
            else
                break;
           }
    	Array[ Hole ] = Tmp;
    }

/// allocates the heap array.
    void GetArray( int NewMaxSize ) {
    	Array = new Etype [ NewMaxSize + 1 ];
    }

/** If heap is full, double heap array.
 *  Used for Toss() and Insert().
 */
    void CheckSize( )   {
    	if( CurrentSize == MaxSize )
          {
           Etype *Old = Array;
           GetArray( MaxSize * 2 );
           for( int i = 0; i <= MaxSize; i++ )
               Array[ i ] = Old[ i ];
           delete [ ] Old;
           MaxSize *= 2;
          }
    }
};
#endif
