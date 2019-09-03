/*
  utlHashMap.h

  Program:   utl
  Module:    $RCSfile: utlHashMap.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 14:44:12 $
  Version:   $Revision: 1.4 $
*/

#ifndef __UTLHASHMAP_H__
#define __UTLHASHMAP_H__

#include "utlConfig.h"


/* for STL compatibility test use: -DUSE_STL */
#if defined (USE_STL) && !defined (__PGI) && !defined (_MSC_VER) && !defined (__KCC) && !defined(__xlC5) && !defined(__INTEL_COMPILER) && !defined (__SUN_CC)

#if (__GNUC__ < 3) 
#include <hash_map>
#define utlHashMap hash_map 
#define utlHashMultiMap hash_multimap 
#define utlHashFun hash 
#else
#include <ext/hash_map>
#define utlHashMap __gnu_cxx::hash_map 
#define utlHashMultiMap __gnu_cxx::hash_multimap 
#define utlHashFun __gnu_cxx::hash 
#endif

#else

// implicit data structures

#include "utl.h"
#include "utlIteratorBase.h"
#include "utlReverseIterator.h"
#include "utlAllocator.h"
#include "utlPair.h"
#include "utlString.h"
#include "utlVector.h"
#include "utlList.h"


/// forward declaration of utlHashMap.
template <class Key, class T, class hashFun, class Cmp> class utlHashMap;

/// forward declaration of utlHashMultiMap.
template <class Key, class T, class hashFun, class Cmp> class utlHashMultiMap;

///forward declaration of utlHashFun.
template <class Key> class utlHashFun;

/**	compares the keys of two objects. 
 *
 *	@param x first object.
 *	@param y second object.	
 *	@return true if x == y, and false otherwise.
 */
 template <class Etype> class eqt_to {
 
  public:

  /// empty constructor.
  eqt_to ( ) { }

  /// function call operator.
  bool operator ( ) ( const Etype& x, const Etype& y ) const {
   return (x == y);
  }
 };

/**	Hashed map with external chaining. 
 *
 *	Hash_map is a Hashed Associative Container that associates 
 *	objects of type Key with objects of type Data. 
 * 	Hash_map is a Pair Associative Container,
 *	meaning that its value type is pair<const Key, Data>. 
 *	It is also a Unique Associative Container, meaning that 
 *	no two elements have keys that compare equal using EqualKey. 
 *
 *	Looking up an element in a hash_map by its key is efficient, 
 *	so hash_map is useful for "dictionaries" where the order of 
 *	elements is irrelevant. If it is important
 *	for the elements to be in a particular order, however, 
 *	then map is more appropriate. 
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.4 $ $Date: 2018/10/11 14:44:12 $
 *	
 *	Modified from Breymann book "Designing Components with 
 *	C++ STL".
 */
template < class Key, class T, class hashFun=utlHashFun<Key>, class Cmp=eqt_to<Key> >
class utlHashMap
{
 public:

/* 
   The template parameter Key stands for the type of the key; 
   T stands for the class of data associated to a key; and hashFun
   is the placeholder for the data type of the function objects
   used for address calculation. Below, a function object for
   address calculation is proposed, but any other one can be used
   as well. In analogy to map, value_type is the type of the
   elements that are stored in a utlHashMap object. value_type is a
   pair consisting of a constant key and the associated data. 
*/

  /// key type.
  typedef Key key_type;

  /// data type.
  typedef T data_type;

  /// a pair <const key,value>.
  typedef utlPair<const Key,T> value_type;

  /// pointer type.
  typedef T* pointer;

  /// const_pointer type.
  typedef const T* const_pointer;

  /// reference type.
  typedef T& reference;

  /// const_reference type.
  typedef const T& const_reference;

  /// size type.
  typedef size_t size_type;

  ///  pointer difference type.
  typedef ptrdiff_t difference_type;

  // define more readable denominations
  /// a list of values.
  typedef utlList<value_type> list_type;

  /// a vector of list pointers.
  typedef utlVector<list_type*> vector_type;

  /// the hash_map's hash function.
  typedef hashFun hasher;

  /// the hash_map's comparison function.
  typedef Cmp key_equal;

  /// forward declaration of iterator class.
  class iterator;

  /// forward declaration of const_iterator.
  class const_iterator;

  /// iterator is a friend class of utlHashMap.
  friend class iterator;

  /// const_iterator is a friend class of utlHashMap.
  friend class const_iterator;

  /** iterator for hash_maps.
    * The nested class iterator closely cooperates with utlHashMap, so that
    * both are mutually declared as friend. iterator is only supposed
    * to allow forward traversal. An iterator object
    * allows you to visit all elements of a utlHashMap object one after
    * the other. Neither an order nor a sorting is defined for the
    * elements. The visiting order of the iterator is given by the
    * implicit data structure (see below, operator++()). 
    */
  class iterator : public utlIteratorBase<iterator> {

