/*
   utlSorter.h

   Sorts arrays.
	
  Program:   utl
  Module:    $RCSfile: utlSorter.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 15 September 2000.
*/

#ifndef __UTLSORTER_H__
#define __UTLSORTER_H__

#include "utlConfig.h"

/**	compares the keys of two objects. 
 *
 *	@param x first object.
 *	@param y second object.	
 *	@return true if x < y, and false otherwise.
 */
 template <class Etype> class compare_to {
 
  public:

  /// empty constructor.
  compare_to ( ) { }

  /// function call operator.
  bool operator ( ) ( const Etype& x, const Etype& y ) const {
   return (x < y);
  }

  /// function neq operator.
  bool eq ( const Etype& x, const Etype& y ) const { return x==y; }

  /// function neq operator.
  bool neq ( const Etype& x, const Etype& y ) const { return x!=y; }

 };

/** Implements several algorithms for sorting arrays.
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	Sorts arrays. To sort a vector v, just call: sort (&v[0], v.size()); <br>
 *	
 *	<pre>
 *      Example:
 *
 *      utlSorter<int> SI;
 *      int A[] = {4,3,90,34,56,1,2};
 *      const int N = sizeof(A)/sizeof(int);
 *      SI.sort ( A, N );
 *      // int* is an iterator: unbelievable, isn't it?
 *      cgcUtil::copy (A, A+N, utl_ostream_iterator<int>(cout, " "));
 *      cout << endl;
 *      </pre>
 *
 *	Cmp is a binary function object to compare the keys
 *	of two records. <br>
 *      <pre>
 *
 *      // returns true if r1 less than r2 
 *      //         false otherwise.
 *      bool compare ( const Etype& r1, const Etype& r2 ) const; 
 *	</pre>
 */
