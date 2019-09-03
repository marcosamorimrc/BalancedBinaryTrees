/*
   utlDeque.h

   Double ended queue - deque.
	
  Program:   utl
  Module:    $RCSfile: utlDeque.h,v $
  Language:  C++
  Date:      $Date: 2005/10/09 22:40:44 $
  Version:   $Revision: 1.2 $

   Date: 07 August 2001.
*/

#ifndef __UTLDEQUE_H__
#define __UTLDEQUE_H__

#include "utlAlgo.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#define utlDeque deque
#else

#include "utl.h"
#include "utlIteratorBase.h"
#include "utlReverseIterator.h"
#include "utlAllocator.h"


/** A simpler implementation of STL class deque.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2005/10/09 22:40:44 $
 *	
 *	A deque is very much like a vector: like vector, it is a sequence that 
 *	supports random access to elements, constant time insertion and removal of elements
 *	at the end of the sequence, and linear time insertion and removal of elements in the middle. 
 *
 *	The main way in which deque differs from vector is that deque also supports 
 *	constant time insertion and removal of elements at the beginning of the sequence.
 *	Additionally, deque does not have any member functions analogous to vector's 
 *	capacity() and reserve(), and does not provide any of the guarantees
 *	on iterator validity that are associated with those member functions. 
 */
