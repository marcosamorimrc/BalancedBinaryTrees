/*
**	utlAlgo.h - class with algorithms for STL.
**
**	purpose: to provide adequate algorithms for the STL classes.
**	author: Paulo Roma cavalcanti.
**

  Program:   cgc util
  Module:    $RCSfile: utlAlgo.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

**
*/

#ifndef __UTLALGO_H__
#define __UTLALGO_H__

#include "utlPair.h"

#if !defined(__GNUC__) || !defined(USE_STL) || defined(__INTEL_COMPILER) // gnu g++ has an extended version of STL

/// name space with SGI's STL extensions.
namespace cgcUtilExt {

/**	returns true if the range [first, last) is sorted in ascending order, 
 *	and false otherwise. 
 *
 *	Compares objects using operator<(), and 
 *	returns true if and only if, for every iterator i in the
 *	range [first, last - 1), *(i + 1) < *i is false. 
 *
 *	@param first beginning of the range.
 *	@param last  end of the range.
 *	@return true if the range is sorted, and false otherwise.
 */
template <class ForwardIterator>
inline bool is_sorted(ForwardIterator first, ForwardIterator last) {
    ForwardIterator i = first;
    while ( i != last - 1 ) {
      if ( *(i + 1) < *i ) return false;
      ++i;
    }
    return true;
}


/**	returns true if the range [first, last) is sorted in ascending order, 
 *	and false otherwise. 
 *
 *	Compares objects using the function object comp, and 
 *	returns true if and only if, for every iterator i in the
 *	range [first, last - 1), comp (*(i + 1), *i) is false. 
 *
 *	@param first beginning of the range.
 *	@param last  end of the range.
 *	@param comp  comparison function object.
 *	@return true if the range is sorted, and false otherwise.
 */
template <class ForwardIterator, class StrictWeakOrdering>
inline bool is_sorted(ForwardIterator first, ForwardIterator last,
               StrictWeakOrdering comp) {
    ForwardIterator i = first;
    while ( i != last - 1 ) {
      if ( comp (*(i + 1), *i) ) return false;
      ++i;
    }
    return true;
}

/**	assigns sequentially increasing values to a range. 
 *
 *	That is, it assigns value to *first, value + 1 to *(first + 1) and so on. 
 *	In general, each iterator i in the range [first, last) is assigned value + (i - first).  
 * 
 *	Conditions: <br>
 *	1) T is Assignable. <br> 
 *    	2) If x is an object of type T, then x++ is defined. <br>
 *    	3) T is convertible to ForwardIterator's value type. 
 *
 *	@param first where to start assigning.
 *	@param last where to end assigning.
 *	@param value initial value to assign.
 */
template<class ForwardIterator, class T> 
inline void iota(ForwardIterator first, ForwardIterator last, T value) {
    for( ForwardIterator itr = first; itr != last; ++itr, ++value ) {
	*itr = value;
    }
}

} // cgcUtilExt
#endif // __GNU__


#ifndef USE_STL

#include "utlSorter.h"
#include "utlInserter.h"

