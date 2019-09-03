/*
  utlMap.h

  Program:   utl
  Module:    $RCSfile: utlMap.h,v $
  Language:  C++
  Date:      $Date: 2005/10/10 01:32:51 $
  Version:   $Revision: 1.2 $
*/

#ifndef __UTLMAP_H__
#define __UTLMAP_H__

#include "utlPair.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#define utlMap map 
#define utlMultiMap multimap 
#else

// implicit data structures
#include "utlIteratorBase.h"
#include "utlIterate.h"
#include "utlReverseIterator.h"
#include "Splay.h"
#include "AVLTree.h"
#include "RedBlack.h"
#include "AATree.h"

/// forward declaration of utlMultiMap.
template <class Key, class T, class Cmp, class Tree> class utlMultiMap;


  /**	compares the keys of two pairs. 
   *
   *	@param x first pair.
   *	@param y second pair.	
   *	@return comp(x.first, y.first) || 
   *          (!comp(y.first, x.first) && x.second < y.second)).
   */
  template <class Etype> class compare_pair {
 
  public:

  /// empty constructor.
  compare_pair ( ) {}

  /// function call operator.
  template <class Key, class T>
  bool operator ( ) ( const utlPair<const Key,T>& x, const utlPair<const Key,T>& y ) const {
   return (x.first < y.first) || (!(y.first < x.first) && x.second < y.second);
  }

  /// function eq operator.
  template <class Key, class T>
  bool eq ( const utlPair<const Key,T>& x, const utlPair<const Key,T>& y ) const {
   return (x.first == y.first && x.second == y.second);
  }

  /// function neq operator.
  template <class Key, class T>
  bool neq ( const utlPair<const Key,T>& x, const utlPair<const Key,T>& y ) const {
   return !(x.first == y.first);
  }

  };

/**	map for sorted dictionaries. 
 *
 *	Map is a Sorted Associative Container that associates objects 
 *	of type Key with objects of type Data. Map is a Pair Associative Container, 
 *	meaning that its value type is pair<const Key, Data>. 
 *	It is also a Unique Associative Container, meaning that no two elements have the same key. 
 *
 *	Map has the important property that inserting a new element into a map does not 
 *	invalidate iterators that point to existing elements. Erasing an element from a
 *	map also does not invalidate any iterators, except, of course, for iterators that 
 *	actually point to the element that is being erased. 
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2005/10/10 01:32:51 $
 *	
 */
template < class Key, class T, class Cmp  = compare_pair<utlPair<const Key,T> >,
                               class Tree = AVLTree<utlPair<const Key,T>, Cmp> >
class utlMap
{
 public:

/* 
   The template parameter Key stands for the type of the key; 
   T stands for the class of data associated to a key; 
   value_type is the type of the elements that are stored in 
   a utlMap object. value_type is a pair consisting of a 
   constant key and the associated data. 
*/

  /// key type.
  typedef Key key_type;

  /// data type.
  typedef T data_type;

  /// a pair <const key,value>.
  typedef utlPair<const Key,T> value_type;

  /// function object that compares two keys for ordering. 
  typedef Cmp key_compare;

  /// pointer type.
  typedef T* pointer;

  /// reference type.
  typedef T& reference;

  /// const_reference type.
  typedef const T& const_reference;

  /// size type.
  typedef size_t size_type;

  /// pointer difference type.
  typedef ptrdiff_t difference_type;

  /// type of the search tree.
  typedef Tree tree_type;

  /// type of iterator for the search tree.
  typedef InOrder <value_type,Cmp> iterator_type; 

  /// a binary node definition.
  typedef typename SearchTree<value_type,Cmp>::BinaryNode BinaryNode;

  /// function object that compares two values for ordering. 
  class value_compare {
      /// utlMap is a friend class.
      friend class utlMap<Key,T,Cmp,Tree>;
      protected:
        /// function object that compares two keys.
        key_compare cmp;
        /// constructor.
        value_compare (key_compare c) : cmp(c) {}
      public:
        /// compares x and y.
        bool operator () (const value_type& x, const value_type& y) const {
             return cmp(x,y);
        }
   };

