/*
**	utlList.h - class for manipulating lists.
**
**	purpose: to provide adequate classes for the manipulation
**	of linear doubly linked lists.
**

  Program:   cgc util
  Module:    $RCSfile: utlList.h,v $
  Language:  C++
  Date:      $Date: 2005/10/10 00:10:07 $
  Version:   $Revision: 1.3 $

**
*/

#ifndef __UTLLIST_H__
#define __UTLLIST_H__

#include "utlAlgo.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include "utlDefs.h"
#define utlList list
#else

#include "utl.h"
#include "utlIteratorBase.h"
#include "utlReverseIterator.h"
#include "utlAllocator.h"

/** A simpler implementation of STL class list.
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.3 $ $Date: 2005/10/10 00:10:07 $
 *	
 *	A list is a doubly linked list. That is, it is a Sequence that 
 *	supports both forward and backward traversal, and (amortized) 
 *	constant time insertion and removal of
 *	elements at the beginning or the end, or in the middle. 
 *	Lists have the important property that insertion and splicing 
 *	do not invalidate iterators to list elements, and
 *	that even removal invalidates only the iterators that point to 
 *	the elements that are removed. The ordering of iterators may be changed 
 *	(that is, list<T>::iterator might have a different predecessor or 
 *	successor after a list operation than it did before), 
 *	but the iterators themselves will not be
 *	invalidated or made to point to different elements 
 *	unless that invalidation or mutation is explicit.  
 *
 *	Etype objects in the list must provide: <br>
 *	Etype() (empty constructor). <br>
 *	operator==() <br>
 *	operator< () <br>
 *	operator<<() <br>
 */
template <class Etype, class A = utlAllocator<Etype> > 
class utlList {
 
#if defined (__xlC__) || defined (__SUN_CC)
 public:
#else
 protected:
#endif

/// the structure of a list node.
  struct ListNode {
	
    /** @name constructors  */ 
    //@{

    ///	empty constructor: sets NULL next and prev.
    ListNode( ) : element(Etype()), next(NULL), prev(NULL) {} 
	
    ///	constructor from a node, its successor and its predecessor.
    ListNode ( const Etype& e, ListNode* n = NULL, ListNode* p = NULL ) :
     		  element(e), next(n), prev(p) {}

    //@}

    ///	inserts this node after the given node.
    ListNode* insertAfter ( ListNode* p ) { 
      this->prev = p; 
      p->next = this;
	    
      return this;
    }

    ///	returns the successor of this node.
    ListNode* getNext ( ) const { return this->next; }

    /**	sets the successor of this node.
     *	
     *	@param p pointer to the node to be inserted in this list.
     *	@return p.
     */
    ListNode* setNext ( ListNode* p ) { this->next = p; return p; }
	
    /**	sets the predecessor of this node.
     *	
     *	@param p pointer to the node to be inserted in this list.
     *	@return p.
     */
    ListNode* setPrev ( ListNode* p ) { this->prev = p; return p; }
	
    /**	compares this node to a given node. <br>
     *	Once operator < is supplyed, the standard library 
     *	automatically defines the >, <=, and >= operators,
     *	using the following logic (pg 313 Horstmann): <br>
     *	this  > b ---   b < this  <br>
     *	this <= b --- !(b < this) <br>
     *	this >= b --- !(this < b) <br> 
     *	
     *	@param b node to be compared to this node.
     *	@return 1 whether this node is less than the given node, and 0 otherwise.
     */
    int compare ( const ListNode* b ) const { 
      return (this->element < b->element ? -1 : (this->element == b->element ? 0 : 1)); }
	
	
    ///	assignment operator. 
    ListNode& operator = ( const ListNode& nd ) { 
      this->element = nd.element;
      this->next = NULL;
      this->prev = NULL;
	    
      return *this;
    }

    ///	object in this node of the list.
    Etype element;
	
    /// next node in the list.
    ListNode *next;
	
    ///	previous node in the list. 
    ListNode *prev;
	
  }; // struct ListNode

 public:

  /// value type.
  typedef Etype value_type;

  /// pointer type.
  typedef Etype* pointer;

  /// const_pointer type.
  typedef const Etype* const_pointer;

  /// reference type.
  typedef Etype& reference;

  /// const_reference type.
  typedef const Etype& const_reference;

  /// size type.
  typedef size_t size_type;

  ///  pointer difference type.
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