namespace cgcUtil {

/**	applies the function object f to each element in the range [first, last); 
 *	f's return value, if any, is ignored. Applications are performed
 *	in forward order, i.e. from first to last. For_each returns the function 
 *	object after it has been applied to each element.  
 *	
 *	@param first where to start applying.
 *	@param last where to end applying.
 *	@param f unary function object.
 *	@return the unary function object.
 */
template <class InputIterator, class UnaryFunction>
inline UnaryFunction for_each(InputIterator first, InputIterator last, UnaryFunction f) {
    while ( first != last ) 
	f(*first++);
    
    return f;
}

/**	returns the first iterator i in the range [first, last) such that *i == value. 
 *	Returns last if no such iterator exists. 
 *	
 *	@param first where to start the searching.
 *	@param last where to end the searching.
 *	@param value given value.
 *	@return position where the value is or "last", if the value was not found.	
 */
template<class In, class EqualityComparable> 
inline In find(In first, In last, const EqualityComparable& value) {
    for( In itr = first; itr != last; ++itr ) {
	if ( value == *itr ) return itr;
    }
    return last;
}

/**	returns the first iterator i in the range [first, last) such that 
 *	pred(*i) is true. Returns last if no such iterator exists. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param pred given predicate.
 *	@return position where pred is true or "last", if pred is never true.	
 */
template<class InputIterator, class Predicate>
inline InputIterator find_if(InputIterator first, InputIterator last,
                             Predicate pred) {
    for( InputIterator itr = first; itr != last; ++itr ) {
	if ( pred(*itr) ) return itr;
    }
    return last;
}

/**	copies elements from a container to another.
 *
 *	Copies elements from the range [first, last) to the 
 *	range [result, result + (last - first)). That is, it performs the
 *	assignments *result = *first, *(result + 1) = *(first + 1), and so on. 
 *	Generally, for every integer n from 0 to last - first, 
 *	copy() performs the assignment *(result + n) = *(first + n). 
 *	Assignments are performed in forward order, i.e. in order of
 *	increasing n.  
 *
 *	The return value is result + (last - first) 
 *	
 *	@param first where to start copying.
 *	@param last where to end copying.
 *	@param result where to copy.
 *	@return result + (last-first).	
 */
template<class InputIterator, class OutputIterator> 
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    for( InputIterator itr = first; itr != last; ++itr, ++result ) {
	*result = *itr;
    }
    return result;
}


/**	lexicographical comparison. Compares sequences in general.
 *
 *	lexicographical_compare() returns true if the range of elements [first1, last1) is 
 *	lexicographically less than the range of elements [first2, last2), and false 
 *	otherwise. Lexicographical comparison means "dictionary" (element-by-element) ordering. 
 *	That is, [first1, last1) is less than [first2, last2) if *first1 is less than *first2, 
 *	and greater if *first1 is greater than *first2. If the two first
 *	elements are equivalent then lexicographical_compare() compares the two second elements, 
 *	and so on. As with ordinary dictionary order, the first range is considered to be 
 *	less than the second if every element in the first range is equal to the 
 *	corresponding element in the second but the second contains more elements. 
 *
 *	This version compares objects using operator<(). 
 *	
 *	@param first beginning of the first sequence.
 *	@param last  end of the first sequence.
 *	@param first2 beginning of the second sequence.
 *	@param last2 end of the second sequence.
 *	@return true if the first sequence is less than the second, and false otherwise.
 */
template<class In, class In2> 
inline bool lexicographical_compare (In first, In last, In2 first2, In2 last2) {
    while ( first != last && first2 != last2 ) {
      if ( *first < *first2 ) return true;
      if ( *first2++ < *first++ ) return false;
    }
    return ( first == last && first2 != last2 );
}

/**	lexicographical comparison. Compares sequences in general.
 *	This version compares objects using a function object cmp. 
 *	
 *	@param first beginning of the first sequence.
 *	@param last  end of the first sequence.
 *	@param first2 beginning of the second sequence.
 *	@param last2 end of the second sequence.
 *	@param cmp comparison function object. 
 *	@return true if the first sequence is less than the second, and false otherwise.
 *	@see cgcUtil::lexicographical_compare().
 */
template<class In, class In2, class Cmp> 
inline bool lexicographical_compare (In first, In last, In2 first2, In2 last2, Cmp cmp) {
    while ( first != last && first2 != last2 ) {
      if ( cmp(*first,*first2) ) return true;
      if ( cmp(*first2++,*first++) ) return false;
    }
    return ( first == last && first2 != last2 );
}