  /// forward declaration of iterator class.
  class iterator;

  /// forward declaration of const_iterator class.
  class const_iterator;

  /// iterator is a friend class of utlMap.
  friend class iterator;

  /// forward declaration of reverse_iterator.
  typedef utl_reverse_iterator <iterator> reverse_iterator;

  /// forward declaration of const_reverse_iterator.
  typedef utl_reverse_iterator <const_iterator> const_reverse_iterator;

  /** iterator for map.
    * The nested class iterator closely cooperates with utlMap, so that
    * both are mutually declared as friend. iterator is only supposed
    * to allow forward traversal. An iterator object
    * allows you to visit all elements of a utlMap object one after
    * the other. An order or sorting is defined for the
    * elements. The visiting order of the iterator is given by the
    * implicit data structure (see below, operator++()). 
    */
  class iterator : public utlIteratorBase<iterator> {

    /// utlMap is a friend class of iterator.
    friend class utlMap<Key,T,Cmp,Tree>;
    /// utlMultiMap is a friend class of iterator.
    friend class utlMultiMap<Key,T,Cmp,Tree>;

  public:

    /// iterator category.
    typedef typename utlIteratorBase<iterator>::bidirectional_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlMap<Key,T,Cmp,Tree>::value_type value_type;
    /// type of difference between two iterators.
    typedef typename utlMap<Key,T,Cmp,Tree>::difference_type difference_type;
    /// return type of operator ->().
    typedef value_type* pointer;
    /// return type of operator *().
    typedef value_type& reference;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    iterator() : tree(0), itr(0) {}

    /// constructor from a tree.
    iterator(tree_type* t) : tree(t), itr(t) {}

    /// constructor from a node and a tree.
    iterator(const BinaryNode* n, tree_type* t) : tree(t), itr(n,t) {}

    /// copy constructor.
    iterator(const iterator& it) { 
      *this = it;
    }

  //@}

    /// const dereferencing operator.
    const reference operator*() const {
      return itr();
    }  

    /** non-const dereferencing operator.
     *  The non-const variation is required to be able to modify
     *	data independently from the key. Modification of the key
     *	must be excluded because it requires a new address
     *	calculation. Constancy is guaranteed by the const
     *	declaration in the type definition of value_type.
     */
    reference operator*() {
      return (reference) itr();
    }

    /// non-const pointer operator.
    pointer operator->() {
      return (pointer) &itr();
    }

    /// prefix increment operator. 
    iterator& operator++() {
      ++itr;
      return *this;
    }

    /// posfix increment operator.
    iterator operator++(int) {
      iterator temp = *this;
      operator++();
      return temp;
    }

    /// prefix decrement operator. 
    iterator& operator--() {
      iterator prv = *this; prv.itr.First();
      iterator tmp = prv; ++tmp;
      for ( ; tmp != *this; ++tmp, ++prv );
      *this = prv;
      return *this;
    }

    /// posfix decrement operator.
    iterator operator--(int) {
      iterator temp = *this;
      operator--();
      return temp;
    }

    /// assignment operator.
    iterator& operator = ( const iterator& rhs ) {
      tree = rhs.tree;
      itr = rhs.itr;
      return *this;
    }

  private:
    /// const_iterator is a friend class.
    friend class const_iterator;

    /// == is a friend operator.
    friend bool operator==(const iterator& it1, const iterator& it2);

    /// != is a friend operator.
    friend bool operator!=(const iterator& it1, const iterator& it2);

    /// a pointer to the tree being iterated. 
    tree_type* tree;

    /// iterator for the tree.
    iterator_type itr; 

  }; // iterator

  /// compares two utlMap iterators. 
  friend inline bool operator==(const iterator& it1, const iterator& it2) {
      return ( it1.itr == it2.itr );
  }  

  /// compares two utlMap iterators.
  friend inline bool operator!=(const iterator& it1, const iterator& it2) {
      return !(it1.itr==it2.itr);
  }

