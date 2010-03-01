// http://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html

// these should not be needed, as long == int.
long __ashldi3 (long a, int b) {
	return a << b;
}

long __negdi2(long a) {
	return -a;
}

long __muldi3 (long a, long b) {
	return a*b;
}

long __divdi3 (long a, long b) {
	return a/b;
}

long __moddi3(long a, long b) {
	return a%b;
}

unsigned long __umoddi3 (unsigned long a, unsigned long b) {
	return a%b;
}

unsigned long __udivdi3 (unsigned long a, unsigned long b) {
	return a/b;
}
