/*
**	utlInserter.h - class for inserting elements.
**
**	purpose: to provide a way for inserting elements into a container.
**

  Program:   cgc util
  Module:    $RCSfile: utlInserter.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

**
*/

#ifndef __UTLINSERT_H__
#define __UTLINSERT_H__

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#define utl_insert_iterator insert_iterator
#else

#include "utlIteratorBase.h"

/** 	Insert iterator.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *
 *	Insert_iterator is an iterator adaptor that functions as an Output Iterator: 
 *	assignment through an insert_iterator inserts an object into a
 *	Container. Specifically, if ii is an insert_iterator, then ii keeps track of 
 *	a Container c and an insertion point p; the expression *ii = x performs
 *	the insertion c.insert(p, x). 
 *
 *	Note the difference between assignment through a Container::iterator and 
 *	assignment through an insert_iterator<Container>. If i is a valid Sequence::iterator, 
 *	then it points to some particular element in the container; 
 *	the expression *i = t replaces that element with t, and
 *	does not change the total number of elements in the container. 
 *	If ii is a valid insert_iterator<container>, however, then the expression *ii =
 *	t is equivalent, for some container c and some valid container::iterator j, 
 *	to the expression c.insert(j, t). That is, it does not
 *	overwrite any of c's elements and it does change c's size. 
 *
 *	There are two different Container concepts that define this expression: 
 *	Sequence, and Sorted Associative Container. Both concepts define insertion into a
 *	container by means of c.insert(p, x), but the semantics of this expression is very 
 *	different in the two cases. 
 *
 *	For a Sequence S, the expression S.insert(p, x) means to insert the value x 
 *	immediately before the iterator p. That is, the two-argument version of
 *	insert allows you to control the location at which the new element will be inserted. 
 *	For a Sorted Associative Container, however, no such control is
 *	possible: the elements in a Sorted Associative Container always appear in ascending 
 *	order of keys. Sorted Associative Containers define the two-argument
 *	version of insert as an optimization. The first argument is only a hint: 
 *	it points to the location where the search will begin. 
 *
 *	If you assign through an insert_iterator several times, 
 *	then you will be inserting several elements into the underlying container. 
 *	In the case of a Sequence, they will appear at a particular location in the 
 *	underlying sequence, in the order in which they were inserted: one of the arguments to
 *	insert_iterator's constructor is an iterator p, and the new range will be inserted 
 *	immediately before p. 
 *
 *	In the case of a Sorted Associative Container, however, the iterator in the 
 *	insert_iterator's constructor is almost irrelevant. The new elements will not
 *	necessarily form a contiguous range; they will appear in the appropriate location 
 *	in the container, in ascending order by key. The order in which they are
 *	inserted only affects efficiency: inserting an already-sorted range into a 
 *	Sorted Associative Container is an O(N) operation. 
 */
template <class Container>
class utl_insert_iterator : public utlIteratorBase<Container> {
 public:

  /// iterator category.
  typedef typename utlIteratorBase<utl_insert_iterator>::output_iterator_tag iterator_category;

  /** @name constructors  */ 
  //@{

  /** constructor from a container and iterator.
   *
   *  Constructs an insert_iterator that inserts objects in C. 
   *  If Container is a Sequence, then each object will be inserted 
   *  immediately before the element pointed to by i. 
   *  If C is a Sorted Associative Container, then the first insertion
   *  will use i as a hint for beginning the search. The iterator i must be a
   *  dereferenceable or past-the-end iterator in C.  
   *  Creates an utl_insert_iterator such that assignment of t 
   *  through it is equivalent to s << t. 
   */
  utl_insert_iterator ( Container& C, typename Container::iterator i ) : cntr(&C), itr(i) {}

  /// copy constructor.
  utl_insert_iterator ( const utl_insert_iterator& rhs ) {
    *this = rhs;
  }

  /// destructor.
  virtual ~utl_insert_iterator () { }
 
  //@}

  /// dereference operator. 
  utl_insert_iterator& operator*() { 
	return *this; 
  }

  /// assignment operator.
  utl_insert_iterator& operator = ( const utl_insert_iterator& rhs ) { 
      if ( this == &rhs ) return *this;
      this->cntr = rhs.cntr;
      this->itr = rhs.itr;
      return *this;
  }

  /** assignment operator. Used to implement the output iterator expression *i = x.  
   *
   *  Note how assignment through an insert_iterator is implemented. 
   *  In general, unary operator* must be defined so that it returns a proxy object,
   *  where the proxy object defines operator= to perform the insert operation. 
   *  In this case, for the sake of simplicity, the proxy object is the
   *  insert_iterator itself. That is, *i simply returns i, and *i = t is equivalent to i = t. 
   *  You should not, however, rely on this behavior. It is an
   *  implementation detail, and it is not guaranteed to remain the same in future versions. 
   */
  utl_insert_iterator& operator = ( const typename Container::value_type& x ) { 
      itr = cntr->insert(itr,x);
      ++itr;
      return *this;
  }

  /// prefix increment operator.
  utl_insert_iterator& operator++() {
    return *this;
  }

  /// posfix increment operator.
  utl_insert_iterator& operator++(int) {
    return *this;
  }


 protected:
  /// container.
  Container* cntr;
  /// iterator.
  typename Container::iterator itr;

};

namespace cgcUtil {

 ///  equivalent to utl_insert_iterator<Container>(C, i).  
 template<class Container, class Iter> 
 inline utl_insert_iterator<Container> inserter(Container& C, Iter i) {
    return utl_insert_iterator<Container> ( C, i );
 }

}

#endif
#endif
