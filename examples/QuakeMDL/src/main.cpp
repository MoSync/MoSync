#define BENCHMARK

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
