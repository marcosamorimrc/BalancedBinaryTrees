/*
   utlVector.h

   Resizable vector.
	
  Program:   utl
  Module:    $RCSfile: utlVector.h,v $
  Language:  C++
  Date:      $Date: 2005/10/09 22:40:44 $
  Version:   $Revision: 1.2 $

   Date: 30 July 1998.
*/

#ifndef __UTLVECTOR_H__
#define __UTLVECTOR_H__

#include "utlAlgo.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#define utlVector vector
#else

#include "utl.h"
#include "utlIteratorBase.h"
#include "utlReverseIterator.h"
#include "utlAllocator.h"


/** A simpler implementation of STL class vector.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 *	A vector is a Sequence that supports random access to elements, 
 *	constant time insertion and removal of elements at the end, 
 *	and linear time insertion and removal of elements at the 
 *	beginning or in the middle. 
 *	The number of elements in a vector may vary dynamically; 
 *	memory management is automatic. Vector is the simplest of the 
 *	STL container classes, and in many cases the most efficient. 
 */
template <class Etype, class A = utlAllocator<Etype> > class utlVector {

public:	

  /// value type.
  typedef Etype value_type;

  /// pointer type.
  typedef Etype* pointer;

  /// pointer type.
  typedef const Etype* const_pointer;

  /// reference type.
  typedef Etype& reference;

  /// const_reference type.
  typedef const Etype& const_reference;

  /// size type.
  typedef size_t size_type;

  /// pointer difference type.
  typedef ptrdiff_t difference_type;

  /// allocator type.
  typedef A allocator_type;

  /// forward declaration of iterator.
  class iterator;

  /// forward declaration of const_iterator.
  class const_iterator;

  /// forward declaration of reverse_iterator.
  typedef utl_reverse_iterator <iterator> reverse_iterator;

  /// forward declaration of const_reverse_iterator.
  typedef utl_reverse_iterator <const_iterator> const_reverse_iterator;

  /** @name constructors  */ 
  //@{

  /**	constructor: allocates the vector.
   *	size() is set to n.
   *	
   *	@param n initial capacity of the vector.
   *	@param a an allocator.
   */
  utlVector ( size_type n = 0, const allocator_type& a = allocator_type() ) : 
							         buffer_size(0), 
							         n_entries(0), 
					                         ebuffer(NULL), 
								 alloc(a) {
    resize ( n ); 
  }


  /**	constructor: creates a vector with n copies of val.
   *	size() is set to n.
   *	
   *	@param n initial capacity of the vector.
   *	@param val initial value for all positions.
   *	@param a an allocator.
   */
  utlVector ( size_type n, const Etype& val, const allocator_type& a = allocator_type() ) : 
								  buffer_size(0),
							          n_entries(0),
								  ebuffer(NULL),
								  alloc(a) {
    resize ( n, val ); 
  }


  /**	copy constructor.
   *	
   *	@param irt given vector.
   */
  utlVector ( const utlVector& irt ) : buffer_size(0), n_entries(0), 
				       ebuffer(NULL), alloc(irt.alloc) {
    *this = irt;
  }

  /// constructor as a copy range.
  template <class InputIterator>
  utlVector(InputIterator first, InputIterator last) : buffer_size(0), 
						       n_entries(0),
						       ebuffer(NULL), 
						       alloc(allocator_type()) {
    resize ( cgcUtil::distance ( first, last ) );
    cgcUtil::copy ( first, last, begin() ); 
  }

  /// destructor: deletes this vector.
  ~utlVector ()  { 
    destroy ( this->ebuffer, size() ); 
    alloc.deallocate (this->ebuffer,capacity()); 
    this->ebuffer = NULL; 
    this->buffer_size = 0; 
  }

  //@}


  /**	reserves an amount of memory for this vector.
   *	If n is less or equal to capacity(),
   *	this call has no effect. Otherwise, it is
   *	a request for allocation of additional memory.
   *	If the request is successful, then capacity() is 
   *	greater than or equal to n; otherwise, 
   *	capacity() is unchanged. In either case, size()
   *	is unchanged.
   *	
   *	@param n number of positions to be reserved.
   */
  void reserve ( size_type n ) { 
    if ( n > capacity() ) getMemory ( n ); 
  }


  /**	resizes this vector, by inserting or erasing elements
   *	at the end.
   *	If n is less than capacity(),
   *	this vector is shrunk and all positions
   *	from this->begin() + n to this->end() are lost.
   *	Otherwise, additional positions are created
   *	and initialized to the given value. Anyway, size() = n.
   *	
   *	@param n new vector capacity.
   *	@param val initial value.
   */
  void resize ( size_type n, const Etype& val = Etype() ) { 
    if ( n == capacity() ) return;

    size_type old_size = size();
    getMemory ( n );
    pointer p = this->ebuffer + old_size;
    while ( p < this->ebuffer+n ) 
       alloc.construct ( p++, val );

    this->n_entries = n;
  }


  /**	appends a new element at the end of this vector,
   *	which is resized if necessary.
   *	
   *	@param elem element to be inserted at the end.
   */
  void push_back ( const Etype& elem ) {
    if ( size()+1 >= capacity() ) 
         reserve ( MAX(2*capacity(),(size_type)16) );
    this->ebuffer[this->n_entries++] = elem;
  }


  /**	removes the last element at the end of this vector.
   *	
   */
  void pop_back ( ) { 
    if ( size() > 0 )
      --this->n_entries;
  }


  /**	returns the current size of this vector.
   *	
   *	@return number of entries.	
   */
  size_type size ( ) const { return this->n_entries; }

  /// returns this vector capacity (how much memory is allocated).
  size_type capacity() const { return this->buffer_size; }

  /**	returns the maximum possible size of a vector.
   *	
   *	@return number of bytes.	
   */
  size_type max_size ( ) const { return (size_type)1073741823; }

  /// returns whether this vector is empty.
  bool empty ( ) const { return size() <= 0; }

  /// resets this vector. size() is set to 0.
  void clear ( ) { this->n_entries = 0; }

  /**	returns the i-th entry in this vector for reading.
   *	
   *	@param i an index.
   *	@return the i-th element into this vector.
   */
  const_reference operator [] ( size_type i ) const  { 
#ifdef __DEBUG__
    if ( i >= size() ) {
      cerr << "utlVector - operator []: out of range access has ";
      cerr << "been detected. " << i << endl;
      return front(); 
    }
#endif
 
    return this->ebuffer[i]; 
  }

  /**	returns the i-th entry in this vector for writing.
   *	
   *	@param i an index.
   *	@return the i-th element into this vector.
   */
  reference operator [] ( size_type i ) { 
#ifdef __DEBUG__
    if ( i >= size() )  {
      cerr << "utlVector -- operator []: out of range access has ";
      cerr << "been detected. " << i << endl;
      return front(); 
    }
#endif

    return this->ebuffer[i]; 
  }

  /**	assignment operator.
   *	
   *	@return a reference to this utlVector.
   */
  utlVector& operator = ( const utlVector& rhs ) {
    if ( this == &rhs ) return *this; // same vector

    if ( capacity() < rhs.size() ) 
       reserve ( rhs.capacity() );

    pointer p = this->ebuffer;
    pointer q = rhs.ebuffer;
    while ( p < this->ebuffer+rhs.size() )
      alloc.construct (p++, *q++);
    this->n_entries = rhs.size();
 
    return *this;
  }

  /**	comparison operator. Tests two vectors for equality.
   *	
   *	@return v1 == v2.
   */
  friend bool operator == ( const utlVector<Etype,A>& v1, const utlVector<Etype,A>& v2 ) {
    if ( &v1 == &v2 ) return true; // same vector
    if ( v1.size() != v2.size() ) return false;

    return cgcUtil::equal ( v1.begin(), v1.end(), v2.begin() );
  } 

  /**	comparison operator. Tests two vectors for inequality.
   *	
   *	@return v1 != v2.
   */
  friend bool operator != ( const utlVector<Etype,A>& v1, const utlVector<Etype,A>& v2 ) {
    return !(v1 == v2);
  }

  /**	lexicographical comparison. 
   *	
   *	@return v1 < v2.
   */
  friend bool operator < ( const utlVector<Etype,A>& v1, const utlVector<Etype,A>& v2 ) {
    return cgcUtil::lexicographical_compare ( v1.begin(), v1.end(), v2.begin(), v2.end() );
  }

  /**	returns whether a given object is in this vector. 
   *	NOT in STL standard.
   *	
   *	@param rec object to be searched for. 
   *	@return 1 if the given object is in this vector, and 0 otherwise.
   *	
   */
  bool isFound ( const Etype& rec ) const {
    return ( cgcUtil::find ( begin(), end(), rec ) != end() );
  }

  /** 	swaps the contents of this and a given vector.
   *
   *	@param v given vector.
   */
   void swap ( utlVector& v ) {
     pointer buf = this->ebuffer;
     this->ebuffer = v.ebuffer;
     v.ebuffer = buf;

     int temp = this->n_entries;
     this->n_entries = v.n_entries;
     v.n_entries = temp;

     temp = this->buffer_size;
     this->buffer_size = v.buffer_size;
     v.buffer_size = temp;
   }

  /** 	output operator. Prints this vector. 
   *	
   *	@param os file stream.
   *	@param v given vector.
   *	@return file stream. 
   */
  friend ostream& operator<<(ostream& os, const utlVector<Etype,A>& v) {
    os << "Vector Contents: ";
    if ( v.empty() )
      os << "Empty Vector" << endl;
    else {
      os << endl; 
      for ( size_type i=0; i < v.size(); ++i) {
	os << v.ebuffer[i] << endl; 
      }
    }

    return os; 
  }

  /// iterator for vectors.
  class iterator : public utlIteratorBase<iterator> {
  public:

    /// iterator category.
    typedef typename utlIteratorBase<iterator>::random_access_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlVector<Etype,A>::value_type value_type;
    /// type of difference between two iterators.
    typedef typename utlVector<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlVector<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlVector<Etype,A>::reference reference;

    /** @name constructors  */ 
    //@{
    /// empty constructor.
    iterator () : current(0), vec(0) { }
	
    /// copy constructor.
    iterator ( const iterator& itr ) : 
      current(itr.current), vec(itr.vec) { }

    /// contructor from a current position and a vector.
    iterator (int pos, utlVector* v) : 
      current(pos), vec(v) {}

    ///	destructor: does nothing.
    ~iterator ( ) { }
    //@}

    /** assignment operator. Only effective if both iterators come from the 
     *	same vector. 
     */
    iterator& operator = ( const iterator& ritr ) { 
      this->current = ritr.current;
      this->vec = ritr.vec;
      return *this;
    }

    /// prefix increment operator. Increments the current pointer.
    iterator& operator ++ ( ) { 
      if ( this->current < (int)vec->size() ) this->current++; 
      return *this;
    }

    /// posfix increment operator. Increments the current pointer.
    iterator operator ++ ( int ) {
      iterator tmp = *this;
      ++*this;
      return tmp; 
    }

    /// prefix decrement operator. Decrements the current pointer.
    iterator& operator -- ( ) { 
      if ( this->current >= 0 ) this->current--;
      return *this;
    }

    /// posfix decrement operator. Decrements the current pointer.
    iterator operator -- ( int ) { 
      iterator tmp = *this;
      --*this;
      return tmp; 
    }

    /// iterator addition. 
    iterator operator + ( difference_type distance ) const { 
      int pos = this->current + distance;
      int sz = (int)vec->size();
      if ( pos > sz ) pos = sz;
      else if ( pos < 0 ) pos = -1;
      return iterator ( pos, vec ); 
    }

    /// forward motion. 
    iterator& operator += ( difference_type distance ) { 
      int pos = this->current + distance;
      int sz = (int)vec->size();
      if ( pos > sz ) pos = sz;
      else if ( pos < 0 ) pos = -1;
      this->current = pos;
      return *this;
    }

    /// iterator subtraction. 
    iterator operator - ( difference_type distance ) const { 
      return operator + ( -distance ); 
    }

    /// backward motion. 
    iterator& operator -= ( difference_type distance ) { 
      return operator += ( -distance );
    }

    /**	element operator. i[n] is equivalent to *(i + n).
     *	
     *	@param n an index.
     *	@return the n-th element into the vector having this iterator as origin.
     */
    const_reference operator [] ( size_type n ) const  { 
      return *(operator + (n)); 
    }

    /**	element assignment. i[n] = t is equivalent to *(i + n) = t. 
     *	
     *	@param n an index.
     *	@return the n-th element into the vector having this iterator as origin.
     */
    reference operator [] ( size_type n ) { 
      return *(operator + (n)); 
    }

    /// dereference operator. Returns a reference to the element in the current position.
    reference operator * ( ) { return (*vec)[this->current]; }

    /// dereference operator. Returns a reference to the element in the current position.
    const_reference operator * ( ) const { return (*vec)[this->current]; }

    /// dereference operator. Returns a pointer to the element in the current position.
    pointer operator->() { return &(operator*()); }

  protected:
    /// const_iterator is a friend class.
    friend class const_iterator;
    /// utlVector is a friend class.
    friend class utlVector<Etype,A>;
    /// < is a friend operator.
    friend bool operator < ( const iterator& i, const iterator& j ); 
    /// > is a friend operator.
    friend bool operator > ( const iterator& i, const iterator& j ); 
    /// <= is a friend operator.
    friend bool operator <= ( const iterator& i, const iterator& j );
    /// == is a friend operator.
    friend bool operator == ( const iterator& itr1, const iterator& itr2);
    /// != is a friend operator.
    friend bool operator != ( const iterator& itr1, const iterator& itr2);
    /// - is a friend operator.
    friend difference_type operator - ( const iterator& i, const iterator& j );
    /// points to the current position
    int current;
    /// a pointer to the vector being iterated.
    utlVector* vec;
  }; 	// iterator
    
  /// returns a number n such that i == j + n.
  friend inline difference_type operator - ( const iterator& i, const iterator& j ) { 
      return (i.current - j.current);
  }

  /// returns whether i is reachable from j. 
  friend inline bool operator < ( const iterator& i, const iterator& j ) { 
      return (i.current < j.current); 
  }

  /// returns whether j is reachable from i. 
  friend inline bool operator > ( const iterator& i, const iterator& j ) { 
      return (i.current > j.current); 
  }

  /// returns whether i is reachable from j. 
  friend inline bool operator <= ( const iterator& i, const iterator& j ) { 
      return (i.current <= j.current); 
  }

  /// returns whether the two given iterators point to the same element. 
  friend inline bool operator == ( const iterator& itr1, const iterator& itr2) {
      if (itr1.vec != itr2.vec) return false;
      return ( itr1.current == itr2.current );
  }

  /// returns whether the two given iterators point to different elements. 
  friend inline bool operator != ( const iterator& itr1, const iterator& itr2) {
      return !(itr1 == itr2);
  }

  /// constant iterator for vectors.
  class const_iterator :  public utlIteratorBase<const_iterator> {
  public:

    /// iterator category.
    typedef typename utlIteratorBase<const_iterator>::random_access_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlVector<Etype,A>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlVector<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlVector<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlVector<Etype,A>::const_reference reference;

    /** @name constructors  */ 
    //@{
    /// empty constructor.
    const_iterator () : current(0), vec(0) { }
	
    /// copy constructor.
    const_iterator ( const const_iterator& itr ) : 
      current(itr.current), vec(itr.vec) { }

    /// constructor from an iterator.
    const_iterator ( const iterator& itr ) : 
      current(itr.current), vec(itr.vec) { }

    /// contructor from a current position and a vector.
    const_iterator (int pos, const utlVector* v) : 
      current(pos), vec(v) {}

    ///	destructor: does nothing.
    ~const_iterator ( ) { }
    //@}


    /// prefix increment operator. Increments the current pointer.
    const_iterator& operator ++ ( ) const { 
      int* curr = (int*)&current;
      if ( *curr < (int)vec->size() ) (*curr)++;  
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix increment operator. Increments the current pointer.
    const_iterator operator ++ ( int ) const {
      const_iterator tmp = *this;
      ++*this;
      return tmp; 
    }

    /// prefix decrement operator. Decrements the current pointer.
    const_iterator& operator -- ( ) const { 
      int* curr = (int*)&current;
      if ( *curr >= 0 ) (*curr)--;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix decrement operator. Decrements the current pointer.
    const_iterator operator -- ( int ) const { 
      const_iterator tmp = *this;
      --*this;
      return tmp; 
    }

    /// const_iterator addition. 
    const_iterator operator + ( difference_type distance ) const { 
      int pos = this->current + distance;
      int sz = (int)vec->size();
      if ( pos > sz ) pos = sz;
      else if ( pos < 0 ) pos = -1;
      return const_iterator ( pos, vec ); 
    }

    /// forward motion. 
    const_iterator& operator += ( difference_type distance ) { 
      this->pos = this->current + distance;
      int sz = (int)vec->size();
      if ( this->pos > sz ) this->pos = sz;
      else if ( this->pos < 0 ) this->pos = -1;
      int* curr = (int*)&current;
      *curr = this->pos;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// const_iterator subtraction. 
    const_iterator operator - ( difference_type distance ) const { 
      return operator + ( -distance ); 
    }

    /// backward motion. 
    const_iterator& operator -= ( difference_type distance ) { 
      return operator += ( -distance );
    }

    /**	element operator. i[n] is equivalent to *(i + n).
     *	
     *	@param n an index.
     *	@return the n-th element into the vector having this const_iterator as origin.
     */
    const_reference operator [] ( size_type n ) const  { 
      return *(operator + (n)); 
    }
    
    /** assignment operator. Only effective if both const_iterators come from the 
     *	same vector. 
     */
    const_iterator& operator = ( const const_iterator& ritr ) const { 
      int* curr = (int*)&current;
      *curr = ritr.current;
      utlVector** vec_p = (utlVector**)&vec;
      *vec_p = (utlVector*)ritr.vec;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// dereference operator. Returns a reference to the element in the current position.
    const_reference operator * ( ) const { return (*vec)[this->current]; }

    /// dereference operator. Returns a pointer to the element in the current position.
    pointer operator->() { return &(operator*()); }
    /// dereference operator. Returns a constant pointer to the element in the current position.
    const_pointer operator->() const { return &(operator*()); }

  protected:
    /// - is a friend operator.
    friend difference_type operator - ( const const_iterator& i, const const_iterator& j ); 
    /// < is a friend operator.
    friend bool operator < ( const const_iterator& i, const const_iterator& j ); 
    /// <= is a friend operator.
    friend bool operator <= ( const const_iterator& i, const const_iterator& j ); 
    /// == is a friend operator.
    friend bool operator == ( const const_iterator& itr1, const const_iterator& itr2);
    /// != is a friend operator.
    friend bool operator != ( const const_iterator& itr1, const const_iterator& itr2);
    /// points to the current position
    int current;
    /// a pointer to the vector being iterated.
    const utlVector* vec;
  }; 	// const_iterator
    
  /// returns a number n such that i == j + n.
  friend inline difference_type operator - ( const const_iterator& i, const const_iterator& j ) { 
      return (i.current - j.current);
  }

  /// returns whether i is reachable from j. 
  friend inline bool operator < ( const const_iterator& i, const const_iterator& j ) { 
      return (i.current < j.current); 
  }

  /// returns whether i is reachable from j. 
  friend inline bool operator <= ( const const_iterator& i, const const_iterator& j ) { 
      return (i.current <= j.current); 
  }

  /// returns whether the two given const_iterators point to the same element. 
  friend inline bool operator == ( const const_iterator& itr1, const const_iterator& itr2) {
      if (itr1.vec != itr2.vec) return false;
      return ( itr1.current == itr2.current );
  }

  /// returns whether the two given const_iterators point to different elements. 
  friend inline bool operator != ( const const_iterator& itr1, const const_iterator& itr2) {
      return !(itr1==itr2);
  }

  /*========= methods using the iterator class ==============*/

  /// returns an iterator pointing to the beginning of the vector.
  iterator begin() { 
    if (empty()) return end();
    return iterator(0, this); 
  }

  /// returns an iterator pointing to the end of the vector.
  iterator end() { return iterator(n_entries, this); } 

  /// returns a const_iterator pointing to the beginning of the vector.
  const_iterator begin() const { 
    if (empty()) return end();
    return const_iterator(0, this); 
  }

  /// returns a const_iterator pointing to the end of the vector.
  const_iterator end() const { return const_iterator(n_entries, this); } 

  /// returns a reverse_iterator pointing to the beginning of the reversed vector.
  reverse_iterator rbegin() { 
    return reverse_iterator(end()); 
  }

  /// returns a reverse_iterator pointing to the end of the reversed vector.
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /// returns a const_reverse_iterator pointing to the beginning of the reversed vector.
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }

  /// returns a const_reverse_iterator pointing to the end of the reversed vector.
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }


  /**	removes an object given in an iterator from this vector. 
   *	The size() of the vector is changed, and the elements after
   *	the erased element are copied into the freed positions.
   *	An iterator pointing to an element of this vector after
   *	erase() may point to another element or to no element at all. 
   *	
   *	@param it given iterator.
   *	@return successor of the removed element.
   *	
   */
  iterator erase ( iterator it ) {
    if ( it == this->end() ) return it;

    iterator pos = it;

    for ( iterator i = it; ++it != this->end(); ++i )
          *i = *it;
    
    --this->n_entries;

    return ++pos;
  }

  /**	erases the range [first, last).
   *  	Destroys the elements in the range [first,last) and removes them from the vector.
   *	size() is decremented by the distance from first to last. The relative order
   *    of the other elements in the sequence is unchanged. The return value is an
   *    iterator to the element immediately following the ones that were erased. 
   *	
   *	@param first beginning of the range.
   *	@param last end of the range.
   *	@return successor of the removed elements.
   */
  iterator erase ( iterator first, iterator last ) {
    if ( last <= first ) return last;

    difference_type distance = last - first;
    iterator i = first;
    for ( ; last != this->end(); ++first, ++last )
          *first = *last;
    
    this->n_entries -= distance;

    return i;
  }

  /**	inserts an object before an element in a given iterator. 
   *	The size() of the vector is incremented by 1, and the elements after
   *	the inserted element are shifted one position to the right.
   *    The relative order of elements already in the sequence is unchanged. 
   *	An iterator pointing to an element of this vector after
   *	insert() may point to another element or to no element at all. 
   *	
   *	@param pos given iterator.
   *	@param rec given element.
   *	@return iterator to the inserted element.
   */
  iterator insert ( iterator pos, const Etype& rec ) {
    this->push_back ( this->back() ); 
    for ( int j = size()-2; j != pos.current-1; --j )
          ebuffer[j+1] = ebuffer[j];

    ebuffer[pos.current] = rec;

    return pos;
  }

  /**	inserts the range [first, last) before pos. 
   *	The size() of the vector is incremented by the distance from i to j, 
   *    and the elements after the inserted elements are shifted to the right.
   *    The relative order of elements already in the sequence is unchanged. 
   *	An iterator pointing to an element of this vector after
   *	insert() may point to another element or to no element at all. 
   *	
   *	@param pos given iterator.
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator> void insert(iterator pos,
                                             InputIterator f, InputIterator l) {
    difference_type dist = cgcUtil::distance ( f, l );
    reserve ( size() + dist );
    for ( int j = size()-1; j != pos.current-1; --j )
          ebuffer[j+dist] = ebuffer[j];

    this->n_entries += dist;
    cgcUtil::copy ( f, l, pos );
  }

  /**	inserts n copies of x before pos. 
   *
   *	@param pos given iterator.
   *	@param n number of copies.
   *	@param x value to be copied.
   */
  void insert(iterator pos, size_type n, const Etype& x) {
    reserve ( size() + n );
    for ( int j = size()-1; j != pos.current-1; --j )
          ebuffer[j+n] = ebuffer[j];

    this->n_entries += n;
    for ( size_type i = pos.current; i < pos.current+n; ++i )
          ebuffer[i] = x;
  }

  /// returns the first element for writing.
  reference front () { return ebuffer[0]; }

  /// returns the first element for reading.
  const_reference front () const { return ebuffer[0]; }

  /// returns the last element for writing.
  reference back ()  { return ebuffer[n_entries-1]; }

  /// returns the last element for reading.
  const_reference back () const { return ebuffer[n_entries-1]; }

 private:

  /// destroys the contents of a buffer.
  void destroy ( pointer buf, size_type n ) {
#if 0
     if ( buf == NULL ) return;
     pointer p = buf;
     while ( p < buf+n )
             alloc.destroy (p++);
#endif
  }

  /**	gets a new amount of memory for this vector.
   *	
   *	@param n number of positions to be acquired
   */
  void getMemory ( size_type n ) { 
      pointer p = alloc.allocate ( n );
      pointer q = this->ebuffer, v = p;

      size_type npos = MIN(size(),n);
      while ( q < this->ebuffer+npos ) 
         alloc.construct ( p++, *q++ );

      destroy ( this->ebuffer, size() );
      alloc.deallocate ( this->ebuffer, capacity() );
      this->ebuffer = v;
      this->buffer_size = n;
  }


  /// vector dimension.
  size_type buffer_size;

  /// number of elements into the vector.
  size_type n_entries;

  /// array for holding Etype elements.
  pointer ebuffer;

  /// an allocator object.
  allocator_type alloc;
};	// utlVector

#endif // USE_STL
#endif // __UTLVECTOR_H__

