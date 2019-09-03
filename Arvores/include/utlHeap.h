/*
   utlHeap.h

   An adaptor implementing a heap from a given container.

  Program:   utl
  Module:    $RCSfile: utlHeap.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 18 June 2002.
*/

#ifndef __UTLHEAP__
#define __UTLHEAP__

#include "utlSorter.h"
#include "utlVector.h"
#include "utlStream.h"

/** A non-standard implementation of a heap.
 *
 *      @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *      @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *
 *      A heap is an adaptor that provides a restricted subset of
 *      Container functionality: it provides insertion of elements,
 *      and inspection and removal of the top element.
 *      It is guaranteed that the top element is the largest element in the heap,
 *      where the function object Cmp is used for comparisons.
 *      Heap does not allow iteration through its elements.
 *
 * 	Internally, a heap is simply a tree represented as a sequential range.  
 *	The tree is constructed so that each node is less than or equal to
 *	its parent node.
 *
 *	This version of heap allows removal of any element from the heap,
 *	and assumes that class T has two methods that are called 
 *	whenever its position is changed in the heap (set_heap_pos(int)), 
 *	or whenever it is deleted from the heap (not_in_heap()).
 *	This is completely non-standard, but necessary some times.
 */
template <class T, class C = utlVector<T>, class Cmp = compare_to <typename C::value_type> >
class utlHeap {

 protected:

 /// container to hold the heap.
 C c;

 public:
 
 /// value type.
 typedef typename C::value_type value_type;
 
 /// size type.
 typedef typename C::size_type size_type;

 /// underlying container type.
 typedef C container_type;

 /// iterator
 typedef typename C::iterator iterator;

 /// iterator
 typedef typename C::const_iterator const_iterator;

 /// function object that compares two keys for ordering.
 Cmp compare;

public:
 /** @name constructors  */
 //@{
    /// constructor from a container.
    explicit utlHeap ( const C& a = C() ) : c(a), compare(Cmp()) {}

    /// constructor from a comparison function object.
    utlHeap ( const Cmp& comp ) : c(C()), compare(comp()) {}

    /// copy constructor.
    utlHeap ( const utlHeap& heap ) { *this = heap; }
 //@}

    /// inserts a new element into the heap.
    void push ( value_type& t ) {
       int i = size();
       t.set_heap_pos(i);
       c.push_back(t);

       upheap(i);
    }

    /// updates element at position i, which can go up or down the heap.
    void update ( size_type i ) {
       if( i > 0 && compare ( c[parent(i)], c[i] ) )
           upheap(i);
       else
           downheap(i);
    }

    /// returns the size of the heap.
    size_type size() const { return c.size(); }

    /// resets this heap.
    void clear() { c.clear(); }

    /// returns whether this heap is empty.
    bool empty() const { return c.empty(); }

    /// resizes the container.
    void resize ( size_type n ) { c.resize (n); }

    /// returns the element at position i for writing.
    value_type&       item(size_type i)       { return c[i]; }

    /// returns the element at position i for reading.
    const value_type& item(size_type i) const { return c[i]; }

    /// returns the first element of the heap for reading.
    const value_type& top() const { return c.front(); }

    /// returns the first element of the heap for writing.
    value_type& top() { return c.front(); }

    /// removes the first element of the heap.
    void pop() {
       if( c.empty() ) return; 

       swap(0, size()-1);
       c.back().not_in_heap();
       c.pop_back();

       downheap(0);
    }

    /// removes element at position i from the heap.
    void remove( size_type i ) {
       if ( c.empty() ) return;

       value_type t = c[i];
       swap(i, size()-1);
       c.back().not_in_heap();
       c.pop_back();

       if( compare ( c[i], t ) )
           downheap(i);
       else
           upheap(i);
    }

    /// copy operator.
    utlHeap& operator = ( const utlHeap& heap ) {
        c = heap.c;
        compare = heap.compare; 
        return *this;
    }

    /// output operator.
    friend ostream& operator << ( ostream& os, const utlHeap<T,C,Cmp>& heap ) {
#if __GNUC__ >= 3
      utlHeap::iterator ith;
      for(ith=heap.c.begin(); ith != heap.c.end(); ++ith) os << *ith << " ";
#else
      cgcUtil::copy ( heap.c.begin(), 
		      heap.c.end(), 
		      utl_ostream_iterator<T>(os," ") );
#endif
      os << "\n"; 
      return os;
    }

 private:

    /// places value x at position i.
    void place(value_type& x, size_type i) {
        x.set_heap_pos(i);
	c[i] = x;
    }

    /// swaps elements i and j.
    void swap(size_type i, size_type j) {
        value_type tmp = c[i];
        place ( c[j], i );
        place ( tmp, j );
    }

    /// returns the parent of a node. 
    size_type parent(size_type i) const { return (i-1)/2; }

    /// returns the left child of a node. 
    size_type left(size_type i) const { return 2*i+1; }

    /// returns the right child of a node. 
    size_type right(size_type i) const { return 2*i+2; }

    /// move element at position i up the heap.
    void upheap(size_type i) {
       value_type moving = c[i];
       size_type index = i;
       size_type p = parent(i);

       while( index > 0 && compare ( c[p], moving ) ) {
         place(c[p], index);
         index = p;
         p = parent(p);
       }

       if( index != i )
           place(moving, index);
    }

    /// move element at position i down the heap.
    void downheap(size_type i) {
       value_type moving = c[i];
       size_type index = i;
       size_type l = left(i);
       size_type r = right(i);
       size_type largest;

       while( l < size() ) {
         if( r < size() && compare ( c[l], c[r] ) )
             largest = r;
         else
             largest = l;

         if ( compare ( moving, c[largest] ) )
           {
            place(c[largest], index);
            index = largest;
            l = left(index);
            r = right(index);
           }
         else
            break;
       }

       if( index != i )
           place(moving, index);
    }

};

#endif