    /// utlHashMap is a friend class of iterator.
    friend class utlHashMap<Key, T, hashFun, Cmp>;
    /// utlHashMultiMap is a friend class of iterator.
    friend class utlHashMultiMap<Key, T, hashFun, Cmp>;
    /// const_iterator is a friend.
    friend class const_iterator;

  private:
    /// an iterator for the list of collisions. 
    typename list_type::iterator current;

    /// current hash_map address.
    size_type Address;

    /// a pointer to the hash vector.
    const vector_type *pVec;

  public:

    /// iterator category.
    typedef typename utlIteratorBase<iterator>::input_iterator_tag iterator_category;
    /// type of element.
    typedef typename utlHashMap<Key,T,hashFun,Cmp>::value_type value_type;
    /// type of difference between two iterators.
    typedef typename utlHashMap<Key,T,hashFun,Cmp>::difference_type difference_type;
    /// return type of operator ->().
    typedef value_type* pointer;
    /// return type of operator *().
    typedef value_type& reference;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    iterator() : pVec(NULL) {}

    /// constructor from a list iterator, address, and a hash vector.
    iterator(typename list_type::iterator LI,
	     size_type A, const vector_type* C)
      : current(LI), Address(A), pVec(C) {}

    /// copy constructor.
    iterator(const iterator& it)
      : current(it.current), Address(it.Address), pVec(it.pVec) {}

  //@}

    /** returns the hash vector.
     *  The void operator allows you to check a utlHashMap iterator
     *	in the condition part of an if or while as to whether it is at
     *	all defined. 
     */
    operator const void* () const {
      return pVec;
    }  

    /// returns whether this iterator points to a NULL hash vector.
    bool operator!() const {
      return pVec == NULL;
    }

    /** const dereferencing operator.
     *  The operator for dereferencing occurs both in the const
     *	variation and in the non-const variation. Thus,
     *	dereferencing of an undefined iterator is punished with a
     *	program abort, which is a clear message to you to check the
     *	program that uses the iterator. 
     */
    const_reference operator*() const {
      assert(pVec);
      return *current;
    }  

    /** non-const dereferencing operator.
     *  The non-const variation is required to be able to modify
     *	data independently from the key. Modification of the key
     *	must be excluded because it requires a new address
     *	calculation. Constancy is guaranteed by the const
     *	declaration in the type definition of value_type.
     */
    reference operator*() {
      assert(pVec);
      return *current;
    }

    /// non-const pointer operator.
    pointer operator->() {
      return current.operator->();
    }

    /// prefix increment operator. 
    iterator& operator++() {
      /*	
     	 How does the utlHashMap iterator move from one element to the
     	 other with operator++()? First, current is incremented:
      */
      ++current;

      /* If after this, current points to a list element, a
	 reference to the iterator is returned (see below: return
	 this. Otherwise, the end of the list is reached. 
      */
      if(current == (*pVec)[Address]->end())
	{

	  /* At this point, one address after the other is checked
	     in the vector, until either a list entry is found or
	     the end of the vector is reached. In the latter case,
	     the iterator becomes invalid, because it can only move
	     forward. In order to exclude further use, pVec is set
	     to 0: 
          */
	  while(++Address < pVec->size())
	    if((*pVec)[Address])
	      {
		current = (*pVec)[Address]->begin();
		break;
	      }

	  if(Address == pVec->size()) // end of vector reached
	    pVec = NULL;
	}
      return *this;
    }

    /// posfix increment operator.
    iterator operator++(int) {
      iterator temp = *this;
      operator++();
      return temp;
    }

    /**  compares two utlHashMap iterators. Two
     *   undefined or invalidated iterators are always considered as
     *   equal. 
     */
    bool operator==(const iterator& x) const {
      return ( current == x.current || (!pVec && !x.pVec) );
    }  

    /**  compares two utlHashMap iterators. Two
     *   undefined or invalidated iterators are always considered as
     *   equal. 
     */
    bool operator!=(const iterator& x) const {
      return !operator==(x);
    }
  }; // iterator

  /** const_iterator for hash_maps.
    * The nested class const_iterator closely cooperates with utlHashMap, so that
    * both are mutually declared as friend. const_iterator is only supposed
    * to allow forward traversal. An iterator object
    * allows you to visit all elements of a utlHashMap object one after
    * the other. Neither an order nor a sorting is defined for the
    * elements. The visiting order of the iterator is given by the
    * implicit data structure (see below, operator++()). 
    */
  class const_iterator : public utlIteratorBase<const_iterator> {