template <class Etype, class Cmp=compare_to<Etype> > class utlSorter {

public:

enum {
/// selects merge sort.
MSORT, 
/// selects heap sort.
HSORT, 
/// selects quick sort.
QSORT,
/// selects insertion sort.
ISORT
};	

 /** @name constructor  */ 
 //@{

 /// empty constructor.
 utlSorter () : compare (Cmp()) { }

 /// constructor from a comparison function object.
 utlSorter ( const Cmp& C ) : compare (C()) { }

 //@}

/** sorts an array by using compare().
 *	Vector X = (X0, X1, ...., Xn-1)
 *	is sorted into nondecreasing order of the keys accessed
 *	by compare().
 *	
 *	@param X array to be sorted.
 *	@param n number of elements in X.
 *	@param alg selects the sorting algorithm.
 *	@see compare_to<Etype>(), msort(), hsort(), qsort(), insort().
 */
  void sort ( Etype* X, int n, int alg=QSORT ) const {
     if ( n <= 1 ) return;

     --n;

     switch ( alg )
       {
       case MSORT: msort ( X, n );
	 break;
       case HSORT: hsort ( X, n );
	 break;
       case QSORT: qsort ( X, 0, n );
	 break;
       case ISORT: insort ( X, n );
	 break;
       }
   }

 /** quick select. finds Kth smallest of first N items in array X=(Xm, ..., Xn).
  *	A recursive routine finds Kth smallest and puts it at X[k+m-1].
  *	The average computational time is O(n), and the worst case is 
  *	O (n**2). 
  *	
  *	@param X given array.
  *	@param m first index of the subarray.
  *	@param n last index of the subarray.
  *	@param k given index.
  *	@see choosePivot(), partition().
  */
  void qselect ( Etype* X, int m, int n, int k ) const {
   if ( m < n ) 
     {
       choosePivot ( X, m, n ); 

       int j = partition ( X, m, n );	// get pivot final position
       int x_rnk = j - m + 1;		// rank of the pivot

       if ( k == x_rnk ) 
	 return;
       else if ( x_rnk == 1 )		// in fact, there was no partition
	 qselect ( X, m+1, n, k-1 );  	// fixed the code from several books
       else if ( k < x_rnk )
	 qselect ( X, m, j-1, k );	// select from left subarray
       else 
	 qselect ( X, j+1, n, k-j-1 );	// select from right subarray  
     }
  } 

 /** calculates the median of a set.
  *	
  *	@param X given set.
  *	@param m first index of the array.
  *	@param n last index of the array.
  *	@return median of the set.
  *	@see qselect().
  */
  Etype median ( Etype* X, int m, int n ) const {
   int size = n-m+1;
   if ( size <= 1 ) return X[m];

   int imed = (n+m)/2;

   qselect ( X, m, n, imed-m+1 );
   return X[imed]; 
  }

 /** tests if the array R holds a heap.
  *
  *	@param R array to be tested.
  *	@param n maximum index of R.
  *	@return true if R is a heap, and false otherwise.
  */
  bool iheap ( const Etype* R, int n ) const {
   int j;
   for ( int i = PARENT(n); i >= 0; --i ) {
     j = LCHILD ( i );
     if ( j <= n && R[i] < R[j] ) return false;
     j = RCHILD ( i );
     if ( j <= n && R[i] < R[j] ) return false;
   }
   return true;
  }
    
 /** makes a heap. The array R is transformed into a heap.
  *
  *	@param R array to be turned into a heap.
  *	@param n maximum index of R.
  *	@see adjust().
  */
  void mheap ( Etype* R, int n ) const {
   for ( int i = PARENT(n); i >= 0; --i ) 	// convert R into a heap
     adjust ( R, i, n );
  }
    
 /** sorts a heap. The array R is supposed to hold a heap.
  *
  *	@param R heap to be sorted.
  *	@param n maximum index of R.
  *	@see adjust().
  */
  void sheap ( Etype* R, int n ) const {
   for ( int i = n-1; i >= 0; --i ) 		// sort R
     {
       SWAP ( R, i+1, 0 );			// interchange R0 and Ri+1
       adjust ( R, 0, i ); 			// recreate heap
     }
  }

 /** adjusts the binary tree with root i to satisfy the heap property.
  *	It is supposed that the only element that can break the heap
  *	property is just the element at the root.
  *
  *	A heap is a complete binary tree with the property that the value 
  *	of each node is at least as large as the value of its children 
  *	nodes (if they exist), i.e., <br> 
  *	PARENT(j) >= j or K[j/2] >= K[j], for 1 <= j/2 < j <=n. <br>
  *	The left and right subtrees of i, i.e., with roots 2i and 2i+1,
  *	already satisfy the heap property. The nodes of the trees contain 
  *	records, R, with keys K. No node has index greater than n.
  *	
  *	@param X tree to be adjusted.
  *	@param i root index.
  *	@param n maximum index of the tree.
  */
  void adjust ( Etype* X, int i, int n ) const {
   int j = LCHILD(i);	// left child of i
   
   Etype R = X[i];  	// save the record at the root
   while ( j <= n ) {
     if ( j < n && compare ( X[j], X[j+1] ) ) 
       ++j; 	// find max of right and left child
       
     // compare max child with root. If root is max then done
     if ( !compare ( R, X[j] ) ) break;

     X[PARENT(j)] = X[j]; j = LCHILD(j); 	// move Rj up the tree
   } 
   X[PARENT(j)] = R;
  }

 private:

/** merge sort. Sorts X=(X0, X1, ..., Xn) 
 *	into nondecreasing order of keys. This algorithm 
 *	has a worst case computational time of O(n log n).
 *	Stable. <br>
 *	Note: This algorithm has the inconvenience of needing a temporary 
 *	array of the same size of the array to be sorted.
 *	
 *	@param X array to be sorted.
 *	@param n maximum index of X.
 *	@see mpass().
 */
  void msort ( Etype* X, int n ) const {
   Etype* Y = new Etype [n+1];	// auxiliary array
   int l = 1;			// size of subarrays being merged
   while ( l <= n ) {
     mpass ( X, Y, n, l );
     l *= 2;
     mpass ( Y, X, n, l );	// interchange role of X and Y
     l *= 2;
   }
   delete[] Y;
  }

 /** merges two sorted arrays.
  *	Arrays (Xl, ..., Xm) and (Xm+1, ...,Xn)
  *	are merged to obtain the sorted array (Zl, ..., Zn).
  *	
  *	@param X area holding two sorted arrays.
  *	@param l beginning of the first array.
  *	@param m end of the first array.
  *	@param n end of the second array.
  *	@param Z merged array.
  */
  void merge ( const Etype* X, int l, int m, int n, Etype* Z ) const {
   int i = l;
   int k = l;
   int j = m + 1;
   
   while ( i <= m && j <= n ) {
     Z[k++] = ( !compare ( X[j], X[i] ) ? X[i++] : X[j++] );
   }
    
   if ( i > m ) {
     for ( ; j <= n; j++ ) Z[k++] = X[j];
   }
   else {
     for ( ; i <= m; i++ ) Z[k++] = X[i];
   }
  }

 /** performs one pass of merge sort. It merges
  *	adjacent pairs of subarrays of length l from X to Y.
  *	
  *	@param X source array.
  *	@param Y target array.
  *	@param n maximum index of X.
  *	@param l length of subarrays of X.
  *	@see merge().
  */
  void mpass ( const Etype* X, Etype* Y, int n, int l ) const {
   int i = 0;
    
   while ( i < n-2*l+1 ) {
     merge ( X, i, i+l-1, i+2*l-1, Y );
     i = i + 2*l;
   }
   if ( i+l-1 < n ) 
     // merge remaining subarray of length < 2l
     merge ( X, i, i+l-1, n, Y );
   else
     // could not form a pair (n<=i+l-1). Just append the remaining of the array.
     for ( ; i <= n; i++ ) Y[i] = X[i];
  }

  /// returns left child of node i.
  int LCHILD ( int i ) const { return 2*i+1; }

  /// returns right child of node i.
  int RCHILD ( int i ) const { return 2*i+2; }

  /// returns parent of node i.
  int PARENT ( int i ) const { return (i-1)/2; }

  /// swaps the records of X at positions i and j.
  void SWAP ( Etype* X, int i, int j ) const {
       if ( i != j ) {
          Etype T = X[i]; 
          X[i] = X[j]; 
          X[j] = T;
       }
  } 

 /** heap sort. The array R is interpreted as a tree, 
  *	with root at 1.
  *	The parent node of a node at location i is at i/2.
  *	The left child is at 2i and the right child at 2i+1.
  *	If 2i or 2i+1 is greater than n, then the corresponding
  *	children does not exist. This algorithm has a worst case
  *	computational time of O (n log n). Unstable. <br>
  *	Note: since the first index (root) is 0, the parent of a node 
  *	is at (i-1)/2, the left child is at 2i+1 and right child at 2i+2.
  *	
  *	@param R array to be sorted.
  *	@param n maximum index of R.
  *	@see adjust().
  */
  void hsort ( Etype* R, int n ) const {
   mheap ( R, n ); 	// convert R into a heap
   sheap ( R, n ); 	// sort the heap
  }

 /** quick sort. Sorts array X=(Xm, ..., Xn) into nondecreasing order on key K.
  *	Key Km is arbitrarily chosen as the control key. Pointers i and j
  *	are used to partition the subarray so that at any time Kl <= k, l < i
  *	and Kl >= K, l > j. It is assumed that Km <= Kn+1.
  *	The average computational time is O(n log n), and the worst case is 
  *	O (n**2). Unstable. <br>
  *	Note: This algorithm has the best average behavior 
  *	among all the sorting methods implemented here.
  *	
  *	@param X array to be sorted.
  *	@param m first index of the subarray.
  *	@param n last index of the subarray.
  *	@see choosePivot().
  */
  void qsort ( Etype* X, int m, int n ) const {
   if ( m < n ) 
     {
       int i = m; int j = n+1; 

       choosePivot ( X, m, n );
       Etype R = X[m];

       while ( 1 ) 		// partition the array
	 {
           do
	     ++i;
           while ( compare ( X[i], R ) );  // until X[i] >= R
           do
	     --j;
           while ( compare ( R, X[j] ) );  // until X[j] <= R
           if ( i < j ) {
	     SWAP ( X, i, j );
	   }
           else
	     break;
	 }
       SWAP ( X, m, j );	// move the pivot to its final position
       qsort ( X, m, j-1 );
       qsort ( X, j+1, n );  
     }
  } 

 /** inserts record R with key K into an ordered sequence.
  *	Array X = (X0, X1, ..., Xi) will be also ordered on key K, 
  *	after the insertion.
  *	
  *	@param R record to be inserted.
  *	@param X ordered array.
  *	@param i maximum index of X.
  */
  void insert ( const Etype& R, Etype* X, int i ) const {
   int j = i;

   while ( j >= 0 && compare ( R, X[j] ) ) {
     X[j+1] = X[j]; 	// move Rj one space up as R is to be inserted left of Xj
     --j;
   }

   X[j+1] = R;
  }


 /** insertion sort. Sorts the records (X0, X1, ..., Xn) in
  *	nondecreasing value of the key K. This algorithm has 
  *	a worst case computational time of O(n**2). Stable. <br>
  *	Note: It is a good algorithm for n <= 20-25.
  *	
  *	@param X array to be sorted.
  *	@param n maximum index of X.
  *	@see insert().
  */
  void insort ( Etype* X, int n ) const {
   Etype T;
   for ( int j = 1; j <= n; ++j )
     {
       T = X[j];
       insert ( T, X, j-1 );	// inserts records R0 to Rn
     }
  }
 

 /** executes the 3-way partition. Array X=(Xp, ..., Xr)
  *	is partioned in three pieces: {X[p..q-1], x, X[q+1,..r]}.
  *	
  *	@param X given array.
  *	@param p first index of the array.
  *	@param r last index of the array.
  *	@return final position of the pivot.
  */
  int partition ( Etype* X, int p, int r ) const {
   Etype x = X[p];	// pivot in X[p]
   int q = p;

   for ( int s = p+1; s <= r; ++s ) 
     if ( compare ( X[s], x ) )
       {
	 ++q;
	 SWAP ( X, q, s );
       }
   SWAP ( X, p, q );	// put the pivot into final position
   return q;		// return location of the pivot
  }


 /** chooses the pivot element. The pivot is put at the first position
  *	of array X=(Xm, ..., Xn)..
  *	
  *	@param X given array.
  *	@param m first index of the array.
  *	@param n last index of the array.
  */
  void choosePivot ( Etype* X, int m, int n ) const {
   int size = n-m+1;
   if ( size > 4 )	// get a random pivot
     { 
       int pivot = m+(int) ((float) size*rand()/(RAND_MAX+1.0));
       SWAP ( X, pivot, m ); 
     }
  }
    
 /// comparison function object.
 Cmp compare;

};

