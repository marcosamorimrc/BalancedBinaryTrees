/*
   utlBitSet.h

   Implements a bitset array.
	
  Program:   utl
  Module:    $RCSfile: utlBitSet.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $

   Date: 14 October 2000.
*/


#ifndef __UTLBITSET_H__
#define __UTLBITSET_H__

#ifdef __USE_BITSETLIGHT__ 
#include "utlBitSetLight.h"
#define utlBitSet utlBitSetLight
#else

#include "utlConfig.h"

#ifdef __PIO
#include "xdrStream.h"
#endif

#define CGCBITS 96
#define CGCBITS2 96

#if defined ( _AIX ) || defined ( __SGI_CC )
#define USFMT " %hu"
#else
#define USFMT " %hhu"
#endif

/** A simpler implementation of STL class bitset.
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *	class for providing bitset arrays.
 *
 *	bitset is very similar to vector<bool> (also known as bit_vector): 
 *	it contains a collection of bits, and provides constant-time access to each bit. 
 *	There are two main differences between bitset and vector<bool>. 
 *	First, the size of a bitset cannot be changed: bitset's template parameter BITS, which
 *	specifies the number of bits in the bitset, must be an integer constant. 
 *	Second, bitset is not a Sequence; in fact, it is not an STL Container at all. 
 *	It does not have iterators, for example, or begin() and end() member functions. 
 *	Instead, bitset's interface resembles that of unsigned integers. It defines bitwise
 *	arithmetic operators such as &=, |=, and ^=. 
 *
 *	In general, bit 0 is the least significant bit and bit BITS-1 is the most significant bit. 
 */
template <size_t BITS>
class utlBitSet {

public:
  /** @name constructors  */ 
  //@{

/// empty constructor. The default is an array of BITS bits.
utlBitSet() { 
    initBytes();
}

/** constructor from an array of bytes.
 * 
 *  @param init array of bytes.
 *  @param n number of bytes.
 */
utlBitSet ( const unsigned char* init, size_t n=0 ) {
    initBytes();
    if(init == NULL) return; // default to all 0

    size_t nbytes = size();
    size_t j = 0;
    if ( n < 1 ) n = nbytes;
    // Translate from bytes into bit sequence:
    for(int index = n-1; index >= 0 && j < nbytes; --index, ++j)
      for(size_t offset = 0; offset < CHAR_BIT; offset++)
	if(init[index] & (1 << offset))
	  set(j * CHAR_BIT + offset);
}

/** constructor from a bit string. 
 *
 *  @param binary a string of 1s and 0s.
 */
utlBitSet ( const char* binary ) {
    int slen = strlen(binary);
    int i = 0;

    // skip white spaces
    size_t nbits = 0; 
    for ( i = 0; i < slen; ++i )
          if (binary[i] == '1' || binary[i] == '0') ++nbits;

    if ( nbits > bits() ) nbits = bits();
    initBytes();
    size_t j = 0;
    for(i = slen-1; i >= 0 && j < nbits; --i)
      if(binary[i] == '1') set(j++);
      else if (binary[i] == '0') ++j;
}

/** copy constructor.
 *
 *  @param bset a bitset array.
 */
utlBitSet ( const utlBitSet& bset ) {
    initBytes();
    *this = bset;
}

/** constructor from an unsigned long int. 
 *
 *  @param uint given unsigned long int.
 */
utlBitSet ( unsigned long int uint ) {
    initBytes();
    size_t nbits = sizeof(unsigned long int)*CHAR_BIT; 
    if ( nbits > bits() ) nbits = bits();
    for(size_t i = 0; i < nbits; i++)
      if ( uint & (1L << i) ) set(i);
}

/// destructor. Deletes the bit array.
~utlBitSet () {
    delete [] bytes;
}

