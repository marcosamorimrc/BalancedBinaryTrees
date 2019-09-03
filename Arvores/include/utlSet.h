/*
  utlSet.h

  Program:   utl
  Module:    $RCSfile: utlSet.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 14:44:13 $
  Version:   $Revision: 1.3 $
*/

#ifndef __UTLSET_H__
#define __UTLSET_H__

#include "utlPair.h"

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#define utlSet set
#define utlMultiSet multiset
#else

// implicit data structures
#include "utlIteratorBase.h"
#include "utlIterate.h"
#include "utlReverseIterator.h"
#include "Splay.h"
#include "AVLTree.h"
#include "RedBlack.h"
#include "AATree.h"

/// forward declaration of utlMultiSet.
template <class Key, class Cmp, class Tree> class utlMultiSet;

/**	Set for sorted sets. 
 *
 *	Set is a Sorted Associative Container that stores objects of type Key. 
 *	Set is a Simple Associative Container, meaning that its value type, 
 *	as well as its key type, is Key. It is also a Unique Associative Container, 
 *	meaning that no two elements are the same. 
 *
 *	Set and multiset are particularly well suited to the set algorithms includes, 
 *	set_union, set_intersection, set_difference, and set_symmetric_difference. 
 *	The reason for this is twofold. First, the set algorithms require their arguments to 
 *	be sorted ranges, and, since set and multiset are Sorted Associative Containers, 
 *	their elements are always sorted in ascending order. 
 *	Second, the output range of these algorithms is always sorted, 
 *	and inserting a sorted range into a set or multiset is a fast operation: 
 *	the Unique Sorted Associative Container and Multiple Sorted
 *	Associative Container requirements guarantee that inserting a range takes only 
 *	linear time if the range is already sorted. 
 *
 *	Set has the important property that inserting a new element into a set does not 
 *	invalidate iterators that point to existing elements. Erasing an element from
 *	a set also does not invalidate any iterators, except, of course, 
 *	for iterators that actually point to the element that is being erased.  
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.3 $ $Date: 2018/10/11 14:44:13 $
 *	
 */
template < class Key, class Cmp=compare_to<Key>, class Tree = SplayTree<Key,Cmp> >
class utlSet
{
 public:

  /// key type.
  typedef Key key_type;

  /// data type.
  typedef Key value_type;

  /// function object that compares two keys for ordering. 
  typedef Cmp key_compare;

  /// function object that compares two values for ordering. 
  typedef Cmp value_compare;

  /// pointer type.
  typedef Key* pointer;

  /// reference type.
  typedef Key& reference;

  /// const_reference type.
  typedef const Key& const_reference;

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

  /// forward declaration of const_iterator class.
  class const_iterator;

  /// forward declaration of const_reverse_iterator.
  typedef utl_reverse_iterator <const_iterator> const_reverse_iterator;

  /// iterator and const_iterator are the same type.
  typedef const_iterator iterator;

  /// reverse_iterator and const_reverse_iterator are the same type.
  typedef const_reverse_iterator reverse_iterator;

  /** const_iterator for set.
    * The nested class const_iterator closely cooperates with utlSet, so that
    * both are mutually declared as friend. const_iterator is only supposed
    * to allow forward traversal. A const_iterator object
    * allows you to visit all elements of a utlSet object one after
    * the other. An order or sorting is defined for the
    * elements. The visiting order of the const_iterator is given by the
    * implicit data structure (see below, operator++()). 
    */
  class const_iterator : public utlIteratorBase<const_iterator> {

    /// utlSet is a friend class of const_iterator.
    friend class utlSet<Key,Cmp,Tree>;
    /// utlMultiSet is a friend class of const_iterator.
    friend class utlMultiSet<Key,Cmp,Tree>;

  public:

    /// const_iterator category.
    typedef typename utlIteratorBase<const_iterator>::bidirectional_iterator_tag iterator_category;

    /// type of element.
    typedef typename utlSet<Key,Cmp,Tree>::value_type value_type;
    /// type of difference between two const_iterators.
    typedef typename utlSet<Key,Cmp,Tree>::difference_type difference_type;
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
    /// == is a friend operator.
    friend bool operator==(const const_iterator& it1, const const_iterator it2); 
    /// != is a friend operator.
    friend bool operator!=(const const_iterator& it1, const const_iterator it2);
    /// a pointer to the tree being iterated. 
    const tree_type* tree;

    /// const_iterator for the tree.
    iterator_type itr; 

  }; // const_iterator

  /// compares two utlSet const_iterators. 
  friend inline bool operator==(const const_iterator& it1, const const_iterator it2) {
      return ( it1.itr == it2.itr );
  }  

  /// compares two utlSet const_iterators.
  friend inline bool operator!=(const const_iterator& it1, const const_iterator it2) {
      return !(it1.itr==it2.itr);
  }

