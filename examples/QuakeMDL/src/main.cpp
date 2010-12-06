#include "main.h"

/**
 * Set the correct project configuration to run benchmark
 * or render mode.
 *
 */

extern "C"
int MAMain ( void ) GCCATTRIB(noreturn);

extern "C"
int MAMain ( void )
{
#ifdef BENCHMARK
	BenchMain( );
#else
	RenderMain( );
#endif
}
