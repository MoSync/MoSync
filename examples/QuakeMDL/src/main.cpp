/**
 * Set the correct project configuration to run benchmark
 * or render mode.
 *
 */
int BenchMain ( void );
int RenderMain ( void );


extern "C"
int MAMain ( void )
{
#ifdef BENCHMARK
	return BenchMain( );
#else
	return RenderMain( );
#endif
}