  /// allocator for list nodes.
  typedef typename A::template rebind<ListNode>::other link_allocator_type;

  /** @name constructors  */ 
  //@{

  /// empty constructor: allocates Header and Tail.
  utlList ( ) : alloc(allocator_type()) { this->initList(); }

  /// creates a list with n elements, each of which is a copy of Etype().
  utlList ( size_type n ) : alloc(allocator_type()) {
      this->initList();
      for ( size_type i = 0; i < n; ++i )
            this->push_back ( Etype() ); 
  } 
	
  /// creates a list with n copies of t.
  utlList ( size_type n, const Etype& t ) : alloc(allocator_type()) {
      this->initList();
      for ( size_t i = 0; i < n; ++i )
            this->push_back ( t ); 
  } 
	
  /// constructor as a copy range.
  template <class InputIterator>
  utlList ( InputIterator first, InputIterator last ) : alloc(allocator_type()) {
      this->initList();
      size_type n = cgcUtil::distance ( first, last );
      for ( size_type i = 0; i < n; ++i )
            this->push_back ( Etype() ); 
      cgcUtil::copy ( first, last, begin() ); 
  }

  /// copy constructor. 
  utlList ( const utlList& rhs ) : alloc(rhs.alloc) { 
    this->initList();
    *this = rhs; 
  }

  /// destructor: deletes this list.
  ~utlList ( ) { 
    clear( ); 
    this->link_alloc.deallocate(Header,1); 
    this->link_alloc.deallocate(Tail,1); 
  }

  //@}

  /// makes this list empty.
  void clear ( ) {
      erase ( begin(), end() );
  }

  /// inserts or erases elements at the end such that size() becomes n.
  void resize ( size_type n, const Etype& val = Etype() )
    {
      size_type i, sz = this->size();
      if ( n < sz ) { 
         for ( i = 0; i < sz-n; ++i )
               pop_back();  
      }
      else if ( n > sz ) {
         for ( i = 0; i < n-sz; ++i )
               push_back ( val );  
      }
    }

  /**	returns the list size.
   *
   *	You should not assume that this function is constant time. 
   *	It is permitted to be O(N), where N is the number of elements 
   *	in the list. If you wish to test whether a list is
   *	empty, you should write L.empty() rather than L.size() == 0.
   *	
   *	@return list size.
   */
  size_type size ( ) const {
      return cgcUtil::distance ( begin(), end() ); 
  }

  /**	returns the maximum possible size of a list.
   *	
   *	@return number of bytes.	
   */
  size_type max_size ( ) const { return (size_type)1073741823 /*4294967295*/; }

  ///	returns whether this list is empty.
  bool empty ( ) const { return Header->next == Tail; }

  /**	inserts an object at the beginning of this list.
   *	
   *	@param rec given object. 
   */
  void push_front ( const Etype& rec ) {
      insertNode ( Header, rec );
  }

  /**	inserts an object at the end of this list.
   *	
   *	@param rec given object. 
   */
  void push_back ( const Etype& rec ) {
      insertNode ( Tail->prev, rec );
  }

  /// removes the first element.
  void pop_front ( ) {
      if ( !empty() )
          deleteNode ( Header );
  }

  /// removes the last element.
  void pop_back ( ) {
      if ( !empty() )
          deleteNode ( Tail->prev->prev );
  }

  /**	returns whether a given object is in the list. 
   *	NOT in STL standard.
   *	
   *	@param rec object to be searched for. 
   *	@return 1 if the given object is in the list, and 0 otherwise.
   *	
   */
  bool isFound ( const Etype& rec ) const {
    return ( cgcUtil::find ( begin(), end(), rec ) != end() );
  }

  /**	removes all elements that compare equal to val. 
   *	The relative order of elements that are not
   *	removed is unchanged, and iterators to elements 
   *	that are not removed remain valid. This
   *	function is linear time: it performs exactly size() comparisons for equality. 
   *
   *	@param val object to be removed.
   */
  void remove ( const Etype& val )
    {
      ListNode* ptr = Header;

      while ( 1 ) {
        for ( ; ptr->next != Tail; ptr = ptr->next )
	  if ( ptr->next->element == val ) break;

        if ( ptr->next == Tail )
	  return;			// search fails

        deleteNode ( ptr ); 
      }
    }