/**	compares sequences in general.
 *	Tells whether all corresponding pairs of elements of two 
 *	sequences compare equal.
 *
 *	equal() returns true if the two ranges [first1, last1) and 
 *	[first2, first2 + (last1 - first1)) are identical when compared
 *	element-by-element, and otherwise returns false. 
 *
 *	Note that this is very similar to the behavior of mismatch(): 
 *	The only real difference is that while equal() will simply return false if the two 
 *	ranges differ, mismatch() returns the first location where they do differ. 
 *	The expression equal(f1, l1, f2) is precisely equivalent to the expression
 *	mismatch(f1, l1, f2).first == l1, and this is in fact how equal could be implemented. 
 *
 *	This version of equal() returns true if and only if for every iterator i in 
 *	[first1, last1), *i == *(first2 + (i - first1))
 *	
 *	@param first beginning of the first sequence.
 *	@param last  end of the first sequence.
 *	@param first2 beginning of the second sequence.
 *	@return true if the two sequences are equal, and false otherwise.
 *	@see cgcUtil::mismatch().
 */
template<class In, class In2> 
inline bool equal (In first, In last, In2 first2) {
    while ( first != last  ) {
      if ( !(*first++ == *first2++) ) return false;
    }
    return true;
}

/**	compares sequences in general.
 *	Tells whether all corresponding pairs of elements of two 
 *	sequences compare equal.
 *
 *	This version of equal() returns true if and only if for every iterator i in 
 *	[first1, last1), binary_pred(*i, *(first2 + (i - first1)) is true. 
 *
 *	@param first beginning of the first sequence.
 *	@param last  end of the first sequence.
 *	@param first2 beginning of the second sequence.
 *	@param p binary predicate used for comparison.
 *	@return true if the two sequences are equal, and false otherwise.
 *	@see cgcUtil::mismatch(), cgcUtil::equal().
 */
template<class In, class In2, class BinPred> 
inline bool equal (In first, In last, In2 first2, BinPred p) {
    while ( first != last ) {
      if ( !p(*first++,*first2++) ) return false;
    }
    return true;
}

/**	finds the first position where the two ranges [first1, last1) and 
 *	[first2, first2 + (last1 - first1)) differ.
 *
 *	mismatch() finds the first iterator i in [first1, last1) 
 *	such that *i != *(first2 + (i - first1)). 
 *	The return value is a pair whose first element is i and whose 
 *	second element is (first2 + (i - first1)). If no such iterator i exists, 
 *	the return value is a pair whose first element is last1 and whose 
 *	second element is (first2 + (last1 - first1)). 
 *
 *	@param first1 beginning of the first range.
 *	@param last1 end of the first range.
 *	@param first2 beginning of the second range.
 *	@return a pair with two iterators.
 */
template <class InputIterator1, class InputIterator2>
inline utlPair<InputIterator1, InputIterator2> 
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    while ( first1 != last1 && *first1 == *first2 ) {
	++first1;
        ++first2;
    }
    return cgcUtil::make_pair ( first1, first2 );
}

/**	finds the first position where the two ranges [first1, last1) and 
 *	[first2, first2 + (last1 - first1)) differ.
 *
 *	mismatch() finds the first iterator i in [first1, last1) such that 
 *	binary_pred(*i, *(first2 + (i - first1)) is false. 
 *	The return value is a pair whose first element is i and whose 
 *	second element is (first2 + (i - first1)). If no such iterator i exists, 
 *	the return value is a pair whose first element is last1 and whose 
 *	second element is (first2 + (last1 - first1)). 
 *
 *	@param first1 beginning of the first range.
 *	@param last1 end of the first range.
 *	@param first2 beginning of the second range.
 *	@param binary_pred comparison function object.
 *	@return a pair with two iterators.
 */
template <class InputIterator1, class InputIterator2, 
          class BinaryPredicate>
inline utlPair<InputIterator1, InputIterator2> 
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
         BinaryPredicate binary_pred) {
    while ( first1 != last1 && binary_pred(*first1,*first2) ) {
	++first1;
        ++first2;
    }
    return cgcUtil::make_pair ( first1, first2 );
}

