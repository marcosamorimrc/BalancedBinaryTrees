/*
   utlReverseIterator.h

   Reverse iterator.
	
  Program:   utl
  Module:    $RCSfile: utlReverseIterator.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 06 January 2001.
*/

#ifndef __REVITER__
#define __REVITER__

#include "utlDefs.h"
#include "utlIteratorBase.h"

/** A simpler implementation of STL reverse_iterator class.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	A reverse_iterator is implemented using an iterator called current.
 *	That iterator can only point to the element of its sequence plus one-past-the-end 
 *	element. However, the reverse_iterator's one-past-the-end element is the original
 *	sequence's (inaccessible) one-before-the-beginning element. Thus, to avoid
 *	access violations, current points to the element after the one the
 *	reverse_iterator refers to. This implies that * returns the value *(current-1)
 *	and that ++ is implemented using -- on current.
 */
template <class Iter>
class utl_reverse_iterator {

  protected:
    /// points to the element after the one *this refers to.
    Iter current;

  public:
    /// this utl_reverse_iterator type.
    typedef Iter iterator_type;

    /// this utl_reverse_iterator category.
    typedef typename utlIteratorBase<utl_reverse_iterator>::random_access_iterator_tag iterator_category;
    //typedef cgcUtil::iterator_traits<Iter>::iterator_category iterator_category;

    /// this utl_reverse_iterator value type.
    typedef typename cgcUtil::iterator_traits<Iter>::value_type value_type;

    /// this utl_reverse_iterator difference type.
    typedef typename cgcUtil::iterator_traits<Iter>::difference_type difference_type;

    /// this utl_reverse_iterator pointer type.
    typedef typename cgcUtil::iterator_traits<Iter>::pointer pointer;

    /// this utl_reverse_iterator reference type.
    typedef typename cgcUtil::iterator_traits<Iter>::reference reference;


    /** @name constructors  */ 
    //@{
    /// empty constructor.
    utl_reverse_iterator () : current () {}
	
    /// template constructor.
    template <class U> 
    utl_reverse_iterator ( const utl_reverse_iterator<U>& x ) : current(x.base()) {}

    /// contructor from an Iter.
    utl_reverse_iterator (Iter x) : current (x) {}
    //@}

    /// returns current iterator value.
    Iter base () const { return current; } 

    /// prefix increment operator. Decrements the current pointer.
    utl_reverse_iterator& operator ++ ( ) { 
      --current;
      return *this;
    }

    /// posfix increment operator. Decrements the current pointer.
    utl_reverse_iterator operator ++ ( int ) {
      utl_reverse_iterator tmp = current;
      --current;
      return tmp; 
    }

    /// prefix decrement operator. Increments the current pointer.
    utl_reverse_iterator& operator -- ( ) { 
      ++current;
      return *this;
    }

    /// posfix decrement operator. Increments the current pointer.
    utl_reverse_iterator operator -- ( int ) { 
      utl_reverse_iterator tmp = current;
      ++current;
      return tmp; 
    }

    /// assignment operator.
    utl_reverse_iterator& operator = ( const utl_reverse_iterator& rhs ) {
      this->current = rhs.current;
      return *this;
    }

    /// comparison operator.
    friend bool operator == ( const utl_reverse_iterator& itr1, 
		              const utl_reverse_iterator& itr2 ) {
      return itr1.current == itr2.current;
    }

    /// comparison operator.
    friend bool operator != ( const utl_reverse_iterator& itr1, 
		              const utl_reverse_iterator& itr2 ) {
      return itr1.current != itr2.current;
    }

    /// returns whether i is reachable from j. 
    friend bool operator < ( const utl_reverse_iterator& i, 
			     const utl_reverse_iterator& j ) { 
      return (j.current < i.current); 
    }

    /// returns a number n such that i == j + n.
    friend difference_type operator - ( const utl_reverse_iterator& i, 
		                        const utl_reverse_iterator& j ) { 
      return (j.current - i.current);
    }

    /// dereference operator. 
    reference operator * () const { 
      Iter tmp = current; 
      return *--tmp; 
    } 

    /// dereference operator. 
    pointer operator -> () const { return &operator*(); } 

    /// returns n-th element. 
    reference operator [] (difference_type n) const { 
      return *(base()-(n+1)); 
    }

    /// iterator addition. 
    utl_reverse_iterator operator + (difference_type n) const { 
      return base()-n; 
    } 

    /// forward motion. 
    utl_reverse_iterator& operator += (difference_type n) { 
      current-=n; 
      return *this; 
    } 

    /// iterator subtraction. 
    utl_reverse_iterator operator - (difference_type n) const { 
      return base()+n; 
    } 

    /// backward motion. 
    utl_reverse_iterator& operator -= (difference_type n) { 
      current+=n; 
      return *this; 
    } 

  }; 	// utl_reverse_iterator

#endif