    /// utlHashMap is a friend class of const_iterator.
    friend class utlHashMap<Key, T, hashFun, Cmp>;
    /// utlHashMultiMap is a friend class of const_iterator.
    friend class utlHashMultiMap<Key, T, hashFun, Cmp>;

  private:
    /// a const_iterator for the list of collisions. 
    typename list_type::const_iterator current;

    /// current hash_map address.
    size_type Address;

    /// a pointer to the hash vector.
    const vector_type *pVec;

  public:

    /// const_iterator category.
    typedef typename utlIteratorBase<const_iterator>::input_iterator_tag iterator_category;
    /// type of element.
    typedef typename utlHashMap<Key,T,hashFun,Cmp>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlHashMap<Key,T,hashFun,Cmp>::difference_type difference_type;
    /// return type of operator ->().
    typedef value_type* pointer;
    /// return type of operator *().
    typedef value_type& reference;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    const_iterator() : pVec(NULL) {}

    /// constructor from a list const_iterator, address, and a hash vector.
    const_iterator(typename list_type::const_iterator LI,
	     size_type A, const vector_type* C)
      : current(LI), Address(A), pVec(C) {}

    /// copy constructor.
    const_iterator(const const_iterator& it)
      : current(it.current), Address(it.Address), pVec(it.pVec) {}

    /// constructor from an iterator.
    const_iterator(const iterator& it)
      : current(it.current), Address(it.Address), pVec(it.pVec) {}
  //@}

    /** returns the hash vector.
     *  The void operator allows you to check a utlHashMap const_iterator
     *	in the condition part of an if or while as to whether it is at
     *	all defined. 
     */
    operator const void* () const {
      return pVec;
    }  

    /// returns whether this const_iterator points to a NULL hash vector.
    bool operator!() const {
      return pVec == NULL;
    }

    /// const dereferencing operator.
    const_reference operator*() const {
      assert(pVec);
      return (const reference)*current;
    }  

    /// dereferencing operator.
    reference operator*() {
      assert(pVec);
      return (reference)*current;
    }  

    /// const_pointer operator.
    const_pointer operator->() const {
      return (const_pointer)&(*current); 
    }

    /// pointer operator.
    pointer operator->() {
      return (pointer)&(*current); 
    }

    /// prefix increment operator. 
    const_iterator& operator++() const {
      /*	
     	 How does the utlHashMap const_iterator move from one element to the
     	 other with operator++()? First, current is incremented:
      */
#if !defined(__KCC) && !defined(__xlC5) && (__GNUC__ < 3) && !defined(__PGI) && !defined(__INTEL_COMPILER) && !defined(__SUN_CC)
      ++current;
#else 
      // trick to emulate mutable current
      typename list_type::iterator& lcurrent = (typename list_type::iterator&)current;
      ++lcurrent; // idea: 
#endif
      /* If after this, current points to a list element, a
	 reference to the const_iterator is returned (see below: return
	 this. Otherwise, the end of the list is reached. 
      */
      if(current == (*pVec)[Address]->end())
	{

	  /* At this point, one address after the other is checked
	     in the vector, until either a list entry is found or
	     the end of the vector is reached. In the latter case,
	     the const_iterator becomes invalid, because it can only move
	     forward. In order to exclude further use, pVec is set
	     to 0: 
          */
          size_t* add = (size_t*)&Address;
	  while(++(*add) < pVec->size())
	    if((*pVec)[Address])
	      {
#if !defined(__KCC) && !defined(__xlC5) && (__GNUC__ < 3) && !defined(__PGI) && !defined(__INTEL_COMPILER) && !defined(__SUN_CC)
		current = (*pVec)[Address]->begin();
#else
		lcurrent = (*pVec)[Address]->begin();
#endif
		break;
	      }

	  if(Address == pVec->size()) { // end of vector reached
            vector_type** tmp_vec = (vector_type**)&pVec;
	    *tmp_vec = NULL;
          } 
	}
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix increment operator.
    const_iterator operator++(int) const {
      const_iterator temp = *this;
      operator++();
      return temp;
    }

    /**  compares two utlHashMap const_iterators. Two
     *   undefined or invalidated const_iterators are always considered as
     *   equal. 
     */
    bool operator==(const const_iterator& x) const {
      return ( current == x.current || (!pVec && !x.pVec) );
    }  

    /**  compares two utlHashMap const_iterators. Two
     *   undefined or invalidated const_iterators are always considered as
     *   equal. 
     */
    bool operator!=(const const_iterator& x) const {
      return !operator==(x);
    }
  }; // const_iterator

  /* With this, the nested class iterator is concluded, so
     that now the data and methods of the utlHashMap class can follow:
  */
 protected:

  /// hash vector. It is the vector whose elements are pointers to linked lists.
  vector_type v;

