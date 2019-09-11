
/*
  Program:   utlSwapByte
  Module:    $RCSfile: utlSwapByte.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
*/

#ifndef __UTLSWAPBYTE_H__
#define __UTLSWAPBYTE_H__

#include "utlConfig.h"

/**
 * These routines are necessary for reversing the byte order of binary data
 * for binary portability between big-endian and little-endian machines. 
 * Examples of big-endian machines are IBM RS6000, SUN, NeXT. 
 * Examples of little endian machines are PC's and DEC.
 *
 * Endian representation of selected data types:
 * type   size ex value   big_endian little_endian
 * byte     1   0x12        0x12        0x12
 * short    2   0x1234      0x1234      0x3412
 * long     4   0x12345678  0x12345678  0x78563412
 * float    4   1.2e3       0x44960000  0x00009644
 * double   8   1.2e3       0x4092c00000000 0x00000000000c09240
 */
class utlSwapByte {
 public:
  /// swap a short integer
  static void swap_short_2(short *vni2) {
    *vni2=(((*vni2>>8)&0xff) | ((*vni2&0xff)<<8));  
  }

  /// swap an unsigned short integer
  static void swap_u_short_2(unsigned short *vni2) {
      *vni2=(((*vni2>>8)&0xff) | ((*vni2&0xff)<<8));  
  }

  /// swap a 4 byte integer
  static void swap_int_4(int *vni4) {
    *vni4=(((*vni4>>24)&0xff) | ((*vni4&0xff)<<24) |
	   ((*vni4>>8)&0xff00) | ((*vni4&0xff00)<<8));  
  }

  /// swap an unsigned integer
  static void swap_u_int_4(unsigned int *vni4) {
    *vni4=(((*vni4>>24)&0xff) | ((*vni4&0xff)<<24) |
	   ((*vni4>>8)&0xff00) | ((*vni4&0xff00)<<8));  
  }

  /// swap a long integer
  static void swap_long_4(long *vni4) {
    *vni4=(((*vni4>>24)&0xff) | ((*vni4&0xff)<<24) |
	   ((*vni4>>8)&0xff00) | ((*vni4&0xff00)<<8));  
  }

  /// swap an unsigned long integer
  static void swap_u_long_4(unsigned long *vni4) {
    *vni4=(((*vni4>>24)&0xff) | ((*vni4&0xff)<<24) |
	   ((*vni4>>8)&0xff00) | ((*vni4&0xff00)<<8));  
  }

  /// swap a float
  static void swap_float_4(float *vnf4) {
    int *vni4=(int *)vnf4;
    *vni4=(((*vni4>>24)&0xff) | ((*vni4&0xff)<<24) |
	   ((*vni4>>8)&0xff00) | ((*vni4&0xff00)<<8));  
  }

  /// swap a double
  static void swap_double_8(double *vndd8) {
    char *vnd8=(char *)vndd8;
    char vnc;
    
    vnc= *vnd8;
    *vnd8= *(vnd8+7);
    *(vnd8+7)=vnc;
    
    vnc= *(vnd8+1);
    *(vnd8+1)= *(vnd8+6);
    *(vnd8+6)=vnc;

    vnc= *(vnd8+2);
    *(vnd8+2)= *(vnd8+5);
    *(vnd8+5)=vnc;

    vnc= *(vnd8+3);
    *(vnd8+3)= *(vnd8+4);
    *(vnd8+4)=vnc;
  }

  /// general function to swap bytes of any size
  static void swapbyte ( void *vn, int size ) {
    unsigned char buffer[8];
    unsigned char *s = (unsigned char*) vn;
	int i;
    for(i=0; i<size; ++i) {
      buffer[i] = s[size - i -1];
    }
    for(i=0; i<size; ++i) {
      s[i] = buffer[i];
    }
  }
};


/// general template function to handle byte-swaping
template <class Type> inline void swapByte( Type *vn ) { 
  cerr << "swapping bytes for this type has not been implemented!\n";
}

// specialization
template <> inline
void swapByte<short>( short *vn ) { utlSwapByte::swap_short_2( vn ); }
template <> inline
void swapByte<unsigned short>( unsigned short *vn ) { 
  utlSwapByte::swap_u_short_2( vn ); 
}
template <> inline 
void swapByte<int>( int *vn ) { utlSwapByte::swap_int_4( vn ); }
template <> inline
void swapByte<unsigned int>( unsigned int *vn ) { 
utlSwapByte::swap_u_int_4( vn ); 
}
template <> inline
void swapByte<long>( long *vn ) { utlSwapByte::swap_long_4( vn ); }
template <> inline 
void swapByte<unsigned long>( unsigned long *vn ) { 
  utlSwapByte::swap_u_long_4( vn ); 
}
template <> inline
void swapByte<float>( float *vn ) { utlSwapByte::swap_float_4( vn ); }
template <> inline 
void swapByte<double>( double *vn ) { utlSwapByte::swap_double_8( vn ); }

#endif // __UTLSWAPBYTES_H__

