#include "assert.h"
#include "stdlib.h"
#include "errno.h"

int errno;

void _assert(int condition, const char* description) {
	ASSERT_MSG(condition, description);
}
void abort(void) {
	maPanic(0, "abort()");
}
