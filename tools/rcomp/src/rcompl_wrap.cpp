#if defined(__VC32__)
// disable myriad warnings caused by use of ~0 in yy_check 
#pragma warning( disable : 4245 )	// 'initializing' : conversion from 'const int' to 'unsigned char', signed/unsigned mismatch

#if 0
\RCOMP\SRC\rcompl.cpp(481) : warning C4100: 'sv' : unreferenced formal parameter
\RCOMP\SRC\rcompl.cpp(717) : warning C4244: '=' : conversion from 'int' to 'char', possible loss of data
\RCOMP\SRC\rcompl.cpp(744) : warning C4127: conditional expression is constant
\RCOMP\SRC\rcompl.cpp(772) : warning C4127: conditional expression is constant
\RCOMP\SRC\rcompl.cpp(1056) : warning C4102: 'yy_more' : unreferenced label
\RCOMP\SRC\rcompl.cpp(1048) : warning C4102: 'yy_reject' : unreferenced label
\RCOMP\SRC\rcompl.cpp(1112) : warning C4244: '=' : conversion from 'int' to 'char', possible loss of data
\RCOMP\SRC\rcompl.cpp(1049) : warning C4702: unreachable code
\RCOMP\SRC\rcompl.cpp(1050) : warning C4702: unreachable code
\RCOMP\SRC\rcompl.cpp(1057) : warning C4702: unreachable code
\RCOMP\SRC\rcompl.cpp(1058) : warning C4702: unreachable code
\RCOMP\SRC\rcompl.cpp(1060) : warning C4702: unreachable code
#endif

#pragma warning( disable : 4100 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4102 )
#pragma warning( disable : 4702 )
#endif

#include "rcompl.h"
