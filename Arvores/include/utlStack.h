/*
   utlStack.h

   An adaptor implementing a stack from a given container.
	
  Program:   utl
  Module:    $RCSfile: utlStack.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 15 January 2001.
*/

#ifndef __UTLSTACK_H__
#define __UTLSTACK_H__

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#define utlStack stack
#else

#include "utlVector.h"
#include "utlList.h"

/** A simpler implementation of STL class stack.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	A stack is an adaptor that provides a restricted subset of Container functionality: 
 *	it provides insertion, removal, and inspection of the element at the top of the
 *	stack. Stack is a "last in first out" (LIFO) data structure: 
 *	the element at the top of a stack is the one that was most recently added. Stack does not
 *	allow iteration through its elements.  
 *
 *	Stack is a container adaptor, meaning that it is implemented on top of some 
 *	underlying container type. By default that underlying type is deque, but a
 *	different type may be selected explicitly. 
 */
template <class T, class C = utlVector<T> > class utlStack {

 protected:

 /// container to hold the stack.
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
 explicit utlStack ( const C& a = C() ) : c(a) {}

 //@}

 /// returns whether the stack is empty.
 bool empty () const { return c.empty(); }

 /// returns the size of the stack.
 size_type size () const { return c.size(); }

 /// returns the element at the top of the stack for writing.
 value_type& top () { return c.back(); }

 /// returns the element at the top of the stack for reading.
 const value_type& top () const { return c.back(); }

 /// pushes an element onto the stack.
 void push ( const value_type& x ) { return c.push_back(x); }

 /// pops the element at the top of the stack.
 void pop () { c.pop_back(); }

 /**	assignment operator.
  *	
  *	@return a reference to this utlStack.
  */
 utlStack<T,C>& operator = ( const utlStack<T,C>& rhs ) {
   c = rhs.c;
   return *this;
 }

 /**	comparison operator. Tests two stacks for equality.
  *	
  *	@return s1 == s2.
  */
 friend bool operator == ( const utlStack<T,C>& s1, const utlStack<T,C>& s2 ) {
   return s1.c == s2.c;
 } 

 /**	comparison operator. Tests two stacks for inequality.
  *	
  *	@return s1 != s2.
  */
 friend bool operator != ( const utlStack<T,C>& s1, const utlStack<T,C>& s2 ) {
   return !(s1.c == s2.c);
 }

 /**	lexicographical comparison. 
  *	
  *	@return s1 < s2.
  */
 friend bool operator < ( const utlStack<T,C>& s1, const utlStack<T,C>& s2 ) {
   return s1.c < s2.c;
 }
};

#endif
#endif
