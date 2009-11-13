
#if defined(__GNUC__) || defined(__SYMBIAN32__)
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#define ATTRIB(a) __attribute__ ((a))
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#else
#error Unsupported compiler!
#endif

#if defined(_MSC_VER) || defined(__SYMBIAN32__)
#define PRINTF_ATTRIB(a,b)
#elif defined(__GNUC__)
#define PRINTF_ATTRIB(a,b) __attribute((format(printf, a, b)))
#else
#error Unsupported platform!
#endif
