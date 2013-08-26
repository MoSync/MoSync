/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <helpers/maapi_defs.h>

template <typename T>
inline void convertRet(int& r14, int& r15, T type) {
	r14 = (int)type;
}

template <>
inline void convertRet<double>(int& r14, int& r15, double type) {
	MA_DV dv;
	dv.d = type;
	r14 = dv.hi;
	r15 = dv.lo;
}

template <>
inline void convertRet<longlong>(int& r14, int& r15, longlong type) {
	r14 = (type&0xffffffff);
	r15 = (int)((unsigned long long)type>>32);
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
	dv.hi = arg1;
	dv.lo = arg2;
	return dv.d;
}


template <typename T>
inline T convertPointerArg(int arg) {
	// validate
	return (T) ((byte*)mem_ds+arg);
}