/**	equal_range() is a version of binary search: it attempts to find the 
 *	element value in an ordered range [first, last). The value returned by
 *	equal_range() is essentially a combination of the values returned by lower_bound 
 *	and upper_bound: it returns a pair of iterators i and j such that
 *	i is the first position where value could be inserted without violating 
 *	the ordering and j is the last position where value could be inserted 
 *	without violating the ordering. It follows that every element in the 
 *	range [i, j) is equivalent to value, and that [i, j) is the largest 
 *	subrange of [first, last) that has this property. 
 *
 *	This version of equal_range() uses operator<() for comparison and 
 *	returns a pair of iterators [i, j). i is the furthermost iterator in 
 *	[first, last) such that, for every iterator k in
 *	[first, i), *k < value. j is the furthermost iterator in [first, last) such that, 
 *	for every iterator k in [first, j), value < *k is
 *	false. For every iterator k in [i, j), neither value < *k nor *k < value is true.  
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param value element looked for.
 *	@return a pair with two iterators.
 */
template <class ForwardIterator, class LessThanComparable>
inline utlPair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last,
                   const LessThanComparable& value) {
       ForwardIterator it1 = first, it2 = first;
       if ( *it1 < value ) {
         do
           ++it1;
         while ( it1 != last && *it1 < value );
         it2 = it1;
         if ( it1 != last ) {	// found at least one
           do
             ++it2;
           while ( it2 != last && !(value < *it2) );
         }
       }
       return cgcUtil::make_pair ( it1, it2 ); 
}

/**	this version uses a function object comp and returns a pair of iterators [i, j). 
 *	i is the furthermost iterator *	in [first, last) such that, for every iterator 
 *	k in [first, i), comp(*k, value) is true. 
 *	j is the furthermost iterator in [first, last) 	such that, for every iterator 
 *	k in [first, j), comp(value, *k) is false. 
 *	For every iterator k in [i, j), neither comp(value, *k) nor comp(*k, value) is true.  
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param value element looked for.
 *	@param comp comparison function object.
 *	@return a pair with two iterators.
 *	@see cgcUtil::equal_range().
 */
template <class ForwardIterator, class T, class StrictWeakOrdering>
inline utlPair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value,
                   StrictWeakOrdering comp) {
       ForwardIterator it1 = first, it2 = first;
       if ( comp(*it1, value) ) {
         do
           ++it1;
         while ( it1 != last && comp(*it1, value) );
         it2 = it1;
         if ( it1 != last ) {	// found at least one
           do 
             ++it2;
           while ( it2 != last && !comp(value,*it2) );
         }
       }
       return cgcUtil::make_pair ( it1, it2 ); 
}

/**	finds the number of elements in [first, last) that are equal to value. 
 *	More precisely, it returns the number
 *	of iterators i in [first, last) such that *i == value. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param value value to compare.
 *	@return number of matches.
 */
template <class InputIterator, class EqualityComparable>
inline typename cgcUtil::iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const EqualityComparable& value) {
    typename cgcUtil::iterator_traits<InputIterator>::difference_type n = 0;
    while ( first != last ) {
      if ( *first++ == value ) ++n;
    }
    return n;
}

/**	finds the number of elements in [first, last) that satisfy the predicate pred. 
 *	More precisely, it returns the number of iterators i in [first, last) 
 *	such that pred(*i) is true. This version of count adds to n the number of
 *	iterators i in [first, last) such that pred(*i) is true. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param pred given predicate.
 *	@return number of matches.
 */
template <class InputIterator, class Predicate>
inline typename cgcUtil::iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
    typename cgcUtil::iterator_traits<InputIterator>::difference_type n = 0;
    while ( first != last ) {
      if ( pred (*first++) ) ++n;
    }
    return n;
}

