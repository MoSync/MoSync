#define FE_ALL_EXCEPT 0
#define FE_OVERFLOW 0
#define FE_UNDERFLOW 0
#define FE_INEXACT 0

inline int feclearexcept(int excepts) {
	return 0;
}

inline int fetestexcept (int excepts) {
	return 0;
}