  /** const_iterator for map.
    * The nested class const_iterator closely cooperates with utlMap, so that
    * both are mutually declared as friend. iterator is only supposed
    * to allow forward traversal. A const_iterator object
    * allows you to visit all elements of a utlMap object one after
    * the other. An order or sorting is defined for the
    * elements. The visiting order of the const_iterator is given by the
    * implicit data structure (see below, operator++()). 
    */
  class const_iterator : public utlIteratorBase<const_iterator> {

    /// utlMap is a friend class of const_iterator.
    friend class utlMap<Key,T,Cmp,Tree>;
    /// utlMultiMap is a friend class of const_iterator.
    friend class utlMultiMap<Key,T,Cmp,Tree>;

  public:

    /// const_iterator category.
    typedef typename utlIteratorBase<const_iterator>::bidirectional_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlMap<Key,T,Cmp,Tree>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlMap<Key,T,Cmp,Tree>::difference_type difference_type;
    /// return type of operator ->().
    typedef value_type* pointer;
    /// return type of operator *().
    typedef const value_type& reference;

  /** @name constructors  */ 
  //@{

    /// empty constructor.
    const_iterator() : tree(0), itr(0) {}

    /// constructor from a tree.
    const_iterator(const tree_type* t) : tree(t), itr(t) {}

    /// constructor from a node and a tree.
    const_iterator(const BinaryNode* n, const tree_type* t) : tree(t), itr(n,t) {}

    /// copy constructor.
    const_iterator(const const_iterator& it) { 
      *this = it;
    }

    /// constructor from an iterator.
    const_iterator(const iterator& it) : tree(it.tree), itr(it.itr) {}

  //@}

    /// const dereferencing operator.
    const reference operator*() const {
      return itr();
    }  

    /// const pointer operator.
    const pointer operator->() const {
      return (const pointer) &itr();
    }

    /// prefix increment operator. 
    const_iterator& operator++() const {
      iterator_type* itTemp = (iterator_type*)&itr;
      ++(*itTemp);  
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix increment operator.
    const_iterator operator++(int) const {
      const_iterator temp = *this;
      operator++();
      return temp;
    }

    /// prefix decrement operator. 
    const_iterator& operator--() const {
      const_iterator prv = *this; prv.itr.First();
      const_iterator tmp = prv; ++tmp;
      for ( ; tmp != *this; ++tmp, ++prv );

      iterator_type* itTemp = (iterator_type*)&itr;
      *itTemp = prv.itr;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

    /// posfix decrement operator.
    const_iterator operator--(int) const {
      const_iterator temp = *this;
      operator--();
      return temp;
    }

    /// compares two utlMap const_iterators. 
    friend inline bool operator==(const const_iterator& it1, const const_iterator it2) {
      return ( it1.itr == it2.itr );
    }  

    /// compares two utlMap const_iterators.
    friend inline bool operator!=(const const_iterator& it1, const const_iterator it2) {
      return !(it1.itr==it2.itr);
    }

    /// assignment operator.
    const_iterator& operator = ( const const_iterator& rhs ) const {
      iterator_type* tempItr = (iterator_type*)&itr;
      *tempItr = rhs.itr;
      tree_type** tempTree = (tree_type**)&tree;
      *tempTree = (tree_type*)rhs.tree;
      const_iterator* lthis = (const_iterator*)this;
      return *lthis;
    }

  private:
    /// a pointer to the tree being iterated. 
    const tree_type* tree;

    /// const_iterator for the tree.
    iterator_type itr; 

  }; // const_iterator

 protected:

  /// map tree. 
  tree_type mT;

  /// the number of stored pairs of keys and data. 
  size_type Count;

  /// function object that compares two keys for ordering. 
  key_compare compare;

  /// function object that compares two values for ordering.
  value_compare val_compare;

 public:

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlMap( const tree_type& t = tree_type(key_compare()) ) :
      mT(t), Count(0), compare(key_compare()), val_compare(key_compare()) {}

  /// creates an empty map, using comp as the key_compare object. 
  utlMap(const key_compare& comp) : 
     mT(tree_type(comp)), Count(0), compare(comp), val_compare(comp) {}

  /// creates a map with a copy of a range. 
  template <class InputIterator> 
  utlMap(InputIterator f, InputIterator l) : 
  	          	mT(tree_type(key_compare())), Count(0), 
                        compare(key_compare()), val_compare(key_compare()) {
      insert ( f, l );
  }

  /// creates a map with a copy of a range, using comp as the key_compare object. 
  template <class InputIterator>
  utlMap(InputIterator f, InputIterator l, const key_compare& comp) : 
			mT(tree_type(comp)), Count(0), 
                        compare(comp), val_compare(comp) {
      insert ( f, l );
  }

  /// copy constructor.
  utlMap(const utlMap& S) {
    *this = S;
  }

  /** 	destructor. 
   *
   *  	@see clear().
   */
  ~utlMap() {
    this->clear();   
  }
  //@}

  /// returns the number of pairs <const key, value> in this map.
  size_type size() const { return Count;}

  /** returns the maximum size of this map.
   *
   *  @return maximum size of this map.
   */
  size_type max_size() const { return 1; }

  /// returns whether this map is empty.
  bool empty() const { return mT.IsEmpty(); } 

  /// returns the key_compare object used by the map. 
  key_compare key_comp() const { return compare; }

  /// returns the value_compare object used by the map. 
  value_compare value_comp() const { return val_compare; }

  /// makes this map empty. 
  void clear() {
    mT.MakeEmpty();
    Count = 0;
  }

  /// returns an iterator pointing to the beginning of the map.
  iterator begin() {
    iterator iter (&mT); 
    iter.itr.First();
    return iter;
  }

  /// returns an iterator pointing to the end of the map.
  iterator end() {
    return iterator(&mT);
  }

  /// returns a const_iterator pointing to the beginning of the map.
  const_iterator begin() const {
    const_iterator iter (&mT); 
    iter.itr.First();
    return iter;
  }

  /// returns a const_iterator pointing to the end of the map.
  const_iterator end() const {
    return const_iterator(&mT);
  }

  /// returns a reverse_iterator pointing to the beginning of the reversed map.
  reverse_iterator rbegin() { 
    return reverse_iterator(end()); 
  }

  /// returns a reverse_iterator pointing to the end of the reversed map.
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /// returns a const_reverse_iterator pointing to the beginning of the reversed map.
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }

