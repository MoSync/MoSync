
#define	CHAR_BIT	8		/* number of bits in a char */

#define	SCHAR_MAX	0x7f		/* max value for a signed char */
#define SCHAR_MIN	(-0x7f-1)	/* min value for a signed char */

#define	UCHAR_MAX	0xffU		/* max value for an unsigned char */
#ifdef __machine_has_unsigned_chars
# define CHAR_MIN	0		/* min value for a char */
# define CHAR_MAX	0xff		/* max value for a char */
#else
# define CHAR_MAX	0x7f
# define CHAR_MIN	(-0x7f-1)
#endif

#define	USHRT_MAX	0xffffU		/* max value for an unsigned short */
#define	SHRT_MAX	0x7fff		/* max value for a short */
#define SHRT_MIN        (-0x7fff-1)     /* min value for a short */

#define	UINT_MAX	0xffffffffU	/* max value for an unsigned int */
#define	INT_MAX		0x7fffffff	/* max value for an int */
#define	INT_MIN		(-0x7fffffff-1)	/* min value for an int */

#ifdef __LP64__
# define ULONG_MAX	0xffffffffffffffffUL
					/* max value for unsigned long */
# define LONG_MAX	0x7fffffffffffffffL	
					/* max value for a signed long */
# define LONG_MIN	(-0x7fffffffffffffffL-1)	
					/* min value for a signed long */
#else
# define ULONG_MAX	0xffffffffUL	/* max value for an unsigned long */
# define LONG_MAX	0x7fffffffL	/* max value for a long */
# define LONG_MIN	(-0x7fffffffL-1)/* min value for a long */
#endif

#if __BSD_VISIBLE || __ISO_C_VISIBLE >= 1999
# define ULLONG_MAX	0xffffffffffffffffULL	
					/* max value for unsigned long long */
# define LLONG_MAX	0x7fffffffffffffffLL	
					/* max value for a signed long long */
# define LLONG_MIN	(-0x7fffffffffffffffLL-1)	
					/* min value for a signed long long */
#endif

#if __BSD_VISIBLE
# define UID_MAX	UINT_MAX	/* max value for a uid_t */
# define GID_MAX	UINT_MAX	/* max value for a gid_t */
#endif


#if __XPG_VISIBLE
# ifdef __LP64__
#  define LONG_BIT	64
# else
#  define LONG_BIT	32
# endif
# define WORD_BIT	32

/* float.h defines these as well */
# if !defined(DBL_DIG)
#  if defined(__DBL_DIG)
#   define DBL_DIG	__DBL_DIG
#   define DBL_MAX	__DBL_MAX
#   define DBL_MIN	__DBL_MIN

#   define FLT_DIG	__FLT_DIG
#   define FLT_MAX	__FLT_MAX
#   define FLT_MIN	__FLT_MIN
#  else
#   define DBL_DIG	15
#   define DBL_MAX	1.7976931348623157E+308
#   define DBL_MIN	2.2250738585072014E-308
 
#   define FLT_DIG	6
#   define FLT_MAX	3.40282347E+38F
#   define FLT_MIN	1.17549435E-38F
#  endif
# endif
#endif