/** 	returns a bool indicating whether a value was present in an ordered sequence.
 *	It performs a binary search, that is, the sequence is supposed
 *	to be sorted and it is split in the middle until the value is
 *	found. The complexity is O (log N), where N is the size of the sequence.
 *
 *	binary_search() is a version of binary search: it attempts to find the element 
 *	value in an ordered range [first, last). It returns true if an
 *	element that is equivalent to value is present in [first, last) and false 
 *	if no such element exists. This version of binary_search()
 *	uses operator<() for comparison. 
 *	Specifically, it returns true if and only if there exists an 
 *	iterator i in [first, last) such that *i < value and value < *i are
 *	both false. 
 *
 *	@param first beginning of the sequence.
 *	@param last  end of the sequence.
 *	@param val   value looked for.
 *	@return true if the value was found, and false otherwise.
 */    
template <class For, class T>
inline bool binary_search( For first, For last, const T& val ) {
    For mid;
    while ( first < last ) {
	mid = first + ( (last - first) / 2 );
	if( *mid < val )
	   first = mid + 1;
	else if ( val < *mid )
	   last = mid;
        else
           return true;
    }
    return false;
}

/**	this version uses the function object comp.
 *	Specifically, it returns true if and only if there exists an 
 *	iterator i in [first, last) such that comp(*i, value) and comp(value, *i) are both false. 
 *
 *	@param first beginning of the sequence.
 *	@param last  end of the sequence.
 *	@param val   value looked for.
 *	@param comp  comparison function object.
 *	@return true if the value was found, and false otherwise.
 *	@see cgcUtil::binary_search().
 */
template <class ForwardIterator, class T, class StrictWeakOrdering>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& val,
                   StrictWeakOrdering comp) {
    ForwardIterator mid;
    while ( first < last ) {
	mid = first + ( (last - first) / 2 );
	if( comp(*mid,val) )
	   first = mid + 1;
	else if ( comp(val,*mid) )
	   last = mid;
        else
           return true;
    }
    return false;
}



/**	combines two sorted ranges [first1, last1) and [first2, last2) 
 *	into a single sorted range. That is, it copies elements from
 *	[first1, last1) and [first2, last2) into 
 *	[result, result + (last1 - first1) + (last2 - first2)) such that the
 *	resulting range is in ascending order. merge() is stable, meaning both that 
 *	the relative order of elements within each input range is preserved, and that for
 *	equivalent elements in both input ranges the element from the first 
 *	range precedes the element from the second. The return value is result +
 *	(last1 - first1) + (last2 - first2). 
 *
 *	Note that you may use an ordering that is a strict weak ordering but not 
 *	a total ordering; that is, there might be values x and y such that x < y, x > y,
 *	and x == y are all false. 
 *	(See the LessThan Comparable requirements for a more complete discussion.) 
 *	Two elements x and y are equivalent if neither x < y nor y < x. 
 *	If you're using a total ordering, however (if you're using strcmp, for example, 
 *	or if you're using ordinary arithmetic comparison on integers), 
 *	then you can ignore this technical distinction: for a total ordering, 
 *	equality and equivalence are the same. 
 *
 *	This version uses operator<(). That is, the input ranges and the output 
 *	range satisfy the condition that for every pair of iterators i and j such that 
 *	i precedes j, *j < *i is false. 
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@return last element in the sorted range.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result) {
   while ( first1 != last1 && first2 != last2 ) {
     *result++ = ( !(*first2 < *first1) ? *first1++ : *first2++ );
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/** 	this version uses function object comp. That is, the input ranges and the output 
 *	range satisfy the condition that for every pair of iterators i and j such that 
 *	i precedes j, comp(*j, *i) is false. 
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@param comp  comparison function object.
 *	@return last element in the sorted range.
 *	@see cgcUtil::merge().
 */
template <class InputIterator1, class InputIterator2, class OutputIterator, 
          class StrictWeakOrdering>