  /// returns a const_reverse_iterator pointing to the end of the reversed map.
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  /** 	finds an element whose key is k.
   *
   *	@param k given key.
   *	@return an iterator with the found element, or an end() iterator,
   *	        if the element has not been found.
   */
  iterator find(const key_type& k) {
    mT.FindP ( value_type(k,data_type()) );
    const BinaryNode* node = mT.GetCurrent();
    return (node != mT.GetNullNode() ? iterator( node, &mT ) : end());
  }

  /** 	finds an element whose key is k.
   *
   *	@param k given key.
   *	@return a const_iterator with the found element, or an end() const_iterator,
   *	        if the element has not been found.
   */
  const_iterator find(const key_type& k) const {
    mT.FindP ( value_type(k,data_type()) );
    const BinaryNode* node = mT.GetCurrent();
    return (node != mT.GetNullNode() ? const_iterator( node, &mT ) : end());
  }

  /** 	assignment operator.
   *
   * 	@see clear().
   */
  utlMap& operator=(const utlMap& S) {
    if(this != &S)
      {
       this->mT = S.mT;
       this->Count = S.Count;
      }
    return *this;
  }

  /** 	returns the k-th entry into the map for writing.
   *
   *	Since operator[] might insert a new element into the map, 
   *	it can't possibly be a const member function. Note that the definition 
   *	of operator[] is extremely simple: m[k] is equivalent to 
   *	(*((m.insert(value_type(k, data_type()))).first)).second. Strictly speaking, this
   *	member function is unnecessary: it exists only for convenience. 
   */
  reference operator[](const key_type& k) {
      return (*((insert(value_type(k, data_type()))).first)).second;
  }