 protected:

  /// set tree. 
  tree_type mT;

  /// the number of stored elements.
  size_type Count;

  /// function object that compares two keys for ordering. 
  key_compare compare;

 public:

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlSet( const tree_type& t = tree_type(key_compare()) ) : mT(t), Count(0), compare(key_compare()) {}

  /// creates an empty set, using comp as the key_compare object. 
  utlSet(const key_compare& comp) : mT(tree_type(comp)), Count(0), compare(comp) {}

  /// creates a set with a copy of a range. 
  template <class InputIterator> 
  utlSet(InputIterator f, InputIterator l) : 
			mT(tree_type(key_compare())), Count(0), compare(key_compare()) {
      insert ( f, l );
  }

  /// creates a set with a copy of a range, using comp as the key_compare object. 
  template <class InputIterator>
  utlSet(InputIterator f, InputIterator l, const key_compare& comp) : 
			mT(tree_type(comp)), Count(0), compare(comp) {
      insert ( f, l );
  }

  /// copy constructor.
  utlSet(const utlSet& S) {
    *this = S;
  }

  /** 	destructor. 
   *
   *  	@see clear().
   */
  ~utlSet() {
    this->clear();   
  }
  //@}

  /// returns the number of elements in this set.
  size_type size() const { return Count;}

  /** returns the maximum size of this set.
   *
   *  @return maximum size of this set.
   */
  size_type max_size() const { return 1; }

  /// returns whether this set is empty.
  bool empty() const { return mT.IsEmpty(); } 

  /// returns the key_compare object used by the set. 
  key_compare key_comp() const { return compare; }

  /// returns the value_compare object used by the set. 
  value_compare value_comp() const { return compare; }

  /// makes this set empty. 
  void clear() {
    mT.MakeEmpty();
    Count = 0;
  }

  /// returns an iterator pointing to the beginning of the set.
  iterator begin() const {
    iterator iter (&mT); 
    iter.itr.First();
    return iter;
  }

  /// returns an iterator pointing to the end of the set.
  iterator end() const {
    return iterator(&mT);
  }

  /// returns a reverse_iterator pointing to the beginning of the reversed set.
  reverse_iterator rbegin() const { 
    return reverse_iterator(end()); 
  }

  /// returns a reverse_iterator pointing to the end of the reversed set.
  reverse_iterator rend() const { return reverse_iterator(begin()); }

  /** 	finds an element whose key is k.
   *
   *	@param k given key.
   *	@return an iterator with the found element, or an end() iterator,
   *	        if the element has not been found.
   */
  iterator find(const key_type& k) const {
    mT.Find ( k );
    const BinaryNode* node = mT.GetCurrent();
    return (node != mT.GetNullNode() ? iterator( node, &mT ) : end());
  }


  /** 	assignment operator.
   *
   * 	@see clear().
   */
  utlSet& operator=(const utlSet& S) {
    if(this != &S)
      {
       this->mT = S.mT;
       this->Count = S.Count;
       this->compare = S.compare;
      }
    return *this;
  }

  /** inserts an element into the set.
   *  As in the STL, insert() returns a pair whose first part
   *  consists of the iterator that points to the found position. The
   *  second part indicates whether the insertion has taken place or
   *  not. 
   *
   *  @param P given pair.
   *  @return a pair with a found value and a boolean.
   */
  utlPair<iterator, bool> insert(const value_type& P) {
    iterator temp = find(P);
    bool inserted = false;

    if( temp == end() ) // not present
      {
       mT.Insert ( P );
       ++Count;
       temp = find(P); // redefine temp
       inserted = true;
      }

    return cgcUtil::make_pair(temp, inserted);
  }

  /// inserts x into the set, using pos as a hint to where it will be inserted. 
  iterator insert(iterator pos, const value_type& x) {
    return insert ( x ).first;
  }

  /**	inserts a range into the set. 
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
   *	set that have a given key. In a utlMap, this can at most be one
   *	element, but in a utlMultiSet, several elements might be
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
       All elements to be deleted are save in a list
       to avoid loosing the return point into the stack,
       which keeps the path from the root to the current
       element.
    */
    utlVector<value_type> delem;
    while(first != last)
       delem.push_back ( *first++ );

