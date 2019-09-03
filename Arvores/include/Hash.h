/*
   Hash.h

   Hash table.

  Program:   weiss
  Module:    $RCSfile: Hash.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 26 November 2000.
*/
#ifndef __HashTable
#define __HashTable

#include "AbsHash.h"
#include "utlConfig.h"

/** HashTable class interface.
 *
 *  Uses open address and quadratic probing.
 *  Quadratic probing is a collision resolution method that eliminates
 *  the primary clustering problem of linear probing. Quadratic probing
 *  derives its name from the use of the formula F(i) = i**2 to 
 *  resolve collisions.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *
 *  Etype: must have zero-parameter and copy constructor,
 *     operator= and operator!=  <br>
 *  CONSTRUCTION: with (a) no initializer; <br>
 *  Copy construction of HashTable objects is DISALLOWED. <br>
 *  Deep copy is supported. <br>
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *  int Insert( const Etype& X )  --> Insert X <br>
 *  int Remove( const Etype& X )  --> Remove X <br>
 *  Etype Find( const Etype& X )  --> Return item that matches X <br>
 *  int WasFound( )        	  --> Return 1 if last Find succeeded <br>
 *  int IsFound( const Etype& X ) --> Return 1 if X would be found <br>
 *  int IsEmpty( )         	  --> Return 1 if empty; else return 0 <br>
 *  int IsFull( )          	  --> Return 1 if full; else return 0 <br>
 *  void MakeEmpty( )      	  --> Remove all items <br>
 *  ******************ERRORS******************************** <br>
 *  Predefined exception is propagated if new fails. <br>
 */

template <class Etype>
class HashTable : public AbsHTable<Etype>
{
  public:
/// status of a hash table entry.
    enum KindOfEntry { 
	Active, 
	Empty, 
	Deleted 
    };

  /** @name constructors  */ 
  //@{
    /// empty constructor. 
    HashTable( ) {
        ArraySize = DefaultSize;
    	AllocateArray( );
    	CurrentSize = 0;
    }

    /// destructor.
    virtual ~HashTable( ) { delete [ ] Array; }
  //@}

/// copy operator (deep).
    const HashTable& operator=( const HashTable & Rhs ) {
    	if( this != &Rhs )
    	  {
           delete [ ] Array;
           ArraySize = Rhs.ArraySize;
           AllocateArray( );
           for( int i = 0; i < ArraySize; i++ )
               Array[ i ] = Rhs.Array[ i ];
           CurrentSize = Rhs.CurrentSize;
          }

    	return *this;
    } 

/** inserts an item into hash table.
 *  Rehash automatically as needed.
 *
 *  @param X item to be inserted.
 *  @return false if X is a duplicate and true otherwise.
 */
    virtual int Insert( const Etype& X ) {
    	unsigned int CurrentPos = FindPos( X );

        // Don't insert duplicates
    	if( Array[ CurrentPos ].Info == Active )
           return 0;

        // Insert X as active
    	Array[ CurrentPos ] = HashEntry( X, Active );
    	if( ++CurrentSize  < ArraySize / 2 )
           return 1;

#if 1
        // REHASHING CODE
        // Save old table
    	HashEntry* OldArray = Array;
    	int OldArraySize = ArraySize;

        // Create a new double-sized, empty table
    	CurrentSize = 0;
    	ArraySize = NextPrime( 2 * OldArraySize );
    	AllocateArray( );

        // Copy table over
    	for( int i = 0; i < OldArraySize; i++ )
           if( OldArray[ i ].Info == Active )
               Insert( OldArray[ i ].Element );

        // Recycle OldArray
    	delete [ ] OldArray;
#endif
    	return 1;
    }

/** removes an item from hash table.
 *
 *  @param X item to be removed.
 *  @return true if X was removed, ans false otherwise.
 */
    virtual int Remove( const Etype& X )        {
    	unsigned int CurrentPos = FindPos( X );

    	if( Array[ CurrentPos ].Info != Active )
            return 0;

    	Array[ CurrentPos ].Info = Deleted;
    	return 1;
    }

/** returns item in hash table that matches X.
 *  Success can be tested by WasFound.
 *
 *  @param X element to be searched for.
 *  @return found item.
 */
    virtual const Etype& Find( const Etype& X ) {
    	unsigned int CurrentPos = FindPos( X );
    	LastFindOK = Array[ CurrentPos ].Info == Active;
    	return Array[ CurrentPos ].Element;
    }

/** returns whether X is in the hash table.
 *
 *  @param X element to be searched for.
 *  @return 1 if found and 0 otherwise.
 */
    virtual int IsFound( const Etype& X ) const {
    	unsigned int CurrentPos = FindPos( X );
    	return Array[ CurrentPos ].Info == Active;
    }

/// returns true if last Find() operation was successful.
    virtual int WasFound( ) const { return LastFindOK; }

/** phony IsEmpty routine.
 *  Does not work with deletions.
 */
    virtual int IsEmpty( ) const {
    	return CurrentSize == 0;   
    }

/** returns whether this hash table is full.
 *
 *  @return 1 if full, and 0 otherwise.
 */
    virtual int IsFull( ) const { return 0; }    

/// clears the hash table.
    virtual void MakeEmpty( ) {
    	CurrentSize = 0;
    	for( int i = 0; i < ArraySize; i++ )
           Array[ i ].Info = Empty;
    }

