#include <stdint.h>

typedef int hp_timing_t;

#define HP_TIMING_AVAIL 0

#define HP_TIMING_DIFF_INIT()
#define HP_TIMING_NOW(a)
#define HP_TIMING_DIFF(a,b,c)

static impl_t** __last_impl_p = (impl_t**)&__stop_impls;

#undef IMPL
#define IMPL(name, test) \
  static impl_t tst_ ## name							\
    = { #name, (void (*) (void))name, test }; \
	void initImpl##name() __attribute__ ((constructor)); \
	void initImpl##name() { \
	**__last_impl_p = tst_ ## name;	(*__last_impl_p)++; }

#define MIN(a,b) ((a) < (b) ? (a) : (b))
