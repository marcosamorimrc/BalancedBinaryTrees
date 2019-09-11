/*
   utlPriority_queue.h

   An adaptor implementing a priority_queue from a given container.
	
  Program:   utl
  Module:    $RCSfile: utlPriorityQueue.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 16 January 2001.
*/

#ifndef __UTLPRQUEUE_H__
#define __UTLPRQUEUE_H__

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#include <queue>
#define utlPriority_queue priority_queue
#else

#include "utlVector.h"

/** A simpler implementation of STL class priority_queue.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	A priority_queue is an adaptor that provides a restricted subset of 
 *	Container functionality: it provides insertion of elements, 
 *	and inspection and removal of the top element. 
 *	It is guaranteed that the top element is the largest element in the priority_queue, 
 *	where the function object Cmp is used for comparisons.  
 *	Priority_queue does not allow iteration through its elements.  
 *
 *	This restriction is the only reason for priority_queue to exist at all. 
 *	If iteration through elements is important, you can either use a vector that is
 * 	maintained in sorted order, or a set, or a vector that is maintained as a heap 
 *	using make_heap, push_heap, and pop_heap. Priority_queue is,
 *	in fact, implemented as a random access container that is maintained as a heap. 
 *	The only reason to use the container adaptor priority_queue, instead of
 *	performing the heap operations manually, is to make it clear that you are 
 *	never performing any operations that might violate the heap invariant. 
 *
 *	Priority_queue is a container adaptor, meaning that it is implemented on top 
 *	of some underlying container type. By default that underlying type is
 *	vector, but a different type may be selected explicitly. 
 */
template <class T, class C = utlVector<T>, class Cmp = compare_to <typename C::value_type> > 
class utlPriority_queue {

 public:
 
 /// value type.
 typedef typename C::value_type value_type;

 /// size type.
 typedef typename C::size_type size_type;

 /// underlying container type.
 typedef C container_type;

 /** @name constructors  */ 
 //@{

 /// empty constructor.
 explicit utlPriority_queue ( const C& a2 = C() ) : c(a2), cmp(Cmp()) {}

 /// constructor from a comparison function object.
 utlPriority_queue ( const Cmp& comp ) : c(C()), cmp(comp), SI(comp) {}

 /** creates a priority_queue initialized to contain the elements in the
  *  range [first, last), and using Cmp() as the comparison function. 
  */
 utlPriority_queue(const value_type* first, const value_type* last) : c(C()), cmp(Cmp()) {
     while ( first != last ) c.push_back(*first++);
     SI.mheap ( &c[0], c.size()-1 ); 
 }

 /** creates a priority_queue initialized to contain the elements in the
  *  range [first, last), and using comp as the comparison function. 
  */
 utlPriority_queue(const value_type* first, const value_type* last,
                                            const Cmp& comp) : c(C()), cmp(comp), SI(comp) {
     while ( first != last ) c.push_back(*first++);
     SI.mheap ( &c[0], c.size()-1 ); 
 }

 /// copy constructor.
 utlPriority_queue ( const utlPriority_queue& pr ) { *this = pr; }

 //@}

 /// returns whether the priority_queue is empty.
 bool empty () const { return c.empty(); }

 /// returns the size of the priority_queue.
 size_type size () const { return c.size(); }

 /** 	returns a const reference to the element at the top of the priority_queue. 
  *	The element at the top is guaranteed to be the largest element in 
  *	the priority queue, as determined by the comparison function Cmp. 
  *	That is, for every other element x in the priority_queue, 
  *	Cmp(Q.top(), x) is false. Precondition: empty() is false. 
  */
 const value_type& top () const { return c.front(); }

 /// pushes an element onto the priority_queue.
 void push ( const value_type& x ) { 
   c.push_back ( x );
   SI.mheap ( &c[0], c.size()-1 ); 
 }

 /**	removes the element at the top of the priority_queue, that is, 
  *	the largest element in the priority_queue. 
  *	Precondition: empty() is false. Postcondition: size() will be 
  *	decremented by 1. 
  */
 void pop () { 
   if ( c.empty() ) return;
   value_type temp = c.front();
   c.front() = c.back();
   c.back() = temp;
   SI.adjust ( &c[0], 0, c.size()-2 );
   c.pop_back ( );
 }

 /**	assignment operator.
  *	
  *	@return a reference to this utlPriority_queue.
  */
 utlPriority_queue<T,C,Cmp>& operator = ( const utlPriority_queue<T,C,Cmp>& rhs ) {
   if ( &rhs != this ) {
     c = rhs.c;
     cmp = rhs.cmp;
     SI = rhs.SI;
   }
   return *this;
 }

 protected:

 /// container to hold the priority_queue.
 C c;

 /// comparion function object.
 Cmp cmp;

 /// sorter object.
 utlSorter<value_type,Cmp> SI;

};

#endif
#endif