  /** 	output operator. Prints this hash table. 
   *	
   *	@param os file stream.
   *	@param ht given hash table.
   *	@return file stream. 
   */
  friend ostream& operator<<(ostream& os, const HashTable<Etype>& ht) {
    os << "\nHash Table Contents (size = " << ht.ArraySize << "):\n\n";
    if ( ht.IsEmpty() )
      os << "Empty Hash Table" << endl;
    else {
      for ( int i=0; i < ht.ArraySize; ++i) {
        if ( ht.Array[i].Info == Active )
	    os << "pos = " << i << " value = " << ht.Array[i]; 
      }
    }

    return os; 
  }

  private:

/// holds the structure of an entry into the hash table.
    struct HashEntry
    {
    /// the item.
        Etype Element;         
    /// active, empty, or deleted.
        KindOfEntry Info;      

  /** @name constructors  */ 
  //@{
        /// empty constructor.
        HashEntry( ) : Info( HashTable<Etype>::Empty ) { }

        /// constructor form element and entry type.
        HashEntry( const Etype& E, KindOfEntry i = Empty ) :
            Element( E ), Info( i ) { }
  //@}
	/// output operator.
        friend ostream& operator<<(ostream& os, const HashEntry& he) {
            os << he.Element;
            return os;
        }
    };

/// initial hash table size.
    enum { 
	/// must be a prime number.
	DefaultSize = 11 
    };

/// the size of this array.
    int ArraySize;       
/// the number of non-empty items.
    int CurrentSize;     
/// true if last search was successful.
    int LastFindOK;      
/// the array of elements.
    HashEntry *Array;    

    // Some internal routines

/// allocates the hash table array.
    void AllocateArray( ) {
    	Array = new HashEntry[ ArraySize ];
    }

/** resolves collisions and locates
 *  cell that must contain X if X is found.
 *
 *  @param X element to be searched for.
 *  @return X position.
 */
    unsigned int FindPos( const Etype& X ) const {
    	unsigned int i = 0;   // The number of failed probes
    	unsigned int InitialPos = Hash( X, ArraySize );
        unsigned int CurrentPos = InitialPos;

    	while( Array[ CurrentPos ].Info != Empty &&
               Array[ CurrentPos ].Element != X )
    	   {
#if 1
            CurrentPos += 2 * ++i - 1;    // Compute ith probe
            if( CurrentPos >= (unsigned int)ArraySize ) // Implement the mod
                CurrentPos -= ArraySize;
#else
            ++i;
            CurrentPos = InitialPos + i + 3*i*i;
            CurrentPos %= ArraySize;
#endif
    	   }

    	return CurrentPos;
    }

/// finds next prime > N. Assumes N >= 5.
    int NextPrime( int N ) {
    	if( N % 2 == 0 )
           N++;

        int i;
    	for( ; ; N += 2 )
           {
            for( i = 3; i * i <= N; i += 2 )
                if( N % i == 0 )
                   break;
            if( i * i > N )
                return N;
           }
    }

/// hash function.
    unsigned int Hash ( const Etype& X, int size ) const {
     	return X.hash (size);
    }

};
#endif
