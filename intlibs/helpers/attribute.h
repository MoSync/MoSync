
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

#define PRINTF_ATTRIB(a,b) GCCATTRIB(format(printf, a, b))