  /// hash function. It is the function object used for calculation of the hash address. 
  hasher hf;

  /// the number of stored pairs of keys and data. 
  size_type Count;

  /// the comparison function object.
  key_equal compare;

  /**   constructs a utlHashMap object out of another one.
   *	Auxiliary function which is called both in the 
   *	copy constructor and in the assignment operator. 
   *	It has been extracted in order to avoid code duplication in 
   *	copy constructor and assignment operator. 
   *
   *	@see utlHashMap(), operator=().
   */
  void construct(const utlHashMap& S) {
    hf = S.hf;
    v = vector_type(S.bucket_count(),NULL);
    Count = 0;
    compare = S.compare;

    // begin(), end(), insert(): see below
    const_iterator t = S.begin();
    while(t != S.end())
      insert(*t++);
  }

  /**   rehashes this utlHashMap.
   *    A new hash vector with a capacity equal
   *	to the next prime greater than the double 
   *    of the previous capacity is used.
   *
   *	@see resize().
   */
  void rehash() {
    resize ( nextPrime(2*bucket_count()) );
  }

 public:

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlHashMap( ) : v (hashFun().tableSize(),NULL), hf(hashFun()), 
                  Count(0), compare(key_equal()) {}

  /// constructor from number of buckets. Creates an empty hash_map with at least n buckets.
  utlHashMap(size_type n) : v(n,NULL), hf(hashFun()), 
                            Count(0), compare(key_equal()) {
	hf.setTableSize ( n );
  }

  /** constructor from number of buckets and a hash function. 
   *  Creates an empty hash_map with at least n buckets, using f as the hash function.
   */
  utlHashMap(size_type n, const hasher& f) : v(n,NULL), hf(f), 
                                             Count(0), compare(key_equal()) {
	hf.setTableSize ( n );
  }

  /** 	creates an empty hash_map with at least n buckets, using h as the hash function 
   *	and k as the key equal function. 
   */
  utlHashMap(size_type n, const hasher& h, const key_equal& k) :
                                             v(n,NULL), hf(h), 
                                             Count(0), compare(k) {
	hf.setTableSize ( n );
  }

  /// creates a hash_map with a copy of a range. 
  template <class InputIterator> utlHashMap(InputIterator f, InputIterator l) : 
  		          v (hashFun().tableSize(),NULL), hf(hashFun()), 
                          Count(0), compare(key_equal()) {
        insert ( f, l );
  }

  /// creates a hash_map with a copy of a range and a bucket count of at least n. 
  template <class InputIterator> 
  utlHashMap(InputIterator f, InputIterator l, size_type n) : 
  			  v(n,NULL), hf(hashFun()), Count(0), compare(key_equal()) {
	hf.setTableSize ( n );
        insert ( f, l );
  }

  /** creates a hash_map with a copy of a range and a bucket count of at least n, 
   *  using h as the hash function. 
   */
  template <class InputIterator>
  utlHashMap(InputIterator f, InputIterator l, size_type n, const hasher& h) :
  			  v(n,NULL), hf(h), Count(0), compare(key_equal()) {
	hf.setTableSize ( n );
        insert ( f, l );
  }

  /** creates a hash_map with a copy of a range and a bucket count of at least n, 
   *  using h as the hash function and k as the key equal function. 
   */
  template <class InputIterator>
  utlHashMap(InputIterator f, InputIterator l, size_type n, const hasher& h, 
             const key_equal& k) : v(n,NULL), hf(h), Count(0), compare(k) {
	hf.setTableSize ( n );
        insert ( f, l );
  }

  /** 	copy constructor.
   *
   *	@see construct().
   */
  utlHashMap(const utlHashMap& S) {
    this->construct(S);
  }

  /** 	destructor. 
   *
   *  	@see clear().
   */
  virtual ~utlHashMap() {
    this->clear();                       // see below
  }
  //@}

  /// returns next prime greater than N. Assumes N >= 5.
  int nextPrime( int N ) {
    	if( N % 2 == 0 )	// N is even 
           N++;

        int i;
    	for( ; ; N += 2 )	// N is odd
           {
            for( i = 3; i * i <= N; i += 2 )
                if( N % i == 0 )
                   break;
            if( i * i > N )
                return N;
           }
    }

  /// returns the number of pairs <const key, value> in this hash_map.
  size_type size() const { return Count;}

  /** returns the capacity of this hash table vector.
   *  As opposed to other containers, max_size() does not indicate the maximum
   *  number of elements that can be stored in a utlHashMap container,
   *  which is only limited by the capacity of the lists, but the
   *  number of available hash table entries. This information is
   *  more sensible, because the efficiency of a utlHashMap depends on the
   *  occupation range alpha, assuming a good hash function. The
   *  occupation rate can easily be determined: alpha = size()/max_size(). 
   *
   *  @return capacity of this hash vector.
   */
  size_type max_size() const { return v.capacity(); }

