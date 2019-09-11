
/*
  utlConfig.h - definitions for util

  Program:   util 
  Module:    $RCSfile: utlConfig.h,v $
  Language:  C++
  Date:      $Date: 2003/10/31 23:10:11 $
  Version:   $Revision: 1.1.1.1 $
*/

#ifndef __UTLCONFIG_H__
#define __UTLCONFIG_H__

#if __IBMCPP__ >= 500
#define __xlC5
#endif

#if defined(sun) || defined(__sun)
#if defined (__GNUC__)
#define __GNUSUN__
#else
#if defined (__SUNPRO_CC)
#define __SUN_CC
#endif
#endif
#endif

#if defined(__GNUC__) // gnu g++
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory> // this includes <stl_tempbuf.h>  that includes stl_pair -> conflict!
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits.h>
#if __GNUC__ >= 3
using namespace std;
#endif
#endif
 
#if defined(__PGI)  // Portland Group compiler
#  include <stdio.h>
#  include <stdlib.h>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <memory>
#  include <assert.h>
#  include <fcntl.h>
#  include <math.h>
#  include <string.h>
#  include <unistd.h>
#  include <limits.h>
#endif

#if defined(__KCC) // Intel KAI KCC
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory> 
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits>
using namespace std;
#  define __KCC_CC 1
#  define HUGE 3.40282347e+38F 
#endif

#if defined(__SUN_CC) // SUN CC
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory> 
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits>
using namespace std;
#endif

#if defined(__sgi) && !defined(__GNUC__) // sgi CC
#  include <stdio.h>  // 7.3
#  include <stdlib.h> // 7.3
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <math.h>   // 7.3
#  include <string.h> // 7.3 
#  include <memory>
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits.h>
using namespace std;
#  define __SGI_CC 1
#endif

#if defined(_MSC_VER)  // microsoft visual C++
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory>
#  include <assert.h>
#  include <fcntl.h>
#  include <io.h>
#  include <limits.h>
#  define _read read
#  define _write write
#  define _lseek lseek
using namespace std;
#endif


#if defined(__xlC5) // IBM VisualAge C++ for AIX
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory> 
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits>
using namespace std;
#endif


#if defined(__INTEL_COMPILER) // Intel icc, default value = 700 
#  include <cstdio>
#  include <cstdlib>
#  include <iostream>
#  include <iomanip>
#  include <fstream>
#  include <cmath>
#  include <cstring>
#  include <memory>
#  include <assert.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits.h>
using namespace std;
#endif


// others like xlC 3.6 (in general requires .h)
#if !defined(__GNUC__) && !defined(__PGI) && !defined(_MSC_VER) && !defined(__SGI_CC) && !defined(__KCC) && !defined(__xlC5) && !defined(__INTEL_COMPILER) &&!defined(__SUN_CC)
#  include <stdio.h>
#  include <stdlib.h>
#  include <iostream.h>
#  include <iomanip.h>
#  include <fstream.h>
#  include <math.h>
#  include <assert.h>
#  include <string.h>
#  include <memory.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <limits.h>
#endif

#if defined(__GNUC__)
#define NAME_SPACE_std std
#else
#define NAME_SPACE_std
#endif

#if defined (__xlC__) && !defined(__xlC5)
#  ifndef bool
#     define bool int
#  endif
#endif


#ifndef true
#  define true (1)
#endif

#ifndef false
#  define false (0)
#endif

#ifndef NULL
#  define NULL 0l
#endif

#if defined ( __MINGW32__ ) || defined ( __GNUWIN32__ )
#  define HUGE HUGE_VAL
#endif 

#endif


