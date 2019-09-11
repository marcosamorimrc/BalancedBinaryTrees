/*
**	utlStream.h - class for manipulating streams.
**
**	purpose: to provide adequate classes for the manipulation
**	of streams.
**

  Program:   cgc util
  Module:    $RCSfile: utlStream.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 18:49:11 $
  Version:   $Revision: 1.2 $

**
*/

#ifndef __UTLSTREAM_H__
#define __UTLSTREAM_H__

/* for STL compatibility test use: -DUSE_STL */
#ifdef USE_STL
#include <iterator>
#define utl_ostream_iterator std::ostream_iterator
#define utl_istream_Iterator std::istream_iterator
#else

#include "utlIteratorBase.h"

/** 	Output stream iterator.
 *
 *	@author <A HREF="mailto:ulisses@watson.ibm.com">Ulisses Mello</a> and
 *	<A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2018/10/11 18:49:11 $
 *
 *	An ostream_iterator is an Output Iterator that performs 
 *	formatted output of objects of type Etype to a particular ostream. 
 *	Note that all of the restrictions of an Output Iterator must be obeyed, 
 *	including the restrictions on the ordering of operator* and operator++ operations. 
 *	
 *	Ostream iterator is used to work directly with output streams. They are 
 *	useful for writing to sequential files. It uses << operator from the standard 
 *	input. <br>
 *
 *	Etype must be a type such that cout << Etype is a valid expression. 
 */
template <class Etype>
class utl_ostream_iterator : public utlIteratorBase<Etype> {
 public:

  /// iterator category.
  typedef typename utlIteratorBase<utl_ostream_iterator>::output_iterator_tag iterator_category;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utl_ostream_iterator() : stream(&cout), cnt(0), txt(0) { }

  /** constructor from an ostream. 
   *  Creates an utl_ostream_iterator such that assignment of t 
   *  through it is equivalent to s << t. 
   */
  utl_ostream_iterator(ostream& s) : stream(&s), cnt(0), txt(0) {}

  /** constructor from an ostream and a delimitator. 
   *  Creates an utl_ostream_iterator such that assignment of t 
   *  through it is equivalent to s << t << delim. 
   */
  utl_ostream_iterator(ostream& s, const char* delim) : stream(&s), cnt(0) { 
    this->txt = new char[strlen(delim)+1];
    strcpy (this->txt,delim); 
  }

  /// copy constructor.
  utl_ostream_iterator( const utl_ostream_iterator& rhs ) : txt(0) {
    *this = rhs;
  }


  /// destructor.
  ~utl_ostream_iterator () {
     if ( this->txt ) delete [] this->txt;
  }
 
  //@}

  /// dereference operator. 
  utl_ostream_iterator& operator*() { 
	return *this; 
  }

  /// assignment operator.
  utl_ostream_iterator& operator = ( const utl_ostream_iterator& T ) { 
      this->stream = T.stream;
      if ( this->txt ) delete [] this->txt;
      this->txt = 0;
      if ( T.txt ) {
        this->txt = new char[strlen(T.txt)+1];
        strcpy (this->txt,T.txt); 
      }
      this->cnt = T.cnt;
      return *this;
  }

  /// assignment operator. Send T through the stream.
  utl_ostream_iterator& operator = ( const Etype& T ) { 
      *stream << T;
      if ( txt ) *stream << txt;
      if ( !(++cnt % 20) ) *stream << "\n";
      return *this;
  }

  /// prefix increment operator.
  utl_ostream_iterator& operator++() {
    return *this;
  }
  /// posfix increment operator.
  utl_ostream_iterator& operator++(int) {
    return *this;
  }

 protected:
  /// output stream.
  ostream* stream;
  /// counter.
  int cnt;
  /// delimitator.
  char* txt;

};


/** 	Input stream iterator.
 *
 *	@author <A HREF="mailto:ulisses@watson.ibm.com">Ulisses Mello</a> and
 *	<A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.2 $ $Date: 2018/10/11 18:49:11 $
 *
 *	An istream_iterator is an Input Iterator that performs formatted input 
 *	of objects of type Etype from a particular istream. When end of stream is
 *	reached, the istream_iterator takes on a special end of stream value, 
 *	which is a past-the-end iterator. Note that all of the restrictions of an
 *	Input Iterator must be obeyed, including the restrictions on the 
 *	ordering of operator* and operator++ operations. 
 *	
 *	Istream iterator is used to work directly with input streams. They are 
 *	useful for reading sequential files. It uses >> operator from the standard 
 *	input. <br>
 *	ex: <br>
 *	
 *	<pre>
 *	ifstream fsin("data.fac");
 *	istream_Iterator<plgPolygon> isItr(fsin), isEnd;
 *	
 *	if(isItr == isEnd) {
 *	cerr << "File not found!\n";
 *	} else {
 *  	  while (isItr != isEnd) {
 *	    cout << *isItr << endl;
 *	    ++isItr;
 *	  }
 *	}
 *	</pre> <br>
 *	
 *	Another example: <br>
 *	
 *	<pre>
 *	istream_iterator <int> is(cin);
 *	int i1 = *is;
 *	++is;
 *	int i2 = *is;
 *	</pre>
 */
template <class Etype>
class utl_istream_Iterator : public utlIteratorBase<Etype> {
 public:

  /// iterator category.
  typedef typename utlIteratorBase<utl_istream_Iterator>::input_iterator_tag iterator_category;

  /** @name constructors  */ 
  //@{

  /// empty constructor.
  utl_istream_Iterator() : stream(&cin), end_marker(false) { }

  /// constructor from an istream. Reads the first element if present.
  utl_istream_Iterator(istream& s) : stream(&s) { this->read(); }

  /// copy constructor.
  utl_istream_Iterator( const utl_istream_Iterator<Etype>& rhs ) {
    this->stream = rhs.stream;
    this->end_marker = rhs.end_marker;
  }

  /// destructor.
  ~utl_istream_Iterator() {}

  //@}

  /// dereference operator. 
  const Etype& operator*() const { return this->element; }

  /// dereference operator. 
  const Etype* operator->() const { return &element; }

  /// prefix increment operator.
  utl_istream_Iterator<Etype>& operator++() {
    this->read(); 
    return *this;
  }
  /// posfix increment operator.
  utl_istream_Iterator<Etype> operator++(int) {
    utl_istream_Iterator<Etype> tmp = *this;
    this->read(); 
    return tmp;
  }

  /**  	streams iterators are also equal when both point to the end of 
   *	streams. 
   */
  friend bool operator == (const utl_istream_Iterator<Etype>& x,
			   const utl_istream_Iterator<Etype>& y) {
    return (x.stream == y.stream && x.end_marker == y.end_marker) ||
           (x.end_marker == false && y.end_marker == false);
  }

 protected:
  /// read object.
  Etype element;
  /// input stream.  
  istream* stream;
  /// true if not yet reached.
  bool end_marker;

  /// reads an element if the stream is ok and not empty.
  void read() {
    this->end_marker = (*stream) ? true : false;
    if(this->end_marker) *stream >> this->element;
    this->end_marker = (*stream) ? true : false;
  }
};

#endif
#endif