  //@}

/// returns the number of bits of the bitset.
size_t bits() const { return size()*CHAR_BIT; }

/// returns the size of the bitset.
size_t size() const { return (BITS+CHAR_BIT-1)/CHAR_BIT; }

/** sets bit at position pos to val.
 *
 *  @param pos position in the bitset array.
 *  @param val value of the bit.
 */ 
utlBitSet& set ( size_t pos, int val = 1 ) {
    assert(pos < bits()); // size_t is unsigned int and thus is >= 0

    size_t index = pos / CHAR_BIT;
    size_t offset = pos % CHAR_BIT;
    unsigned char mask;

    if ( val ) {
       mask = (1 << offset);
       this->bytes[index] |= mask;
    } else {
       mask = ~(1 << offset);
       this->bytes[index] &= mask;
    }
    return *this;
}

/// sets every bit to 1.
utlBitSet& set () {
    for(size_t i = 0; i < bits(); i++) set(i);
    return *this;
}

/// returns whether any bit is set.
bool any () const {
    for(size_t i = 0; i < size(); i++) 
       if ( this->bytes[i] )
           return true;
    return false;
}

/// returns whether no bit is set.
bool none () const {
    return !any();
}

/// returns the number of bits that are set.
size_t count () const {
    size_t n = 0;
    for(size_t i = 0; i < bits(); i++) 
       if ( read(i) )
           ++n;
    return n;
}

/** gets the bit at position pos.
 *
 *  @param pos position in the bitset array.
 */ 
int read ( size_t pos ) const {
    assert(pos < bits()); // size_t is unsigned int and thus is >= 0

    size_t index = pos / CHAR_BIT;
    size_t offset = pos % CHAR_BIT;
    unsigned char mask = (1 << offset);
    return this->bytes[index] & mask;
}

/** returns true if bit n is set.
 *
 *  @param n position in the bitset array.
 */ 
bool test ( size_t n ) const {
    assert ( n < bits() );
    return read ( n ); 
}

private:

/**  a proxy class that acts as a reference to a single bit. 
 *   It contains an assignment operator, a conversion
 *   to bool, an operator~, and a member function flip. 
 *   It exists only as a helper class for bitset's operator[]. 
 *   That is, it supports the expressions x = b[i], b[i] = x, 
 *   b[i] = b[j], x = ~b[i], and b[i].flip(). 
 *   (Where b is a bitset and x is a bool.) 
 */
class reference {

private:

/// a pointer to a utlBitSet.
utlBitSet<BITS>* bSet;

/// current bit.
size_t pos;

public:

/// constructor.
reference ( utlBitSet<BITS>* b, size_t i ) : bSet ( b ), pos ( i ) {}

/// assignment operator.
reference& operator = ( bool x ) { bSet->set(pos,x); return *this; }

/// assignment operator.
reference& operator = ( const reference& ref ) { bSet->set(pos,ref); return *this; }

/// casting to bool.
operator bool ( ) const { return bSet->read( pos ); }

/// flips the current bit.
reference& flip ( ) { bSet->flip( pos ); return *this; }

/// flip operator. 
bool operator ~ ( ) const { return ~bSet->read(pos); }

};


public:

/** returns a reference to bit n.
 *
 *  Note that reference is a proxy class with an assignment operator
 *  and a conversion to bool, which allows you to use operator[] 
 *  for assignment. That is, you can write both x = b[n] and b[n] = x. 
 *
 *  @param n position in the bitset array.
 */ 
reference operator[] ( size_t n ) {
    reference ref ( this, n );
    return ref; 
}

/** returns true if bit n is set.
 *
 *  @param n position in the bitset array.
 */ 
bool operator[] ( size_t n ) const {
    return read(n);
}

/** returns the bitwise inclusive or operation between two given bitsets.
 *
 *  @param bset1 first bitset.
 *  @param bset2 second bitset.
 */ 
friend utlBitSet operator | ( const utlBitSet& bset1, const utlBitSet& bset2 ) {
    size_t numbytes = bset1.size();
    if ( numbytes > bset2.size() ) numbytes = bset2.size();

    utlBitSet aset;
    for(size_t i = 0; i < numbytes; i++)
      *(aset.bytes+i) = *(bset1.bytes+i) | *(bset2.bytes+i); 

    return aset;
}

/** returns the bitwise inclusive or operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSet& operator |= ( const utlBitSet& bset ) {
    size_t numbytes = this->size();
    if ( numbytes > bset.size() ) numbytes = bset.size();

    for(size_t i = 0; i < numbytes; i++)
      *(this->bytes+i) |=  *(bset.bytes+i); 

    return *this;
}

/** returns the bitwise and operation between two given bitsets.
 *
 *  @param bset1 first bitset.
 *  @param bset2 second bitset.
 */ 
friend utlBitSet operator & ( const utlBitSet& bset1, const utlBitSet& bset2 ) {
    size_t numbytes = bset1.size();
    if ( numbytes > bset2.size() ) numbytes = bset2.size();

    utlBitSet aset;
    for(size_t i = 0; i < numbytes; i++)
      *(aset.bytes+i) = *(bset1.bytes+i) & *(bset2.bytes+i); 

    return aset;
}

/** returns the bitwise and operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSet& operator &= ( const utlBitSet& bset ) {
    size_t numbytes = this->size();
    if ( numbytes > bset.size() ) numbytes = bset.size();

    for(size_t i = 0; i < numbytes; i++)
      *(this->bytes+i) &= *(bset.bytes+i); 

    return *this;
}

/** returns the bitwise exclusive or operation between two given bitsets.
 *
 *  @param bset1 first bitset.
 *  @param bset2 second bitset.
 */ 
friend utlBitSet operator ^ ( const utlBitSet& bset1, const utlBitSet& bset2 ) {
    size_t numbytes = bset1.size();
    if ( numbytes > bset2.size() ) numbytes = bset2.size();

    utlBitSet aset;
    for(size_t i = 0; i < numbytes; i++)
      *(aset.bytes+i) = *(bset1.bytes+i) ^ *(bset2.bytes+i); 

    return aset;
}

/** returns the bitwise exclusive or operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSet& operator ^= ( const utlBitSet& bset ) {
    size_t numbytes = this->size();
    if ( numbytes > bset.size() ) numbytes = bset.size();

    for(size_t i = 0; i < numbytes; i++)
      *(this->bytes+i) ^= *(bset.bytes+i); 

    return *this;
}

/** left shift.
 *
 *  Returns a copy of *this shifted left by offset bits. 
 *  Note that the expression b << offset is equivalent to constructing a 
 *  temporary copy of b and then using operator<<=. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSet operator << ( size_t offset ) const {
    utlBitSet aset;
    size_t j = this->bits()-1;
    for ( int i = j-offset; i >= 0; --i, --j )
          aset.set(j, read(i));

    for ( j = 0; j < offset; ++j )
        aset.reset(j);

    return aset;
}

/** left shifts this bitset.
 *
 *  Left shift, where bit 0 is considered the least significant bit. 
 *  Bit i takes on the previous value of bit i - offset, or zero if no such bit exists. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSet& operator <<= ( size_t offset ) {
    size_t j = this->bits()-1;
    for ( int i = j-offset; i >= 0; --i, --j )
          set(j, read(i));

    for ( j = 0; j < offset; ++j )
        reset(j);

    return *this;
}

/** right shift.
 *
 *  Returns a copy of *this shifted right by offset bits. 
 *  Note that the expression b >> offset is equivalent to constructing a 
 *  temporary copy of b and then using operator>>=. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSet operator >> ( size_t offset ) const {
    size_t nbits = this->bits();

    utlBitSet aset;
    size_t i, j = 0;
    for ( i = offset; i < nbits ; ++i, ++j )
          aset.set(j, read(i));

    for ( i = nbits-offset; i < nbits; ++i, ++j )
          aset.reset(i);

    return aset;
}

/** right shifts this bitset.
 *
 *  Right shift, where bit 0 is considered the least significant bit. 
 *  Bit i takes on the previous value of bit i + offset, or zero if no such bit exists. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSet& operator >>= ( size_t offset ) {
    size_t nbits = this->bits();

    size_t i, j = 0;
    for ( i = offset; i < nbits ; ++i, ++j )
          set(j, read(i));

    for ( i = nbits-offset; i < nbits; ++i )
          reset(i);

    return *this;
}

/** copies a bitset array.
 *
 *  @param bset a bitset array.
 */ 
utlBitSet& operator=( const utlBitSet& bset ) {
    if ( this == &bset ) return *this; // same bitset
    size_t numbytes = this->size();
    if ( numbytes > bset.size() ) numbytes = bset.size();
    for(size_t i = 0; i < numbytes; i++)
      *(this->bytes+i) = *(bset.bytes+i);
    return *this;
}

/** compares two bitset arrays.
 *
 *  @param bset a bitset array.
 */ 
bool operator==( const utlBitSet& bset ) const {
    if ( this == &bset ) return true; // same bitset
    size_t numbytes = this->size();
    if ( numbytes != bset.size() ) return false;
    for(size_t i = 0; i < numbytes; i++)
        if ( *(this->bytes+i) != *(bset.bytes+i) )
           return false;
       
    return true;
}

/** compares two bitset arrays.
 *
 *  @param bset a bitset array.
 */ 
bool operator!=( const utlBitSet& bset ) const {
    return !(*this == bset);
}

/// sets every bit to 0.
utlBitSet& reset () {
    for ( size_t i = 0; i < size(); ++i )
          this->bytes[i] = 0;
    return *this;
}

/** sets bit in position pos to 0.
 *
 *  @param pos position in the bitset array.
 */ 
utlBitSet& reset ( size_t pos ) {
    assert(pos < bits()); // size_t is unsigned int and thus is >= 0

    size_t index = pos / CHAR_BIT;
    size_t offset = pos % CHAR_BIT;
    unsigned char mask = ~(1 << offset);
    this->bytes[index] &= mask;
    return *this;
}

/// flips the value of every bit.
utlBitSet& flip () {
    for(size_t i = 0; i < bits(); i++) 
        this->flip(i);
    return *this;
}
  
/** flips the value of the bit at position pos.
 *
 *  @param pos position in the bitset array.
 */ 
utlBitSet& flip ( size_t pos ) {
    set(pos, !read(pos));
    return *this;
}

/// returns a copy of *this with all of its bits flipped.
utlBitSet operator~() const { 
	utlBitSet bset ( *this );
        return bset.flip(); 
}

/// casting operator for an unsigned long int. 
unsigned long int to_ulong () const
{
 size_t nbits = sizeof(unsigned long int)*CHAR_BIT;
 if ( nbits > bits() ) nbits = bits();
 unsigned long int uint=0; 
 for(size_t i = 0; i < nbits; ++i )
     if ( read(i) ) uint |= (1L << i);
 return uint;
} 

/** writes this bitset to an ascii file.
 *
 *  @param f_ptr file pointer.
 */ 
void write ( FILE* f_ptr ) const {
    // in irix, size_t is unsigned long, so force a cast
    unsigned int numbytes = this->size(), nbits = this->bits();
    fprintf ( f_ptr, " %u %u\n", numbytes, nbits );
    for(int i = numbytes - 1; i >= 0; --i)
      fprintf ( f_ptr, USFMT, this->bytes[i] );

    fprintf ( f_ptr, "\n" );
}

/** reads this bitset from an ascii file.
 *
 *  @param f_ptr file pointer.
 *  @return 1 if the read operation was successful, and 0 otherwise.
 */ 
int read ( FILE* f_ptr ) {
    unsigned int nb, nbits; // in irix, size_t is unsigned long - PRC 19-02-2003
    if ( fscanf ( f_ptr, "%u %u", &nb, &nbits ) != 2 )
         return 0;
    size_t nbytes = nb;
    if ( nbytes > this->size() ) nbytes = this->size();
    unsigned short b;	// PRC 05-06-2003 - SUN does not like hhu
    for(int i = nbytes - 1; i >= 0; i--) {
       if ( fscanf ( f_ptr, "%hu", &b ) != 1 )  return 0;
       this->bytes[i] = (unsigned char)b;
    }
    return 1;
}

/** writes this bitset to a binary file.
 *
 *  @param f_ptr file pointer.
 */ 
void writeb ( FILE* f_ptr ) const {
    unsigned int numbytes = this->size();
    unsigned numbits = this->bits();
    fwrite ( &numbytes, sizeof (unsigned int), 1, f_ptr );
    fwrite ( &numbits, sizeof (unsigned int), 1, f_ptr );
    for(int i = numbytes - 1; i >= 0; --i)
      fwrite ( (this->bytes+i), sizeof (unsigned char), 1, f_ptr );
}

/** reads this bitset from a binary file.
 *
 *  @param f_ptr file pointer.
 *  @return 1 if the read operation was successful, and 0 otherwise.
 */ 
int readb ( FILE* f_ptr ) {
    unsigned int nbytes, nbits;
    if ( fread ( &nbytes, sizeof (unsigned int), 1, f_ptr ) != 1 ) return 0;
    if ( fread ( &nbits,  sizeof (unsigned int), 1, f_ptr ) != 1 ) return 0;
    if ( nbytes > this->size() ) nbytes = this->size();
    for(int i = nbytes - 1; i >= 0; i--) {
      if ( fread ( (this->bytes+i), sizeof (unsigned char), 1, f_ptr ) != 1 )
	   return 0;
    }  
    return 1;
}

/// prints a bitset in binary format.
void print ( const char* msg ) const {
    cerr << msg << "\n";
    cerr << *this;
    cerr << "\n";
}

/** 	output operator. Prints a given bitset. The array is
 *	printed from left to right, that is, the most significant bit
 *	is printed first.
 *	
 *  	@param os file stream.
 *  	@param bset given bitset.
 *  	@return file stream. 
 */
friend ostream& operator<<(ostream& os, const utlBitSet& bset) {
    if ( bset.bytes == NULL ) {
       cerr << "bit array is NULL\n";
       return os;
    }

    os << bset.size() << " " << bset.bits() << " ";

    for(int i = bset.bits()-1; i >= 0; --i){
      // Format into byte blocks:
      if((i+1) % CHAR_BIT == 0) os << ' ';

      os << (bset.read(i) ? '1' : '0');
    }

    return os; 
}

/** 	input operator. Reads a given bitset. The array is
 *	read from left to right, that is, the most significant bit
 *	is read first.
 *
 *  	@param is file stream.
 *  	@param bset given bitset.
 *  	@return file stream. 
 */
friend istream& operator>>(istream& is, utlBitSet& bset) {
    bset.reset();
    size_t nbytes, nbits;
    is >> nbytes;
    is >> nbits;

    if ( nbits > bset.bits() ) nbits = bset.bits();
    int i = nbits - 1;
    unsigned char c;
    while ( i >= 0 ){
      is >> c; 
      if( c == '1') 
         bset.set ( i-- );
      else if ( c == '0' )
         --i; 
    }

    return is; 
}

#ifdef __PIO
  /**
   *  This will serialize this object into xdr output stream for output.
   *
   *  @param out - xdr output stream
   */
 virtual void xdrEncode(xdrOStream& os) const {
   os<<bytes;
 }
  
