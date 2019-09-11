/*
   utlAllocator.h

   Standard allocator.
	
  Program:   utl
  Module:    $RCSfile: utlAllocator.h,v $
  Language:  C++
  Date:      $Date: 2005/10/09 22:40:44 $
  Version:   $Revision: 1.2 $

   Date: 04 February 2001.
*/

#ifndef __UTLALLOCATOR_H__
#define __UTLALLOCATOR_H__

#include "utl.h"

/// forward declaration of utlAllocator.
template <class T> class utlAllocator;

/// ultra-simplified specialization of utlAllocator.
template <> class utlAllocator<void> {
public:

/// universal pointer type.
typedef void* pointer;
/// universal const_pointer type.
typedef const void* const_pointer;

// no reference.

/// value type.
typedef void value_type;
/// template constructor.
template <class U>
struct rebind { typedef utlAllocator<U> other; };
};

/** A simpler implementation of STL allocator.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 *	allocators encapsulate allocation and deallocation of memory. 
 *	They provide a low-level interface that permits efficient allocation of many small objects;
 *	different allocator types represent different schemes for memory management. 
 *
 *	Note that allocators simply allocate and deallocate memory, as opposed to 
 *	creating and destroying objects. The STL also includes several low-level
 *	algorithms for manipulating uninitialized memory. 
 *
 *	Note also that allocators do not attempt to encapsulate multiple memory models. 
 *	The C++ language only defines a single memory model (the difference of
 *	two pointers, for example, is always ptrdiff_t), and this memory model is the 
 *	only one that allocators support. This is a major change from the
 *	definition of allocators in the original STL.  
 */
 template <class T> class utlAllocator {
 public:

  /// value type.
  typedef T value_type;

  /// pointer type.
  typedef T* pointer;

  /// const pointer type.
  typedef const T* const_pointer;

  /// reference type.
  typedef T& reference;

  /// const_reference type.
  typedef const T& const_reference;

  /// size type.
  typedef size_t size_type;

  /// pointer difference type.
  typedef ptrdiff_t difference_type;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlAllocator() throw() {}

  /// template constructor.
  template <class U> 
  utlAllocator (const utlAllocator<U>&) throw() {}

  /// destructor.
  ~utlAllocator() throw() {}
  //@}

  /// returns a pointer from a reference. 
  pointer address (reference r) { return &r; }

  /// returns a const_pointer from a const_reference.
  const_pointer address (const_reference r) const { return &r; }

  /** allocates space for n Ts.
   *
   *  The optional hint argument is completely implementation-dependent
   *  It is intended as a help to allocators for systems where locality
   *  is important.
   */
  pointer allocate (size_type n, utlAllocator<void>::const_pointer hint = NULL) {
#ifdef __RAW_MEM__
     return ( n > 0 ? (pointer) malloc (n*sizeof(T)) : NULL ); 
#else
     if ( n == 1 )
          return new T;
     else if ( n > 0 )
          return new T[n]; 
     return NULL;
#endif
  }

  /// deallocates space from n Ts.
  void deallocate (pointer p, size_type n) {
#ifdef __RAW_MEM__
       if ( p ) free ( p );
#else
       if ( p ) {
          if ( n == 1 )
             delete p;
          else if ( n > 0 ) 
             delete[] p;
       }
#endif
  }

 /** initializes *p by val.
  *
  *  In C++, the operator new allocates memory for an object and then 
  *  creates an object at that location by calling a constructor. 
  *  Occasionally, however, it is useful to separate those two operations. 
  *  If p is a pointer to memory that has been allocated but not initialized, 
  *  then construct(p, value) creates an object of type T at the location pointed to by p. 
  *  The argument value is passed as an argument to T's constructor. 
  */
  void construct (pointer p, const T& val) { 
        // *p = val;
	new(p) T(val); 
  }

 /** destroys *p but do not deallocate.
  * 
  *  In C++, the operator delete destroys an object by calling its destructor, 
  *  and then deallocates the memory where that object was stored. Occasionally,
  *  however, it is useful to separate those two operations.  
  *  Destroy calls an object's destructor without deallocating the memory 
  *  where the object was stored. 
  */
  void destroy (pointer p) { p->~T(); }

  /// returns maximum chunk of memory that can be allocated.
  size_type max_size() const { return 0; }

  /** in effect: typedef utlAllocator<U> other.
   *
   *  Provided to allow an allocator to allocate objects of arbitrary type.
   */
  template <class U>
  struct rebind { typedef utlAllocator<U> other; };

  /// comparison operator.
  friend bool operator == (const utlAllocator<T>&, const utlAllocator<T>&) throw() { return false; }

  /// comparison operator.
  friend bool operator != (const utlAllocator<T>&, const utlAllocator<T>&) throw() { return false; }

  /// comparison operator.
  friend bool operator > (const utlAllocator<T>&, const utlAllocator<T>&) throw() { return false; }

  /// comparison operator.
  friend bool operator < (const utlAllocator<T>&, const utlAllocator<T>&) throw() { return false; }

};
  
#endif

