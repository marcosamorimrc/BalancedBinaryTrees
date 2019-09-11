/*
   AbsHash.h

   Hash table.

  Program:   weiss
  Module:    $RCSfile: AbsHash.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 26 November 2000.
*/
#ifndef __AbsHash
#define __AbsHash

/** HashTable abstract class interface.
 *
 *  Etype: must have zero-parameter constructor and
 *     operator==; implementation may require operator!=;
 *     implementation will require either. <br>
 *     operator= or copy constructor, perhaps both. <br>
 *  CONSTRUCTION: with (a) no initializer; <br>
 *     copy constructor of HashTable objects is DISALLOWED. <br>
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *     All of the following are pure virtual functions. <br>
 *  int Insert( const Etype& X )  --> Insert X <br>
 *  int Remove( const Etype& X )  --> Remove X <br>
 *  Etype Find( const Etype& X )  --> Return item that matches X <br>
 *  int WasFound( )        	  --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br>
 *  int IsEmpty( )         	  --> Return 1 if empty; else return 0 <br>
 *  int IsFull( )          	  --> Return 1 if full; else return 0 <br>
 *  void MakeEmpty( )      	  --> Remove all items <br>
 */

template <class Etype>
class AbsHTable
{
  public:

  /** @name constructors  */ 
  //@{
    /// empty constructor. 
    AbsHTable( ) { }
    /// destructor.
    virtual ~AbsHTable( ) { }
  //@} 

/// inserts a given element into the hash table.    
    virtual int Insert( const Etype& X ) = 0;
/// removes a given element from the hash table.    
    virtual int Remove( const Etype& X ) = 0;
/// finds a given element into the hash table.    
    virtual const Etype & Find( const Etype& X ) = 0;
/// returns whether the last Find() was successful.
    virtual int WasFound( ) const = 0;
/// returns whether a given element is into the hash table.
    virtual int IsFound( const Etype& X ) const = 0;
/// returns whether this hash table is empty.
    virtual int IsEmpty( ) const = 0;
/// returns whether this hash table is full.
    virtual int IsFull( ) const = 0;
/// makes this hash table empty.
    virtual void MakeEmpty( ) = 0;

  private:
     
    /// no copy constructor.   
    AbsHTable( const AbsHTable& ) { }
};
#endif