  /** inserts a pair <const key,value> into the map.
   *  As in the STL, insert() returns a pair whose first part
   *  consists of the iterator that points to the found position. The
   *  second part indicates whether the insertion has taken place or
   *  not. 
   *
   *  @param P given pair.
   *  @return a pair with a found value and a boolean.
   */
  utlPair<iterator, bool> insert(const value_type& P) {
    iterator temp = find(P.first);
    bool inserted = false;

    if( temp == end() ) // not present
      {
       mT.Insert ( P );
       ++Count;
       temp = find(P.first); // redefine temp
       inserted = true;
      }

    return cgcUtil::make_pair(temp, inserted);
  }

  /// inserts x into the map, using pos as a hint to where it will be inserted. 
  iterator insert(iterator pos, const value_type& x) {
    return insert ( x ).first;
  }

  /**	inserts a range into the map. 
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
     mT.Remove ( *q );
     --Count;
  }

  /** 	erases all elements having a given key.
   *    Sometimes, one would probably like to delete all elements of a
   *	map that have a given key. In a utlMap, this can at most be one
   *	element, but in a utlMultiMap, several elements might be
   *	affected. 
   *
   *	@param k given key.
   *	@return number of deleted elements.
   */
  size_type erase(const key_type& k) {
    utlPair<iterator,iterator> itp = equal_range ( k );

    size_type deleted_elements = cgcUtil::distance (itp.first,itp.second); 
    if ( deleted_elements > 0 ) erase ( itp.first, itp.second );

    return deleted_elements;
  }

  /// erases all elements in a range. 
  void erase(iterator first, iterator last) {
    /* In the following loop, the tree is searched. 
       All elements to be deleted are saved in a list
       to avoid loosing the return point into the stack,
       which keeps the path from the root to the current
       element.
    */
    utlVector<utlPair<Key,T> > delem;
    while(first != last) 
       delem.push_back ( *first++ );

    for ( typename utlVector<utlPair<Key,T> >::iterator it = delem.begin(); it != delem.end(); ++it ) {
       mT.Remove ( *it );
       --Count;
    }

  }

  /// swaps the contents of this and a given utlMap.
  void swap(utlMap& s) {
    mT.swap ( s.mT ); 
    std::swap(Count, s.Count);
  }

  /// comparison operator. Tests two maps for equality.
  friend bool operator == ( const utlMap& map1, const utlMap& map2 ) {
    if ( &map1 == &map2 ) return true;
    if ( map1.size() != map2.size() ) return false;    

    return cgcUtil::equal ( map1.begin(), map1.end(), map2.begin() );
  }

  /// lexicographical comparison.
  friend bool operator < ( const utlMap& map1, const utlMap& map2 ) {
    if ( &map1 == &map2 ) return true;

    return cgcUtil::lexicographical_compare ( map1.begin(), map1.end(), map2.begin(), map2.end() );
  }

  /// output operator.
  friend ostream& operator<<(ostream& out, const utlMap& obj) {
    iterator iter = obj.begin();
    while(iter != obj.end()) {
      cout << (*iter).first << ':'
	   << (*iter).second      
	   << endl;
      +iter;
    }
    return out;
  }
    
  /// returns how many pairs with a given key are in the multimap.
  size_type count ( const key_type& k ) const { 
    size_type nb = 0;

    const_iterator it = find( k );
    const_iterator itr = it;
    if ( it != end() ) {
      do {
         ++nb;
         ++it;
      }
      while ( it != end() && (*it).first == k );
      while ( itr != begin() && (*itr).first == k ) {
         ++nb;
         --itr;
      }  
      if ( (*itr).first != k ) --nb;
    }  
  
    return nb; 
  }

  /**  returns two iterators pointing to the beginning and the end of a subsequence
   *   of elements whose keys are k.
   *
   *   If there is any element that have the same key as k, then the return value of
   *   the second iterator points to one past the last such element. 
   *
   *   @param k given key.
   *   @return a pair of iterators.
   */
  utlPair<iterator, iterator> equal_range ( const key_type& k ) { 
       iterator it1 = find( k ), it2 = end();
       if ( it1 != end() ) {	// found at least one
         it2 = it1;
         do
           ++it2;
         while ( it2 != end() && (*it2).first == k );
         while ( it1 != begin() && (*it1).first == k )
           --it1;
         if ( (*it1).first != k ) ++it1;
       }
       return cgcUtil::make_pair ( it1, it2 ); 
  }