  /**	removes all elements *i such that p(*i) is true. 
   *	The relative order of elements that are not
   *    removed is unchanged, and iterators to elements that are not 
   *	removed remain valid. This function is linear time: 
   *	it performs exactly size() applications of p. 
   *
   *	@param p given predicate.
   */
 template<class Predicate> void remove_if(Predicate p) 
    {
      ListNode* ptr = Header;

      while ( 1 ) {
        for ( ; ptr->next != Tail; ptr = ptr->next )
	  if ( p(ptr->next->element) ) break;

        if ( ptr->next == Tail )
	  return;			// search fails

        deleteNode ( ptr ); 
      }
    }

  /** 	swaps the contents of this and a given list.
   *
   *	@param l given list.
   */
   void swap ( utlList& l ) {
     ListNode* h = this->Header;
     ListNode* t = this->Tail;

     Header = l.Header;
     Header->next->prev = Header;
     Tail = l.Tail;
     Tail->prev->next = Tail;
     l.Header = h;
     l.Header->next->prev = l.Header;
     l.Tail = t;
     l.Tail->prev->next = l.Tail;
   }

  /**	sorts this list by using Etype::operator<(). The sort
   *	is stable, that is, the relative order of equivalent
   *	elements is preserved. All iterators remain valid and
   *	continue to point to the same elements. The number of
   *	comparisons is O ( N log N ), where N
   *	is the list's size.
   *	
   *	@see ListNode::compare().
   */
  void sort ( )
    {
      if ( empty() ) return;

      Header->next->prev = NULL;
      Tail->prev->next   = NULL;

      utl_sort ( &(Header->next), &(Tail->prev) );

      Header->next->prev = Header;
      Tail->prev->next = Tail;
    }

  /**	merges two sorted lists. This list is the target list. 
   *	merge() refrains from copying elements. 
   *	Instead, it removes elements from
   *    the source list and splices them into the target list.
   *	At the end, the source list is empty.
   *	
   *	@param l source list.
   */
  void merge ( utlList& l  )
    {
      if ( l.empty() ) return;

      if ( empty() ) return swap ( l );

      utl_merge ( &(Header->next), Tail->prev, l.Header->next, &(l.Tail->prev) );

      Header->next->prev = Header;
      Tail->prev->next = Tail;
      l.Header->next = l.Tail;	// source list is now empty
      l.Tail->prev = l.Header;
    }

  /**   deletes all but the first element in every consecutive group of equal
   *	elements. The relative order of elements that are not deleted is
   *	unchanged, and iterators to elements that are not deleted remain valid. 
   *	The application to a sorted list leads to the effect that no element
   *	occurs more than once.
   */
  void unique( ) {
    if ( empty() ) return;
    ListNode* ptr = Header->next;
    ListNode* next_node = ptr->next;
    while ( next_node != Tail ) {
      if ( next_node->element == ptr->element ) 
           deleteNode ( ptr );  // deletes ptr->next (next_node)
      else 
           ptr = next_node;
      next_node = ptr->next;
    }
  }

  /**	reverses the order of elements in the list. 
   *	All iterators remain valid and continue to point to the same elements. 
   *	This function is linear time. 
   */
  void reverse() {
    ListNode* first = Header->next;
    ListNode* last = Tail->prev, *ptr_next;

    for ( ListNode* ptr = first; ptr != Tail; ptr = ptr_next ) {
      ptr_next  = ptr->next;
      ptr->next = ptr->prev;
      ptr->prev = ptr_next;
    }
    Header->next = last;
    last->prev   = Header;
    Tail->prev   = first;
    first->next  = Tail;
  } 

  /** 	assignment operator. Copies the whole given source list to this target list. 
   *	
   *	@param rhs list to be copied ( source ).
   *	@return a reference to this list ( target ). 
   */
  utlList& operator = ( const utlList& rhs ) 
  {
    // target and source lists are the same
    if ( this == &rhs )  return *this;
    
    // clean target list
    this->clear ( );	
   
    const_iterator ritr; 
    for ( ritr = rhs.begin(); ritr != rhs.end(); ++ritr )
      this->push_back ( *ritr );
    
    return *this;		// return target list
  }

  /**	comparison operator. Tests two lists for equality.
   *	
   *	@return l1 == l2.
   */
  friend bool operator == ( const utlList<Etype,A>& l1, const utlList<Etype,A>& l2 ) {
    if ( &l1 == &l2 ) return true; // same list 
    if ( l1.size() != l2.size() ) return false;

    return cgcUtil::equal ( l1.begin(), l1.end(), l2.begin() );
  }