#include "utlIteratorBase.h"

#if !defined(__GNUC__) || !defined(USE_STL) // gnu g++ has an extended version of STL

namespace cgcUtilExt {

/**	returns true if the range [first, last) is a heap, and false otherwise.
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 */
template <class RandomAccessIterator>
inline bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      return SI.iheap ( &(*first), last-first-1 ); 
}

} // namespace cgcUtilExt

#endif

#ifndef USE_STL

namespace cgcUtil {

/** 	sorts the elements in [first, last) into ascending order. 
 *	This means that if i and j are any two valid iterators in [first, last) 
 *	such that i precedes j, then *j is not less than *i. <br> 
 *	Note: sort is not guaranteed to be stable. 
 *	That is, suppose that *i and *j are equivalent: neither one
 *	is less than the other. It is not guaranteed that the relative order 
 *	of these two elements will be preserved by sort. 
 *
 *	@param first beginning of the sequence to be sorted.
 *	@param last end of the sequence to be sorted. 
 */ 
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      SI.sort ( &(*first), last-first, utlSorter<value_type>::QSORT ); 
}


/** 	this version compares objects using a function object comp. 
 * 
 *	@param first beginning of the sequence to be sorted.
 *	@param last end of the sequence to be sorted. 
 *	@param comp comparison function object.
 *	@see cgcUtil::sort(). 
 */