    for ( typename utlVector<value_type>::iterator it = delem.begin(); it != delem.end(); ++it ) {
       mT.Remove ( *it );
       --Count;
    }

  }

  /// swaps the contents of this and a given utlSet.
  void swap(utlSet& s) {
    mT.swap ( s.mT ); 
    std::swap(Count, s.Count);
  }

  /// comparison operator. Tests two sets for equality.
  friend bool operator == ( const utlSet& set1, const utlSet& set2 ) {
    if ( &set1 == &set2 ) return true;
    if ( set1.size() != set2.size() ) return false;    

    return cgcUtil::equal ( set1.begin(), set1.end(), set2.begin() );
  }

  /// lexicographical comparison.
  friend bool operator < ( const utlSet& set1, const utlSet& set2 ) {
    if ( &set1 == &set2 ) return true;

    return cgcUtil::lexicographical_compare ( set1.begin(), set1.end(), set2.begin(), set2.end() );
  }

  /// output operator.
  friend ostream& operator<<(ostream& out, const utlSet& obj) {
    iterator iter = obj.begin();
    while(iter != obj.end()) {
      cout << *iter << endl;
      ++iter;
    }
    return out;
  }
    
  /// returns how many pairs with a given key are in the multiset.
  size_type count ( const key_type& k ) const { 
    size_type nb = 0;

    const_iterator it = find( k );
    const_iterator itr = it;
    if ( it != end() ) {
      do {
         ++nb;
         ++it;
      }
      while ( it != end() && *it == k );
      while ( itr != begin() && *itr == k ) {
         ++nb;
         --itr;
      }  
      if ( *itr != k ) --nb;
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
  utlPair<iterator, iterator> equal_range ( const key_type& k ) const { 
       iterator it1 = find( k ), it2 = end();
       if ( it1 != end() ) {	// found at least one
         it2 = it1;
         do
           ++it2;
         while ( it2 != end() && *it2 == k );
         while ( it1 != begin() && *it1 == k )
           --it1;
         if ( *it1 != k ) ++it1;
       }
       return cgcUtil::make_pair ( it1, it2 ); 
  }

};

/**	multiset for sets with duplicate keys.
 *
 *	Multiset is a Sorted Associative Container that stores objects of type Key. 
 *	Multiset is a Simple Associative Container, meaning that its value type,
 *	as well as its key type, is Key. It is also a Multiple Associative Container, 
 *	meaning that two or more elements may be identical. 
 *
 *	Set and multiset are particularly well suited to the set algorithms includes, 
 *	set_union, set_intersection, set_difference, and set_symmetric_difference. 
 *	The reason for this is twofold. First, the set algorithms require their arguments 
 *	to be sorted ranges, and, since set and multiset are Sorted Associative Containers, 
 *	their elements are always sorted in ascending order. Second, the output range of 
 *	these algorithms is always sorted, and inserting a sorted range into a set or 
 *	multiset is a fast operation: the Unique Sorted Associative Container and Multiple Sorted
 *	Associative Container requirements guarantee that inserting a range takes only linear 
 *	time if the range is already sorted. 
 *
 *	Multiset has the important property that inserting a new element into a multiset 
 *	does not invalidate iterators that point to existing elements. Erasing
 *	an element from a multiset also does not invalidate any iterators, except, of course, 
 *	for iterators that actually point to the element that is being erased.  
 *
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.3 $ $Date: 2018/10/11 14:44:13 $
 *	
 */
template < class Key, class Cmp=compare_to<Key>, class Tree = SplayTree<Key,Cmp> >
class utlMultiSet : public utlSet <Key,Cmp,Tree>
{
 public:
  /// an element.
  typedef Key value_type;

  /// iterator declaration.
  typedef typename utlSet<Key,Cmp,Tree>::iterator iterator;

  /// type of the search tree.
  typedef typename utlSet<Key,Cmp,Tree>::tree_type tree_type;

  /// function object that compares two keys for ordering. 
  typedef typename utlSet<Key,Cmp,Tree>::key_compare key_compare;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utlMultiSet( const tree_type& t = tree_type(key_compare()) ) : utlSet<Key,Cmp,Tree>(t) {}

  /// creates a multiset with a copy of a range. 
  template <class InputIterator> 
  utlMultiSet(InputIterator f, InputIterator l) : 
  		        utlSet<Key,Cmp,Tree>() {
      insert ( f, l );
  }

  /// creates a multiset with a copy of a range, using comp as the key_compare object. 
  template <class InputIterator>
  utlMultiSet(InputIterator f, InputIterator l, const key_compare& comp) : 
			utlSet<Key,Cmp,Tree>(comp) {
      insert ( f, l );
  }

  //@}

  /** inserts an element into the multiset.
   *
   *  @param P given pair.
   *  @return iterator to the inserted pair.
   */
  iterator insert(const value_type& P) {

    this->mT.Insert ( P );
    iterator temp = this->find(P);
       
    ++this->Count;
    return temp;
  }

  /// inserts x into the multiset, using pos as a hint to where it will be inserted. 
  iterator insert(iterator pos, const value_type& x) {
    return insert ( x );
  }

  /**	inserts a range into the multiset. 
   *
   *	@param f beginning of the range.
   *	@param l end of the range.
   */
  template <class InputIterator> void insert(InputIterator f, InputIterator l) {
     while ( f != l ) insert ( *f++ ); 
  }

};

#endif   // __UTLSET_H__
#endif
