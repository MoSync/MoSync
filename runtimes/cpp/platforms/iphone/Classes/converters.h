#include <helpers/maapi_defs.h>

template <typename T>
inline void convertRet(int& r14, int& r15, T type) {
	r14 = (int)type;
}

template <>
inline void convertRet<double>(int& r14, int& r15, double type) {
	MA_DV dv;
	dv.d = type;
	r14 = dv.lo;
	r15 = dv.hi;
}

template <>
inline void convertRet<float>(int& r14, int& r15, float type) {
	MA_FV fv;
	fv.f = type;
	r14 = fv.i;
}

template <>
inline void convertRet<void*>(int& r14, int& r15, void* type) {
	r14 = (int)((byte*)type - (int)mem_ds);
}

template <typename T>
inline T convertSingleArg(int arg) {
	return (T)arg;
}

template <>
inline float convertSingleArg<float>(int arg) {
	MA_FV fv;
	fv.i = arg;
	return fv.f;
}

inline double convertDoubleArg(int arg1, int arg2) {
	MA_DV dv;
	dv.lo = arg1;
	dv.hi = arg2;
	return dv.d;
}


template <typename T>
inline T convertPointerArg(int arg) {
	// validate
	return (T) ((byte*)mem_ds+arg);
}
