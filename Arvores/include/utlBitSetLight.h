/*
   utlBitSetLightLight.h

   Implements a bitset array.
	
  Program:   utl
  Module:    $RCSfile: utlBitSetLight.h,v $
  Language:  C++
  Date:      $Date: 2018/10/11 18:29:26 $
  Version:   $Revision: 1.1 $

   Date: 27 October 2000.
*/


#ifndef __UTLBITSETLIGHT_H__
#define __UTLBITSETLIGHT_H__

#include "utl.h"

#define CGCBITS unsigned long int
#define CGCBITS2 unsigned long int

/** An implementation of bitset using long int or unsigned long int 
 *     (maximum of 32 bits or 64 bits in 64-bit OSs).
 *	@author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a>
 *	@version $Revision: 1.1 $ $Date: 2018/10/11 18:29:26 $
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
template <class BITS>
class utlBitSetLight {

public:
  /** @name constructors  */ 
  //@{

/// empty constructor. The default is an array of BITS bits.
utlBitSetLight() { 
    initBytes();
}

/** constructor from an array of bytes.
 * 
 *  @param init array of bytes.
 *  @param n number of bytes.
 */
utlBitSetLight ( const unsigned char* init, size_t n=0 ) {
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
utlBitSetLight ( const char* binary ) {
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
utlBitSetLight ( const utlBitSetLight& bset ) {
    initBytes();
    *this = bset;
}

/** constructor from an unsigned long int. 
 *
 *  @param uint given unsigned long int.
 */
utlBitSetLight ( unsigned long int uint ) {
    initBytes();
    this->bytes = (BITS)uint;
}

/// destructor. Deletes the bit array.
~utlBitSetLight () { }

  //@}

/// returns the number of bits of the bitset.
size_t bits() const { return size()*CHAR_BIT; }

/// returns the size of the bitset.
size_t size() const { return sizeof(BITS); }

/** sets bit at position pos to val.
 *
 *  @param pos position in the bitset array.
 *  @param val value of the bit.
 */ 
utlBitSetLight& set ( size_t pos, int val = 1 ) {
    this->bytes = ( val ? utl_set_bit ( this->bytes, pos ) :
                          utl_reset_bit ( this->bytes, pos ) );
    return *this;
}

/// sets every bit to 1.
utlBitSetLight& set () {
    for(size_t i = 0; i < bits(); i++) set(i);
    return *this;
}

/// returns whether any bit is set.
bool any () const {
    return this->bytes;
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
    return utl_get_bit ( this->bytes, pos ); 
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
utlBitSetLight<BITS>* bSet;

/// current bit.
size_t pos;

public:

/// constructor.
reference ( utlBitSetLight<BITS>* b, size_t i ) : bSet ( b ), pos ( i ) {}

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
friend utlBitSetLight operator | ( const utlBitSetLight& bset1, const utlBitSetLight& bset2 ) {
    return (bset1.bytes | bset2.bytes);
}

/** returns the bitwise inclusive or operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSetLight& operator |= ( const utlBitSetLight& bset ) {
    this->bytes |= bset.bytes; 

    return *this;
}

/** returns the bitwise and operation between two given bitsets.
 *
 *  @param bset1 first bitset.
 *  @param bset2 second bitset.
 */ 
friend utlBitSetLight operator & ( const utlBitSetLight& bset1, const utlBitSetLight& bset2 ) {
    return (bset1.bytes & bset2.bytes);
}

/** returns the bitwise and operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSetLight& operator &= ( const utlBitSetLight& bset ) {
    this->bytes &= bset.bytes; 

    return *this;
}

/** returns the bitwise exclusive or operation between two given bitsets.
 *
 *  @param bset1 first bitset.
 *  @param bset2 second bitset.
 */ 
friend utlBitSetLight operator ^ ( const utlBitSetLight& bset1, const utlBitSetLight& bset2 ) {
    return (bset1.bytes ^ bset2.bytes);
}

/** returns the bitwise exclusive or operation between this and a given bitset.
 *
 *  @param bset given bitset.
 */ 
utlBitSetLight& operator ^= ( const utlBitSetLight& bset ) {
    this->bytes ^= bset.bytes; 

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
utlBitSetLight operator << ( size_t offset ) const {
    return (this->bytes << offset);
}

/** left shifts this bitset.
 *
 *  Left shift, where bit 0 is considered the least significant bit. 
 *  Bit i takes on the previous value of bit i - offset, or zero if no such bit exists. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSetLight& operator <<= ( size_t offset ) {
    this->bytes <<= offset;

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
utlBitSetLight operator >> ( size_t offset ) const {
    return (this->bytes >> offset);
}

/** right shifts this bitset.
 *
 *  Right shift, where bit 0 is considered the least significant bit. 
 *  Bit i takes on the previous value of bit i + offset, or zero if no such bit exists. 
 *
 *  @param offset number of positions to shift.
 */ 
utlBitSetLight& operator >>= ( size_t offset ) {
    this->bytes >>= offset;

    return *this;
}

/** copies a bitset array.
 *
 *  @param bset a bitset array.
 */ 
utlBitSetLight& operator=( const utlBitSetLight& bset ) {
    if ( this == &bset ) return *this; // same bitset
    this->bytes = bset.bytes;

    return *this;
}

/** compares two bitset arrays.
 *
 *  @param bset a bitset array.
 */ 
bool operator==( const utlBitSetLight& bset ) const {
    if ( this == &bset ) return true; // same bitset
       
    return this->bytes == bset.bytes;
}

/** compares two bitset arrays.
 *
 *  @param bset a bitset array.
 */ 
bool operator!=( const utlBitSetLight& bset ) const {
    return !(*this == bset);
}

/// sets every bit to 0.
utlBitSetLight& reset () {
    this->bytes = 0;
    return *this;
}

/** sets bit in position pos to 0.
 *
 *  @param pos position in the bitset array.
 */ 
utlBitSetLight& reset ( size_t pos ) {
    this->bytes = utl_reset_bit ( this->bytes, pos ); 
    return *this;
}

/// flips the value of every bit.
utlBitSetLight& flip () {
    for(size_t i = 0; i < bits(); i++) 
        this->flip(i);
    return *this;
}
  
/** flips the value of the bit at position pos.
 *
 *  @param pos position in the bitset array.
 */ 
utlBitSetLight& flip ( size_t pos ) {
    set(pos, !read(pos));
    return *this;
}

/// returns a copy of *this with all of its bits flipped.
utlBitSetLight operator~() const { 
	utlBitSetLight bset ( *this );
        return bset.flip(); 
}

/// casting operator for an unsigned long int. 
unsigned long int to_ulong () const
{
 return (unsigned long int)this->bytes;
} 

/** writes this bitset to an ascii file.
 *
 *  @param f_ptr file pointer.
 */ 
void write ( FILE* f_ptr ) const {
    size_t numbytes = this->size();
    BITS mask = 255;
    fprintf ( f_ptr, " %u %u\n", numbytes, this->bits() );
    for ( int i = 3; i >= 0; --i ) {
#ifndef _AIX
      fprintf ( f_ptr, " %hhu", (unsigned char) ((this->bytes>>(i*8))&mask) );
#else
      fprintf ( f_ptr, " %hu", (unsigned char) ((this->bytes>>(i*8))&mask) );
#endif
    }
    fprintf ( f_ptr, "\n" );
}

/** reads this bitset from an ascii file.
 *
 *  @param f_ptr file pointer.
 *  @return 1 if the read operation was successful, and 0 otherwise.
 */ 
int read ( FILE* f_ptr ) {
    size_t nbytes, nbits, nb;
    BITS temp;
    if ( fscanf ( f_ptr, "%u %u", &nbytes, &nbits ) != 2 )
         return 0;
    nb = nbytes; 
    if ( nbytes > this->size() ) nbytes = this->size();
    for ( int i = 4; i < nb; ++i )
         if ( fscanf ( f_ptr, "%u", &temp ) != 1 )
              return 0;
    this->bytes = 0; 
    for ( int i = 3; i >= 0; --i ) {
         if ( fscanf ( f_ptr, "%u", &temp ) != 1 )
              return 0;
         this->bytes |= temp << (i*8);
    }
    return 1;
}

/** writes this bitset to a binary file.
 *
 *  @param f_ptr file pointer.
 */ 
void writeb ( FILE* f_ptr ) const {
    size_t numbytes = this->size();
    size_t numbits = this->bits();
    BITS mask = 255, temp;
    fwrite ( &numbytes, sizeof (size_t), 1, f_ptr );
    fwrite ( &numbits, sizeof (size_t), 1, f_ptr );
    for ( int i = 3; i >= 0; --i ) {
          temp = (this->bytes>>(i*8))&mask;
          fwrite ( &temp, sizeof (unsigned char), 1, f_ptr );
    }
}

/** reads this bitset from a binary file.
 *
 *  @param f_ptr file pointer.
 *  @return 1 if the read operation was successful, and 0 otherwise.
 */ 
int readb ( FILE* f_ptr ) {
    size_t nbytes, nbits, nb;
    BITS temp = 0;
    fread ( &nbytes, sizeof (size_t), 1, f_ptr );
    fread ( &nbits, sizeof (size_t), 1, f_ptr );
    nb = nbytes;
    if ( nbytes > this->size() ) nbytes = this->size();
    for ( int i = 4; i < nb; ++i )
         fread ( &temp, sizeof (unsigned char), 1, f_ptr );
    this->bytes = 0; 
    for ( int i = 3; i >= 0; --i ) {
         fread ( &temp, sizeof (unsigned char), 1, f_ptr );
         this->bytes |= temp << (i*8);
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
friend ostream& operator<<(ostream& os, const utlBitSetLight& bset) {
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
friend istream& operator>>(istream& is, utlBitSetLight& bset) {
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

private:

/// allocates and initializes the bitset.
void initBytes ( ) {
     this->bytes = 0;
}

/// bit array.
BITS bytes;

};


#endif // __UTLBITSETLIGHT_H__