inline OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result, StrictWeakOrdering comp) {
   while ( first1 != last1 && first2 != last2 ) {
     *result++ = ( !comp (*first2, *first1) ? *first1++ : *first2++ );
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/**	every time a consecutive group of duplicate elements appears in 
 *	the range [first, last), the algorithm unique removes all but the first element. 
 *	That is, unique() returns an iterator new_last such that the range [first, new_last) 
 *	contains no two consecutive elements that are duplicates. 
 *
 *	Note that the meaning of "removal" is somewhat subtle. 
 *	unique(), like remove(), does not destroy any iterators and does not change the 
 *	distance between first and last. (There's no way that it could do anything of the sort.) 
 *	So, for example, if V is a vector, remove(V.begin(), V.end(), 0) does
 *	not change V.size(): V will contain just as many elements as it did before. 
 *	Unique returns an iterator that points to the end of the resulting range after
 *	elements have been removed from it; it follows that the elements after that 
 *	iterator are of no interest. If you are operating on a Sequence, you may wish to use
 *	the Sequence's erase member function to discard those elements entirely. 
 *
 *	The iterators in the range [new_last, last) are all still dereferenceable, 
 *	but the elements that they point to are unspecified. Unique is stable, meaning that
 *	the relative order of elements that are not removed is unchanged. 
 *
 *	What means for a consecutive group of elements to be duplicate
 *	is simple equality: the elements in a range [f, l) are duplicates if, 
 *	for every iterator i in the range, either i == f or else *i == *(i-1). 
 *
 *	@param first beginning of the range.
 *	@param last  end of the range.
 *	@return new end of the sequence.
 */
template <class ForwardIterator>
inline ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
    ForwardIterator it1 = first;
    ForwardIterator it2 = it1+1;
    ForwardIterator fst, lst;
    while ( it2 != last && it1 != last ) {
      while ( *it1 == *it2 ) {
	 if ( ++it2 == last ) 
            break;
      }

      if ( it2 > it1 + 1 ) {
         fst = it1+1; lst = it2;
         for ( ; lst != last; ++fst, ++lst )
             *fst = *lst;
         last -= ( it2 - it1 - 1 );
      }
     ++it1; 
     it2 = it1+1;
    }
    return last;
}

/**	this version uses a binary predicate for finding duplicate elements.
 *	The test is an arbitrary Binary Predicate binary_pred: 
 *	the elements in [f, l) are duplicates if, for every iterator i in the range, 
 *	either binary_pred(*i,*f) or else binary_pred(*i, *(i-1)) is true.  
 *
 *	@param first beginning of the range.
 *	@param last  end of the range.
 *	@param binary_pred comparison function object.
 *	@return new end of the sequence.
 *	@see cgcUtil::unique().
 */
template<class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last,
                       BinaryPredicate binary_pred) {
    ForwardIterator it1 = first;
    ForwardIterator it2 = it1+1;
    ForwardIterator fst, lst;
    while ( it2 != last && it1 != last ) {
      while ( binary_pred(*it1,*it2) ) {
	 if ( ++it2 == last ) 
            break;
      }

      if ( it2 > it1 + 1 ) {
         fst = it1+1; lst = it2;
         for ( ; lst != last; ++fst, ++lst )
             *fst = *lst;
         last -= ( it2 - it1 - 1 );
      }
     ++it1; 
     it2 = it1+1;
    }
    return last;
}

/**	constructs a sorted range that is the intersection of the sorted ranges 
 *	[first1, last1) and [first2, last2). The return value is the end of the output range. 
 *
 *	In the simplest case, set_intersection() performs the "intersection" operation from set theory: 
 *	the output range contains a copy of every element that
 *	is contained in both [first1, last1) and [first2, last2). 
 *	The general case is more complicated, because the input ranges may contain
 *	duplicate elements. The generalization is that if a value appears m times in 
 *	[first1, last1) and n times in [first2, last2) (where m or n may be zero), 
 *	then it appears min(m,n) times in the output range. Set_intersection is stable, 
 *	meaning both that elements are copied from the first range rather than the second, 
 *	and that the relative order of elements in the output range is the same as in the 
 *	first input range. 
 *
 *	This version compares objects using operator<().
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@return last element in the sorted range.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( *first1 < *first2 ) 
          first1++;
     else if ( *first2 < *first1 )
          first2++;
     else {
         *result++ = *first1++;
         first2++;
     }
   }
    
   return result; 
}

/**	this version compares objects using the function object comp.
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@param comp comparison function object.
 *	@return last element in the sorted range.
 *	@see cgcUtil::set_intersection().
 */