  /**  returns two const_iterators pointing to the beginning and the end of a subsequence
   *   of elements whose keys are k.
   *
   *   If there is any element that have the same key as k, then the return value of
   *   the second iterator points to one past the last such element. 
   *
   *   @param k given key.
   *   @return a pair of const_iterators.
   */
  utlPair<const_iterator, const_iterator> equal_range ( const key_type& k ) const { 
       const_iterator it1 = find( k ), it2 = end();
       if ( it1 != end() ) {	// found at least one
         it2 = it1;
         do
           ++it2;
         while ( it2 != end() && (*it2).first == k );
         while ( it1 != begin() && (*it1).first == k )
           --it1;
         if ( (*it1).first != k ) ++it1;
       }
       return cgcUtil::make_pair ( it1, it2 ); 
  }

};

/**	multimap for sorted dictionaries with duplicate keys.
 *
 *	Multimap is a Sorted Associative Container that associates objects of 
 *	type Key with objects of type Data. multimap is a Pair Associative Container,
 *	meaning that its value type is pair<const Key, Data>. 
 *	It is also a Multiple Associative Container, meaning that there is no limit on the number of
 *	elements with the same key. 
 *
 *	Multimap has the important property that inserting a new element into a multimap 
 *	does not invalidate iterators that point to existing elements. Erasing an
 *	element from a multimap also does not invalidate any iterators, except, of course, 
 *	for iterators that actually point to the element that is being erased.
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2005/10/10 01:32:51 $
 *	
 */
template < class Key, class T, class Cmp  = compare_pair<utlPair<const Key,T> >, 
                               class Tree = AVLTree<utlPair<const Key,T>, Cmp > >
class utlMultiMap : public utlMap <Key,T,Cmp,Tree>
{
 public:
  /// a pair <const key,value>.
  typedef utlPair<const Key,T> value_type;

  /// key type.
  typedef typename utlMap<Key,T,Cmp,Tree>::key_type key_type;

  /// iterator declaration.
  typedef typename utlMap<Key,T,Cmp,Tree>::iterator iterator;

  /// iterator declaration.
  typedef typename utlMap<Key,T,Cmp,Tree>::reference reference;

  /// type of the search tree.
  typedef typename utlMap<Key,T,Cmp,Tree>::tree_type tree_type;

  /// function object that compares two keys for ordering. 
  typedef typename utlMap<Key,T,Cmp,Tree>::key_compare key_compare;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlMultiMap( const tree_type& t = tree_type(key_compare()) ) : 
       utlMap<Key,T,Cmp,Tree>(t) {}

  /// creates a multimap with a copy of a range. 
  template <class InputIterator> 
  utlMultiMap(InputIterator f, InputIterator l) : 
  		        utlMap<Key,T,Cmp,Tree>() {
      insert ( f, l );
  }

  /// creates a multimap with a copy of a range, using comp as the key_compare object. 
  template <class InputIterator>
  utlMultiMap(InputIterator f, InputIterator l, const key_compare& comp) : 
			utlMap<Key,T,Cmp,Tree>(comp) {
      insert ( f, l );
  }

  //@}

  /** inserts a pair <key,value> into the multimap.
   *
   *  @param P given pair.
   *  @return iterator to the inserted pair.
   */
  iterator insert(const value_type& P) {

    this->mT.Insert ( P );
    this->mT.Find(P);
       
    ++this->Count;
    return iterator( this->mT.GetCurrent(), &this->mT );
  }

  /// inserts x into the multimap, using pos as a hint to where it will be inserted. 
  iterator insert(iterator pos, const value_type& x) {
    return insert ( x ).first;
  }

  /**	inserts a range into the multimap. 
   *
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator> void insert(InputIterator f, InputIterator l) {
     while ( f != l ) insert ( *f++ ); 
  }

  /// bracket operator is non-sense in a multimap.
  reference operator[] ( const key_type& k ) { return T(); } 
};

#endif   // __UTLMAP_H__
#endif
