
/*
**	utlSTL.h - header file definitions of STL for util
**

  Program:   util 
  Module:    $RCSfile: utlSTL.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
*/

#ifndef __UTLSTL_H__
#define __UTLSTL_H__

#if __IBMCPP__ >= 500
#define __xlC5
#endif

#if defined(__GNUC__)  // g++
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#     include <iterator>
#  endif
#endif
 
#if defined(__PGI)  // Portland Group compiler
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(__KCC)  // Intel KAI KCC compiler
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(__sgi) && !defined(__GNUC__) // sgi CC
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(_MSC_VER)  // microsoft visual C++
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(__SUN_CC)  // SUN CC Forte
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(__xlC5)  // IBM VisualAge C++ for AIX
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif

#if defined(__INTEL_COMPILER)  // intel icc 7.0 compiler
#  ifdef USE_STL
#     include <list>
#     include <vector>
#     include <string>
#     include <algorithm>
#     include <map>
#     include <set>
#     include <stack>
#     include <queue>
#     include <deque>
#  endif
#endif


// others like xlC 3.6 (in general requires .h)
#if !defined(__GNUC__) && !defined(__PGI) && !defined(_MSC_VER) && !defined(__SGI_CC) && !defined(__KCC) && !defined(__xlC5) && !defined(__INTEL_COMPILER) && !defined(__SUN_CC)
#  ifdef USE_STL
#     include <list.h>
#     include <vector.h>
#     include <string.h>
#     include <algorithm.h>
#     include <map.h>
#     include <set.h>
#     include <stack.h>
#     include <queue.h>
#     include <deque>
#  endif
#endif


#ifdef USE_STL
#define cgcUtil std
#endif

//#if defined(__GNUC__) && defined(USE_STL)
//#define cgcUtilExt std
//#endif

#if !defined(USE_STL) || defined(__INTEL_COMPILER) // gnu g++ has an extended version of STL
# include "utlAlgo.h"
# include "utlSorter.h"
#else
  #if defined(__GNUC__)  // g++
    #if (__GNUC__>=3)
      #     include <ext/algorithm>
      #     include <ext/numeric>
      #define cgcUtilExt __gnu_cxx
    #else
      //using namespace std;
      #define cgcUtilExt std
    #endif
  #else
      //using namespace std;
      #define cgcUtilExt std
  #endif
#endif

#endif // __UTLSTL_H__


