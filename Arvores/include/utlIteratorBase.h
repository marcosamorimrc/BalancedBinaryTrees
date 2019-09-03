
/*
  Program:   utl
  Module:    $RCSfile: utlIteratorBase.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 14:37:19 $
  Version:   $Revision: 1.2 $
*/

#ifndef __UTLITERATORBASE_H__
#define __UTLITERATORBASE_H__

#include "utlConfig.h"

/// base iterator.
template <class Etype> struct utlIteratorBase { 

  /// input iterator category.
  struct input_iterator_tag {};
  /// output iterator category.
  struct output_iterator_tag {};
  /// forward iterator category.
  struct forward_iterator_tag : public input_iterator_tag {};
  /// bidirectional iterator category.
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  /// random access iterator category.
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};

};


#ifndef USE_STL

namespace cgcUtil {

  /// related type of an iterator.
  template <class Iterator>
  struct iterator_traits {
    /// category of iterator. 
    typedef typename Iterator::iterator_category iterator_category;
    /// type of element.
    typedef typename Iterator::value_type        value_type;
    /// type of difference between two iterators.
    typedef typename Iterator::difference_type   difference_type;
    /// return type of operator ->().
    typedef typename Iterator::pointer           pointer;
    /// return type of operator *().
    typedef typename Iterator::reference         reference;
  };

  /// specialization for pointers.
  template <class T>
  struct iterator_traits<T*> {
    /// category of iterator. 
    typedef random_access_iterator_tag iterator_category;
    /// type of element.
    typedef T                          value_type;
    /// type of difference between two iterators.
    typedef ptrdiff_t                  difference_type;
    /// return type of operator ->().
    typedef T*                         pointer;
    /// return type of operator *().
    typedef T&                         reference;
  };

 /** a helper function to select the appropriate algorithm based on the
  *  iterator category. The iterator tag is used exclusively for overload 
  *  resolution; the tag does not take part in the actual computation.
  *
  *  @param first beginning of the range.
  *  @param last end of the range. 
  *  @return distance between first and last.
  */
 template <class In>
 inline typename cgcUtil::iterator_traits<In>::difference_type
 dist_helper(In first, In last, typename utlIteratorBase<In>::input_iterator_tag) {
    typename cgcUtil::iterator_traits<In>::difference_type d = 0;
    while (first!=last) {++d; ++first;}
    return d;
 }

 /** a helper function to select the appropriate algorithm based on the
  *  iterator category. The iterator tag is used exclusively for overload 
  *  resolution; the tag does not take part in the actual computation.
  *
  *  @param first beginning of the range.
  *  @param last end of the range. 
  *  @return distance between first and last.
  */
 template <class Ran>
 inline typename cgcUtil::iterator_traits<Ran>::difference_type
 dist_helper(Ran first, Ran last, typename utlIteratorBase<Ran>::random_access_iterator_tag) {
    return last-first;
 }

 /** finds the distance between first and last, i.e. the number 
  *  of times that first must be incremented until it is equal to last. 
  *
  *  @param first beginning of the range.
  *  @param last end of the range. 
  *  @return distance between first and last.
  */
 template <class In>
 inline typename cgcUtil::iterator_traits<In>::difference_type
 distance(In first, In last) {
    return cgcUtil::dist_helper(first,last,typename cgcUtil::iterator_traits<In>::iterator_category());
 }

 /** a helper function to select the appropriate algorithm based on the
  *  iterator category. The iterator tag is used exclusively for overload 
  *  resolution; the tag does not take part in the actual computation.
  *
  *  @param i an iterator.
  *  @param n a displacement. 
  */
 template <class In, class Distance>
 inline void ad_helper(In& i, Distance n, typename utlIteratorBase<In>::input_iterator_tag) {
    if ( n > 0 ) for ( Distance j = 0; j < n; ++j, ++i );
    else if ( n < 0 ) for ( Distance j = n-1; j >= 0; --j, --i );
 }

 /** a helper function to select the appropriate algorithm based on the
  *  iterator category. The iterator tag is used exclusively for overload 
  *  resolution; the tag does not take part in the actual computation.
  *
  *  @param i an iterator.
  *  @param n a displacement. 
  */
 template <class Ran, class Distance>
 inline void ad_helper(Ran& i, Distance n, typename utlIteratorBase<Ran>::random_access_iterator_tag) {
    i+=n;
 }

 /** advance(i, n) increments the iterator i by the distance n. 
  *  If n > 0 it is equivalent to executing ++i n times, 
  *  and if n < 0 it is equivalent to executing --i n times. 
  *  If n == 0, the call has no effect. 
  *
  *  @param i an iterator.
  *  @param n a displacement. 
  */
 template <class InputIterator, class Distance>
 inline void advance(InputIterator& i, Distance n) {
    ad_helper(i,n,typename cgcUtil::iterator_traits<InputIterator>::iterator_category());
 }

} // cgcUtil

#endif

#endif // __UTLITERATORBASE_H__