template <class Etype, class A = utlAllocator<Etype> > class utlDeque {

public:	

  /// value type.
  typedef Etype value_type;

  /// pointer type.
  typedef Etype* pointer;

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

  /**	constructor: allocates the deque.
   *	size() is set to n.
   *	
   *	@param n initial capacity of the deque.
   *	@param a an allocator.
   */
  utlDeque ( size_type n = 0, const allocator_type& a = allocator_type() ) : 
							buffer_size(0), 
							n_entries(0), 
						        inipos(0),
					                ebuffer(NULL), 
							alloc(a) {
    resize ( n ); 
  }


  /**	constructor: creates a deque with n copies of val.
   *	size() is set to n.
   *	
   *	@param n initial capacity of the deque.
   *	@param val initial value for all positions.
   *	@param a an allocator.
   */
  utlDeque ( size_type n, const Etype& val, const allocator_type& a = allocator_type() ) : 
								  buffer_size(0),
							          n_entries(0),
						                  inipos(0),
								  ebuffer(NULL),
								  alloc(a) {
    resize ( n, val ); 
  }


  /**	copy constructor.
   *	
   *	@param irt given deque.
   */
  utlDeque ( const utlDeque& irt ) : buffer_size(0), n_entries(0), inipos(0), 
				       ebuffer(NULL), alloc(irt.alloc) {
    *this = irt;
  }

  /// constructor as a copy range.
  template <class InputIterator>
  utlDeque(InputIterator first, InputIterator last) : buffer_size(0), 
						      n_entries(0),
						      inipos(0),
						      ebuffer(NULL), 
						      alloc(allocator_type()) {
    resize ( cgcUtil::distance ( first, last ) );
    cgcUtil::copy ( first, last, begin() ); 
  }

  /// destructor: deletes this deque.
  ~utlDeque ()  { 
    destroy ( this->ebuffer, this->inipos, size() ); 
    alloc.deallocate (this->ebuffer, capacity()); 
    this->ebuffer = NULL; 
    this->buffer_size = 0; 
    this->inipos = 0; 
  }

  //@}


  /**	resizes this deque, by inserting or erasing elements
   *	at the end.
   *	If n is less than capacity(),
   *	this deque is shrunk and all positions
   *	from this->begin() + n to this->end() are lost.
   *	Otherwise, additional positions are created
   *	and initialized to the given value. Anyway, size() = n.
   *	
   *	@param n new deque capacity.
   *	@param val initial value.
   */
  void resize ( size_type n, const Etype& val = Etype() ) { 
    if ( n == capacity() ) return;

    size_type old_size = size();
    getMemory ( n );
    pointer p0 = this->ebuffer;
    pointer p = p0 + this->inipos + old_size;
    for ( size_type i = old_size; i < n; ++i ) {
       if ( p >= p0+capacity() ) p = p0;
       alloc.construct ( p++, val );
    }

    this->n_entries = n;
  }


  /**	appends a new element at the end of this deque,
   *	which is resized if necessary.
   *	
   *	@param elem element to be inserted at the end.
   */
  void push_back ( const Etype& elem ) {
    if ( size()+1 >= capacity() ) 
         reserve ( MAX(2*capacity(),(size_type)16) );
    this->ebuffer[lastpos()] = elem;
    this->n_entries++;
  }


  /**	appends a new element at the beginning of this deque,
   *	which is resized if necessary.
   *	
   *	@param elem element to be inserted at the beginning.
   */
  void push_front ( const Etype& elem ) {
    if ( size()+1 >= capacity() ) 
         reserve ( MAX(2*capacity(),(size_type)16) );
    if ( !empty() ) {
      if ( !this->inipos ) 
          this->inipos = capacity()-1;
      else
          --this->inipos;
    }
    this->ebuffer[this->inipos] = elem;
    this->n_entries++;
  }


  /**	removes the last element at the end of this deque.
   *	
   */
  void pop_back ( ) { 
    if ( size() > 0 )
      --this->n_entries;
  }


  /**	removes the first element at the beginning of this deque.
   *	
   */
  void pop_front ( ) { 
    if ( size() > 0 ) {
      this->inipos = (this->inipos+1)%capacity();
      --this->n_entries;
    }
  }


  /**	returns the current size of this deque.
   *	
   *	@return number of entries.	
   */
  size_type size ( ) const { return this->n_entries; }

  /**	returns the maximum possible size of a deque.
   *	
   *	@return number of bytes.	
   */
  size_type max_size ( ) const { return (size_type)1073741823; }

  /// returns whether this deque is empty.
  bool empty ( ) const { return size() <= 0; }

  /// resets this deque. size() is set to 0.
  void clear ( ) { this->n_entries = 0; }

  /**	returns the i-th entry in this deque for reading.
   *	
   *	@param i an index.
   *	@return the i-th element into this deque.
   */
  const_reference operator [] ( size_type i ) const  { 
#ifdef __DEBUG__
    if ( i < 0 || i >= size() ) {
      cerr << "utlDeque - operator []: out of range access has ";
      cerr << "been detected. " << i << endl;
      return front(); 
    }
#endif
 
    return this->ebuffer[(this->inipos+i)%capacity()]; 
  }

  /**	returns the i-th entry in this deque for writing.
   *	
   *	@param i an index.
   *	@return the i-th element into this deque.
   */
  reference operator [] ( size_type i ) { 
#ifdef __DEBUG__
    if ( i < 0 || i >= size() )  {
      cerr << "utlDeque -- operator []: out of range access has ";
      cerr << "been detected. " << i << endl;
      return front(); 
    }
#endif

    return this->ebuffer[(this->inipos+i)%capacity()]; 
  }

  /**	assignment operator.
   *	
   *	@return a reference to this utlDeque.
   */
  utlDeque& operator = ( const utlDeque& rhs ) {
    if ( this == &rhs ) return *this; // same deque

    if ( capacity() < rhs.size() ) 
       reserve ( rhs.capacity() );

    pointer p = this->ebuffer;
    pointer q0 = rhs.ebuffer;
    pointer q = q0+rhs.inipos;
    for ( size_type i = 0; i < rhs.size(); ++i ) {
      if ( q >= q0+capacity() ) q = q0;
      alloc.construct (p++, *q++);
    }
    this->n_entries = rhs.size();
    this->inipos = 0;
 
    return *this;
  }

  /**	comparison operator. Tests two deques for equality.
   *	
   *	@return v1 == v2.
   */
  friend bool operator == ( const utlDeque<Etype,A>& v1, const utlDeque<Etype,A>& v2 ) {
    if ( &v1 == &v2 ) return true; // same deque
    if ( v1.size() != v2.size() ) return false;

    return cgcUtil::equal ( v1.begin(), v1.end(), v2.begin() );
  } 

  /**	comparison operator. Tests two deques for inequality.
   *	
   *	@return v1 != v2.
   */
  friend bool operator != ( const utlDeque<Etype,A>& v1, const utlDeque<Etype,A>& v2 ) {
    return !(v1 == v2);
  }

  /**	lexicographical comparison. 
   *	
   *	@return v1 < v2.
   */
  friend bool operator < ( const utlDeque<Etype,A>& v1, const utlDeque<Etype,A>& v2 ) {
    return cgcUtil::lexicographical_compare ( v1.begin(), v1.end(), v2.begin(), v2.end() );
  }

  /**	returns whether a given object is in this deque. 
   *	NOT in STL standard.
   *	
   *	@param rec object to be searched for. 
   *	@return 1 if the given object is in this deque, and 0 otherwise.
   *	
   */
  bool isFound ( const Etype& rec ) const {
    return ( cgcUtil::find ( begin(), end(), rec ) != end() );
  }

  /** 	swaps the contents of this and a given deque.
   *
   *	@param v given deque.
   */
   void swap ( utlDeque& v ) {
     pointer buf = this->ebuffer;
     this->ebuffer = v.ebuffer;
     v.ebuffer = buf;

     int temp = this->n_entries;
     this->n_entries = v.n_entries;
     v.n_entries = temp;

     temp = this->buffer_size;
     this->buffer_size = v.buffer_size;
     v.buffer_size = temp;

     temp = this->inipos;
     this->inipos = v.inipos;
     v.inipos = temp;
   }

  /** 	output operator. Prints this deque. 
   *	
   *	@param os file stream.
   *	@param v given deque.
   *	@return file stream. 
   */
  friend ostream& operator<<(ostream& os, const utlDeque<Etype,A>& v) {
    os << "Deque Contents: ";
    if ( v.empty() )
      os << "Empty Deque" << endl;
    else {
      os << endl; 
      os << v.inipos << " " << v.lastpos() << "\n";
      for ( size_type i=0; i < v.size(); ++i) {
	os << v[i] << " " << (v.inipos+i)%v.capacity() << endl; 
      }
    }

    return os; 
  }

  /// iterator for deques.
  class iterator : public utlIteratorBase<iterator> {
  public:

    /// iterator category.
    typedef typename utlIteratorBase<iterator>::random_access_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlDeque<Etype,A>::value_type value_type;
    /// type of difference between two iterators.
    typedef typename utlDeque<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlDeque<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlDeque<Etype,A>::reference reference;

    /** @name constructors  */ 
    //@{
    /// empty constructor.
    iterator () : current(0), vec(0) { }
	
    /// copy constructor.
    iterator ( const iterator& itr ) : 
      current(itr.current), vec(itr.vec) { }

    /// contructor from a current position and a deque.
    iterator (int pos, utlDeque* v) : 
      current(pos), vec(v) {}

    ///	destructor: does nothing.
    ~iterator ( ) { }
    //@}

    /** assignment operator. Only effective if both iterators come from the 
     *	same deque. 
     */
    iterator& operator = ( const iterator& ritr ) { 
      this->current = ritr.current;
      this->vec = ritr.vec;
      return *this;
    }

    /// prefix increment operator. Increments the current pointer.
    iterator& operator ++ ( ) { 
      if ( this->current < (int)(vec->size()) ) this->current++; 
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
     *	@return the n-th element into the deque having this iterator as origin.
     */
    const_reference operator [] ( size_type n ) const  { 
      return *(operator + (n)); 
    }

    /**	element assignment. i[n] = t is equivalent to *(i + n) = t. 
     *	
     *	@param n an index.
     *	@return the n-th element into the deque having this iterator as origin.
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
    /// utlDeque is a friend calss.
    friend class utlDeque<Etype,A>;
    /// - is a friend operator.
    friend difference_type operator - ( const iterator& i, const iterator& j );
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
    /// points to the current position
    int current;
    /// a pointer to the deque being iterated.
    utlDeque* vec;
  }; 	// iterator
    
  /// returns a number n such that i == j + n.
  friend difference_type operator - ( const iterator& i, const iterator& j ) { 
      return (i.current - j.current);
  }

  /// returns whether i is reachable from j. 
  friend bool operator < ( const iterator& i, const iterator& j ) { 
      return (i.current < j.current); 
  }

  /// returns whether i is reachable from j. 
  friend bool operator > ( const iterator& i, const iterator& j ) { 
      return (i.current > j.current); 
  }

  /// returns whether i is reachable from j. 
  friend bool operator <= ( const iterator& i, const iterator& j ) { 
      return (i.current <= j.current); 
  }

  /// returns whether the two given iterators point to the same element. 
  friend bool operator == ( const iterator& itr1, const iterator& itr2) {
      if (itr1.vec != itr2.vec) return false;
      return ( itr1.current == itr2.current );
  }

  /// returns whether the two given iterators point to different elements. 
  friend bool operator != ( const iterator& itr1, const iterator& itr2) {
      return !(itr1 == itr2);
  }

  /// constant iterator for deques.
  class const_iterator :  public utlIteratorBase<const_iterator> {
  public:

    /// iterator category.
    typedef typename utlIteratorBase<const_iterator>::random_access_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlDeque<Etype,A>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlDeque<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlDeque<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlDeque<Etype,A>::const_reference reference;

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

    /// contructor from a current position and a deque.
    const_iterator (int pos, const utlDeque* v) : 
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
     *	@return the n-th element into the deque having this const_iterator as origin.
     */
    const_reference operator [] ( size_type n ) const  { 
      return *(operator + (n)); 
    }

    /** assignment operator. Only effective if both const_iterators come from the 
     *	same deque. 
     */
    const_iterator& operator = ( const const_iterator& ritr ) const { 
      int* curr = (int*)&current;
      *curr = ritr.current;
      utlDeque** vec_p = (utlDeque**)&vec;
      *vec_p = (utlDeque*)ritr.vec;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// dereference operator. Returns a reference to the element in the current position.
    const_reference operator * ( ) const { return (*vec)[this->current]; }

    /// dereference operator. Returns a pointer to the element in the current position.
    const pointer operator->() const { return &(operator*()); }

  protected:
    /// - is a friend operator.
    friend difference_type operator - ( const const_iterator& i, const const_iterator& j );
    /// < is a friend operator.
    friend bool operator < ( const const_iterator& i, const const_iterator& j );
    /// == is a friend operator.
    friend bool operator == ( const const_iterator& itr1, const const_iterator& itr2);
    /// != is a friend operator.
    friend bool operator != ( const const_iterator& itr1, const const_iterator& itr2);
    /// points to the current position
    int current;
    /// a pointer to the deque being iterated.
    const utlDeque* vec;
  }; 	// const_iterator
 
  /// returns a number n such that i == j + n.
  friend difference_type operator - ( const const_iterator& i, const const_iterator& j ) { 
      return (i.current - j.current);
  }

  /// returns whether i is reachable from j. 
  friend bool operator < ( const const_iterator& i, const const_iterator& j ) { 
      return (i.current < j.current); 
  }

  /// returns whether the two given const_iterators point to the same element. 
  friend bool operator == ( const const_iterator& itr1, const const_iterator& itr2) {
      if (itr1.vec != itr2.vec) return false;
      return ( itr1.current == itr2.current );
  }

  /// returns whether the two given const_iterators point to different elements. 
  friend bool operator != ( const const_iterator& itr1, const const_iterator& itr2) {
      return !(itr1==itr2);
  }

  /*========= methods using the iterator class ==============*/

  /// returns an iterator pointing to the beginning of the deque.
  iterator begin() { 
    if (empty()) return end();
    return iterator(0, this); 
  }

  /// returns an iterator pointing to the end of the deque.
  iterator end() { return iterator(n_entries, this); } 

  /// returns a const_iterator pointing to the beginning of the deque.
  const_iterator begin() const { 
    if (empty()) return end();
    return const_iterator(0, this); 
  }

  /// returns a const_iterator pointing to the end of the deque.
  const_iterator end() const { return const_iterator(n_entries, this); } 

  /// returns a reverse_iterator pointing to the beginning of the reversed deque.
  reverse_iterator rbegin() { 
    return reverse_iterator(end()); 
  }

  /// returns a reverse_iterator pointing to the end of the reversed deque.
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /// returns a const_reverse_iterator pointing to the beginning of the reversed deque.
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }

  /// returns a const_reverse_iterator pointing to the end of the reversed deque.
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }


  /**	removes an object given in an iterator from this deque. 
   *	The size() of the deque is changed, and the elements after
   *	the erased element are copied into the freed positions.
   *	An iterator pointing to an element of this deque after
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
   *  	Destroys the elements in the range [first,last) and removes them from the deque.
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
   *	The size() of the deque is incremented by 1, and the elements after
   *	the inserted element are shifted one position to the right.
   *    The relative order of elements already in the sequence is unchanged. 
   *	An iterator pointing to an element of this deque after
   *	insert() may point to another element or to no element at all. 
   *	
   *	@param pos given iterator.
   *	@param rec given element.
   *	@return iterator to the inserted element.
   */
  iterator insert ( iterator pos, const Etype& rec ) {
    this->push_back ( this->back() ); 
    for ( int j = size()-2; j != pos.current-1; --j ) {
          operator[](j+1) = operator[](j);
    }

    operator[](pos.current) = rec;

    return pos;
  }

  /**	inserts the range [first, last) before pos. 
   *	The size() of the deque is incremented by the distance from i to j, 
   *    and the elements after the inserted elements are shifted to the right.
   *    The relative order of elements already in the sequence is unchanged. 
   *	An iterator pointing to an element of this deque after
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
    for ( int j = size()-1; j != pos.current-1; --j ) {
          operator[](j+dist) = operator[](j);
    }

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
    for ( int j = size()-1; j != pos.current-1; --j ) {
          operator[](j+n) = operator[](j);
    }

    this->n_entries += n;
    for ( size_type i = pos.current; i < pos.current+n; ++i )
          operator[](i) = x;
  }

  /// returns the first element for writing.
  reference front () { return ebuffer[inipos]; }

  /// returns the first element for reading.
  const_reference front () const { return ebuffer[inipos]; }

  /// returns the last element for writing.
  reference back ()  { return ebuffer[lastpos(-1)]; }

  /// returns the last element for reading.
  const_reference back () const { return ebuffer[lastpos(-1)]; }

 protected:

  /// const_iterator is a friend class.
  friend class const_iterator;

  /// const_iterator is a friend class.
  friend class iterator;

  /// returns this deque capacity (how much memory is allocated).
  size_type capacity() const { return this->buffer_size; }

  /** 
   *  returns this deque last position.
   *
   *  @param sh a displacement from the last position.
   *  @return (inipos+size()+sh)%capacity()
   */
  size_type lastpos(int sh=0) const { return (this->inipos+size()+sh)%capacity(); }

  /**	reserves an amount of memory for this deque.
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


  /// destroys the contents of a buffer.
  void destroy ( pointer buf, size_type fpos, size_type n ) {
#if 0
     if ( buf == NULL ) return;
     pointer p0 = buf;
     pointer p = p0+fpos;
     while ( size_type i = 0; i < n; ++i ) {
             if ( p >= p0+capacity() ) p = p0;
             alloc.destroy (p++);
     }
#endif
  }

  /**	gets a new amount of memory for this deque.
   *	
   *	@param n number of positions to be acquired
   */
  void getMemory ( size_type n ) { 
      pointer p = alloc.allocate ( n );
      pointer q0 = this->ebuffer;
      pointer q = q0+this->inipos, v = p;

      size_type npos = MIN(size(),n);
      for ( size_type i = 0; i < npos; ++i  ) {
         if ( q >= q0+capacity() ) q = q0;
         alloc.construct ( p++, *q++ );
      }

      destroy ( this->ebuffer, this->inipos, size() );
      alloc.deallocate ( this->ebuffer, capacity() );
      this->ebuffer = v;
      this->buffer_size = n;
      this->inipos = 0;
  }


  /// deque dimension.
  size_type buffer_size;

  /// number of elements into the deque.
  size_type n_entries;

  /// initial position.
  size_type inipos;

  /// array for holding Etype elements.
  pointer ebuffer;

  /// an allocator object.
  allocator_type alloc;
};	// utlDeque

#endif // USE_STL
#endif // __UTLDEQUE_H__

