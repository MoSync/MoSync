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

#ifndef _MAAPI_DEFS_H
#define _MAAPI_DEFS_H

#if defined(MAAPI_H) || defined(HAVE_IOCTL_ELLIPSIS)
#define MA_IOCTL_ELLIPSIS , ...
#else
#define MA_IOCTL_ELLIPSIS
#endif

#if defined(__GNUC__) || defined(__SYMBIAN32__)

#ifdef _android
#define ATTRIBUTE(a, func) ((func))
#else
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#endif

#define ATTRIB(a) __attribute__ ((a))
#define GCCATTRIB(a) __attribute__ ((a))
#define IOCTLDEF static inline
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#define GCCATTRIB(a)
#define IOCTLDEF static
#else
#error Unsupported compiler!
#endif

#if defined(__MARM_ARMI__)	//Symbian S60, 2nd edition hardware
#define MA_DV_HI i[1]
#define MA_DV_LO i[0]
#define MA_LL_HI i[0]
#define MA_LL_LO i[1]
#else
#define MA_DV_HI hi
#define MA_DV_LO lo
#define MA_LL_HI hi
#define MA_LL_LO lo
#endif

// Convert between double, 2 ints and long long.
typedef union MA_DV {
#if defined(__MARM_ARMI__)	//Symbian S60, 2nd edition hardware
	int i[2];
#else	//everything else
	struct {
// this seems to have changed again? (now running latest xcode with iphone 4.2 sdk)
//#ifdef __IPHONE__
//		int lo, hi;
//#else
		int hi, lo;
//#endif
	};
#endif	//__MARM_ARMI__
	long long ll;
	double d;
} MA_DV;

// Convert between float and int.
typedef union MA_FV {
	float f;
	int i;
} MA_FV;

#endif	//_MAAPI_DEFS_H
