#ifndef __WINDOWS_PHONE_8_LIB__
#include <maassert.h>
#else
#include "../../../MAStd/maassert.h"
#endif

#define assert(cond) _assert(cond, #cond)

void _assert(int condition, const char* description);