  /// returns the number of buckets used by the hash_map (capacity of the hash table vector).
  size_type bucket_count() const { return v.capacity(); }

  /// returns whether this hash_map is empty.
  bool empty() const { return Count == 0; } 

  /** 	resets this hash_map, which is made empty.
   *
   *	@param f a new hash function.
   *	@see clear().
   */
  void reset(hasher f = hasher()) {
    this->clear();
    this->hf = f; 
    this->v = vector_type(f.tableSize(),NULL); 
    this->Count = 0;
  }

  /**   increases the bucket count to at leats n.
   *    A new hash vector with a capacity equal
   *	to the given number is used.
   */
  void resize ( size_type n ) {
    if ( n <= bucket_count() ) return;
    utlHashMap S ( *this );
    for(size_type i = 0; i < v.size(); i++) v[i]=NULL;
    hf.setTableSize ( n );
    v.resize (hf.tableSize(),NULL);
    Count = 0;

    iterator t = S.begin();
    while(t != S.end())
      insert(*t++);
  }

  /**   makes this hash_map empty. Uses delete to call the destructor of each list
   *	referred to by a vector element. Subsequently, the vector
   *	element is marked as unoccupied.
   */
  void clear() {
    for(size_type i = 0; i < v.size(); i++)
      if(v[i])                   // does list exist?
	{
	 delete v[i];
	 v[i] = NULL;
	}
    Count = 0;
  }

  /// returns the hasher object used by the hash_map.
  hasher hash_funct ( ) const { return hf; } 

  /// returns the key_equal object used by the hash_map.
  key_equal key_eq ( ) const { return compare; } 

  /// returns first non-empty entry.
  iterator begin() {
    size_type adr = 0;
    while(adr < v.size())
      {
	if(!v[adr])    // found nothing?
	  adr++;       // continue search
	else
	  return iterator(v[adr]->begin(), adr, &v);
      }
    return iterator();
  }

  /// returns an empty iterator.
  iterator end() {
    return iterator();
  }

  /// returns first non-empty entry.
  const_iterator begin() const {
    size_type adr = 0;
    while(adr < v.size())
      {
	if(!v[adr])    // found nothing?
	  adr++;       // continue search
	else
	  return const_iterator(v[adr]->begin(), adr, &v);
      }
    return const_iterator();
  }

  /// returns an empty const_iterator.
  const_iterator end() const {
    return const_iterator();
  }

  /** 	finds an element with a given key.
   *    In the following find() and insert() functions, the sought
   *	address within the vector v is calculated directly by means of
   *	the hash function object. If the vector element contains a
   *	pointer to a list, the list is searched in find() by means of
   *	the list iterator temp until an element with the correct key is
   *	found or the list has been completely processed. 
   *
   *	@param k given key.
   *	@return an iterator with the found element, or an end() iterator,
   *	        if the element has not been found.
   */
  iterator find(const key_type& k) {
    size_type address = hf(k);     // calculate address

    if(!v[address])
      return iterator();          // not existent
    typename list_type::iterator temp = v[address]->begin();

    // find k in the list
    while(temp != v[address]->end())
      if (compare((*temp).first, k)) {
	return iterator( temp, address, &v); // -> found!
      } else ++temp;
       
    return iterator();
  }

  /** 	finds an element with a given key.
   *    In the following find() and insert() functions, the sought
   *	address within the vector v is calculated directly by means of
   *	the hash function object. If the vector element contains a
   *	pointer to a list, the list is searched in find() by means of
   *	the list iterator temp until an element with the correct key is
   *	found or the list has been completely processed. 
   *
   *	@param k given key.
   *	@return a const_iterator with the found element, or an end() const_iterator,
   *	        if the element has not been found.
   */
  const_iterator find(const key_type& k) const {
    size_type address = hf(k);     // calculate address

    if(!v[address])
      return iterator();          // not existent
    typename list_type::const_iterator temp = v[address]->begin();

    // find k in the list
    while(temp != v[address]->end())
      if (compare((*temp).first, k)) {
	return const_iterator( temp, address, &v); // -> found!
      } else ++temp;
       
    return const_iterator();
  }

  /**   returns the value of the element with a given key.
   *    A map stores pairs of keys and associated data, where the first
   *	element (first) is the key and the second element (second)
   *	contains the data. find() returns an iterator which can be
   *	interpreted as a pointer to a pair. In order to obtain the data
   *	belonging to a key, the index operator can be called with the
   *	key as argument. 
   *    If the key is not found, a new pair with a default value, is inserted.
   *
   *	@param k given key.
   *	@return a reference to the value of a new pair with the given key,
   *		or the value of the found key.
   */
  reference operator[](const key_type& k) {
       return (*(insert ( value_type ( k, data_type() ) )).first).second;
  }

