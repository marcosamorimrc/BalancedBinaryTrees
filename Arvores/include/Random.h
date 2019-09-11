/*
   Random.h

   Random numbers generator.

  Program:   weiss
  Module:    $RCSfile: Random.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
  Author:    Paulo Roma Cavalcanti, based on Weiss's code.

   Date: 27 November 2000.
*/

#ifndef __RANDOM
#define __RANDOM

#include <math.h>
#include <stdlib.h>

static const long A = 48271L;
static const long M = 2147483647L;
static const long Q = M / A;
static const long R = M % A;

/** Random class interface.
 *
 *  Random package that calls the UNIX routine
 *  random( ) and srandom( ) if it can, but uses
 *  a linear congruential generator otherwise.
 *
 *  @author <A HREF="mailto:roma@lcg.ufrj.br">Paulo Roma Cavalcanti</a> and
 *          <A HREF="mailto:weiss@fiu.edu">Mark Allen Weiss</a>
 *  @version $Revision: 1.1.1.1 $ $Date: 2003/10/31 23:10:11 $
 *	
 *  CONSTRUCTION: with (a) no initializer or (b) an integer
 *     that specifies the initial state of the generator. <br>
 *
 *  ******************PUBLIC OPERATIONS********************* <br>
 *     Return a random number according to some distribution <br>
 *  unsigned long RandomLong( )          --> Uniform, 1 to 2^31-1 <br>
 *  double RandomReal( )                 --> Uniform, 0..1 <br>
 *  long RandLong( long Low, long High ) --> Uniform Low..High <br>
 *  unsigned Poisson( double ExpectedVal )-> Poisson <br>
 *  double NegExp( double ExpectedVal )  --> Negative exponential <br>
 */

class Random
{
  public:

  /** @name constructors  */ 
  //@{
/// constructor given seed.
    Random( unsigned long InitVal = 1 ) : Seed ( InitVal )
    {
#ifdef unix
    srandom( InitVal );
#endif
    }
  //@}

//  uniform distributions.

/** returns the next random number and updates Seed.
 *  If on UNIX, call random instead.
 *  Uniform [1 .. 2^31 - 1].
 */
    unsigned long RandomLong( ) {
#ifdef unix
    	return random( );
#else
    	long TmpSeed = A * ( Seed % Q ) - R * ( Seed / Q );
    	if( TmpSeed >= 0 )
           Seed = TmpSeed;
    	else
           Seed = TmpSeed + M;

    	return Seed;
#endif
    }

/// uniform [0.0 .. 1.0].
    double RandomReal( ) { 
	return RandomLong( ) / 2147483647.0; 
    }

/** not best algorithm for linear congruential generators.
 *  Uniform [LOW .. HIGH].
 */
    long RandLong( long Low, long High ) { 
	return RandomLong( ) % ( High - Low + 1 ) + Low; 
    }

// Non-uniform distribution.

/// non-uniform Poisson.
    unsigned Poisson( double ExpectedValue ) {
    	double Limit = exp( -ExpectedValue );
    	double Product = RandomReal( );

    	int Count;
    	for( Count = 0; Product > Limit; Count++ )
            Product *= RandomReal( );

    	return Count;
    }

/// non-uniform negative exponential.
    double NegExp( double ExpectedValue ) {
    	return - ExpectedValue * log( RandomReal( ) );
    }

  private:

/// seed for random generation.
    unsigned long Seed;
};
#endif