  /**	comparison operator. Tests two lists for inequality.
   *	
   *	@return l1 != l2.
   */
  friend bool operator != ( const utlList<Etype,A>& l1, const utlList<Etype,A>& l2 ) {
    return !(l1 == l2);
  }

  /**	lexicographical comparison.
   *	
   *	@return l1 < l2.
   */
  friend bool operator < ( const utlList<Etype,A>& l1, const utlList<Etype,A>& l2 ) {
    return cgcUtil::lexicographical_compare ( l1.begin(), l1.end(), l2.begin(), l2.end() );
  }

  /** 	output operator. Prints this list. 
   *	
   *	@param os file stream.
   *	@param l given list.
   *	@return file stream. 
   */
  friend ostream& operator<<(ostream& os, const utlList<Etype,A>& l) 
    {
      os << "List Contents: ";
      if ( l.empty() )
	os << "Empty List" << endl;
      else
	{
	  os << endl; 
          const_iterator itr;
	  for ( itr = l.begin(); itr != l.end(); ++itr ) {
#ifndef __KCC
            os << *itr << endl;
#endif 
	  }
	}

      return os; 
    }

  /// iterator for lists.
  class iterator : public utlIteratorBase<iterator> {
  public:

    /// iterator category.
    typedef typename utlIteratorBase<iterator>::bidirectional_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlList<Etype,A>::value_type value_type;
    /// type of difference between two iterators.
    typedef typename utlList<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlList<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlList<Etype,A>::reference reference;

    /** @name constructors  */ 
    //@{

    /// empty constructor.
    iterator () : current(NULL) { }
	
    /// copy constructor.
    iterator ( const iterator& itr ) : 
      current(itr.current) { }

    /// contructor from a list node
    iterator ( ListNode* node ) : current(node) { }

    ///	destructor: does nothing.
    ~iterator ( ) { }
    //@}


    /** assignment operator. Only effective if both iterators come from the 
     *	same list. 
     */
    iterator& operator = ( const iterator& ritr ) { 
      this->current = ritr.current;
      return *this;
    }

    /// prefix increment operator. Increments the current pointer.
    iterator& operator ++ ( ) { 
      if (current) current = current->next;
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
      if (current) current = current->prev;
      return *this;
    }

    /// posfix decrement operator. Decrements the current pointer.
    iterator operator -- ( int ) { 
      iterator tmp = *this;
      --*this;
      return tmp; 
    }
    /// dereference operator. Returns a reference to the element in the current node.
    reference operator * ( ) { return current->element; }

    /// dereference operator. Returns a reference to the element in the current node.
    const_reference operator * ( ) const { return current->element; }

    /// dereference operator. Returns a pointer to the element in the current node.
    pointer operator->() { return &(operator*()); }

  protected:
    /// const_iterator is a friend of iterator.
    friend class const_iterator;
    /// utlList is a friend of iterator.
    friend class utlList<Etype,A>;
    /// operator == is a friend of iterator.
    friend bool operator == ( const iterator& itr1, const iterator& itr2 );
    /// operator != is a friend of iterator.
    friend bool operator != ( const iterator& itr1, const iterator& itr2 );
    ///	points to the current node.
    ListNode* current;
  }; // iterator
    
  /// returns whether the two given iterators point to the same element. 
  friend inline bool operator == ( const iterator& itr1, const iterator& itr2 ) {
      return (itr1.current == itr2.current);
  }

  /// returns whether the two given iterators point to different elements. 
  friend inline bool operator != ( const iterator& itr1, const iterator& itr2 ) {
      return (itr1.current != itr2.current);
  }

  /// constant iterator for lists.
  class const_iterator : public utlIteratorBase<const_iterator> {
  public:

    /// const_iterator category.
    typedef typename utlIteratorBase<const_iterator>::bidirectional_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlList<Etype,A>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlList<Etype,A>::difference_type difference_type;
    /// return type of operator ->().
    typedef typename utlList<Etype,A>::pointer pointer;
    /// return type of operator *().
    typedef typename utlList<Etype,A>::const_reference reference;

    /** @name constructors  */ 
    //@{

    /// empty constructor.
    const_iterator () : current(NULL) { }
	
    /// copy constructor.
    const_iterator ( const const_iterator& itr ) : 
      current(itr.current) { }