  /** 	assignment operator.
   *
   * 	@see clear(), construct().
   */
  utlHashMap& operator=(const utlHashMap& S) {
    if(this != &S)
      {
	this->clear();
	this->construct(S);
      }
    return *this;
  }

  /** inserts a pair <const key,value> into the hash_map.
   *  As in the STL, insert() returns a pair whose first part
   *  consists of the iterator that points to the found position. The
   *  second part indicates whether the insertion has taken place or
   *  not. 
   *
   *  @param P given pair.
   *  @return a pair with a found value and a boolean.
   *  @see rehash().
   */
  utlPair<iterator, bool> insert(const value_type& P) {
    if ( (float)size()/(float)bucket_count() > 0.75 ) 
         rehash();


    iterator temp = find(P.first);
    bool inserted = false;

    if(!temp) // not present
      {
	size_type address = hf(P.first);
	if(!v[address])
	  v[address] = new list_type;
	v[address]->push_back(P);
        /*   
          If the key does not exist, that is, if find() returns an end()
          iterator, a run time error occurs while dereferencing! (See the
          dereferencing operator).
        */
	temp = find(P.first); // redefine temp
	inserted = true;
	Count++;
      }
/*
    After the insertion, temp is redefined, because the iterator at
    first does not point to an existing element. The known
    auxiliary function makepair() generates a pair object to be
    returned. 
*/
    return cgcUtil::make_pair(temp, inserted);
  }

  /**	inserts a range into the hash_map. 
   *
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator> void insert(InputIterator f, InputIterator l) {
     while ( f != l ) insert ( *f++ ); 
  }

  /**   erases the element in a given iterator.
   *
   *	@param q given iterator.
   */
  void erase(iterator q) { 
    /* If the iterator is defined at all, the element function
       erase() of the associated list is called. Subsequently, the
       list is deleted, provided it is now empty, and the vector
       element to which the list is attached, is set to 0. 
    */
    if(q.pVec)             // defined?
      {
	v[q.Address]->erase(q.current);

	if(v[q.Address]->empty())
          {
	    delete v[q.Address];
	    v[q.Address] = NULL;
          }
	Count--;
      }
  }

  /** 	erases all elements having a given key.
   *    Sometimes, one would probably like to delete all elements of a
   *	map that have a given key. In a utlHashMap, this can at most be one
   *	element, but in a utlHashMultiMap, several elements might be
   *	affected. 
   *
   *	@param k given key.
   *	@return number of deleted elements.
   */
  size_type erase(const key_type& k) {
    size_type deleted_elements = 0; // Count
    // calculate address
    size_type address = hf(k);
    if(!v[address])
      return 0;         // not present

    typename list_type::iterator temp = v[address]->begin();

    /* In the following loop, the list is searched. An iterator
       called pos is used to remember the current position for the
       deletion itself. 
    */
    while(temp != v[address]->end())
      {
	if (compare((*temp).first, k))
          {
	    typename list_type::iterator pos = temp++;

	    v[address]->erase(pos);
	    // pos is now undefined

	    Count--;
	    deleted_elements++;
          }
	else ++temp;
      }

    /* The temporary iterator temp is advanced in both branches of
       the if instruction. The operation ++ cannot be extracted in
       order to save the else, because temp would then be identical
       with pos which is undefined after the deletion, and a
       defined ++ operation would no longer be possible. 
    */
    // delete hash table entry if needed
    if(v[address]->empty())
      {
	delete v[address];
	v[address] = NULL;
      }
    return deleted_elements;
  }

  /** erases all elements in a range. 
   *
   *  @param first beginning of the range.
   *  @param last end of the range.
   */
  void erase(iterator first, iterator last) {
      if ( first.Address == last.Address ) {
           size_t address = first.Address;
           v[address]->erase ( first.current, last.current );

           // delete hash table entry if needed
           if(v[address]->empty())
             {
	      delete v[address];
	      v[address] = NULL;
             }
      }
      else {
           iterator temp;
           while ( first != last ) {
             temp = first; ++temp;
             erase (first);
             first = temp;
           } 
      }
  }

  /// swaps the contents of this and a given utlHashMap.
  void swap(utlHashMap& s) {
    v.swap(s.v);
    std::swap(Count, s.Count);
    std::swap(hf, s.hf);        
  }

  /// comparison operator. Tests two hash_maps for equality.
  friend bool operator == ( const utlHashMap& hmap1, const utlHashMap& hmap2 ) {
    if ( &hmap1 == &hmap2 ) return true;
    if ( hmap1.size() != hmap2.size() ) return false;    

    return cgcUtil::equal ( hmap1.begin(), hmap1.end(), hmap2.begin() );
  }