template <class InputIterator1, class InputIterator2, class OutputIterator, class StrictWeakOrdering>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, StrictWeakOrdering comp) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( comp(*first1,*first2) ) 
          first1++;
     else if ( comp(*first2,*first1) )
          first2++;
     else {
         *result++ = *first1++;
         first2++;
     }
   }
    
   return result; 
}

/**	constructs a sorted range that is the union of the sorted ranges 
 *	[first1, last1) and [first2, last2). The return value is the
 *	end of the output range. 
 *
 *	In the simplest case, set_union() performs the "union" operation from set theory: 
 *	the output range contains a copy of every element that is contained in
 *	[first1, last1), [first2, last2), or both. The general case is more complicated, 
 *	because the input ranges may contain duplicate elements.
 *	The generalization is that if a value appears m times in [first1, last1) and n 
 *	times in [first2, last2) (where m or n may be zero), then it
 *	appears max(m,n) times in the output range. Set_union is stable, meaning both that 
 *	the relative order of elements within each input range is
 *	preserved, and that if an element is present in both input ranges it is copied 
 *	from the first range rather than the second. 
 *
 *	This version compares objects using operator<().
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@return last element in the sorted range.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( *first1 < *first2 ) 
          *result = *first1++;
     else if ( *first2 < *first1 )
          *result = *first2++;
     else {
         *result++ = *first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/** 	this version compares objects using the function object comp.
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@param comp comparison function object.
 *	@return last element in the sorted range.
 *	@see cgcUtil::set_union().
 */
template <class InputIterator1, class InputIterator2, class OutputIterator, class StrictWeakOrdering>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, StrictWeakOrdering comp) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( comp(*first1,*first2) ) 
          *result = *first1++;
     else if ( comp(*first2,*first1) )
          *result = *first2++;
     else {
         *result++ = *first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/**	constructs a sorted range that is the set difference of the sorted ranges 
 *	[first1, last1) and [first2, last2). The return value is the end of the output range. 
 *
 *	In the simplest case, set_difference() performs the "difference" operation from set theory: 
 *	the output range contains a copy of every element that is
 *	contained in [first1, last1) and not contained in [first2, last2). 
 *	The general case is more complicated, because the input ranges may
 *	contain duplicate elements. The generalization is that if a value appears m 
 *	times in [first1, last1) and n times in [first2, last2) (where m or n may be zero), 
 *	then it appears max(m-n, 0) times in the output range. Set_difference is stable, 
 *	meaning both that elements are copied from the first range rather than the second, 
 *	and that the relative order of elements in the output range is the same as in the 
 *	first input range. 
 *
 *	This version compares objects using operator<().
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@return last element in the sorted range.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( *first1 < *first2 ) 
          *result = *first1++;
     else if ( *first2 < *first1 )
          first2++;
     else {
         first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   return result; 
}

/**	this version compares objects using the function object comp.
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@param comp comparison function object.
 *	@return last element in the sorted range.
 *	@see cgcUtil::set_difference().
 */
template <class InputIterator1, class InputIterator2, class OutputIterator, class StrictWeakOrdering>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result, StrictWeakOrdering comp) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( comp(*first1,*first2) ) 
          *result = *first1++;
     else if ( comp(*first2,*first1) )
          first2++;
     else {
         first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   return result; 
}



