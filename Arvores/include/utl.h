/*
**      utl.h: general template functions for utl.
**
**	purpose: to provide adequate algorithms for the STL classes.
**	author: Paulo Roma cavalcanti.
**

   Program:   util 
   Module:    $RCSfile: utl.h,v $
   Language:  C++
   Date:      $Date: 2003/10/31 23:10:11 $
   Version:   $Revision: 1.1.1.1 $
*/

#ifndef __UTL_H__
#define __UTL_H__

#include "utlConfig.h"

#ifdef MAX
#undef MAX
#endif
#ifdef MIN
#undef MIN
#endif

/**	returns the maximum of two numbers. 
 *	
 *	@param a first number.
 *	@param b second number.
 *	@return a or b, denpending on which is the greatest. 
 */
template <class T> 
inline T MAX ( const T& a, const T& b ) { 
    return ( a > b ? a : b ); 
}


/**	returns the minimum of two numbers. 
 *	
 *	@param a first number.
 *	@param b second number.
 *	@return a or b, denpending on which is the smallest. 
 */
template <class T> 
inline T MIN ( const T& a, const T& b ) { 
    return ( a < b ? a : b ); 
}


/** 	returns a given integer with its i-th bit set.
 *	The bit index is assumed to be in the range [0..sizeof(int)-1].
 *	
 *	@param word given integer.
 *	@param bit index of the bit to be set.
 *	@return set integer.
 */
template <class T> 
inline T utl_set_bit ( const T& word, unsigned int bit )
{
  return (word | (1 << bit));
}


/** 	returns a given integer with its i-th bit reset to 0.
 *	The bit index is assumed to be in the range [0..sizeof(int)-1].
 *	
 *	@param word given integer.
 *	@param bit index of the bit to be reset.
 *	@return set integer.
 */
template <class T> 
inline T utl_reset_bit ( const T& word, unsigned int bit )
{
  return (word & ~(1 << bit));
}


/** 	returns whether the i-th bit in a given integer is set.
 *	The bit index is assumed to be in the range [0..sizeof(int)-1].
 *	
 *	@param word given integer.
 *	@param bit index of the bit to be tested.
 *	@return 1 if the given bit is set, and 0 otherwise.
 */
template <class T> 
inline int utl_get_bit ( const T& word, unsigned int bit )
{
  return (word & (1 << bit));
}


/**
 *	reallocates an array. The original array is 
 *	deleted and a new array containing the same
 *	data of the original one is returned.
 *	
 *	@param n1 old size.
 *	@param v1 given array.
 *	@param n2 new size.
 *	@return a pointer to a new array.
*/
template <class T> 
inline T* utl_realloc ( int n1, T* v1, int n2 )
{
 T* ptr = new T [n2];	// allocate the new size

 if ( v1 != NULL )
    {
     if ( n2 < n1 ) n1 = n2;  // PRC - 26/12/00
     for (int i = 0; i < n1; i++ ) ptr[i] = v1[i];
     delete[] v1;
    }
 return ptr;
}


/**     merges two sorted linked lists ended by NULL.
 *	
 *	@param  root a pointer to the first element of the first list.  
 *	@param  lt a pointer to the last element of the first list.  
 *	@param  r a pointer to the first element of the second list.  
 *	@param  tail a pointer to the last element of the second list.  
 *	@see utl_sort().
 */
template <class Name> 
inline void utl_merge ( 
	Name **root,    // a pointer to the first element of the first list  (in/out)
	Name *lt, 	// a pointer to the last element of the first list   (in)
	Name *r, 	// a pointer to the first element of the second list (in)
	Name **tail )	// a pointer to the last element of the second list  (in/out)
{
 Name header; 				// header of the merged list
 Name* l = *root, *m=&header;	

 while ( l != NULL && r != NULL )
   {
    int t = l->compare ( r );
    if ( t <= 0 /* l <= r*/ ) 		// l goes to the merged list
       { 
	m = l->insertAfter ( m );	// inserts after m in the merged list 
	l = l->getNext( ); 		// advance to the next element of l list
       }
    if ( t >= 0 /* r <= l */ ) 		// r goes to the merged list
       {
	m = r->insertAfter( m );
	r = r->getNext ( );		// Note: if r == l both must go to the merged list 
       }
   }

 if ( l != NULL )			// append the rest of l list to merged list
    {
     m = l->insertAfter ( m );
     *tail = lt;
    }
 else if ( r != NULL ) 			// append the rest of r list to merged list 
    {
     m = r->insertAfter ( m );
    }

 header.getNext()->setPrev ( NULL ); 
 *root = header.getNext( );
}


/**     sorts a linked list, ended by NULL, by using mergesort.
 *	
 *	@param root a pointer to the first element of the list.
 *	@param tail a pointer to the last element of the list.
 *	@see utl_merge().
 */
template <class Name> 
inline void utl_sort (	
	Name **root, 	// a pointer to the first element of the list  	(in/out) 
	Name **tail )	// a pointer to the last element of the list 	(in/out)
{
 Name *p, *m;

 m = p = *root;
 if ( p == NULL || p->getNext( ) == NULL ) return;	// zero or one length 
 for (;;)						// find middle
   {
    p = p->getNext( ); if ( p == NULL ) break;
    p = p->getNext( ); if ( p == NULL ) break;
    m = m->getNext( );
   }
 p = m->getNext ( );
 m->setNext ( NULL ); 

 utl_sort ( root, &m );				// recursive call for the first half
 utl_sort ( &p, tail );				// recursive call for the second half
 utl_merge ( root, m, p, tail ); 		// mergesort 
}

#endif // __UTL_H__


