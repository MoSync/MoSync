#include <maassert.h>

#define assert(cond) _assert(cond, #cond)

void _assert(int condition, const char* description);