template <class RandomAccessIterator, class StrictWeakOrdering>
inline void sort(RandomAccessIterator first, RandomAccessIterator last,
                 StrictWeakOrdering comp) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI (comp);
      SI.sort ( &(*first), last-first, utlSorter<value_type>::QSORT ); 
}


/**	sorts the elements in [first, last) into ascending order. 
 *	stable_sort() is much like sort().
 *	This means that if i and j are any two valid
 *	iterators in [first, last) such that i precedes j, then *j is not less than *i. 
 *	Stable_sort differs from sort in two ways. 
 *	First, stable_sort uses an algorithm that has different run-time complexity than sort. 
 *	Second, as the name suggests, stable_sort is stable: it
 *	preserves the relative ordering of equivalent elements. 
 *	That is, if x and y are elements in [first, last) such that x precedes y, 
 *	and if the two elements are equivalent (neither x < y nor y < x) then a 
 *	postcondition of stable_sort is that x still precedes y.  
 *
 *	@param first beginning of the sequence to be sorted.
 *	@param last end of the sequence to be sorted. 
 */ 
template <class RandomAccessIterator>
inline void stable_sort(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      SI.sort ( &(*first), last-first, utlSorter<value_type>::MSORT ); 
}


/** 	this version compares objects using a function object comp. 
 * 
 *	@param first beginning of the sequence to be sorted.
 *	@param last end of the sequence to be sorted. 
 *	@param comp comparison function object.
 *	@see cgcUtil::stable_sort(). 
 */