    /// constructor from an iterator.
    const_iterator ( const iterator& itr ) : 
      current(itr.current) { }

    /// contructor from a list node
    const_iterator ( ListNode* node ) : current(node) { }

    ///	destructor: does nothing.
    ~const_iterator ( ) { }
    //@}


    /** assignment operator. Only effective if both const_iterators come from the 
     *	same list. 
     */
    const_iterator& operator = ( const const_iterator& ritr ) const { 
      ListNode** curr = (ListNode**)&current;
      (*curr) = (ListNode*) ritr.current;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// prefix increment operator. Increments the current pointer.
    const_iterator& operator ++ ( ) const { 
      ListNode** curr = (ListNode**)&current;
      if (current) (*curr) = (*curr)->next;
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
      ListNode** curr = (ListNode**)&current;
      if (current) (*curr) = (*curr)->prev;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix decrement operator. Decrements the current pointer.
    const_iterator operator -- ( int ) const { 
      const_iterator tmp = *this;
      --*this;
      return tmp; 
    }

    /// dereference operator. Returns a reference to the element in the current node.
    const_reference operator * ( ) const { return current->element; }

    /// dereference operator. Returns a pointer to the element in the current node.
    pointer operator->() { return &(operator*()); }

    /// dereference operator. Returns a constant pointer to the element in the current node. 
    const_pointer operator->() const { return &(operator*()); }

  protected:
    /// == is a friend operator.
    friend bool operator == ( const const_iterator& itr1, const const_iterator& itr2 );
    /// != is a friend operator.
    friend bool operator != ( const const_iterator& itr1, const const_iterator& itr2 );
    ///	points to the current node.
    const ListNode* current;
  }; // const_iterator
    
  /// returns whether the two given const_iterators point to the same element. 
  friend inline bool operator == ( const const_iterator& itr1, const const_iterator& itr2 ) {
      return (itr1.current == itr2.current);
  }

  /// returns whether the two given const_iterators point to different elements. 
  friend inline bool operator != ( const const_iterator& itr1, const const_iterator& itr2 ) {
      return (itr1.current != itr2.current);
  }

  /*========= methods using the iterator class ==============*/

  /// returns an iterator pointing to the beginning of the list.
  iterator begin() { return iterator(Header->next); }

  /// returns an iterator pointing to the end of the list.
  iterator end() { return iterator(Tail); }

  /// returns a const_iterator pointing to the beginning of the list.
  const_iterator begin() const { return const_iterator(Header->next); }

  /// returns a const_iterator pointing to the end of the list.
  const_iterator end() const { return const_iterator(Tail); }

  /// returns a reverse_iterator pointing to the beginning of the reversed list.
  reverse_iterator rbegin() { return reverse_iterator(end()); }

  /// returns a reverse_iterator pointing to the end of the reversed list.
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /// returns a const_reverse_iterator pointing to the beginning of the reversed list.
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  /// returns a const_reverse_iterator pointing to the end of the reversed list.
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  /// returns the first element for writing.
  reference front () { return Header->next->element; }

  /// returns the first element for reading.
  const_reference  front () const { return Header->next->element; }

  /// returns the last element for writing.
  reference back ()  { return Tail->prev->element; }

  /// returns the last element for reading.
  const_reference back () const { return Tail->prev->element; }

  /**	removes the element given in an iterator from this list.
   *	Nothing is done if the given iterator points to the header 
   *	or tail of the list.
   *	
   *	@param it given iterator.
   *	@return iterator to the element following the one that was deleted.
   */
  iterator erase ( iterator it ) {
      if ( it.current != NULL && it.current != Header && it.current != Tail ) 
          deleteNode ( (--it).current ); 
      return ++it;
  }

  /**	erases the range [first, last).
   *	
   *	@param first beginning of the range.
   *	@param last end of the range.
   *	@return iterator to the element immediately following the ones that were erased. 
   */
   iterator erase(iterator first, iterator last) {
      ListNode* next_node = NULL;
      ListNode* prev = first.current->prev;

      for ( ListNode* ptr = first.current; ptr != last.current; ptr = next_node )
	{
	  next_node = ptr->next;	// for not loosing ptr
	  this->link_alloc.deallocate (ptr,1);
	}
      prev->next = last.current;
      last.current->prev = prev;
      return last;
  }

  /**	inserts an object before the element in a 
   *	given iterator. 
   *	
   *	@param it given iterator.
   *	@param rec given object.
   *	@return an iterator to the inserted object. 
   */
  iterator insert ( iterator it, const Etype& rec )
    {
      if ( it.current == NULL ) return it;

      if ( it.current == Header ) 
	{
	  this->push_front ( rec );
	  return this->begin();
	}

      insertNode ( (it.current)->prev, rec );

      return (--it);
    }

  /**	inserts n copies of x before pos. 
   *	
   *	@param pos reference iterator.
   *	@param n number of copies.
   *	@param x given object.
   */
  void insert(iterator pos, size_type n, const Etype& x) {
       for ( size_type i = 0; i < n; ++i ) 
          insert ( pos, x );
  }

  /**	inserts the range [f, l) before pos. 
   *	
   *	@param pos reference iterator.
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator>
  void insert(iterator pos, InputIterator f, InputIterator l) {
       cgcUtil::copy ( f, l, cgcUtil::inserter(*this,pos) );
  }

  /**   moves (splices) elements from a given range to this list.
   *	position must be a valid iterator in *this, and [first, last) must be a valid range in x. 
   *	position may not be an iterator in the range [first, last). Splice moves the
   *    elements in [first, last) from x to *this, inserting them before position. 
   *	All iterators remain valid, including iterators that point to elements of x. 
   *	This function is constant time. 
   *
   *	@param position reference iterator into this list.
   *	@param x list owning the range (possibly this list).
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  void splice(iterator position, utlList& x, iterator f, iterator l) {
    if ( position.current == this->Header ) return;
    ListNode* cnode = position.current;
    ListNode* first = f.current;
    ListNode* last  = l.current;
    ListNode* fprev = first->prev;

    cnode->prev->next = first;
    first->prev = cnode->prev;
    last->prev->next = cnode;
    cnode->prev = last->prev;

    fprev->next = last;
    last->prev = fprev;
  }

  /**   moves (splices) all elements from a given list to this list.
   *	position must be a valid iterator in *this, and x must be a list 
   *	that is distinct from *this (that is, it is required that &x != this). 
   *	All of the elements of x are inserted before position and removed from x. 
   *	All iterators remain valid, including iterators that point to elements of x. 
   *	This function is constant time. 
   *
   *	@param position reference iterator into this list.
   *	@param x a given list.
   */ 
  void splice(iterator position, utlList& x) {
    if ( &x == this || x.empty() ) return;
    splice ( position, x, x.begin(), x.end() );
  }


  /**   moves (splices) a single element from a given list to this list.
   *	position must be a valid iterator in *this, and i must be a dereferenceable iterator in x.
   *    Splice moves the element pointed to by i from x to *this, inserting it before position.
   *    All iterators remain valid, including iterators that point to elements of x. 
   *	If position == i or position == ++i, this function is a null operation. 
   *	This function is constant time. 
   *
   *	@param position reference iterator into this list.
   *	@param x list owning the element (possibly this list).
   *	@param i iterator pointing to the element.
   */ 
  void splice(iterator position, utlList& x, iterator i) {
     if ( i.current == x.Tail || i.current == x.Header ) return;
     iterator j = i;
     if ( position == i || position == ++j ) return;
     splice ( position, x, i, j );
  }

 private:

  /// initializes this list, by allocating its header and tail.
  void initList() {
    Header = this->link_alloc.allocate ( 1 );  
    Tail = this->link_alloc.allocate ( 1 ); 
    Header->next = Tail; 
    Tail->prev = Header; 
  }

  /// deletes the successor of a given node.
  void deleteNode ( ListNode* ptr ) {
    ListNode* deleted_node = ptr->next;
    deleted_node->next->prev = ptr;
    ptr->next = deleted_node->next;
    this->link_alloc.deallocate (deleted_node,1);
  }

  /// inserts a new node after a given node.
  void insertNode ( ListNode* ptr, const Etype& rec ) {
    ListNode* new_node = this->link_alloc.allocate(1); 
    this->link_alloc.construct ( new_node, ListNode ( rec, ptr->next, ptr ) );

    ptr->next->prev = new_node;
    ptr->next       = new_node;
  }

  /// header of this list: points to the first node.
  ListNode* Header;

  /// tail of this list: points to the last node.
  ListNode* Tail;

  /// list allocator.
  allocator_type alloc;

  /// link allocator.
  link_allocator_type link_alloc;
};


#endif //STL

#endif // __UTLLLIST_H__

