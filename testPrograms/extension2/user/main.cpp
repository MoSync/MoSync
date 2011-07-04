#include "mx_Test.h"
#include <conprint.h>
#include <maassert.h>

int MAMain() GCCATTRIB(noreturn);
int MAMain() {
	printf("Hello World!\n");
	printf("Extension result: %i\n", foo(7));
	FREEZE;
}
