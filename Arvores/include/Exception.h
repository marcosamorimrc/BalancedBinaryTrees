#ifndef __EXCEPTION
#define __EXCEPTION

#include "utlConfig.h"

/** simulates an exception.
 *
 *  @param Condition error code.
 *  @param ErrMsg error message.
 */
#define EXCEPTION(Condition,ErrMsg) {			\
    if( Condition )					\
      {							\
       cerr << "Exception: " << ErrMsg << endl;		\
       abort( );					\
      }							\
}
#endif
