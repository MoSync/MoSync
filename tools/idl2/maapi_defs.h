#ifndef _MAAPI_DEFS_H
#define _MAAPI_DEFS_H

#ifdef MAPIP
#define MA_IOCTL_ELLIPSIS , ...
#else
#define MA_IOCTL_ELLIPSIS
#endif

#if defined(__GNUC__) || defined(__SYMBIAN32__)
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#define ATTRIB(a) __attribute__ ((a))
#define GCCATTRIB(a) __attribute__ ((a))
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#define GCCATTRIB(a)
#else
#error Unsupported compiler!
#endif

#ifdef __IPHONE__
#define __MARM_ARMI__
#endif

// Convert between double, 2 ints and long long.
union MA_DV {
#if defined(__MARM_ARMI__)	//Symbian S60, 2nd edition hardware
	int i[2];
#else	//everything else
	struct {
		int hi, lo;
	};
#endif	//__MARM_ARMI__
	long long ll;
	double d;
};

// Convert between float and int.
union MA_FV {
	float f;
	int i;
};

#endif	//_MAAPI_DEFS_H