  /**
   *  This method will restore an rtEdge object from the input xdr stream.
   *
   *  @param in - xdr input stream
   */
 virtual void xdrDecode(xdrIStream& is) {
   char *buf=0;
   is>>buf;
   utlBitSet<BITS> obj(buf);
   *this=obj;
   delete [] buf;
 }
  
  /**
   *  overloaded output operator for xdrOStream
   * 
   *  @param os - an instance of xdrOStream
   *  @param o  - an instance of utlBitSet for outputing to xdr output stream
   *  @return reference for the output streamer
   */
  friend xdrOStream& operator<<(xdrOStream& os, const utlBitSet<BITS>& o) { o.xdrEncode(os); return os; }
  
  /**
   *  overloaded xdr input stream to restore a utlBitSet from the input stream
   *
   *  @param is - input xdr stream
   *  @param o  - a utlBitSet object restored from the input streamer
   *  @return reference to the input streamer
   */
  friend xdrIStream& operator>>(xdrIStream& is, utlBitSet<BITS>& o) { o.xdrDecode(is); return is; }
#endif

private:

/// allocates and initializes the bitset.
void initBytes ( ) {
    size_t nb = size();
    // this->bytes = new (unsigned char) [nb]; // SGI CC 7.3 does not like this
    this->bytes = new unsigned char [nb];
    assert ( this->bytes );
    for ( size_t i = 0; i < nb; ++i )
          this->bytes[i] = 0;
}

/// bit array.
unsigned char* bytes;

};

#endif // __UTLBITSET_H__
#endif