template <class RandomAccessIterator, class StrictWeakOrdering>
inline void stable_sort(RandomAccessIterator first, RandomAccessIterator last,
                        StrictWeakOrdering comp) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI (comp);
      SI.sort ( &(*first), last-first, utlSorter<value_type>::MSORT ); 
}


/**	partially orders a range of elements. 
 *	nth_element is similar to partial_sort, in that  
 *	it arranges the range [first, last) such that the
 *	element pointed to by the iterator nth is the same as the element that 
 *	would be in that position if the entire range [first, last) had been sorted.
 *	Additionally, none of the elements in the range [nth, last) is less than 
 *	any of the elements in the range [first, nth)
 *
 *	@param first beginning of the sequence.
 *	@param nth given iterator.
 *	@param last end of the sequence. 
 */
template <class RandomAccessIterator>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                        RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      int n = 1;
      for ( RandomAccessIterator it = first; it != last; ++it ) 
            if ( *it < *nth ) ++n;
      SI.qselect ( &(*first), 0, last-first-1, n ); 
}


/**	turns the range [first, last) into a heap. 
 *
 *	A heap is a particular way of ordering the elements in a range of 
 *	Random Access Iterators [f, l). The reason heaps are useful 
 *	(especially for sorting, or as priority queues) is that they satisfy 
 *	two important properties. First, *f is the largest element in the heap. 
 *	Second, it is possible to add an element to a heap
 *	(using push_heap), or to remove *f, in logarithmic time. 
 *	Internally, a heap is simply a tree represented as a sequential range. 
 *	The tree is constructed so that that each node is less than or equal to 
 *      its parent node. 
 *
 *	Compares objects using operator<(), and
 *	the postcondition is that is_heap(first, last) is true. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 *	@see cgcUtil::is_heap().
 */
template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      SI.mheap ( &(*first), last-first-1 ); 
}


/**	turns a heap [first, last) into a sorted range. 
 *	Note that this is not a stable sort: the relative order of equivalent 
 *	elements is not guaranteed to be preserved. 
 *
 *	@param first beginning of the range.
 *	@param last end of the range.
 */
template <class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      SI.sheap ( &(*first), last-first-1 ); 
}


/**	adds an element to a heap. 
 *	It is assumed that [first, last - 1) is already a heap; 
 *	the element to be added to the heap is *(last - 1). 
 *
 *	@param first beginning of the heap.
 *	@param last end of the heap.
 */
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      SI.mheap ( &(*first), last-first-1 ); 
}


/**	removes the largest element (that is, *first) from the heap [first, last). 
 *
 * 	removes the largest element from a heap, and shrinks the heap. 
 *	This means that if you keep calling pop_heap until only a single element
 *	is left in the heap, you will end up with a sorted range where the heap used to be. 
 *	This, in fact, is exactly how sort_heap is implemented. 
 *
 *	@param first beginning of the heap.
 *	@param last end of the heap.
 */
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
      typedef typename cgcUtil::iterator_traits<RandomAccessIterator>::value_type value_type;
      utlSorter<value_type> SI;
      value_type ft = *first;
      *first = *--last;
      *last = ft; 
      SI.adjust ( &(*first), 0, last-first-1 ); 
}

} // namespace cgcUtil


#endif

#endif


