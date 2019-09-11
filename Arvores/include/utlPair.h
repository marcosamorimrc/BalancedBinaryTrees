/*
   utlPair.h

  Program:   utl
  Module:    $RCSfile: utlPair.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 20 Mar 2000.
*/

#ifndef __UTLPAIR_H__
#define __UTLPAIR_H__

/* fos STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
//#define cgcUtil std
#define utlPair pair
#else

/** 	STL compatible pair.
 *
 *	@author <A HREF="mailto:ulisses@watson.ibm.com">Ulisses T. Mello</a> and
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	Pair<Type1,Type2> is a heterogeneous pair: it holds one object of type Type1 
 *	and one of type Type2. A pair is much like a Container, in that it "owns" its elements. 
 *	It is not actually a model of Container, though, because it does not support 
 *	the standard methods (such as iterators) for accessing the elements of a Container. 
 *
 *	Functions that need to return two values often return a pair. 
 *
 */
template <class Type1, class Type2> struct utlPair {

      /// the key.
      typedef Type1 first_type;
      /// the data.
      typedef Type2 second_type;

      /// first type: key.
      first_type first;
      /// second type: value.
      second_type second;

  /** @name constructors  */ 
  //@{

   /// empty constructor.
   utlPair() : first(first_type()), second(second_type()) {}

   /// constructor from two types.
   utlPair(const first_type& a, const second_type& b) : first(a), second(b) {}

   /// template constructor. It exists only to allow conversions in the initializer.
   template<class U, class V>
   utlPair(const utlPair<U,V>& p) : first(p.first), second(p.second) {}

   /// copy constructor.
   utlPair ( const utlPair& p ) : first(p.first), second(p.second) {}

  //@}

   /// assignment operator.
   utlPair& operator = ( const utlPair& rhs ) {
       first = rhs.first;
       second = rhs.second;
       return *this;
   }

   /**  compares two pairs.
    *
    *	The equality operator. The return value is true if and only the first 
    *	elements of x and y are equal, and the second elements of x and y are equal. 
    *	This operator may only be used if both Type1 and Type2 are EqualityComparable. 
    */
   friend bool operator == (const utlPair<Type1, Type2>& x, const utlPair<Type1, Type2>& y)
    { 
      return (x.first == y.first && x.second == y.second); 
    }

   /**  compares two pairs.
    *
    *	The inequality operator. The return value is true if and only the first 
    *	elements of x and y are equal. 
    *	This operator may only be used if Type1 is EqualityComparable. 
    */
   friend bool operator != (const utlPair<Type1, Type2>& x, const utlPair<Type1, Type2>& y)
    { 
      return !(x.first == y.first); 
    }

    /** returns whether first element of first pair is less than first element of second pair.
     *
     *	The comparison operator. It uses lexicographic comparison: 
     *	the return value is true if the first element of x is less than 
     *	the first element of y, and false if the first element of y is less than 
     *	the first element of x. If neither of these is the case, 
     *	then operator< returns the result of comparing the second elements
     *  of x and y. This operator may only be used if both Type1 and Type2 are
     *  LessThanComparable. 
     */
    friend bool operator < (const utlPair<Type1, Type2>& x, const utlPair<Type1, Type2>& y)
    { 
      return x.first < y.first || 
	(!(y.first < x.first) && x.second < y.second); 
    }

    /// returns whether first element of first pair is greater than first element of second pair.
    friend bool operator > (const utlPair<Type1, Type2>& x, const utlPair<Type1, Type2>& y)
    { 
      return x.first > y.first || 
	(!(y.first > x.first) && x.second > y.second); 
    }

};

/// namespace for avoiding conflict with std::
namespace cgcUtil {

/// returns a new pair. Equivalent to utlPair<Type1, Type2>(x, y). 
template <class Type1, class Type2>
inline utlPair<Type1, Type2> make_pair(const Type1& x, const Type2& y) {
      return utlPair<Type1, Type2>(x, y);
}

} // namespace

#endif // USE_STL
#endif // __UTLPAIR_H__