  /// output operator.
  friend ostream& operator<<(std::ostream& out, const utlHashMap& obj) {
    const_iterator iter = obj.begin();
    while(iter != obj.end()) {
      cout << (*iter).first << ':'
	   << (*iter).second      
	   << endl;
      ++iter;
    }
    return out;
  }
    
  /// returns how many pairs with a given key are in the hash_multimap.
  size_type count ( const key_type& k ) const { 
    size_type nb = 0;

    const_iterator temp = find( k );
    if ( temp ) {
      typename list_type::const_iterator it = temp.current; 
      while ( compare((*it).first, k) ) {
         ++nb;
         ++it;
      }
    }  
  
    return nb; 
  }

  /**  returns two iterators pointing to the beginning and the end of a subsequence
   *   of elements with a given key.
   *
   *   @param k given key.
   *   @return a pair of iterators.
   */
  utlPair<iterator, iterator> equal_range ( const key_type& k ) { 
       iterator it1, it2 = end(); 
       
       it1 = find( k );
       if ( it1 ) {	// found at least one
         it2 = it1;
         typename list_type::iterator last = v[it1.Address]->end(), it;
         do {
           ++it2;
           it = it2.current; 
         }
         while ( it != last && compare((*it).first, k) );
       }
       return cgcUtil::make_pair ( it1, it2 ); 
  }

  /**  returns two const_iterators pointing to the beginning and the end of a subsequence
   *   of elements with a given key.
   *
   *   @param k given key.
   *   @return a pair of const_iterators.
   */
  utlPair<const_iterator, const_iterator> equal_range ( const key_type& k ) const { 
       const_iterator it1, it2 = end(); 
       
       it1 = find( k );
       if ( it1 ) {	// found at least one
         it2 = it1;
         typename list_type::const_iterator last = v[it1.Address]->end(), it;
         do {
           ++it2;
           it = it2.current; 
         }
         while ( it != last && compare((*it).first, k) );
       }
       return cgcUtil::make_pair ( it1, it2 ); 
  }

};


/**	Hashed multimap with external chaining. 
 *
 *	Hash_multimap is a Hashed Associative Container that 
 *	associates objects of type Key with objects of type Data. 
 *	Hash_multimap is a Pair Associative Container, 
 *	meaning that its value type is pair<const Key, Data>. 
 *	It is also a Multiple Associative Container, meaning that there 
 *	is no limit on the number of elements whose keys may compare 
 *	equal using EqualKey. 
 *
 *	Looking up an element in a hash_multimap by its key is efficient, 
 *	so hash_multimap is useful for "dictionaries" where the order of 
 *	elements is irrelevant. 
 *	If it is important for the elements to be in a particular order, 
 *	however, then multimap is more appropriate. 
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.4 $ $Date: 2018/10/11 14:44:12 $
 *	
 */
template < class Key, class T, class hashFun = utlHashFun<Key>, class Cmp=eqt_to<Key> >
class utlHashMultiMap : public utlHashMap <Key,T,hashFun,Cmp>
{
 public:
  /// a pair <const key,value>.
  typedef utlPair<const Key,T> value_type;
  /// iterator declaration.
  typedef typename utlHashMap<Key,T,hashFun,Cmp>::iterator iterator;
  /// const_iterator declaration.
  typedef typename utlHashMap<Key,T,hashFun,Cmp>::const_iterator const_iterator;
  /// the hash_map's hash function.
  typedef hashFun hasher;

  /// size type.
  typedef size_t size_type;

  /// the hash_map's comparison function.
  typedef Cmp key_equal;

  /// reference type.
  typedef T& reference;

  /// const_reference type.
  typedef const T& const_reference;

  /// key type.
  typedef Key key_type;

  /// a list of values.
  typedef utlList<value_type> list_type;

  /// a vector of list pointers.
  typedef utlVector<list_type*> vector_type;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlHashMultiMap( ) : utlHashMap<Key,T,hashFun,Cmp>() {}

  /// creates a hash_multimap with a copy of a range. 
  template <class InputIterator> utlHashMultiMap(InputIterator f, InputIterator l) : 
		utlHashMap<Key,T,hashFun,Cmp>() {
       insert ( f, l );
  }

  /// creates a hash_multimap with a copy of a range and a bucket count of at least n. 
  template <class InputIterator> 
  utlHashMultiMap(InputIterator f, InputIterator l, size_type n) : 
		utlHashMap<Key,T,hashFun,Cmp>(n) {
        insert ( f, l );
  }