/**	constructs a sorted range that is the set symmetric difference of the sorted ranges 
 *	[first1, last1) and [first2, last2). The return value is the end of the output range. 
 *
 *	In the simplest case, set_symmetric_difference() performs a set theoretic calculation: 
 *	it constructs the union of the two sets A - B and B - A,
 *	where A and B are the two input ranges. That is, the output range contains a copy of 
 *	every element that is contained in [first1, last1) but not
 *	[first2, last2), and a copy of every element that is contained in [first2, last2) 
 *	but not [first1, last1). The general case is more complicated, because the input 
 *	ranges may contain duplicate elements. The generalization is that if a value 
 *	appears m times in [first1, last1) and n times in [first2, last2) 
 *	(where m or n may be zero), then it appears |m-n| times in the output range. 
 *	Set_symmetric_difference is stable, meaning that the relative order of elements 
 *	within each input range is preserved. 
 *	This version compares objects using operator<().
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@return last element in the sorted range.
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, 
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( *first1 < *first2 ) 
          *result = *first1++;
     else if ( *first2 < *first1 )
          *result = *first2++;
     else {
         first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/**	this version compares objects using a function object comp. 
 *
 *	@param first1 beginning of the first range.
 *	@param last1  end of first the range.
 *	@param first2 beginning of second the range.
 *	@param last2  end of the second range.
 *	@param result output sorted range.
 *	@param comp comparison function object.
 *	@return last element in the sorted range.
 *	@see cgcUtil::set_simetric_difference().
 */
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class StrictWeakOrdering>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result, 
                                        StrictWeakOrdering comp) {
   while ( first1 != last1 && first2 != last2 ) {
     if ( comp(*first1,*first2) ) 
          *result = *first1++;
     else if ( comp(*first2,*first1) )
          *result = *first2++;
     else {
         first1++;
         first2++;
     }
   }
    
   if ( first2 == last2 ) {
     for ( ; first1 != last1; ++first1 ) *result++ = *first1;
   }
   else {
     for ( ; first2 != last2; ++first2 ) *result++ = *first2;
   }
   return result; 
}

/**	finds the smallest element in the range [first, last). 
 *	min_element() returns the first iterator i in [first, last) such that no other iterator
 *	in [first, last) points to a value smaller than *i. The return value is last 
 *	if and only if [first, last) is an empty range. 
 *
 *	This version compares objects using operator<().
 *	It returns the first iterator i in [first, last) such that, 
 *	for every iterator j in [first, last), *j < *i is
 *	false. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@return minimum element in the range.
 */
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
    ForwardIterator min = first;
    while ( ++first != last )
      if ( *first < *min ) min = first;	
    return min;
}

/**	this version compares objects using a function object comp. 
 *
 *	min_element() returns the first iterator i in [first, last) such that, 
 *	for every iterator j in [first, last), comp(*j, *i) is false. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param comp comparison function object.
 *	@return minimum element in the range.
 *	@see cgcUtil::min_element().
 */
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            BinaryPredicate comp) {
    ForwardIterator min = first;
    while ( ++first != last )
      if ( comp(*first,*min) ) min = first;	
    return min;
}

/**	finds the largest element in the range [first, last). 
 *	max_element() returns the first iterator i in [first, last) such that no other iterator
 *	in [first, last) points to a value greater than *i. The return value is last 
 *	if and only if [first, last) is an empty range. 
 *
 *	This version compares objects using operator<().
 *	It returns the first iterator i in [first, last) such that, 
 *	for every iterator j in [first, last), *i < *j is
 *	false. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@return maximum element in the range.
 */
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
    ForwardIterator max = first;
    while ( ++first != last )
      if ( *max < *first ) max = first;	
    return max;
}

/**	this version compares objects using a function object comp. 
 *
 *	max_element() returns the first iterator i in [first, last) such that, 
 *	for every iterator j in [first, last), comp(*i,*j) is false. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@param comp comparison function object.
 *	@return maximum element in the range.
 *	@see cgcUtil::max_element().
 */
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            BinaryPredicate comp) {
    ForwardIterator max = first;
    while ( ++first != last )
      if ( comp(*max,*first) ) max = first;	
    return max;
}

}      // cgcUtil

#endif // USE_STL

#endif // __UTLALGO_H__

