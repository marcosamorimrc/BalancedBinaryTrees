/*
   utlQueue.h

   An adaptor implementing a queue from a given container.
	
  Program:   utl
  Module:    $RCSfile: utlQueue.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 15 January 2001.
*/

#ifndef __UTLQUEUE_H__
#define __UTLQUEUE_H__

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#define utlQueue queue
#else

#include "utlList.h"
#include "utlDeque.h"

/** A simpler implementation of STL class queue.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 * 
 *	A queue is an adaptor that provides a restricted subset of Container functionality.
 *	A queue is a "first in first out" (FIFO) data structure. That is, elements
 *	are added to the back of the queue and may be removed from the front; 
 *	Q.front() is the element that was added to the queue least recently. 
 *	Queue does not allow iteration through its elements.  
 *
 *	Queue is a container adaptor, meaning that it is implemented on top of some 
 *	underlying container type. By default that underlying type is deque, but a
 *	different type may be selected explicitly. 
 */
template <class T, class C = utlDeque<T> > class utlQueue {

 protected:

 /// container to hold the queue.
 C c;

 public:
 
 /// value type.
 typedef typename C::value_type value_type;

 /// size type.
 typedef typename C::size_type size_type;

 /// underlying container type.
 typedef C container_type;

 /** @name constructors  */ 
 //@{

 /// constructor from a container.
 explicit utlQueue ( const C& a = C() ) : c(a) {}

 //@}

 /// returns whether the queue is empty.
 bool empty () const { return c.empty(); }

 /// returns the size of the queue.
 size_type size () const { return c.size(); }

 /// returns the element at the front of the queue for writing.
 value_type& front () { return c.front(); }

 /// returns the element at the front of the queue for reading.
 const value_type& front () const { return c.front(); }

 /// returns the element at the back of the queue for writing.
 value_type& back () { return c.back(); }

 /// returns the element at the back of the queue for reading.
 const value_type& back () const { return c.back(); }

 /// pushes an element at the back of the queue.
 void push ( const value_type& x ) { return c.push_back(x); }

 /// pops the element at the front of the queue.
 void pop () { c.pop_front(); }

 /**	assignment operator.
  *	
  *	@return a reference to this utlQueue.
  */
 utlQueue<T,C>& operator = ( const utlQueue<T,C>& rhs ) {
   c = rhs.c;
   return *this;
 }

 /**	comparison operator. Tests two queues for equality.
  *	
  *	@return q1 == q2.
  */
 friend bool operator == ( const utlQueue<T,C>& q1, const utlQueue<T,C>& q2 ) {
   return q1.c == q2.c;
 } 

 /**	comparison operator. Tests two queues for inequality.
  *	
  *	@return q1 != q2.
  */
 friend bool operator != ( const utlQueue<T,C>& q1, const utlQueue<T,C>& q2 ) {
   return !(q1.c == q2.c);
 }

 /**	lexicographical comparison. 
  *	
  *	@return q1 < q2.
  */
 friend bool operator < ( const utlQueue<T,C>& q1, const utlQueue<T,C>& q2 ) {
   return q1.c < q2.c;
 }

};

#endif
#endif