  /** creates a hash_multimap with a copy of a range and a bucket count of at least n, 
   *  using h as the hash function. 
   */
  template <class InputIterator>
  utlHashMultiMap(InputIterator f, InputIterator l, size_type n, const hasher& h) :
		utlHashMap<Key,T,hashFun,Cmp>(n,h) {
        insert ( f, l );
  }

  /** creates a hash_multimap with a copy of a range and a bucket count of at least n, 
   *  using h as the hash function and k as the key equal function. 
   */
  template <class InputIterator>
  utlHashMultiMap(InputIterator f, InputIterator l, size_type n, const hasher& h, 
                  const key_equal& k) : utlHashMap<Key,T,hashFun,Cmp>(n,h,k) {
        insert ( f, l );
  }

  /** 	copy constructor.
   *
   *	@see construct().
   */
  utlHashMultiMap(const utlHashMultiMap& S) {
    this->construct(S);
  }

  //@}

  /** inserts a pair <const key,value> into the hash_multimap.
   *
   *  @param P given pair.
   *  @return iterator to the inserted pair.
   *  @see rehash().
   */
  iterator insert (const value_type& P) {
    if ( (float)this->size()/(float)this->bucket_count() > 0.75 ) 
         this->rehash();

    iterator temp = this->find(P.first);

    if(!temp) // not present
      {
       size_type address = this->hf(P.first);
       if(!this->v[address])
          this->v[address] = new list_type;
       this->v[address]->push_back(P);
       /*   
         If the key does not exist, that is, if find() returns an end()
         iterator, a run time error occurs while dereferencing! (See the
         dereferencing operator).
       */
       temp = this->find(P.first); 
      }
    else {	// found
       typename list_type::iterator it = temp.current; 
       this->v[temp.Address]->insert ( it, P );	// insert before the found pair 
       --(temp.current);
    }
       
    this->Count++;
    return temp;
  }

  /**	inserts a range into the hash_multimap. 
   *
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator> void insert(InputIterator f, InputIterator l) {
     while ( f != l ) insert ( *f++ ); 
  }

  /// bracket operator is non-sense in a hash_multimap.
  reference operator[] ( const key_type& k ) { return T(); } 

  /** 	assignment operator.
   *
   * 	@see clear(), construct().
   */
  utlHashMultiMap& operator=(const utlHashMultiMap& S) {
    if(this != &S)
      {
	this->clear();
	this->construct(S);
      }
    return *this;
  }

  private:

  /**   constructs a utlHashMultiMap object out of another one.
   *	Auxiliary function which is called both in the 
   *	copy constructor and in the assignment operator. 
   *	It has been extracted in order to avoid code duplication in 
   *	copy constructor and assignment operator. 
   *
   *	@see utlHashMultiMap(), operator=().
   */
  void construct(const utlHashMultiMap& S) {
    this->hf = S.hf;
    this->v = vector_type(S.bucket_count(),NULL);
    this->Count = 0;
    this->compare = S.compare;

    // begin(), end(), insert(): see below
    const_iterator t = S.begin();
    while(t != S.end())
      insert(*t++);
  }


};


/// hash function.
template<class Key>
class utlHashFun {
 public:

  /** @name constructors  */ 
  //@{

  /** constructor.
   *
   *  @param prime hash table size.
   */
  utlHashFun(size_t prime=1009) : tabSize(prime) {}

  /// copy constructor.
  utlHashFun( const utlHashFun& hFun ) {
     *this = hFun;
  }
  //@}

  /// assignment operator. 
  utlHashFun& operator = ( const utlHashFun& hFun ) {
     this->tabSize = hFun.tabSize;
     return *this;
  }

  /// returns the address of a given key.
  template <class T> 
  size_t operator()(const T& p) const { 
#if 0
     return size_t(p) % tabSize; 
#else
     size_t res = 0;
     size_t len = sizeof(T);
     const char* cp = reinterpret_cast<const char*> (&p);
     while (len--) res = (res<<1)^*cp++;
     return res % tabSize;
#endif
  }

  /// returns the address of a given string.
  size_t operator()(const utlString& p) const { 
    return operator()(p.c_str());
  }

  /// returns the address of a given const char*.
  size_t operator()(const char* p) const { 
#if 0
     size_t h = 0; 
     for ( ; *s; ++s )
       h = 5*h + *s;
     return h % tabSize;
#else
     size_t res = 0;
     while (*p) res = (res<<1)^*p++;
     return res % tabSize;
#endif
  }

  /// returns hash table size.
  size_t tableSize() const { return tabSize; }

  /// sets a new table size. Do NOT call this method.
  void setTableSize ( size_t ts ) { tabSize = ts; }

 private:
  /// hash table size.
  size_t tabSize;
};


#endif   // __UTLHASHMAP_H__
#endif
