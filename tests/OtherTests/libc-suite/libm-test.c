/* Copyright (C) 1997-2006, 2007, 2009, 2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* Part of testsuite for libm.

   This file is processed by a perl script.  The resulting file has to
   be included by a master file that defines:

   Macros:
   FUNC(function): converts general function name (like cos) to
   name with correct suffix (e.g. cosl or cosf)
   MATHCONST(x):   like FUNC but for constants (e.g convert 0.0 to 0.0L)
   FLOAT:	   floating point type to test
   - TEST_MSG:	   informal message to be displayed
   CHOOSE(Clongdouble,Cdouble,Cfloat,Cinlinelongdouble,Cinlinedouble,Cinlinefloat):
   chooses one of the parameters as delta for testing
   equality
   PRINTF_EXPR	   Floating point conversion specification to print a variable
   of type FLOAT with printf.  PRINTF_EXPR just contains
   the specifier, not the percent and width arguments,
   e.g. "f".
   PRINTF_XEXPR	   Like PRINTF_EXPR, but print in hexadecimal format.
   PRINTF_NEXPR Like PRINTF_EXPR, but print nice.  */

/* This testsuite has currently tests for:
   acos, acosh, asin, asinh, atan, atan2, atanh,
   cbrt, ceil, copysign, cos, cosh, erf, erfc, exp, exp10, exp2, expm1,
   fabs, fdim, floor, fma, fmax, fmin, fmod, fpclassify,
   frexp, gamma, hypot,
   ilogb, isfinite, isinf, isnan, isnormal,
   isless, islessequal, isgreater, isgreaterequal, islessgreater, isunordered,
   j0, j1, jn,
   ldexp, lgamma, log, log10, log1p, log2, logb,
   modf, nearbyint, nextafter,
   pow, remainder, remquo, rint, lrint, llrint,
   round, lround, llround,
   scalb, scalbn, scalbln, signbit, sin, sincos, sinh, sqrt, tan, tanh, tgamma, trunc,
   y0, y1, yn, significand

   and for the following complex math functions:
   cabs, cacos, cacosh, carg, casin, casinh, catan, catanh,
   ccos, ccosh, cexp, clog, cpow, cproj, csin, csinh, csqrt, ctan, ctanh.

   At the moment the following functions aren't tested:
   drem, nan

   Parameter handling is primitive in the moment:
   --verbose=[0..3] for different levels of output:
   0: only error count
   1: basic report on failed tests (default)
   2: full report on all tests
   -v for full output (equals --verbose=3)
   -u for generation of an ULPs file
 */

/* "Philosophy":

   This suite tests some aspects of the correct implementation of
   mathematical functions in libm.  Some simple, specific parameters
   are tested for correctness but there's no exhaustive
   testing.  Handling of specific inputs (e.g. infinity, not-a-number)
   is also tested.  Correct handling of exceptions is checked
   against.  These implemented tests should check all cases that are
   specified in ISO C99.

   Exception testing: At the moment only divide-by-zero and invalid
   exceptions are tested.  Overflow/underflow and inexact exceptions
   aren't checked at the moment.

   NaN values: There exist signalling and quiet NaNs.  This implementation
   only uses quiet NaN as parameter but does not differenciate
   between the two kinds of NaNs as result.

   Inline functions: Inlining functions should give an improvement in
   speed - but not in precission.  The inlined functions return
   reasonable values for a reasonable range of input values.  The
   result is not necessarily correct for all values and exceptions are
   not correctly raised in all cases.  Problematic input and return
   values are infinity, not-a-number and minus zero.  This suite
   therefore does not check these specific inputs and the exception
   handling for inlined mathematical functions - just the "reasonable"
   values are checked.

   Beware: The tests might fail for any of the following reasons:
   - Tests are wrong
   - Functions are wrong
   - Floating Point Unit not working properly
   - Compiler has errors

   With e.g. gcc 2.7.2.2 the test for cexp fails because of a compiler error.


   To Do: All parameter should be numbers that can be represented as
   exact floating point values.  Currently some values cannot be
   represented exactly and therefore the result is not the expected
   result.  For this we will use 36 digits so that numbers can be
   represented exactly.  */

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include "libm-test-ulps.h"
#include <complex.h>
#include <math.h>
#include <float.h>
#include <fenv.h>
#include <limits.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if 0
#include <argp.h>
#endif

/* Possible exceptions */
#define NO_EXCEPTION			0x0
#define INVALID_EXCEPTION		0x1
#define DIVIDE_BY_ZERO_EXCEPTION	0x2
/* The next flags signals that those exceptions are allowed but not required.   */
#define INVALID_EXCEPTION_OK		0x4
#define DIVIDE_BY_ZERO_EXCEPTION_OK	0x8
#define EXCEPTIONS_OK INVALID_EXCEPTION_OK+DIVIDE_BY_ZERO_EXCEPTION_OK
/* Some special test flags, passed togther with exceptions.  */
#define IGNORE_ZERO_INF_SIGN		0x10

/* Various constants (we must supply them precalculated for accuracy).  */
#define M_PI_6l			.52359877559829887307710723054658383L
#define M_E2l			7.389056098930650227230427460575008L
#define M_E3l			20.085536923187667740928529654581719L
#define M_2_SQRT_PIl		3.5449077018110320545963349666822903L	/* 2 sqrt (M_PIl)  */
#define M_SQRT_PIl		1.7724538509055160272981674833411451L	/* sqrt (M_PIl)  */
#define M_LOG_SQRT_PIl		0.57236494292470008707171367567652933L	/* log(sqrt(M_PIl))  */
#define M_LOG_2_SQRT_PIl	1.265512123484645396488945797134706L	/* log(2*sqrt(M_PIl))  */
#define M_PI_34l		(M_PIl - M_PI_4l)		/* 3*pi/4 */
#define M_PI_34_LOG10El		(M_PIl - M_PI_4l) * M_LOG10El
#define M_PI2_LOG10El		M_PI_2l * M_LOG10El
#define M_PI4_LOG10El		M_PI_4l * M_LOG10El
#define M_PI_LOG10El		M_PIl * M_LOG10El
#define M_SQRT_2_2		0.70710678118654752440084436210484903L /* sqrt (2) / 2 */

static FILE *ulps_file;	/* File to document difference.  */
static int output_ulps;	/* Should ulps printed?  */

static int noErrors;	/* number of errors */
static int noTests;	/* number of tests (without testing exceptions) */
static int noExcTests;	/* number of tests for exception flags */
static int noXFails;	/* number of expected failures.  */
static int noXPasses;	/* number of unexpected passes.  */

static int verbose;
static int output_max_error;	/* Should the maximal errors printed?  */
static int output_points;	/* Should the single function results printed?  */
static int ignore_max_ulp;	/* Should we ignore max_ulp?  */

static FLOAT minus_zero, plus_zero;
static FLOAT plus_infty, minus_infty, nan_value, max_value, min_value;

static FLOAT max_error, real_max_error, imag_max_error;


#define BUILD_COMPLEX(real, imag) \
  ({ __complex__ FLOAT __retval;					      \
     __real__ __retval = (real);					      \
     __imag__ __retval = (imag);					      \
     __retval; })

#define BUILD_COMPLEX_INT(real, imag) \
  ({ __complex__ int __retval;						      \
     __real__ __retval = (real);					      \
     __imag__ __retval = (imag);					      \
     __retval; })


#define MANT_DIG CHOOSE ((LDBL_MANT_DIG-1), (DBL_MANT_DIG-1), (FLT_MANT_DIG-1),  \
                         (LDBL_MANT_DIG-1), (DBL_MANT_DIG-1), (FLT_MANT_DIG-1))

static void
init_max_error (void)
{
  max_error = 0;
  real_max_error = 0;
  imag_max_error = 0;
  feclearexcept (FE_ALL_EXCEPT);
}

static void
set_max_error (FLOAT current, FLOAT *curr_max_error)
{
  if (current > *curr_max_error)
    *curr_max_error = current;
}


/* Should the message print to screen?  This depends on the verbose flag,
   and the test status.  */
static int
print_screen (int ok, int xfail)
{
  if (output_points
      && (verbose > 1
	  || (verbose == 1 && ok == xfail)))
    return 1;
  return 0;
}


/* Should the message print to screen?  This depends on the verbose flag,
   and the test status.  */
static int
print_screen_max_error (int ok, int xfail)
{
  if (output_max_error
      && (verbose > 1
	  || ((verbose == 1) && (ok == xfail))))
    return 1;
  return 0;
}

/* Update statistic counters.  */
static void
update_stats (int ok, int xfail)
{
  ++noTests;
  if (ok && xfail)
    ++noXPasses;
  else if (!ok && xfail)
    ++noXFails;
  else if (!ok && !xfail)
    ++noErrors;
}

static void
print_ulps (const char *test_name, FLOAT ulp)
{
  if (output_ulps)
    {
      fprintf (ulps_file, "Test \"%s\":\n", test_name);
      fprintf (ulps_file, "%s: %.0" PRINTF_NEXPR "\n",
	       CHOOSE("ldouble", "double", "float",
		      "ildouble", "idouble", "ifloat"),
	       FUNC(ceil) (ulp));
    }
}

static void
print_function_ulps (const char *function_name, FLOAT ulp)
{
  if (output_ulps)
    {
      fprintf (ulps_file, "Function: \"%s\":\n", function_name);
      fprintf (ulps_file, "%s: %.0" PRINTF_NEXPR "\n",
	       CHOOSE("ldouble", "double", "float",
		      "ildouble", "idouble", "ifloat"),
	       FUNC(ceil) (ulp));
    }
}


static void
print_complex_function_ulps (const char *function_name, FLOAT real_ulp,
			     FLOAT imag_ulp)
{
  if (output_ulps)
    {
      if (real_ulp != 0.0)
	{
	  fprintf (ulps_file, "Function: Real part of \"%s\":\n", function_name);
	  fprintf (ulps_file, "%s: %.0" PRINTF_NEXPR "\n",
		   CHOOSE("ldouble", "double", "float",
			  "ildouble", "idouble", "ifloat"),
		   FUNC(ceil) (real_ulp));
	}
      if (imag_ulp != 0.0)
	{
	  fprintf (ulps_file, "Function: Imaginary part of \"%s\":\n", function_name);
	  fprintf (ulps_file, "%s: %.0" PRINTF_NEXPR "\n",
		   CHOOSE("ldouble", "double", "float",
			  "ildouble", "idouble", "ifloat"),
		   FUNC(ceil) (imag_ulp));
	}


    }
}



/* Test if Floating-Point stack hasn't changed */
static void
fpstack_test (const char *test_name)
{
#ifdef i386
  static int old_stack;
  int sw;

  asm ("fnstsw" : "=a" (sw));
  sw >>= 11;
  sw &= 7;

  if (sw != old_stack)
    {
      printf ("FP-Stack wrong after test %s (%d, should be %d)\n",
	      test_name, sw, old_stack);
      ++noErrors;
      old_stack = sw;
    }
#endif
}


static void
print_max_error (const char *func_name, FLOAT allowed, int xfail)
{
  int ok = 0;

  if (max_error == 0.0 || (max_error <= allowed && !ignore_max_ulp))
    {
      ok = 1;
    }

  if (!ok)
    print_function_ulps (func_name, max_error);


  if (print_screen_max_error (ok, xfail))
    {
      printf ("Maximal error of `%s'\n", func_name);
      printf (" is      : %.0" PRINTF_NEXPR " ulp\n", FUNC(ceil) (max_error));
      printf (" accepted: %.0" PRINTF_NEXPR " ulp\n", FUNC(ceil) (allowed));
    }

  update_stats (ok, xfail);
}


static void
print_complex_max_error (const char *func_name, __complex__ FLOAT allowed,
			 __complex__ int xfail)
{
  int ok = 0;

  if ((real_max_error == 0 && imag_max_error == 0)
      || (real_max_error <= __real__ allowed
	  && imag_max_error <= __imag__ allowed
	  && !ignore_max_ulp))
    {
      ok = 1;
    }

  if (!ok)
    print_complex_function_ulps (func_name, real_max_error, imag_max_error);


  if (print_screen_max_error (ok, xfail))
    {
      printf ("Maximal error of real part of: %s\n", func_name);
      printf (" is      : %.0" PRINTF_NEXPR " ulp\n",
	      FUNC(ceil) (real_max_error));
      printf (" accepted: %.0" PRINTF_NEXPR " ulp\n",
	      FUNC(ceil) (__real__ allowed));
      printf ("Maximal error of imaginary part of: %s\n", func_name);
      printf (" is      : %.0" PRINTF_NEXPR " ulp\n",
	      FUNC(ceil) (imag_max_error));
      printf (" accepted: %.0" PRINTF_NEXPR " ulp\n",
	      FUNC(ceil) (__imag__ allowed));
    }

  update_stats (ok, xfail);
}


/* Test whether a given exception was raised.  */
static void
test_single_exception (const char *test_name,
		       int exception,
		       int exc_flag,
		       int fe_flag,
		       const char *flag_name)
{
#ifndef TEST_INLINE
  int ok = 1;
  if (exception & exc_flag)
    {
      if (fetestexcept (fe_flag))
	{
	  if (print_screen (1, 0))
	    printf ("Pass: %s: Exception \"%s\" set\n", test_name, flag_name);
	}
      else
	{
	  ok = 0;
	  if (print_screen (0, 0))
	    printf ("Failure: %s: Exception \"%s\" not set\n",
		    test_name, flag_name);
	}
    }
  else
    {
      if (fetestexcept (fe_flag))
	{
	  ok = 0;
	  if (print_screen (0, 0))
	    printf ("Failure: %s: Exception \"%s\" set\n",
		    test_name, flag_name);
	}
      else
	{
	  if (print_screen (1, 0))
	    printf ("%s: Exception \"%s\" not set\n", test_name,
		    flag_name);
	}
    }
  if (!ok)
    ++noErrors;

#endif
}


/* Test whether exceptions given by EXCEPTION are raised.  Ignore thereby
   allowed but not required exceptions.
*/
static void
test_exceptions (const char *test_name, int exception)
{
  ++noExcTests;
#ifdef FE_DIVBYZERO
  if ((exception & DIVIDE_BY_ZERO_EXCEPTION_OK) == 0)
    test_single_exception (test_name, exception,
			   DIVIDE_BY_ZERO_EXCEPTION, FE_DIVBYZERO,
			   "Divide by zero");
#endif
#ifdef FE_INVALID
  if ((exception & INVALID_EXCEPTION_OK) == 0)
    test_single_exception (test_name, exception, INVALID_EXCEPTION, FE_INVALID,
			 "Invalid operation");
#endif
  feclearexcept (FE_ALL_EXCEPT);
}


static void
check_float_internal (const char *test_name, FLOAT computed, FLOAT expected,
		      FLOAT max_ulp, int xfail, int exceptions,
		      FLOAT *curr_max_error)
{
  int ok = 0;
  int print_diff = 0;
  FLOAT diff = 0;
  FLOAT ulp = 0;

  test_exceptions (test_name, exceptions);
  if (isnan (computed) && isnan (expected))
    ok = 1;
  else if (isinf (computed) && isinf (expected))
    {
      /* Test for sign of infinities.  */
      if ((exceptions & IGNORE_ZERO_INF_SIGN) == 0
	  && signbit (computed) != signbit (expected))
	{
	  ok = 0;
	  printf ("infinity has wrong sign.\n");
	}
      else
	ok = 1;
    }
  /* Don't calc ulp for NaNs or infinities.  */
  else if (isinf (computed) || isnan (computed) || isinf (expected) || isnan (expected))
    ok = 0;
  else
    {
      diff = FUNC(fabs) (computed - expected);
      switch (fpclassify (expected))
	{
	case FP_ZERO:
	  /* ilogb (0) isn't allowed. */
	  ulp = diff / FUNC(ldexp) (1.0, - MANT_DIG);
	  break;
	case FP_NORMAL:
	  ulp = diff / FUNC(ldexp) (1.0, FUNC(ilogb) (expected) - MANT_DIG);
	  break;
	case FP_SUBNORMAL:
	  ulp = (FUNC(ldexp) (diff, MANT_DIG)
		 / FUNC(ldexp) (1.0, FUNC(ilogb) (expected)));
	  break;
	default:
	  /* It should never happen. */
	  abort ();
	  break;
	}
      set_max_error (ulp, curr_max_error);
      print_diff = 1;
      if ((exceptions & IGNORE_ZERO_INF_SIGN) == 0
	  && computed == 0.0 && expected == 0.0
	  && signbit(computed) != signbit (expected))
	ok = 0;
      else if (ulp <= 0.5 || (ulp <= max_ulp && !ignore_max_ulp))
	ok = 1;
      else
	{
	  ok = 0;
	  print_ulps (test_name, ulp);
	}

    }
  if (print_screen (ok, xfail))
    {
      if (!ok)
	printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         % .20" PRINTF_EXPR "  % .20" PRINTF_XEXPR "\n",
	      computed, computed);
      printf (" should be:  % .20" PRINTF_EXPR "  % .20" PRINTF_XEXPR "\n",
	      expected, expected);
      if (print_diff)
	{
	  printf (" difference: % .20" PRINTF_EXPR "  % .20" PRINTF_XEXPR
		  "\n", diff, diff);
	  printf (" ulp       : % .4" PRINTF_NEXPR "\n", ulp);
	  printf (" max.ulp   : % .4" PRINTF_NEXPR "\n", max_ulp);
	}
    }
  update_stats (ok, xfail);

  fpstack_test (test_name);
}


static void
check_float (const char *test_name, FLOAT computed, FLOAT expected,
	     FLOAT max_ulp, int xfail, int exceptions)
{
  check_float_internal (test_name, computed, expected, max_ulp, xfail,
			exceptions, &max_error);
}


static void
check_complex (const char *test_name, __complex__ FLOAT computed,
	       __complex__ FLOAT expected,
	       __complex__ FLOAT max_ulp, __complex__ int xfail,
	       int exception)
{
  FLOAT part_comp, part_exp, part_max_ulp;
  int part_xfail;
  char str[200];

  sprintf (str, "Real part of: %s", test_name);
  part_comp = __real__ computed;
  part_exp = __real__ expected;
  part_max_ulp = __real__ max_ulp;
  part_xfail = __real__ xfail;

  check_float_internal (str, part_comp, part_exp, part_max_ulp, part_xfail,
			exception, &real_max_error);

  sprintf (str, "Imaginary part of: %s", test_name);
  part_comp = __imag__ computed;
  part_exp = __imag__ expected;
  part_max_ulp = __imag__ max_ulp;
  part_xfail = __imag__ xfail;

  /* Don't check again for exceptions, just pass through the
     zero/inf sign test.  */
  check_float_internal (str, part_comp, part_exp, part_max_ulp, part_xfail,
			exception & IGNORE_ZERO_INF_SIGN,
			&imag_max_error);
}


/* Check that computed and expected values are equal (int values).  */
static void
check_int (const char *test_name, int computed, int expected, int max_ulp,
	   int xfail, int exceptions)
{
  int diff = computed - expected;
  int ok = 0;

  test_exceptions (test_name, exceptions);
  noTests++;
  if (abs (diff) <= max_ulp)
    ok = 1;

  if (!ok)
    print_ulps (test_name, diff);

  if (print_screen (ok, xfail))
    {
      if (!ok)
	printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %d\n", computed);
      printf (" should be:  %d\n", expected);
    }

  update_stats (ok, xfail);
  fpstack_test (test_name);
}


/* Check that computed and expected values are equal (long int values).  */
static void
check_long (const char *test_name, long int computed, long int expected,
	    long int max_ulp, int xfail, int exceptions)
{
  long int diff = computed - expected;
  int ok = 0;

  test_exceptions (test_name, exceptions);
  noTests++;
  if (labs (diff) <= max_ulp)
    ok = 1;

  if (!ok)
    print_ulps (test_name, diff);

  if (print_screen (ok, xfail))
    {
      if (!ok)
	printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %ld\n", computed);
      printf (" should be:  %ld\n", expected);
    }

  update_stats (ok, xfail);
  fpstack_test (test_name);
}


/* Check that computed value is true/false.  */
static void
check_bool (const char *test_name, int computed, int expected,
	    long int max_ulp, int xfail, int exceptions)
{
  int ok = 0;

  test_exceptions (test_name, exceptions);
  noTests++;
  if ((computed == 0) == (expected == 0))
    ok = 1;

  if (print_screen (ok, xfail))
    {
      if (!ok)
	printf ("Failure: ");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %d\n", computed);
      printf (" should be:  %d\n", expected);
    }

  update_stats (ok, xfail);
  fpstack_test (test_name);
}


/* check that computed and expected values are equal (long int values) */
static void
check_longlong (const char *test_name, long long int computed,
		long long int expected,
		long long int max_ulp, int xfail,
		int exceptions)
{
  long long int diff = computed - expected;
  int ok = 0;

  test_exceptions (test_name, exceptions);
  noTests++;
  if (llabs (diff) <= max_ulp)
    ok = 1;

  if (!ok)
    print_ulps (test_name, diff);

  if (print_screen (ok, xfail))
    {
      if (!ok)
	printf ("Failure:");
      printf ("Test: %s\n", test_name);
      printf ("Result:\n");
      printf (" is:         %lld\n", computed);
      printf (" should be:  %lld\n", expected);
    }

  update_stats (ok, xfail);
  fpstack_test (test_name);
}


#if 0
/* This is to prevent messages from the SVID libm emulation.  */
int
matherr (struct exception *x __attribute__ ((unused)))
{
  return 1;
}
#endif

/****************************************************************************
  Tests for single functions of libm.
  Please keep them alphabetically sorted!
****************************************************************************/

static void
acos_test (void)
{
  errno = 0;
  FUNC(acos) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("acos (inf) == NaN plus invalid exception",  FUNC(acos) (plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("acos (-inf) == NaN plus invalid exception",  FUNC(acos) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("acos (NaN) == NaN",  FUNC(acos) (nan_value), nan_value, 0, 0, 0);

  /* |x| > 1: */
  check_float ("acos (1.125) == NaN plus invalid exception",  FUNC(acos) (1.125L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("acos (-1.125) == NaN plus invalid exception",  FUNC(acos) (-1.125L), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("acos (0) == pi/2",  FUNC(acos) (0), M_PI_2l, 0, 0, 0);
  check_float ("acos (-0) == pi/2",  FUNC(acos) (minus_zero), M_PI_2l, 0, 0, 0);
  check_float ("acos (1) == 0",  FUNC(acos) (1), 0, 0, 0, 0);
  check_float ("acos (-1) == pi",  FUNC(acos) (-1), M_PIl, 0, 0, 0);
  check_float ("acos (0.5) == M_PI_6l*2.0",  FUNC(acos) (0.5), M_PI_6l*2.0, 0, 0, 0);
  check_float ("acos (-0.5) == M_PI_6l*4.0",  FUNC(acos) (-0.5), M_PI_6l*4.0, 0, 0, 0);
  check_float ("acos (0.75) == 0.722734247813415611178377352641333362",  FUNC(acos) (0.75L), 0.722734247813415611178377352641333362L, 0, 0, 0);
  check_float ("acos (2e-17) == 1.57079632679489659923132169163975144",  FUNC(acos) (2e-17L), 1.57079632679489659923132169163975144L, 0, 0, 0);
  check_float ("acos (0.0625) == 1.50825556499840522843072005474337068",  FUNC(acos) (0.0625L), 1.50825556499840522843072005474337068L, 0, 0, 0);
  print_max_error ("acos", 0, 0);
}

static void
acosh_test (void)
{
  errno = 0;
  FUNC(acosh) (7);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("acosh (inf) == inf",  FUNC(acosh) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("acosh (-inf) == NaN plus invalid exception",  FUNC(acosh) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  /* x < 1:  */
  check_float ("acosh (-1.125) == NaN plus invalid exception",  FUNC(acosh) (-1.125L), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("acosh (1) == 0",  FUNC(acosh) (1), 0, 0, 0, 0);
  check_float ("acosh (7) == 2.63391579384963341725009269461593689",  FUNC(acosh) (7), 2.63391579384963341725009269461593689L, 0, 0, 0);

  print_max_error ("acosh", 0, 0);
}

static void
asin_test (void)
{
  errno = 0;
  FUNC(asin) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("asin (inf) == NaN plus invalid exception",  FUNC(asin) (plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("asin (-inf) == NaN plus invalid exception",  FUNC(asin) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("asin (NaN) == NaN",  FUNC(asin) (nan_value), nan_value, 0, 0, 0);

  /* asin x == NaN plus invalid exception for |x| > 1.  */
  check_float ("asin (1.125) == NaN plus invalid exception",  FUNC(asin) (1.125L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("asin (-1.125) == NaN plus invalid exception",  FUNC(asin) (-1.125L), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("asin (0) == 0",  FUNC(asin) (0), 0, 0, 0, 0);
  check_float ("asin (-0) == -0",  FUNC(asin) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("asin (0.5) == pi/6",  FUNC(asin) (0.5), M_PI_6l, 0, 0, 0);
  check_float ("asin (-0.5) == -pi/6",  FUNC(asin) (-0.5), -M_PI_6l, 0, 0, 0);
  check_float ("asin (1.0) == pi/2",  FUNC(asin) (1.0), M_PI_2l, 0, 0, 0);
  check_float ("asin (-1.0) == -pi/2",  FUNC(asin) (-1.0), -M_PI_2l, 0, 0, 0);
  check_float ("asin (0.75) == 0.848062078981481008052944338998418080",  FUNC(asin) (0.75L), 0.848062078981481008052944338998418080L, 0, 0, 0);

  print_max_error ("asin", 0, 0);
}

static void
asinh_test (void)
{
  errno = 0;
  FUNC(asinh) (0.7L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("asinh (0) == 0",  FUNC(asinh) (0), 0, 0, 0, 0);
  check_float ("asinh (-0) == -0",  FUNC(asinh) (minus_zero), minus_zero, 0, 0, 0);
#ifndef TEST_INLINE
  check_float ("asinh (inf) == inf",  FUNC(asinh) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("asinh (-inf) == -inf",  FUNC(asinh) (minus_infty), minus_infty, 0, 0, 0);
#endif
  check_float ("asinh (NaN) == NaN",  FUNC(asinh) (nan_value), nan_value, 0, 0, 0);
  check_float ("asinh (0.75) == 0.693147180559945309417232121458176568",  FUNC(asinh) (0.75L), 0.693147180559945309417232121458176568L, 0, 0, 0);

  print_max_error ("asinh", 0, 0);
}

static void
atan_test (void)
{
  errno = 0;
  FUNC(atan) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("atan (0) == 0",  FUNC(atan) (0), 0, 0, 0, 0);
  check_float ("atan (-0) == -0",  FUNC(atan) (minus_zero), minus_zero, 0, 0, 0);

  check_float ("atan (inf) == pi/2",  FUNC(atan) (plus_infty), M_PI_2l, 0, 0, 0);
  check_float ("atan (-inf) == -pi/2",  FUNC(atan) (minus_infty), -M_PI_2l, 0, 0, 0);
  check_float ("atan (NaN) == NaN",  FUNC(atan) (nan_value), nan_value, 0, 0, 0);

  check_float ("atan (1) == pi/4",  FUNC(atan) (1), M_PI_4l, 0, 0, 0);
  check_float ("atan (-1) == -pi/4",  FUNC(atan) (-1), -M_PI_4l, 0, 0, 0);

  check_float ("atan (0.75) == 0.643501108793284386802809228717322638",  FUNC(atan) (0.75L), 0.643501108793284386802809228717322638L, 0, 0, 0);

  print_max_error ("atan", 0, 0);
}



static void
atanh_test (void)
{
  errno = 0;
  FUNC(atanh) (0.7L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();


  check_float ("atanh (0) == 0",  FUNC(atanh) (0), 0, 0, 0, 0);
  check_float ("atanh (-0) == -0",  FUNC(atanh) (minus_zero), minus_zero, 0, 0, 0);

  check_float ("atanh (1) == inf plus division by zero exception",  FUNC(atanh) (1), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("atanh (-1) == -inf plus division by zero exception",  FUNC(atanh) (-1), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("atanh (NaN) == NaN",  FUNC(atanh) (nan_value), nan_value, 0, 0, 0);

  /* atanh (x) == NaN plus invalid exception if |x| > 1.  */
  check_float ("atanh (1.125) == NaN plus invalid exception",  FUNC(atanh) (1.125L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("atanh (-1.125) == NaN plus invalid exception",  FUNC(atanh) (-1.125L), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("atanh (0.75) == 0.972955074527656652552676371721589865",  FUNC(atanh) (0.75L), 0.972955074527656652552676371721589865L, 0, 0, 0);

  print_max_error ("atanh", 0, 0);
}

static void
atan2_test (void)
{
  errno = 0;
  FUNC(atan2) (-0, 1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  /* atan2 (0,x) == 0 for x > 0.  */
  check_float ("atan2 (0, 1) == 0",  FUNC(atan2) (0, 1), 0, 0, 0, 0);

  /* atan2 (-0,x) == -0 for x > 0.  */
  check_float ("atan2 (-0, 1) == -0",  FUNC(atan2) (minus_zero, 1), minus_zero, 0, 0, 0);

  check_float ("atan2 (0, 0) == 0",  FUNC(atan2) (0, 0), 0, 0, 0, 0);
  check_float ("atan2 (-0, 0) == -0",  FUNC(atan2) (minus_zero, 0), minus_zero, 0, 0, 0);

  /* atan2 (+0,x) == +pi for x < 0.  */
  check_float ("atan2 (0, -1) == pi",  FUNC(atan2) (0, -1), M_PIl, 0, 0, 0);

  /* atan2 (-0,x) == -pi for x < 0.  */
  check_float ("atan2 (-0, -1) == -pi",  FUNC(atan2) (minus_zero, -1), -M_PIl, 0, 0, 0);

  check_float ("atan2 (0, -0) == pi",  FUNC(atan2) (0, minus_zero), M_PIl, 0, 0, 0);
  check_float ("atan2 (-0, -0) == -pi",  FUNC(atan2) (minus_zero, minus_zero), -M_PIl, 0, 0, 0);

  /* atan2 (y,+0) == pi/2 for y > 0.  */
  check_float ("atan2 (1, 0) == pi/2",  FUNC(atan2) (1, 0), M_PI_2l, 0, 0, 0);

  /* atan2 (y,-0) == pi/2 for y > 0.  */
  check_float ("atan2 (1, -0) == pi/2",  FUNC(atan2) (1, minus_zero), M_PI_2l, 0, 0, 0);

  /* atan2 (y,+0) == -pi/2 for y < 0.  */
  check_float ("atan2 (-1, 0) == -pi/2",  FUNC(atan2) (-1, 0), -M_PI_2l, 0, 0, 0);

  /* atan2 (y,-0) == -pi/2 for y < 0.  */
  check_float ("atan2 (-1, -0) == -pi/2",  FUNC(atan2) (-1, minus_zero), -M_PI_2l, 0, 0, 0);

  /* atan2 (y,inf) == +0 for finite y > 0.  */
  check_float ("atan2 (1, inf) == 0",  FUNC(atan2) (1, plus_infty), 0, 0, 0, 0);

  /* atan2 (y,inf) == -0 for finite y < 0.  */
  check_float ("atan2 (-1, inf) == -0",  FUNC(atan2) (-1, plus_infty), minus_zero, 0, 0, 0);

  /* atan2(+inf, x) == pi/2 for finite x.  */
  check_float ("atan2 (inf, -1) == pi/2",  FUNC(atan2) (plus_infty, -1), M_PI_2l, 0, 0, 0);

  /* atan2(-inf, x) == -pi/2 for finite x.  */
  check_float ("atan2 (-inf, 1) == -pi/2",  FUNC(atan2) (minus_infty, 1), -M_PI_2l, 0, 0, 0);

  /* atan2 (y,-inf) == +pi for finite y > 0.  */
  check_float ("atan2 (1, -inf) == pi",  FUNC(atan2) (1, minus_infty), M_PIl, 0, 0, 0);

  /* atan2 (y,-inf) == -pi for finite y < 0.  */
  check_float ("atan2 (-1, -inf) == -pi",  FUNC(atan2) (-1, minus_infty), -M_PIl, 0, 0, 0);

  check_float ("atan2 (inf, inf) == pi/4",  FUNC(atan2) (plus_infty, plus_infty), M_PI_4l, 0, 0, 0);
  check_float ("atan2 (-inf, inf) == -pi/4",  FUNC(atan2) (minus_infty, plus_infty), -M_PI_4l, 0, 0, 0);
  check_float ("atan2 (inf, -inf) == 3/4 pi",  FUNC(atan2) (plus_infty, minus_infty), M_PI_34l, 0, 0, 0);
  check_float ("atan2 (-inf, -inf) == -3/4 pi",  FUNC(atan2) (minus_infty, minus_infty), -M_PI_34l, 0, 0, 0);
  check_float ("atan2 (NaN, NaN) == NaN",  FUNC(atan2) (nan_value, nan_value), nan_value, 0, 0, 0);

  check_float ("atan2 (0.75, 1) == 0.643501108793284386802809228717322638",  FUNC(atan2) (0.75L, 1), 0.643501108793284386802809228717322638L, 0, 0, 0);
  check_float ("atan2 (-0.75, 1.0) == -0.643501108793284386802809228717322638",  FUNC(atan2) (-0.75L, 1.0L), -0.643501108793284386802809228717322638L, 0, 0, 0);
  check_float ("atan2 (0.75, -1.0) == 2.49809154479650885165983415456218025",  FUNC(atan2) (0.75L, -1.0L), 2.49809154479650885165983415456218025L, 0, 0, 0);
  check_float ("atan2 (-0.75, -1.0) == -2.49809154479650885165983415456218025",  FUNC(atan2) (-0.75L, -1.0L), -2.49809154479650885165983415456218025L, 0, 0, 0);
  check_float ("atan2 (0.390625, .00029) == 1.57005392693128974780151246612928941",  FUNC(atan2) (0.390625L, .00029L), 1.57005392693128974780151246612928941L, 0, 0, 0);
  check_float ("atan2 (1.390625, 0.9296875) == 0.981498387184244311516296577615519772",  FUNC(atan2) (1.390625L, 0.9296875L), 0.981498387184244311516296577615519772L, 0, 0, 0);

  check_float ("atan2 (-0.00756827042671106339, -.001792735857538728036) == -1.80338464113663849327153994379639112",  FUNC(atan2) (-0.00756827042671106339L, -.001792735857538728036L), -1.80338464113663849327153994379639112L, 0, 0, 0);

  print_max_error ("atan2", 0, 0);
}

#if 0
static void
cabs_test (void)
{
  errno = 0;
  FUNC(cabs) (BUILD_COMPLEX (0.7L, 12.4L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  /* cabs (x + iy) is specified as hypot (x,y) */

  /* cabs (+inf + i x) == +inf.  */
  check_float ("cabs (inf + 1.0 i) == inf",  FUNC(cabs) (BUILD_COMPLEX (plus_infty, 1.0)), plus_infty, 0, 0, 0);
  /* cabs (-inf + i x) == +inf.  */
  check_float ("cabs (-inf + 1.0 i) == inf",  FUNC(cabs) (BUILD_COMPLEX (minus_infty, 1.0)), plus_infty, 0, 0, 0);

  check_float ("cabs (-inf + NaN i) == inf",  FUNC(cabs) (BUILD_COMPLEX (minus_infty, nan_value)), plus_infty, 0, 0, 0);
  check_float ("cabs (-inf + NaN i) == inf",  FUNC(cabs) (BUILD_COMPLEX (minus_infty, nan_value)), plus_infty, 0, 0, 0);

  check_float ("cabs (NaN + NaN i) == NaN",  FUNC(cabs) (BUILD_COMPLEX (nan_value, nan_value)), nan_value, 0, 0, 0);

  /* cabs (x,y) == cabs (y,x).  */
  check_float ("cabs (0.75 + 12.390625 i) == 12.4133028598606664302388810868156657",  FUNC(cabs) (BUILD_COMPLEX (0.75L, 12.390625L)), 12.4133028598606664302388810868156657L, 0, 0, 0);
  /* cabs (x,y) == cabs (-x,y).  */
  check_float ("cabs (-12.390625 + 0.75 i) == 12.4133028598606664302388810868156657",  FUNC(cabs) (BUILD_COMPLEX (-12.390625L, 0.75L)), 12.4133028598606664302388810868156657L, 0, 0, 0);
  /* cabs (x,y) == cabs (-y,x).  */
  check_float ("cabs (-0.75 + 12.390625 i) == 12.4133028598606664302388810868156657",  FUNC(cabs) (BUILD_COMPLEX (-0.75L, 12.390625L)), 12.4133028598606664302388810868156657L, 0, 0, 0);
  /* cabs (x,y) == cabs (-x,-y).  */
  check_float ("cabs (-12.390625 - 0.75 i) == 12.4133028598606664302388810868156657",  FUNC(cabs) (BUILD_COMPLEX (-12.390625L, -0.75L)), 12.4133028598606664302388810868156657L, 0, 0, 0);
  /* cabs (x,y) == cabs (-y,-x).  */
  check_float ("cabs (-0.75 - 12.390625 i) == 12.4133028598606664302388810868156657",  FUNC(cabs) (BUILD_COMPLEX (-0.75L, -12.390625L)), 12.4133028598606664302388810868156657L, 0, 0, 0);
  /* cabs (x,0) == fabs (x).  */
  check_float ("cabs (-0.75 + 0 i) == 0.75",  FUNC(cabs) (BUILD_COMPLEX (-0.75L, 0)), 0.75L, 0, 0, 0);
  check_float ("cabs (0.75 + 0 i) == 0.75",  FUNC(cabs) (BUILD_COMPLEX (0.75L, 0)), 0.75L, 0, 0, 0);
  check_float ("cabs (-1.0 + 0 i) == 1.0",  FUNC(cabs) (BUILD_COMPLEX (-1.0L, 0)), 1.0L, 0, 0, 0);
  check_float ("cabs (1.0 + 0 i) == 1.0",  FUNC(cabs) (BUILD_COMPLEX (1.0L, 0)), 1.0L, 0, 0, 0);
  check_float ("cabs (-5.7e7 + 0 i) == 5.7e7",  FUNC(cabs) (BUILD_COMPLEX (-5.7e7L, 0)), 5.7e7L, 0, 0, 0);
  check_float ("cabs (5.7e7 + 0 i) == 5.7e7",  FUNC(cabs) (BUILD_COMPLEX (5.7e7L, 0)), 5.7e7L, 0, 0, 0);

  check_float ("cabs (0.75 + 1.25 i) == 1.45773797371132511771853821938639577",  FUNC(cabs) (BUILD_COMPLEX (0.75L, 1.25L)), 1.45773797371132511771853821938639577L, 0, 0, 0);

  print_max_error ("cabs", 0, 0);
}


static void
cacos_test (void)
{
  errno = 0;
  FUNC(cacos) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();


  check_complex ("cacos (0 + 0 i) == pi/2 - 0 i",  FUNC(cacos) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("cacos (-0 + 0 i) == pi/2 - 0 i",  FUNC(cacos) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("cacos (-0 - 0 i) == pi/2 + 0.0 i",  FUNC(cacos) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (M_PI_2l, 0.0), 0, 0, 0);
  check_complex ("cacos (0 - 0 i) == pi/2 + 0.0 i",  FUNC(cacos) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (M_PI_2l, 0.0), 0, 0, 0);

  check_complex ("cacos (-inf + inf i) == 3/4 pi - inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (M_PI_34l, minus_infty), 0, 0, 0);
  check_complex ("cacos (-inf - inf i) == 3/4 pi + inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (M_PI_34l, plus_infty), 0, 0, 0);

  check_complex ("cacos (inf + inf i) == pi/4 - inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (M_PI_4l, minus_infty), 0, 0, 0);
  check_complex ("cacos (inf - inf i) == pi/4 + inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (M_PI_4l, plus_infty), 0, 0, 0);

  check_complex ("cacos (-10.0 + inf i) == pi/2 - inf i",  FUNC(cacos) (BUILD_COMPLEX (-10.0, plus_infty)), BUILD_COMPLEX (M_PI_2l, minus_infty), 0, 0, 0);
  check_complex ("cacos (-10.0 - inf i) == pi/2 + inf i",  FUNC(cacos) (BUILD_COMPLEX (-10.0, minus_infty)), BUILD_COMPLEX (M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("cacos (0 + inf i) == pi/2 - inf i",  FUNC(cacos) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (M_PI_2l, minus_infty), 0, 0, 0);
  check_complex ("cacos (0 - inf i) == pi/2 + inf i",  FUNC(cacos) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("cacos (0.1 + inf i) == pi/2 - inf i",  FUNC(cacos) (BUILD_COMPLEX (0.1L, plus_infty)), BUILD_COMPLEX (M_PI_2l, minus_infty), 0, 0, 0);
  check_complex ("cacos (0.1 - inf i) == pi/2 + inf i",  FUNC(cacos) (BUILD_COMPLEX (0.1L, minus_infty)), BUILD_COMPLEX (M_PI_2l, plus_infty), 0, 0, 0);

  check_complex ("cacos (-inf + 0 i) == pi - inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (M_PIl, minus_infty), 0, 0, 0);
  check_complex ("cacos (-inf - 0 i) == pi + inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (M_PIl, plus_infty), 0, 0, 0);
  check_complex ("cacos (-inf + 100 i) == pi - inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, 100)), BUILD_COMPLEX (M_PIl, minus_infty), 0, 0, 0);
  check_complex ("cacos (-inf - 100 i) == pi + inf i",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, -100)), BUILD_COMPLEX (M_PIl, plus_infty), 0, 0, 0);

  check_complex ("cacos (inf + 0 i) == 0.0 - inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);
  check_complex ("cacos (inf - 0 i) == 0.0 + inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("cacos (inf + 0.5 i) == 0.0 - inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, 0.5)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);
  check_complex ("cacos (inf - 0.5 i) == 0.0 + inf i",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, -0.5)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);

  check_complex ("cacos (inf + NaN i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(cacos) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("cacos (-inf + NaN i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(cacos) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("cacos (0 + NaN i) == pi/2 + NaN i",  FUNC(cacos) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (M_PI_2l, nan_value), 0, 0, 0);
  check_complex ("cacos (-0 + NaN i) == pi/2 + NaN i",  FUNC(cacos) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (M_PI_2l, nan_value), 0, 0, 0);

  check_complex ("cacos (NaN + inf i) == NaN - inf i",  FUNC(cacos) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, minus_infty), 0, 0, 0);
  check_complex ("cacos (NaN - inf i) == NaN + inf i",  FUNC(cacos) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, 0);

  check_complex ("cacos (10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacos) (BUILD_COMPLEX (10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cacos (-10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacos) (BUILD_COMPLEX (-10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("cacos (NaN + 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacos) (BUILD_COMPLEX (nan_value, 0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cacos (NaN - 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacos) (BUILD_COMPLEX (nan_value, -0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("cacos (NaN + NaN i) == NaN + NaN i",  FUNC(cacos) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cacos (0.75 + 1.25 i) == 1.11752014915610270578240049553777969 - 1.13239363160530819522266333696834467 i",  FUNC(cacos) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.11752014915610270578240049553777969L, -1.13239363160530819522266333696834467L), 0, 0, 0);
  check_complex ("cacos (-2 - 3 i) == 2.1414491111159960199416055713254211 + 1.9833870299165354323470769028940395 i",  FUNC(cacos) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (2.1414491111159960199416055713254211L, 1.9833870299165354323470769028940395L), 0, 0, 0);

  print_complex_max_error ("cacos", 0, 0);
}

static void
cacosh_test (void)
{
  errno = 0;
  FUNC(cacosh) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();


  check_complex ("cacosh (0 + 0 i) == 0.0 + pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (-0 + 0 i) == 0.0 + pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (0 - 0 i) == 0.0 - pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (-0 - 0 i) == 0.0 - pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (-inf + inf i) == inf + 3/4 pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_34l), 0, 0, 0);
  check_complex ("cacosh (-inf - inf i) == inf - 3/4 pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_34l), 0, 0, 0);

  check_complex ("cacosh (inf + inf i) == inf + pi/4 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_4l), 0, 0, 0);
  check_complex ("cacosh (inf - inf i) == inf - pi/4 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_4l), 0, 0, 0);

  check_complex ("cacosh (-10.0 + inf i) == inf + pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (-10.0, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (-10.0 - inf i) == inf - pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (-10.0, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (0 + inf i) == inf + pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (0 - inf i) == inf - pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (0.1 + inf i) == inf + pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0.1L, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("cacosh (0.1 - inf i) == inf - pi/2 i",  FUNC(cacosh) (BUILD_COMPLEX (0.1L, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);

  check_complex ("cacosh (-inf + 0 i) == inf + pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (plus_infty, M_PIl), 0, 0, 0);
  check_complex ("cacosh (-inf - 0 i) == inf - pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, -M_PIl), 0, 0, 0);
  check_complex ("cacosh (-inf + 100 i) == inf + pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, 100)), BUILD_COMPLEX (plus_infty, M_PIl), 0, 0, 0);
  check_complex ("cacosh (-inf - 100 i) == inf - pi i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, -100)), BUILD_COMPLEX (plus_infty, -M_PIl), 0, 0, 0);

  check_complex ("cacosh (inf + 0 i) == inf + 0.0 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("cacosh (inf - 0 i) == inf - 0 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("cacosh (inf + 0.5 i) == inf + 0.0 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, 0.5)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("cacosh (inf - 0.5 i) == inf - 0 i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, -0.5)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("cacosh (inf + NaN i) == inf + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("cacosh (-inf + NaN i) == inf + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("cacosh (0 + NaN i) == NaN + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);
  check_complex ("cacosh (-0 + NaN i) == NaN + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cacosh (NaN + inf i) == inf + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("cacosh (NaN - inf i) == inf + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("cacosh (10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacosh) (BUILD_COMPLEX (10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cacosh (-10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacosh) (BUILD_COMPLEX (-10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("cacosh (NaN + 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacosh) (BUILD_COMPLEX (nan_value, 0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cacosh (NaN - 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cacosh) (BUILD_COMPLEX (nan_value, -0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("cacosh (NaN + NaN i) == NaN + NaN i",  FUNC(cacosh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cacosh (0.75 + 1.25 i) == 1.13239363160530819522266333696834467 + 1.11752014915610270578240049553777969 i",  FUNC(cacosh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.13239363160530819522266333696834467L, 1.11752014915610270578240049553777969L), 0, 0, 0);
  check_complex ("cacosh (-2 - 3 i) == 1.9833870299165354323470769028940395 - 2.1414491111159960199416055713254211 i",  FUNC(cacosh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (1.9833870299165354323470769028940395L, -2.1414491111159960199416055713254211L), 0, 0, 0);

  print_complex_max_error ("cacosh", 0, 0);
}


static void
carg_test (void)
{
  init_max_error ();

  /* carg (x + iy) is specified as atan2 (y, x) */

  /* carg (x + i 0) == 0 for x > 0.  */
  check_float ("carg (2.0 + 0 i) == 0",  FUNC(carg) (BUILD_COMPLEX (2.0, 0)), 0, 0, 0, 0);
  /* carg (x - i 0) == -0 for x > 0.  */
  check_float ("carg (2.0 - 0 i) == -0",  FUNC(carg) (BUILD_COMPLEX (2.0, minus_zero)), minus_zero, 0, 0, 0);

  check_float ("carg (0 + 0 i) == 0",  FUNC(carg) (BUILD_COMPLEX (0, 0)), 0, 0, 0, 0);
  check_float ("carg (0 - 0 i) == -0",  FUNC(carg) (BUILD_COMPLEX (0, minus_zero)), minus_zero, 0, 0, 0);

  /* carg (x + i 0) == +pi for x < 0.  */
  check_float ("carg (-2.0 + 0 i) == pi",  FUNC(carg) (BUILD_COMPLEX (-2.0, 0)), M_PIl, 0, 0, 0);

  /* carg (x - i 0) == -pi for x < 0.  */
  check_float ("carg (-2.0 - 0 i) == -pi",  FUNC(carg) (BUILD_COMPLEX (-2.0, minus_zero)), -M_PIl, 0, 0, 0);

  check_float ("carg (-0 + 0 i) == pi",  FUNC(carg) (BUILD_COMPLEX (minus_zero, 0)), M_PIl, 0, 0, 0);
  check_float ("carg (-0 - 0 i) == -pi",  FUNC(carg) (BUILD_COMPLEX (minus_zero, minus_zero)), -M_PIl, 0, 0, 0);

  /* carg (+0 + i y) == pi/2 for y > 0.  */
  check_float ("carg (0 + 2.0 i) == pi/2",  FUNC(carg) (BUILD_COMPLEX (0, 2.0)), M_PI_2l, 0, 0, 0);

  /* carg (-0 + i y) == pi/2 for y > 0.  */
  check_float ("carg (-0 + 2.0 i) == pi/2",  FUNC(carg) (BUILD_COMPLEX (minus_zero, 2.0)), M_PI_2l, 0, 0, 0);

  /* carg (+0 + i y) == -pi/2 for y < 0.  */
  check_float ("carg (0 - 2.0 i) == -pi/2",  FUNC(carg) (BUILD_COMPLEX (0, -2.0)), -M_PI_2l, 0, 0, 0);

  /* carg (-0 + i y) == -pi/2 for y < 0.  */
  check_float ("carg (-0 - 2.0 i) == -pi/2",  FUNC(carg) (BUILD_COMPLEX (minus_zero, -2.0)), -M_PI_2l, 0, 0, 0);

  /* carg (inf + i y) == +0 for finite y > 0.  */
  check_float ("carg (inf + 2.0 i) == 0",  FUNC(carg) (BUILD_COMPLEX (plus_infty, 2.0)), 0, 0, 0, 0);

  /* carg (inf + i y) == -0 for finite y < 0.  */
  check_float ("carg (inf - 2.0 i) == -0",  FUNC(carg) (BUILD_COMPLEX (plus_infty, -2.0)), minus_zero, 0, 0, 0);

  /* carg(x + i inf) == pi/2 for finite x.  */
  check_float ("carg (10.0 + inf i) == pi/2",  FUNC(carg) (BUILD_COMPLEX (10.0, plus_infty)), M_PI_2l, 0, 0, 0);

  /* carg(x - i inf) == -pi/2 for finite x.  */
  check_float ("carg (10.0 - inf i) == -pi/2",  FUNC(carg) (BUILD_COMPLEX (10.0, minus_infty)), -M_PI_2l, 0, 0, 0);

  /* carg (-inf + i y) == +pi for finite y > 0.  */
  check_float ("carg (-inf + 10.0 i) == pi",  FUNC(carg) (BUILD_COMPLEX (minus_infty, 10.0)), M_PIl, 0, 0, 0);

  /* carg (-inf + i y) == -pi for finite y < 0.  */
  check_float ("carg (-inf - 10.0 i) == -pi",  FUNC(carg) (BUILD_COMPLEX (minus_infty, -10.0)), -M_PIl, 0, 0, 0);

  check_float ("carg (inf + inf i) == pi/4",  FUNC(carg) (BUILD_COMPLEX (plus_infty, plus_infty)), M_PI_4l, 0, 0, 0);

  check_float ("carg (inf - inf i) == -pi/4",  FUNC(carg) (BUILD_COMPLEX (plus_infty, minus_infty)), -M_PI_4l, 0, 0, 0);

  check_float ("carg (-inf + inf i) == 3 * M_PI_4l",  FUNC(carg) (BUILD_COMPLEX (minus_infty, plus_infty)), 3 * M_PI_4l, 0, 0, 0);

  check_float ("carg (-inf - inf i) == -3 * M_PI_4l",  FUNC(carg) (BUILD_COMPLEX (minus_infty, minus_infty)), -3 * M_PI_4l, 0, 0, 0);

  check_float ("carg (NaN + NaN i) == NaN",  FUNC(carg) (BUILD_COMPLEX (nan_value, nan_value)), nan_value, 0, 0, 0);

  print_max_error ("carg", 0, 0);
}

static void
casin_test (void)
{
  errno = 0;
  FUNC(casin) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("casin (0 + 0 i) == 0.0 + 0.0 i",  FUNC(casin) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("casin (-0 + 0 i) == -0 + 0.0 i",  FUNC(casin) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("casin (0 - 0 i) == 0.0 - 0 i",  FUNC(casin) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("casin (-0 - 0 i) == -0 - 0 i",  FUNC(casin) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("casin (inf + inf i) == pi/4 + inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (M_PI_4l, plus_infty), 0, 0, 0);
  check_complex ("casin (inf - inf i) == pi/4 - inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (M_PI_4l, minus_infty), 0, 0, 0);
  check_complex ("casin (-inf + inf i) == -pi/4 + inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (-M_PI_4l, plus_infty), 0, 0, 0);
  check_complex ("casin (-inf - inf i) == -pi/4 - inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (-M_PI_4l, minus_infty), 0, 0, 0);

  check_complex ("casin (-10.0 + inf i) == -0 + inf i",  FUNC(casin) (BUILD_COMPLEX (-10.0, plus_infty)), BUILD_COMPLEX (minus_zero, plus_infty), 0, 0, 0);
  check_complex ("casin (-10.0 - inf i) == -0 - inf i",  FUNC(casin) (BUILD_COMPLEX (-10.0, minus_infty)), BUILD_COMPLEX (minus_zero, minus_infty), 0, 0, 0);
  check_complex ("casin (0 + inf i) == 0.0 + inf i",  FUNC(casin) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("casin (0 - inf i) == 0.0 - inf i",  FUNC(casin) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);
  check_complex ("casin (-0 + inf i) == -0 + inf i",  FUNC(casin) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (minus_zero, plus_infty), 0, 0, 0);
  check_complex ("casin (-0 - inf i) == -0 - inf i",  FUNC(casin) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (minus_zero, minus_infty), 0, 0, 0);
  check_complex ("casin (0.1 + inf i) == 0.0 + inf i",  FUNC(casin) (BUILD_COMPLEX (0.1L, plus_infty)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("casin (0.1 - inf i) == 0.0 - inf i",  FUNC(casin) (BUILD_COMPLEX (0.1L, minus_infty)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);

  check_complex ("casin (-inf + 0 i) == -pi/2 + inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (-M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("casin (-inf - 0 i) == -pi/2 - inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (-M_PI_2l, minus_infty), 0, 0, 0);
  check_complex ("casin (-inf + 100 i) == -pi/2 + inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, 100)), BUILD_COMPLEX (-M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("casin (-inf - 100 i) == -pi/2 - inf i",  FUNC(casin) (BUILD_COMPLEX (minus_infty, -100)), BUILD_COMPLEX (-M_PI_2l, minus_infty), 0, 0, 0);

  check_complex ("casin (inf + 0 i) == pi/2 + inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("casin (inf - 0 i) == pi/2 - inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (M_PI_2l, minus_infty), 0, 0, 0);
  check_complex ("casin (inf + 0.5 i) == pi/2 + inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, 0.5)), BUILD_COMPLEX (M_PI_2l, plus_infty), 0, 0, 0);
  check_complex ("casin (inf - 0.5 i) == pi/2 - inf i",  FUNC(casin) (BUILD_COMPLEX (plus_infty, -0.5)), BUILD_COMPLEX (M_PI_2l, minus_infty), 0, 0, 0);

  check_complex ("casin (NaN + inf i) == NaN + inf i",  FUNC(casin) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, 0);
  check_complex ("casin (NaN - inf i) == NaN - inf i",  FUNC(casin) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, minus_infty), 0, 0, 0);

  check_complex ("casin (0.0 + NaN i) == 0.0 + NaN i",  FUNC(casin) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, 0);
  check_complex ("casin (-0 + NaN i) == -0 + NaN i",  FUNC(casin) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (minus_zero, nan_value), 0, 0, 0);

  check_complex ("casin (inf + NaN i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(casin) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("casin (-inf + NaN i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(casin) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("casin (NaN + 10.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(casin) (BUILD_COMPLEX (nan_value, 10.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("casin (NaN - 10.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(casin) (BUILD_COMPLEX (nan_value, -10.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("casin (0.75 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(casin) (BUILD_COMPLEX (0.75, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("casin (-0.75 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(casin) (BUILD_COMPLEX (-0.75, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("casin (NaN + NaN i) == NaN + NaN i",  FUNC(casin) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("casin (0.75 + 1.25 i) == 0.453276177638793913448921196101971749 + 1.13239363160530819522266333696834467 i",  FUNC(casin) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.453276177638793913448921196101971749L, 1.13239363160530819522266333696834467L), 0, 0, 0);
  check_complex ("casin (-2 - 3 i) == -0.57065278432109940071028387968566963 - 1.9833870299165354323470769028940395 i",  FUNC(casin) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-0.57065278432109940071028387968566963L, -1.9833870299165354323470769028940395L), 0, 0, 0);

  print_complex_max_error ("casin", 0, 0);
}


static void
casinh_test (void)
{
  errno = 0;
  FUNC(casinh) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("casinh (0 + 0 i) == 0.0 + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("casinh (-0 + 0 i) == -0 + 0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0), 0, 0, 0);
  check_complex ("casinh (0 - 0 i) == 0.0 - 0 i",  FUNC(casinh) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("casinh (-0 - 0 i) == -0 - 0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("casinh (inf + inf i) == inf + pi/4 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_4l), 0, 0, 0);
  check_complex ("casinh (inf - inf i) == inf - pi/4 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_4l), 0, 0, 0);
  check_complex ("casinh (-inf + inf i) == -inf + pi/4 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (minus_infty, M_PI_4l), 0, 0, 0);
  check_complex ("casinh (-inf - inf i) == -inf - pi/4 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (minus_infty, -M_PI_4l), 0, 0, 0);

  check_complex ("casinh (-10.0 + inf i) == -inf + pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (-10.0, plus_infty)), BUILD_COMPLEX (minus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("casinh (-10.0 - inf i) == -inf - pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (-10.0, minus_infty)), BUILD_COMPLEX (minus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("casinh (0 + inf i) == inf + pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("casinh (0 - inf i) == inf - pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("casinh (-0 + inf i) == -inf + pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (minus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("casinh (-0 - inf i) == -inf - pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (minus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("casinh (0.1 + inf i) == inf + pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (0.1L, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("casinh (0.1 - inf i) == inf - pi/2 i",  FUNC(casinh) (BUILD_COMPLEX (0.1L, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);

  check_complex ("casinh (-inf + 0 i) == -inf + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (minus_infty, 0.0), 0, 0, 0);
  check_complex ("casinh (-inf - 0 i) == -inf - 0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (minus_infty, minus_zero), 0, 0, 0);
  check_complex ("casinh (-inf + 100 i) == -inf + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, 100)), BUILD_COMPLEX (minus_infty, 0.0), 0, 0, 0);
  check_complex ("casinh (-inf - 100 i) == -inf - 0 i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, -100)), BUILD_COMPLEX (minus_infty, minus_zero), 0, 0, 0);

  check_complex ("casinh (inf + 0 i) == inf + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("casinh (inf - 0 i) == inf - 0 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("casinh (inf + 0.5 i) == inf + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, 0.5)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("casinh (inf - 0.5 i) == inf - 0 i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, -0.5)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("casinh (inf + NaN i) == inf + NaN i",  FUNC(casinh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("casinh (-inf + NaN i) == -inf + NaN i",  FUNC(casinh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (minus_infty, nan_value), 0, 0, 0);

  check_complex ("casinh (NaN + 0 i) == NaN + 0.0 i",  FUNC(casinh) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, 0);
  check_complex ("casinh (NaN - 0 i) == NaN - 0 i",  FUNC(casinh) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, minus_zero), 0, 0, 0);

  check_complex ("casinh (NaN + inf i) == inf + NaN i plus sign of zero/inf not specified",  FUNC(casinh) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("casinh (NaN - inf i) == inf + NaN i plus sign of zero/inf not specified",  FUNC(casinh) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("casinh (10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(casinh) (BUILD_COMPLEX (10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("casinh (-10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(casinh) (BUILD_COMPLEX (-10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("casinh (NaN + 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(casinh) (BUILD_COMPLEX (nan_value, 0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("casinh (-0.75 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(casinh) (BUILD_COMPLEX (-0.75, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("casinh (NaN + NaN i) == NaN + NaN i",  FUNC(casinh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("casinh (0.75 + 1.25 i) == 1.03171853444778027336364058631006594 + 0.911738290968487636358489564316731207 i",  FUNC(casinh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.03171853444778027336364058631006594L, 0.911738290968487636358489564316731207L), 0, 0, 0);
  check_complex ("casinh (-2 - 3 i) == -1.9686379257930962917886650952454982 - 0.96465850440760279204541105949953237 i",  FUNC(casinh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-1.9686379257930962917886650952454982L, -0.96465850440760279204541105949953237L), 0, 0, 0);

  print_complex_max_error ("casinh", 0, 0);
}


static void
catan_test (void)
{
  errno = 0;
  FUNC(catan) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("catan (0 + 0 i) == 0 + 0 i",  FUNC(catan) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0, 0), 0, 0, 0);
  check_complex ("catan (-0 + 0 i) == -0 + 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0), 0, 0, 0);
  check_complex ("catan (0 - 0 i) == 0 - 0 i",  FUNC(catan) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0, minus_zero), 0, 0, 0);
  check_complex ("catan (-0 - 0 i) == -0 - 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("catan (inf + inf i) == pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (inf - inf i) == pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (-inf + inf i) == -pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (-inf - inf i) == -pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (-M_PI_2l, minus_zero), 0, 0, 0);


  check_complex ("catan (inf - 10.0 i) == pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, -10.0)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (-inf - 10.0 i) == -pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, -10.0)), BUILD_COMPLEX (-M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (inf - 0 i) == pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (-inf - 0 i) == -pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (-M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (inf + 0.0 i) == pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, 0.0)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (-inf + 0.0 i) == -pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, 0.0)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (inf + 0.1 i) == pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (plus_infty, 0.1L)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (-inf + 0.1 i) == -pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_infty, 0.1L)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, 0);

  check_complex ("catan (0.0 - inf i) == pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (-0 - inf i) == -pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (-M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (100.0 - inf i) == pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (100.0, minus_infty)), BUILD_COMPLEX (M_PI_2l, minus_zero), 0, 0, 0);
  check_complex ("catan (-100.0 - inf i) == -pi/2 - 0 i",  FUNC(catan) (BUILD_COMPLEX (-100.0, minus_infty)), BUILD_COMPLEX (-M_PI_2l, minus_zero), 0, 0, 0);

  check_complex ("catan (0.0 + inf i) == pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (-0 + inf i) == -pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (0.5 + inf i) == pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (0.5, plus_infty)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, 0);
  check_complex ("catan (-0.5 + inf i) == -pi/2 + 0 i",  FUNC(catan) (BUILD_COMPLEX (-0.5, plus_infty)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, 0);

  check_complex ("catan (NaN + 0.0 i) == NaN + 0 i",  FUNC(catan) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, 0), 0, 0, 0);
  check_complex ("catan (NaN - 0 i) == NaN - 0 i",  FUNC(catan) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, minus_zero), 0, 0, 0);

  check_complex ("catan (NaN + inf i) == NaN + 0 i",  FUNC(catan) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, 0), 0, 0, 0);
  check_complex ("catan (NaN - inf i) == NaN - 0 i",  FUNC(catan) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, minus_zero), 0, 0, 0);

  check_complex ("catan (0.0 + NaN i) == NaN + NaN i",  FUNC(catan) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);
  check_complex ("catan (-0 + NaN i) == NaN + NaN i",  FUNC(catan) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("catan (inf + NaN i) == pi/2 + 0 i plus sign of zero/inf not specified",  FUNC(catan) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (M_PI_2l, 0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("catan (-inf + NaN i) == -pi/2 + 0 i plus sign of zero/inf not specified",  FUNC(catan) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (-M_PI_2l, 0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("catan (NaN + 10.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(catan) (BUILD_COMPLEX (nan_value, 10.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("catan (NaN - 10.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(catan) (BUILD_COMPLEX (nan_value, -10.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("catan (0.75 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(catan) (BUILD_COMPLEX (0.75, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("catan (-0.75 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(catan) (BUILD_COMPLEX (-0.75, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("catan (NaN + NaN i) == NaN + NaN i",  FUNC(catan) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("catan (0.75 + 1.25 i) == 1.10714871779409050301706546017853704 + 0.549306144334054845697622618461262852 i",  FUNC(catan) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.10714871779409050301706546017853704L, 0.549306144334054845697622618461262852L), 0, 0, 0);
  check_complex ("catan (-2 - 3 i) == -1.4099210495965755225306193844604208 - 0.22907268296853876629588180294200276 i",  FUNC(catan) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-1.4099210495965755225306193844604208L, -0.22907268296853876629588180294200276L), 0, 0, 0);

  print_complex_max_error ("catan", 0, 0);
}

static void
catanh_test (void)
{
  errno = 0;
  FUNC(catanh) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("catanh (0 + 0 i) == 0.0 + 0.0 i",  FUNC(catanh) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("catanh (-0 + 0 i) == -0 + 0.0 i",  FUNC(catanh) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("catanh (0 - 0 i) == 0.0 - 0 i",  FUNC(catanh) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("catanh (-0 - 0 i) == -0 - 0 i",  FUNC(catanh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("catanh (inf + inf i) == 0.0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (inf - inf i) == 0.0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-inf + inf i) == -0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (minus_zero, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-inf - inf i) == -0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (minus_zero, -M_PI_2l), 0, 0, 0);

  check_complex ("catanh (-10.0 + inf i) == -0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (-10.0, plus_infty)), BUILD_COMPLEX (minus_zero, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-10.0 - inf i) == -0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (-10.0, minus_infty)), BUILD_COMPLEX (minus_zero, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-0 + inf i) == -0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (minus_zero, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-0 - inf i) == -0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (minus_zero, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (0 + inf i) == 0.0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (0 - inf i) == 0.0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (0.1 + inf i) == 0.0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (0.1L, plus_infty)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (0.1 - inf i) == 0.0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (0.1L, minus_infty)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);

  check_complex ("catanh (-inf + 0 i) == -0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (minus_zero, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-inf - 0 i) == -0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (minus_zero, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-inf + 100 i) == -0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, 100)), BUILD_COMPLEX (minus_zero, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (-inf - 100 i) == -0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, -100)), BUILD_COMPLEX (minus_zero, -M_PI_2l), 0, 0, 0);

  check_complex ("catanh (inf + 0 i) == 0.0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (inf - 0 i) == 0.0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);
  check_complex ("catanh (inf + 0.5 i) == 0.0 + pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, 0.5)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, 0);
  check_complex ("catanh (inf - 0.5 i) == 0.0 - pi/2 i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, -0.5)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, 0);

  check_complex ("catanh (0 + NaN i) == 0.0 + NaN i",  FUNC(catanh) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, 0);
  check_complex ("catanh (-0 + NaN i) == -0 + NaN i",  FUNC(catanh) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (minus_zero, nan_value), 0, 0, 0);

  check_complex ("catanh (inf + NaN i) == 0.0 + NaN i",  FUNC(catanh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, 0);
  check_complex ("catanh (-inf + NaN i) == -0 + NaN i",  FUNC(catanh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (minus_zero, nan_value), 0, 0, 0);

  check_complex ("catanh (NaN + 0 i) == NaN + NaN i",  FUNC(catanh) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);
  check_complex ("catanh (NaN - 0 i) == NaN + NaN i",  FUNC(catanh) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("catanh (NaN + inf i) == 0.0 + pi/2 i plus sign of zero/inf not specified",  FUNC(catanh) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (0.0, M_PI_2l), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("catanh (NaN - inf i) == 0.0 - pi/2 i plus sign of zero/inf not specified",  FUNC(catanh) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (0.0, -M_PI_2l), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("catanh (10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(catanh) (BUILD_COMPLEX (10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("catanh (-10.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(catanh) (BUILD_COMPLEX (-10.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("catanh (NaN + 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(catanh) (BUILD_COMPLEX (nan_value, 0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("catanh (NaN - 0.75 i) == NaN + NaN i plus invalid exception allowed",  FUNC(catanh) (BUILD_COMPLEX (nan_value, -0.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("catanh (NaN + NaN i) == NaN + NaN i",  FUNC(catanh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("catanh (0.75 + 1.25 i) == 0.261492138795671927078652057366532140 + 0.996825126463918666098902241310446708 i",  FUNC(catanh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.261492138795671927078652057366532140L, 0.996825126463918666098902241310446708L), 0, 0, 0);
  check_complex ("catanh (-2 - 3 i) == -0.14694666622552975204743278515471595 - 1.3389725222944935611241935759091443 i",  FUNC(catanh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-0.14694666622552975204743278515471595L, -1.3389725222944935611241935759091443L), 0, 0, 0);

  print_complex_max_error ("catanh", 0, 0);
}
#endif

static void
cbrt_test (void)
{
  errno = 0;
  FUNC(cbrt) (8);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("cbrt (0.0) == 0.0",  FUNC(cbrt) (0.0), 0.0, 0, 0, 0);
  check_float ("cbrt (-0) == -0",  FUNC(cbrt) (minus_zero), minus_zero, 0, 0, 0);

  check_float ("cbrt (inf) == inf",  FUNC(cbrt) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("cbrt (-inf) == -inf",  FUNC(cbrt) (minus_infty), minus_infty, 0, 0, 0);
  check_float ("cbrt (NaN) == NaN",  FUNC(cbrt) (nan_value), nan_value, 0, 0, 0);

  check_float ("cbrt (-0.001) == -0.1",  FUNC(cbrt) (-0.001L), -0.1L, 0, 0, 0);
  check_float ("cbrt (8) == 2",  FUNC(cbrt) (8), 2, 0, 0, 0);
  check_float ("cbrt (-27.0) == -3.0",  FUNC(cbrt) (-27.0), -3.0, 0, 0, 0);
  check_float ("cbrt (0.9921875) == 0.997389022060725270579075195353955217",  FUNC(cbrt) (0.9921875L), 0.997389022060725270579075195353955217L, 0, 0, 0);
  check_float ("cbrt (0.75) == 0.908560296416069829445605878163630251",  FUNC(cbrt) (0.75L), 0.908560296416069829445605878163630251L, 0, 0, 0);

  print_max_error ("cbrt", 0, 0);
}

#if 0
static void
ccos_test (void)
{
  errno = 0;
  FUNC(ccos) (BUILD_COMPLEX (0, 0));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("ccos (0.0 + 0.0 i) == 1.0 - 0 i",  FUNC(ccos) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);
  check_complex ("ccos (-0 + 0.0 i) == 1.0 + 0.0 i",  FUNC(ccos) (BUILD_COMPLEX (minus_zero, 0.0)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("ccos (0.0 - 0 i) == 1.0 + 0.0 i",  FUNC(ccos) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("ccos (-0 - 0 i) == 1.0 - 0 i",  FUNC(ccos) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);

  check_complex ("ccos (inf + 0.0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccos (inf - 0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccos (-inf + 0.0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccos (-inf - 0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("ccos (0.0 + inf i) == inf - 0 i",  FUNC(ccos) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("ccos (0.0 - inf i) == inf + 0.0 i",  FUNC(ccos) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("ccos (-0 + inf i) == inf + 0.0 i",  FUNC(ccos) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("ccos (-0 - inf i) == inf - 0 i",  FUNC(ccos) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("ccos (inf + inf i) == inf + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (-inf + inf i) == inf + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (inf - inf i) == inf + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (-inf - inf i) == inf + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ccos (4.625 + inf i) == -inf + inf i",  FUNC(ccos) (BUILD_COMPLEX (4.625, plus_infty)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("ccos (4.625 - inf i) == -inf - inf i",  FUNC(ccos) (BUILD_COMPLEX (4.625, minus_infty)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);
  check_complex ("ccos (-4.625 + inf i) == -inf - inf i",  FUNC(ccos) (BUILD_COMPLEX (-4.625, plus_infty)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);
  check_complex ("ccos (-4.625 - inf i) == -inf + inf i",  FUNC(ccos) (BUILD_COMPLEX (-4.625, minus_infty)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);

  check_complex ("ccos (inf + 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, 6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (inf - 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, -6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (-inf + 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, 6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccos (-inf - 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, -6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ccos (NaN + 0.0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ccos (NaN - 0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ccos (NaN + inf i) == inf + NaN i",  FUNC(ccos) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("ccos (NaN - inf i) == inf + NaN i",  FUNC(ccos) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("ccos (NaN + 9.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (nan_value, 9.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccos (NaN - 9.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (nan_value, -9.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccos (0.0 + NaN i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ccos (-0 + NaN i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccos) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ccos (10.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (10.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccos (-10.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (-10.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccos (inf + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccos (-inf + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccos) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccos (NaN + NaN i) == NaN + NaN i",  FUNC(ccos) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("ccos (0.75 + 1.25 i) == 1.38173873063425888530729933139078645 - 1.09193013555397466170919531722024128 i",  FUNC(ccos) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.38173873063425888530729933139078645L, -1.09193013555397466170919531722024128L), 0, 0, 0);
  check_complex ("ccos (-2 - 3 i) == -4.18962569096880723013255501961597373 - 9.10922789375533659797919726277886212 i",  FUNC(ccos) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-4.18962569096880723013255501961597373L, -9.10922789375533659797919726277886212L), 0, 0, 0);

  print_complex_max_error ("ccos", 0, 0);
}


static void
ccosh_test (void)
{
  errno = 0;
  FUNC(ccosh) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("ccosh (0.0 + 0.0 i) == 1.0 + 0.0 i",  FUNC(ccosh) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("ccosh (-0 + 0.0 i) == 1.0 - 0 i",  FUNC(ccosh) (BUILD_COMPLEX (minus_zero, 0.0)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);
  check_complex ("ccosh (0.0 - 0 i) == 1.0 - 0 i",  FUNC(ccosh) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);
  check_complex ("ccosh (-0 - 0 i) == 1.0 + 0.0 i",  FUNC(ccosh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);

  check_complex ("ccosh (0.0 + inf i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccosh (-0 + inf i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccosh (0.0 - inf i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("ccosh (-0 - inf i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("ccosh (inf + 0.0 i) == inf + 0.0 i",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, 0.0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("ccosh (-inf + 0.0 i) == inf - 0 i",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, 0.0)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("ccosh (inf - 0 i) == inf - 0 i",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("ccosh (-inf - 0 i) == inf + 0.0 i",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);

  check_complex ("ccosh (inf + inf i) == inf + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (-inf + inf i) == inf + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (inf - inf i) == inf + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (-inf - inf i) == inf + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ccosh (inf + 4.625 i) == -inf - inf i",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, 4.625)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);
  check_complex ("ccosh (-inf + 4.625 i) == -inf + inf i",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, 4.625)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("ccosh (inf - 4.625 i) == -inf + inf i",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, -4.625)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("ccosh (-inf - 4.625 i) == -inf - inf i",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, -4.625)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);

  check_complex ("ccosh (6.75 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (6.75, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (-6.75 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (-6.75, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (6.75 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (6.75, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ccosh (-6.75 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ccosh) (BUILD_COMPLEX (-6.75, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ccosh (0.0 + NaN i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ccosh (-0 + NaN i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ccosh (inf + NaN i) == inf + NaN i",  FUNC(ccosh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("ccosh (-inf + NaN i) == inf + NaN i",  FUNC(ccosh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("ccosh (9.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (9.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccosh (-9.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (-9.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccosh (NaN + 0.0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ccosh (NaN - 0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ccosh (NaN + 10.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, 10.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccosh (NaN - 10.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, -10.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccosh (NaN + inf i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ccosh (NaN - inf i) == NaN + NaN i plus invalid exception allowed",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ccosh (NaN + NaN i) == NaN + NaN i",  FUNC(ccosh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("ccosh (0.75 + 1.25 i) == 0.408242591877968807788852146397499084 + 0.780365930845853240391326216300863152 i",  FUNC(ccosh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.408242591877968807788852146397499084L, 0.780365930845853240391326216300863152L), 0, 0, 0);

  check_complex ("ccosh (-2 - 3 i) == -3.72454550491532256547397070325597253 + 0.511822569987384608834463849801875634 i",  FUNC(ccosh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-3.72454550491532256547397070325597253L, 0.511822569987384608834463849801875634L), 0, 0, 0);

  print_complex_max_error ("ccosh", 0, 0);
}
#endif

static void
ceil_test (void)
{
  init_max_error ();

  check_float ("ceil (0.0) == 0.0",  FUNC(ceil) (0.0), 0.0, 0, 0, 0);
  check_float ("ceil (-0) == -0",  FUNC(ceil) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("ceil (inf) == inf",  FUNC(ceil) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("ceil (-inf) == -inf",  FUNC(ceil) (minus_infty), minus_infty, 0, 0, 0);
  check_float ("ceil (NaN) == NaN",  FUNC(ceil) (nan_value), nan_value, 0, 0, 0);

  check_float ("ceil (pi) == 4.0",  FUNC(ceil) (M_PIl), 4.0, 0, 0, 0);
  check_float ("ceil (-pi) == -3.0",  FUNC(ceil) (-M_PIl), -3.0, 0, 0, 0);
  check_float ("ceil (0.1) == 1.0",  FUNC(ceil) (0.1), 1.0, 0, 0, 0);
  check_float ("ceil (0.25) == 1.0",  FUNC(ceil) (0.25), 1.0, 0, 0, 0);
  check_float ("ceil (0.625) == 1.0",  FUNC(ceil) (0.625), 1.0, 0, 0, 0);
  check_float ("ceil (-0.1) == -0",  FUNC(ceil) (-0.1), minus_zero, 0, 0, 0);
  check_float ("ceil (-0.25) == -0",  FUNC(ceil) (-0.25), minus_zero, 0, 0, 0);
  check_float ("ceil (-0.625) == -0",  FUNC(ceil) (-0.625), minus_zero, 0, 0, 0);

#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
  check_float ("ceil (4503599627370495.5) == 4503599627370496.0",  FUNC(ceil) (4503599627370495.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("ceil (4503599627370496.25) == 4503599627370497.0",  FUNC(ceil) (4503599627370496.25L), 4503599627370497.0L, 0, 0, 0);
  check_float ("ceil (4503599627370496.5) == 4503599627370497.0",  FUNC(ceil) (4503599627370496.5L), 4503599627370497.0L, 0, 0, 0);
  check_float ("ceil (4503599627370496.75) == 4503599627370497.0",  FUNC(ceil) (4503599627370496.75L), 4503599627370497.0L, 0, 0, 0);
  check_float ("ceil (4503599627370497.5) == 4503599627370498.0",  FUNC(ceil) (4503599627370497.5L), 4503599627370498.0L, 0, 0, 0);

  check_float ("ceil (-4503599627370495.5) == -4503599627370495.0",  FUNC(ceil) (-4503599627370495.5L), -4503599627370495.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370496.25) == -4503599627370496.0",  FUNC(ceil) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370496.5) == -4503599627370496.0",  FUNC(ceil) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370496.75) == -4503599627370496.0",  FUNC(ceil) (-4503599627370496.75L), -4503599627370496.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370497.5) == -4503599627370497.0",  FUNC(ceil) (-4503599627370497.5L), -4503599627370497.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("ceil (4503599627370494.5000000000001) == 4503599627370495.0",  FUNC(ceil) (4503599627370494.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("ceil (4503599627370495.5000000000001) == 4503599627370496.0",  FUNC(ceil) (4503599627370495.5000000000001L), 4503599627370496.0L, 0, 0, 0);
  check_float ("ceil (4503599627370496.5000000000001) == 4503599627370497.0",  FUNC(ceil) (4503599627370496.5000000000001L), 4503599627370497.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370494.5000000000001) == -4503599627370494.0",  FUNC(ceil) (-4503599627370494.5000000000001L), -4503599627370494.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370495.5000000000001) == -4503599627370495.0",  FUNC(ceil) (-4503599627370495.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("ceil (-4503599627370496.5000000000001) == -4503599627370496.0",  FUNC(ceil) (-4503599627370496.5000000000001L), -4503599627370496.0L, 0, 0, 0);
# endif

  check_float ("ceil (9007199254740991.5) == 9007199254740992.0",  FUNC(ceil) (9007199254740991.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (9007199254740992.25) == 9007199254740993.0",  FUNC(ceil) (9007199254740992.25L), 9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (9007199254740992.5) == 9007199254740993.0",  FUNC(ceil) (9007199254740992.5L), 9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (9007199254740992.75) == 9007199254740993.0",  FUNC(ceil) (9007199254740992.75L), 9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (9007199254740993.5) == 9007199254740994.0",  FUNC(ceil) (9007199254740993.5L), 9007199254740994.0L, 0, 0, 0);

  check_float ("ceil (-9007199254740991.5) == -9007199254740991.0",  FUNC(ceil) (-9007199254740991.5L), -9007199254740991.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740992.25) == -9007199254740992.0",  FUNC(ceil) (-9007199254740992.25L), -9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740992.5) == -9007199254740992.0",  FUNC(ceil) (-9007199254740992.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740992.75) == -9007199254740992.0",  FUNC(ceil) (-9007199254740992.75L), -9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740993.5) == -9007199254740993.0",  FUNC(ceil) (-9007199254740993.5L), -9007199254740993.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("ceil (9007199254740991.0000000000001) == 9007199254740992.0",  FUNC(ceil) (9007199254740991.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (9007199254740992.0000000000001) == 9007199254740993.0",  FUNC(ceil) (9007199254740992.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (9007199254740993.0000000000001) == 9007199254740994.0",  FUNC(ceil) (9007199254740993.0000000000001L), 9007199254740994.0L, 0, 0, 0);
  check_float ("ceil (9007199254740991.5000000000001) == 9007199254740992.0",  FUNC(ceil) (9007199254740991.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (9007199254740992.5000000000001) == 9007199254740993.0",  FUNC(ceil) (9007199254740992.5000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (9007199254740993.5000000000001) == 9007199254740994.0",  FUNC(ceil) (9007199254740993.5000000000001L), 9007199254740994.0L, 0, 0, 0);

  check_float ("ceil (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(ceil) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(ceil) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(ceil) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740991.5000000000001) == -9007199254740991.0",  FUNC(ceil) (-9007199254740991.5000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740992.5000000000001) == -9007199254740992.0",  FUNC(ceil) (-9007199254740992.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("ceil (-9007199254740993.5000000000001) == -9007199254740993.0",  FUNC(ceil) (-9007199254740993.5000000000001L), -9007199254740993.0L, 0, 0, 0);
# endif

  check_float ("ceil (72057594037927935.5) == 72057594037927936.0",  FUNC(ceil) (72057594037927935.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("ceil (72057594037927936.25) == 72057594037927937.0",  FUNC(ceil) (72057594037927936.25L), 72057594037927937.0L, 0, 0, 0);
  check_float ("ceil (72057594037927936.5) == 72057594037927937.0",  FUNC(ceil) (72057594037927936.5L), 72057594037927937.0L, 0, 0, 0);
  check_float ("ceil (72057594037927936.75) == 72057594037927937.0",  FUNC(ceil) (72057594037927936.75L), 72057594037927937.0L, 0, 0, 0);
  check_float ("ceil (72057594037927937.5) == 72057594037927938.0",  FUNC(ceil) (72057594037927937.5L), 72057594037927938.0L, 0, 0, 0);

  check_float ("ceil (-72057594037927935.5) == -72057594037927935.0",  FUNC(ceil) (-72057594037927935.5L), -72057594037927935.0L, 0, 0, 0);
  check_float ("ceil (-72057594037927936.25) == -72057594037927936.0",  FUNC(ceil) (-72057594037927936.25L), -72057594037927936.0L, 0, 0, 0);
  check_float ("ceil (-72057594037927936.5) == -72057594037927936.0",  FUNC(ceil) (-72057594037927936.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("ceil (-72057594037927936.75) == -72057594037927936.0",  FUNC(ceil) (-72057594037927936.75L), -72057594037927936.0L, 0, 0, 0);
  check_float ("ceil (-72057594037927937.5) == -72057594037927937.0",  FUNC(ceil) (-72057594037927937.5L), -72057594037927937.0L, 0, 0, 0);

  check_float ("ceil (10141204801825835211973625643007.5) == 10141204801825835211973625643008.0",  FUNC(ceil) (10141204801825835211973625643007.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("ceil (10141204801825835211973625643008.25) == 10141204801825835211973625643009.0",  FUNC(ceil) (10141204801825835211973625643008.25L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("ceil (10141204801825835211973625643008.5) == 10141204801825835211973625643009.0",  FUNC(ceil) (10141204801825835211973625643008.5L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("ceil (10141204801825835211973625643008.75) == 10141204801825835211973625643009.0",  FUNC(ceil) (10141204801825835211973625643008.75L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("ceil (10141204801825835211973625643009.5) == 10141204801825835211973625643010.0",  FUNC(ceil) (10141204801825835211973625643009.5L), 10141204801825835211973625643010.0L, 0, 0, 0);
#endif

  print_max_error ("ceil", 0, 0);
}

#if 0
static void
cexp_test (void)
{
  errno = 0;
  FUNC(cexp) (BUILD_COMPLEX (0, 0));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("cexp (+0 + +0 i) == 1 + 0.0 i",  FUNC(cexp) (BUILD_COMPLEX (plus_zero, plus_zero)), BUILD_COMPLEX (1, 0.0), 0, 0, 0);
  check_complex ("cexp (-0 + +0 i) == 1 + 0.0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_zero, plus_zero)), BUILD_COMPLEX (1, 0.0), 0, 0, 0);
  check_complex ("cexp (+0 - 0 i) == 1 - 0 i",  FUNC(cexp) (BUILD_COMPLEX (plus_zero, minus_zero)), BUILD_COMPLEX (1, minus_zero), 0, 0, 0);
  check_complex ("cexp (-0 - 0 i) == 1 - 0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (1, minus_zero), 0, 0, 0);

  check_complex ("cexp (inf + +0 i) == inf + 0.0 i",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, plus_zero)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("cexp (inf - 0 i) == inf - 0 i",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("cexp (-inf + +0 i) == 0.0 + 0.0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, plus_zero)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("cexp (-inf - 0 i) == 0.0 - 0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);

  check_complex ("cexp (0.0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("cexp (-0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("cexp (0.0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("cexp (-0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("cexp (100.0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (100.0, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("cexp (-100.0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (-100.0, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("cexp (100.0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (100.0, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("cexp (-100.0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(cexp) (BUILD_COMPLEX (-100.0, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("cexp (-inf + 2.0 i) == -0 + 0.0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, 2.0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("cexp (-inf + 4.0 i) == -0 - 0 i",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, 4.0)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);
  check_complex ("cexp (inf + 2.0 i) == -inf + inf i",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, 2.0)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("cexp (inf + 4.0 i) == -inf - inf i",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, 4.0)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);

  check_complex ("cexp (inf + inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("cexp (inf - inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("cexp (-inf + inf i) == 0.0 + 0.0 i plus sign of zero/inf not specified",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (0.0, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("cexp (-inf - inf i) == 0.0 - 0 i plus sign of zero/inf not specified",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("cexp (-inf + NaN i) == 0 + 0 i plus sign of zero/inf not specified",  FUNC(cexp) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (0, 0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("cexp (inf + NaN i) == inf + NaN i",  FUNC(cexp) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("cexp (NaN + 0.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cexp) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cexp (NaN + 1.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(cexp) (BUILD_COMPLEX (nan_value, 1.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("cexp (NaN + inf i) == NaN + NaN i plus invalid exception allowed",  FUNC(cexp) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cexp (0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cexp) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cexp (1 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(cexp) (BUILD_COMPLEX (1, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("cexp (NaN + NaN i) == NaN + NaN i",  FUNC(cexp) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cexp (0.75 + 1.25 i) == 0.667537446429131586942201977015932112 + 2.00900045494094876258347228145863909 i",  FUNC(cexp) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.667537446429131586942201977015932112L, 2.00900045494094876258347228145863909L), 0, 0, 0);
  check_complex ("cexp (-2.0 - 3.0 i) == -0.13398091492954261346140525546115575 - 0.019098516261135196432576240858800925 i",  FUNC(cexp) (BUILD_COMPLEX (-2.0, -3.0)), BUILD_COMPLEX (-0.13398091492954261346140525546115575L, -0.019098516261135196432576240858800925L), 0, 0, 0);

  print_complex_max_error ("cexp", 0, 0);
}


static void
cimag_test (void)
{
  init_max_error ();
  check_float ("cimag (1.0 + 0.0 i) == 0.0",  FUNC(cimag) (BUILD_COMPLEX (1.0, 0.0)), 0.0, 0, 0, 0);
  check_float ("cimag (1.0 - 0 i) == -0",  FUNC(cimag) (BUILD_COMPLEX (1.0, minus_zero)), minus_zero, 0, 0, 0);
  check_float ("cimag (1.0 + NaN i) == NaN",  FUNC(cimag) (BUILD_COMPLEX (1.0, nan_value)), nan_value, 0, 0, 0);
  check_float ("cimag (NaN + NaN i) == NaN",  FUNC(cimag) (BUILD_COMPLEX (nan_value, nan_value)), nan_value, 0, 0, 0);
  check_float ("cimag (1.0 + inf i) == inf",  FUNC(cimag) (BUILD_COMPLEX (1.0, plus_infty)), plus_infty, 0, 0, 0);
  check_float ("cimag (1.0 - inf i) == -inf",  FUNC(cimag) (BUILD_COMPLEX (1.0, minus_infty)), minus_infty, 0, 0, 0);
  check_float ("cimag (2.0 + 3.0 i) == 3.0",  FUNC(cimag) (BUILD_COMPLEX (2.0, 3.0)), 3.0, 0, 0, 0);

  print_max_error ("cimag", 0, 0);
}

static void
clog_test (void)
{
  errno = 0;
  FUNC(clog) (BUILD_COMPLEX (-2, -3));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("clog (-0 + 0 i) == -inf + pi i plus division by zero exception",  FUNC(clog) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_infty, M_PIl), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_complex ("clog (-0 - 0 i) == -inf - pi i plus division by zero exception",  FUNC(clog) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_infty, -M_PIl), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_complex ("clog (0 + 0 i) == -inf + 0.0 i plus division by zero exception",  FUNC(clog) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (minus_infty, 0.0), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_complex ("clog (0 - 0 i) == -inf - 0 i plus division by zero exception",  FUNC(clog) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (minus_infty, minus_zero), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_complex ("clog (-inf + inf i) == inf + 3/4 pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_34l), 0, 0, 0);
  check_complex ("clog (-inf - inf i) == inf - 3/4 pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_34l), 0, 0, 0);

  check_complex ("clog (inf + inf i) == inf + pi/4 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_4l), 0, 0, 0);
  check_complex ("clog (inf - inf i) == inf - pi/4 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_4l), 0, 0, 0);

  check_complex ("clog (0 + inf i) == inf + pi/2 i",  FUNC(clog) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("clog (3 + inf i) == inf + pi/2 i",  FUNC(clog) (BUILD_COMPLEX (3, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("clog (-0 + inf i) == inf + pi/2 i",  FUNC(clog) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("clog (-3 + inf i) == inf + pi/2 i",  FUNC(clog) (BUILD_COMPLEX (-3, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_2l), 0, 0, 0);
  check_complex ("clog (0 - inf i) == inf - pi/2 i",  FUNC(clog) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("clog (3 - inf i) == inf - pi/2 i",  FUNC(clog) (BUILD_COMPLEX (3, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("clog (-0 - inf i) == inf - pi/2 i",  FUNC(clog) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);
  check_complex ("clog (-3 - inf i) == inf - pi/2 i",  FUNC(clog) (BUILD_COMPLEX (-3, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI_2l), 0, 0, 0);

  check_complex ("clog (-inf + 0 i) == inf + pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (plus_infty, M_PIl), 0, 0, 0);
  check_complex ("clog (-inf + 1 i) == inf + pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, 1)), BUILD_COMPLEX (plus_infty, M_PIl), 0, 0, 0);
  check_complex ("clog (-inf - 0 i) == inf - pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, -M_PIl), 0, 0, 0);
  check_complex ("clog (-inf - 1 i) == inf - pi i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, -1)), BUILD_COMPLEX (plus_infty, -M_PIl), 0, 0, 0);

  check_complex ("clog (inf + 0 i) == inf + 0.0 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("clog (inf + 1 i) == inf + 0.0 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, 1)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("clog (inf - 0 i) == inf - 0 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("clog (inf - 1 i) == inf - 0 i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, -1)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("clog (inf + NaN i) == inf + NaN i",  FUNC(clog) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("clog (-inf + NaN i) == inf + NaN i",  FUNC(clog) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("clog (NaN + inf i) == inf + NaN i",  FUNC(clog) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("clog (NaN - inf i) == inf + NaN i",  FUNC(clog) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("clog (0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (3 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (3, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (-0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (-3 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (-3, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("clog (NaN + 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (NaN + 5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (nan_value, 5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (NaN - 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog (NaN - 5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog) (BUILD_COMPLEX (nan_value, -5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("clog (NaN + NaN i) == NaN + NaN i",  FUNC(clog) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("clog (0.75 + 1.25 i) == 0.376885901188190075998919126749298416 + 1.03037682652431246378774332703115153 i",  FUNC(clog) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.376885901188190075998919126749298416L, 1.03037682652431246378774332703115153L), 0, 0, 0);
  check_complex ("clog (-2 - 3 i) == 1.2824746787307683680267437207826593 - 2.1587989303424641704769327722648368 i",  FUNC(clog) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (1.2824746787307683680267437207826593L, -2.1587989303424641704769327722648368L), 0, 0, 0);

  print_complex_max_error ("clog", 0, 0);
}


static void
clog10_test (void)
{
  errno = 0;
  FUNC(clog10) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("clog10 (-0 + 0 i) == -inf + pi i plus division by zero exception",  FUNC(clog10) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_infty, M_PIl), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_complex ("clog10 (-0 - 0 i) == -inf - pi i plus division by zero exception",  FUNC(clog10) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_infty, -M_PIl), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_complex ("clog10 (0 + 0 i) == -inf + 0.0 i plus division by zero exception",  FUNC(clog10) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (minus_infty, 0.0), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_complex ("clog10 (0 - 0 i) == -inf - 0 i plus division by zero exception",  FUNC(clog10) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (minus_infty, minus_zero), 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_complex ("clog10 (-inf + inf i) == inf + 3/4 pi*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI_34_LOG10El), 0, 0, 0);

  check_complex ("clog10 (inf + inf i) == inf + pi/4*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI4_LOG10El), 0, 0, 0);
  check_complex ("clog10 (inf - inf i) == inf - pi/4*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI4_LOG10El), 0, 0, 0);

  check_complex ("clog10 (0 + inf i) == inf + pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (3 + inf i) == inf + pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (3, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-0 + inf i) == inf + pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-3 + inf i) == inf + pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (-3, plus_infty)), BUILD_COMPLEX (plus_infty, M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (0 - inf i) == inf - pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (3 - inf i) == inf - pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (3, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-0 - inf i) == inf - pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI2_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-3 - inf i) == inf - pi/2*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (-3, minus_infty)), BUILD_COMPLEX (plus_infty, -M_PI2_LOG10El), 0, 0, 0);

  check_complex ("clog10 (-inf + 0 i) == inf + pi*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (plus_infty, M_PI_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-inf + 1 i) == inf + pi*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, 1)), BUILD_COMPLEX (plus_infty, M_PI_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-inf - 0 i) == inf - pi*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, -M_PI_LOG10El), 0, 0, 0);
  check_complex ("clog10 (-inf - 1 i) == inf - pi*log10(e) i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, -1)), BUILD_COMPLEX (plus_infty, -M_PI_LOG10El), 0, 0, 0);

  check_complex ("clog10 (inf + 0 i) == inf + 0.0 i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("clog10 (inf + 1 i) == inf + 0.0 i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, 1)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("clog10 (inf - 0 i) == inf - 0 i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("clog10 (inf - 1 i) == inf - 0 i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, -1)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("clog10 (inf + NaN i) == inf + NaN i",  FUNC(clog10) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("clog10 (-inf + NaN i) == inf + NaN i",  FUNC(clog10) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("clog10 (NaN + inf i) == inf + NaN i",  FUNC(clog10) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);
  check_complex ("clog10 (NaN - inf i) == inf + NaN i",  FUNC(clog10) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("clog10 (0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (3 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (3, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (-0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (-3 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (-3, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("clog10 (NaN + 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (NaN + 5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (nan_value, 5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (NaN - 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("clog10 (NaN - 5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(clog10) (BUILD_COMPLEX (nan_value, -5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("clog10 (NaN + NaN i) == NaN + NaN i",  FUNC(clog10) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("clog10 (0.75 + 1.25 i) == 0.163679467193165171449476605077428975 + 0.447486970040493067069984724340855636 i",  FUNC(clog10) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.163679467193165171449476605077428975L, 0.447486970040493067069984724340855636L), 0, 0, 0);
  check_complex ("clog10 (-2 - 3 i) == 0.556971676153418384603252578971164214 - 0.937554462986374708541507952140189646 i",  FUNC(clog10) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (0.556971676153418384603252578971164214L, -0.937554462986374708541507952140189646L), 0, 0, 0);

  print_complex_max_error ("clog10", 0, 0);
}


static void
conj_test (void)
{
  init_max_error ();
  check_complex ("conj (0.0 + 0.0 i) == 0.0 - 0 i",  FUNC(conj) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("conj (0.0 - 0 i) == 0.0 + 0.0 i",  FUNC(conj) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("conj (NaN + NaN i) == NaN + NaN i",  FUNC(conj) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);
  check_complex ("conj (inf - inf i) == inf + inf i",  FUNC(conj) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("conj (inf + inf i) == inf - inf i",  FUNC(conj) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("conj (1.0 + 2.0 i) == 1.0 - 2.0 i",  FUNC(conj) (BUILD_COMPLEX (1.0, 2.0)), BUILD_COMPLEX (1.0, -2.0), 0, 0, 0);
  check_complex ("conj (3.0 - 4.0 i) == 3.0 + 4.0 i",  FUNC(conj) (BUILD_COMPLEX (3.0, -4.0)), BUILD_COMPLEX (3.0, 4.0), 0, 0, 0);

  print_complex_max_error ("conj", 0, 0);
}
#endif

static void
copysign_test (void)
{
  init_max_error ();

  check_float ("copysign (0, 4) == 0",  FUNC(copysign) (0, 4), 0, 0, 0, 0);
  check_float ("copysign (0, -4) == -0",  FUNC(copysign) (0, -4), minus_zero, 0, 0, 0);
  check_float ("copysign (-0, 4) == 0",  FUNC(copysign) (minus_zero, 4), 0, 0, 0, 0);
  check_float ("copysign (-0, -4) == -0",  FUNC(copysign) (minus_zero, -4), minus_zero, 0, 0, 0);

  check_float ("copysign (inf, 0) == inf",  FUNC(copysign) (plus_infty, 0), plus_infty, 0, 0, 0);
  check_float ("copysign (inf, -0) == -inf",  FUNC(copysign) (plus_infty, minus_zero), minus_infty, 0, 0, 0);
  check_float ("copysign (-inf, 0) == inf",  FUNC(copysign) (minus_infty, 0), plus_infty, 0, 0, 0);
  check_float ("copysign (-inf, -0) == -inf",  FUNC(copysign) (minus_infty, minus_zero), minus_infty, 0, 0, 0);

  check_float ("copysign (0, inf) == 0",  FUNC(copysign) (0, plus_infty), 0, 0, 0, 0);
  check_float ("copysign (0, -0) == -0",  FUNC(copysign) (0, minus_zero), minus_zero, 0, 0, 0);
  check_float ("copysign (-0, inf) == 0",  FUNC(copysign) (minus_zero, plus_infty), 0, 0, 0, 0);
  check_float ("copysign (-0, -0) == -0",  FUNC(copysign) (minus_zero, minus_zero), minus_zero, 0, 0, 0);

  /* XXX More correctly we would have to check the sign of the NaN.  */
  check_float ("copysign (NaN, 0) == NaN",  FUNC(copysign) (nan_value, 0), nan_value, 0, 0, 0);
  check_float ("copysign (NaN, -0) == NaN",  FUNC(copysign) (nan_value, minus_zero), nan_value, 0, 0, 0);
  check_float ("copysign (-NaN, 0) == NaN",  FUNC(copysign) (-nan_value, 0), nan_value, 0, 0, 0);
  check_float ("copysign (-NaN, -0) == NaN",  FUNC(copysign) (-nan_value, minus_zero), nan_value, 0, 0, 0);

  print_max_error ("copysign", 0, 0);
}


static void
cos_test (void)
{
  errno = 0;
  FUNC(cos) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("cos (0) == 1",  FUNC(cos) (0), 1, 0, 0, 0);
  check_float ("cos (-0) == 1",  FUNC(cos) (minus_zero), 1, 0, 0, 0);
  errno = 0;
  check_float ("cos (inf) == NaN plus invalid exception",  FUNC(cos) (plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for cos(+inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("cos (-inf) == NaN plus invalid exception",  FUNC(cos) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for cos(-inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("cos (NaN) == NaN",  FUNC(cos) (nan_value), nan_value, 0, 0, 0);
  check_int ("errno for cos(NaN) unchanged", errno, 0, 0, 0, 0);

  check_float ("cos (M_PI_6l * 2.0) == 0.5",  FUNC(cos) (M_PI_6l * 2.0), 0.5, 0, 0, 0);
  check_float ("cos (M_PI_6l * 4.0) == -0.5",  FUNC(cos) (M_PI_6l * 4.0), -0.5, 0, 0, 0);
  check_float ("cos (pi/2) == 0",  FUNC(cos) (M_PI_2l), 0, 0, 0, 0);

  check_float ("cos (0.75) == 0.731688868873820886311838753000084544",  FUNC(cos) (0.75L), 0.731688868873820886311838753000084544L, 0, 0, 0);

#ifdef TEST_DOUBLE
  check_float ("cos (0.80190127184058835) == 0.69534156199418473",  FUNC(cos) (0.80190127184058835), 0.69534156199418473, 0, 0, 0);
#endif

  print_max_error ("cos", 0, 0);
}


static void
cosh_test (void)
{
  errno = 0;
  FUNC(cosh) (0.7L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();
  check_float ("cosh (0) == 1",  FUNC(cosh) (0), 1, 0, 0, 0);
  check_float ("cosh (-0) == 1",  FUNC(cosh) (minus_zero), 1, 0, 0, 0);

#ifndef TEST_INLINE
  check_float ("cosh (inf) == inf",  FUNC(cosh) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("cosh (-inf) == inf",  FUNC(cosh) (minus_infty), plus_infty, 0, 0, 0);
#endif
  check_float ("cosh (NaN) == NaN",  FUNC(cosh) (nan_value), nan_value, 0, 0, 0);

  check_float ("cosh (0.75) == 1.29468328467684468784170818539018176",  FUNC(cosh) (0.75L), 1.29468328467684468784170818539018176L, 0, 0, 0);

  print_max_error ("cosh", 0, 0);
}

#if 0
static void
cpow_test (void)
{
  errno = 0;
  FUNC(cpow) (BUILD_COMPLEX (1, 0), BUILD_COMPLEX (0, 0));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("cpow (1 + 0 i, 0 + 0 i) == 1.0 + 0.0 i",  FUNC(cpow) (BUILD_COMPLEX (1, 0), BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("cpow (2 + 0 i, 10 + 0 i) == 1024.0 + 0.0 i",  FUNC(cpow) (BUILD_COMPLEX (2, 0), BUILD_COMPLEX (10, 0)), BUILD_COMPLEX (1024.0, 0.0), 0, 0, 0);

  check_complex ("cpow (e + 0 i, 0 + 2 * M_PIl i) == 1.0 + 0.0 i",  FUNC(cpow) (BUILD_COMPLEX (M_El, 0), BUILD_COMPLEX (0, 2 * M_PIl)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("cpow (2 + 3 i, 4 + 0 i) == -119.0 - 120.0 i",  FUNC(cpow) (BUILD_COMPLEX (2, 3), BUILD_COMPLEX (4, 0)), BUILD_COMPLEX (-119.0, -120.0), 0, 0, 0);

  check_complex ("cpow (NaN + NaN i, NaN + NaN i) == NaN + NaN i",  FUNC(cpow) (BUILD_COMPLEX (nan_value, nan_value), BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cpow (0.75 + 1.25 i, 0.75 + 1.25 i) == 0.117506293914473555420279832210420483 + 0.346552747708338676483025352060418001 i",  FUNC(cpow) (BUILD_COMPLEX (0.75L, 1.25L), BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.117506293914473555420279832210420483L, 0.346552747708338676483025352060418001L), 0, 0, 0);
  check_complex ("cpow (0.75 + 1.25 i, 1.0 + 1.0 i) == 0.0846958290317209430433805274189191353 + 0.513285749182902449043287190519090481 i",  FUNC(cpow) (BUILD_COMPLEX (0.75L, 1.25L), BUILD_COMPLEX (1.0L, 1.0L)), BUILD_COMPLEX (0.0846958290317209430433805274189191353L, 0.513285749182902449043287190519090481L), 0, 0, 0);
  check_complex ("cpow (0.75 + 1.25 i, 1.0 + 0.0 i) == 0.75 + 1.25 i",  FUNC(cpow) (BUILD_COMPLEX (0.75L, 1.25L), BUILD_COMPLEX (1.0L, 0.0L)), BUILD_COMPLEX (0.75L, 1.25L), 0, 0, 0);
  check_complex ("cpow (0.75 + 1.25 i, 0.0 + 1.0 i) == 0.331825439177608832276067945276730566 + 0.131338600281188544930936345230903032 i",  FUNC(cpow) (BUILD_COMPLEX (0.75L, 1.25L), BUILD_COMPLEX (0.0L, 1.0L)), BUILD_COMPLEX (0.331825439177608832276067945276730566L, 0.131338600281188544930936345230903032L), 0, 0, 0);

  print_complex_max_error ("cpow", 0, 0);
}


static void
cproj_test (void)
{
  init_max_error ();
  check_complex ("cproj (0.0 + 0.0 i) == 0.0 + 0.0 i",  FUNC(cproj) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("cproj (-0 - 0 i) == -0 - 0 i",  FUNC(cproj) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);
  check_complex ("cproj (0.0 - 0 i) == 0.0 - 0 i",  FUNC(cproj) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("cproj (-0 + 0.0 i) == -0 + 0.0 i",  FUNC(cproj) (BUILD_COMPLEX (minus_zero, 0.0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);

  check_complex ("cproj (NaN + NaN i) == NaN + NaN i",  FUNC(cproj) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("cproj (inf + inf i) == inf + 0.0 i",  FUNC(cproj) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("cproj (inf - inf i) == inf - 0 i",  FUNC(cproj) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("cproj (-inf + inf i) == inf + 0.0 i",  FUNC(cproj) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("cproj (-inf - inf i) == inf - 0 i",  FUNC(cproj) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("cproj (1.0 + 0.0 i) == 1.0 + 0.0 i",  FUNC(cproj) (BUILD_COMPLEX (1.0, 0.0)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("cproj (2.0 + 3.0 i) == 2.0 + 3.0 i",  FUNC(cproj) (BUILD_COMPLEX (2.0, 3.0)), BUILD_COMPLEX (2.0, 3.0), 0, 0, 0);

  print_complex_max_error ("cproj", 0, 0);
}


static void
creal_test (void)
{
  init_max_error ();
  check_float ("creal (0.0 + 1.0 i) == 0.0",  FUNC(creal) (BUILD_COMPLEX (0.0, 1.0)), 0.0, 0, 0, 0);
  check_float ("creal (-0 + 1.0 i) == -0",  FUNC(creal) (BUILD_COMPLEX (minus_zero, 1.0)), minus_zero, 0, 0, 0);
  check_float ("creal (NaN + 1.0 i) == NaN",  FUNC(creal) (BUILD_COMPLEX (nan_value, 1.0)), nan_value, 0, 0, 0);
  check_float ("creal (NaN + NaN i) == NaN",  FUNC(creal) (BUILD_COMPLEX (nan_value, nan_value)), nan_value, 0, 0, 0);
  check_float ("creal (inf + 1.0 i) == inf",  FUNC(creal) (BUILD_COMPLEX (plus_infty, 1.0)), plus_infty, 0, 0, 0);
  check_float ("creal (-inf + 1.0 i) == -inf",  FUNC(creal) (BUILD_COMPLEX (minus_infty, 1.0)), minus_infty, 0, 0, 0);
  check_float ("creal (2.0 + 3.0 i) == 2.0",  FUNC(creal) (BUILD_COMPLEX (2.0, 3.0)), 2.0, 0, 0, 0);

  print_max_error ("creal", 0, 0);
}

static void
csin_test (void)
{
  errno = 0;
  FUNC(csin) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("csin (0.0 + 0.0 i) == 0.0 + 0.0 i",  FUNC(csin) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("csin (-0 + 0.0 i) == -0 + 0.0 i",  FUNC(csin) (BUILD_COMPLEX (minus_zero, 0.0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("csin (0.0 - 0 i) == 0 - 0 i",  FUNC(csin) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (0, minus_zero), 0, 0, 0);
  check_complex ("csin (-0 - 0 i) == -0 - 0 i",  FUNC(csin) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("csin (0.0 + inf i) == 0.0 + inf i",  FUNC(csin) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("csin (-0 + inf i) == -0 + inf i",  FUNC(csin) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (minus_zero, plus_infty), 0, 0, 0);
  check_complex ("csin (0.0 - inf i) == 0.0 - inf i",  FUNC(csin) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);
  check_complex ("csin (-0 - inf i) == -0 - inf i",  FUNC(csin) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (minus_zero, minus_infty), 0, 0, 0);

  check_complex ("csin (inf + 0.0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (plus_infty, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (-inf + 0.0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (minus_infty, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (inf - 0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (-inf - 0 i) == NaN + 0.0 i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("csin (inf + inf i) == NaN + inf i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (-inf + inf i) == NaN + inf i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (inf - inf i) == NaN + inf i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (-inf - inf i) == NaN + inf i plus invalid exception and sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("csin (inf + 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(csin) (BUILD_COMPLEX (plus_infty, 6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csin (inf - 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(csin) (BUILD_COMPLEX (plus_infty, -6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csin (-inf + 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(csin) (BUILD_COMPLEX (minus_infty, 6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csin (-inf - 6.75 i) == NaN + NaN i plus invalid exception",  FUNC(csin) (BUILD_COMPLEX (minus_infty, -6.75)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("csin (4.625 + inf i) == -inf - inf i",  FUNC(csin) (BUILD_COMPLEX (4.625, plus_infty)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);
  check_complex ("csin (4.625 - inf i) == -inf + inf i",  FUNC(csin) (BUILD_COMPLEX (4.625, minus_infty)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("csin (-4.625 + inf i) == inf - inf i",  FUNC(csin) (BUILD_COMPLEX (-4.625, plus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csin (-4.625 - inf i) == inf + inf i",  FUNC(csin) (BUILD_COMPLEX (-4.625, minus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);

  check_complex ("csin (NaN + 0.0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (NaN - 0 i) == NaN + 0.0 i plus sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("csin (NaN + inf i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("csin (NaN - inf i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(csin) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("csin (NaN + 9.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (nan_value, 9.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csin (NaN - 9.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (nan_value, -9.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csin (0.0 + NaN i) == 0.0 + NaN i",  FUNC(csin) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, 0);
  check_complex ("csin (-0 + NaN i) == -0 + NaN i",  FUNC(csin) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (minus_zero, nan_value), 0, 0, 0);

  check_complex ("csin (10.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (10.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csin (NaN - 10.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (nan_value, -10.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csin (inf + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csin (-inf + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csin) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csin (NaN + NaN i) == NaN + NaN i",  FUNC(csin) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("csin (0.75 + 1.25 i) == 1.28722291002649188575873510790565441 + 1.17210635989270256101081285116138863 i",  FUNC(csin) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.28722291002649188575873510790565441L, 1.17210635989270256101081285116138863L), 0, 0, 0);
  check_complex ("csin (-2 - 3 i) == -9.15449914691142957346729954460983256 + 4.16890695996656435075481305885375484 i",  FUNC(csin) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-9.15449914691142957346729954460983256L, 4.16890695996656435075481305885375484L), 0, 0, 0);

  print_complex_max_error ("csin", 0, 0);
}


static void
csinh_test (void)
{
  errno = 0;
  FUNC(csinh) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("csinh (0.0 + 0.0 i) == 0.0 + 0.0 i",  FUNC(csinh) (BUILD_COMPLEX (0.0, 0.0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("csinh (-0 + 0.0 i) == -0 + 0.0 i",  FUNC(csinh) (BUILD_COMPLEX (minus_zero, 0.0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("csinh (0.0 - 0 i) == 0.0 - 0 i",  FUNC(csinh) (BUILD_COMPLEX (0.0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("csinh (-0 - 0 i) == -0 - 0 i",  FUNC(csinh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("csinh (0.0 + inf i) == 0.0 + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (0.0, plus_infty)), BUILD_COMPLEX (0.0, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-0 + inf i) == 0.0 + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (0.0, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (0.0 - inf i) == 0.0 + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (0.0, minus_infty)), BUILD_COMPLEX (0.0, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-0 - inf i) == 0.0 + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (0.0, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("csinh (inf + 0.0 i) == inf + 0.0 i",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, 0.0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("csinh (-inf + 0.0 i) == -inf + 0.0 i",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, 0.0)), BUILD_COMPLEX (minus_infty, 0.0), 0, 0, 0);
  check_complex ("csinh (inf - 0 i) == inf - 0 i",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("csinh (-inf - 0 i) == -inf - 0 i",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (minus_infty, minus_zero), 0, 0, 0);

  check_complex ("csinh (inf + inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-inf + inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (inf - inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-inf - inf i) == inf + NaN i plus invalid exception and sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, INVALID_EXCEPTION|IGNORE_ZERO_INF_SIGN);

  check_complex ("csinh (inf + 4.625 i) == -inf - inf i",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, 4.625)), BUILD_COMPLEX (minus_infty, minus_infty), 0, 0, 0);
  check_complex ("csinh (-inf + 4.625 i) == inf - inf i",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, 4.625)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csinh (inf - 4.625 i) == -inf + inf i",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, -4.625)), BUILD_COMPLEX (minus_infty, plus_infty), 0, 0, 0);
  check_complex ("csinh (-inf - 4.625 i) == inf + inf i",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, -4.625)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);

  check_complex ("csinh (6.75 + inf i) == NaN + NaN i plus invalid exception",  FUNC(csinh) (BUILD_COMPLEX (6.75, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csinh (-6.75 + inf i) == NaN + NaN i plus invalid exception",  FUNC(csinh) (BUILD_COMPLEX (-6.75, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csinh (6.75 - inf i) == NaN + NaN i plus invalid exception",  FUNC(csinh) (BUILD_COMPLEX (6.75, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("csinh (-6.75 - inf i) == NaN + NaN i plus invalid exception",  FUNC(csinh) (BUILD_COMPLEX (-6.75, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("csinh (0.0 + NaN i) == 0.0 + NaN i plus sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (0.0, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-0 + NaN i) == 0.0 + NaN i plus sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("csinh (inf + NaN i) == inf + NaN i plus sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("csinh (-inf + NaN i) == inf + NaN i plus sign of zero/inf not specified",  FUNC(csinh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("csinh (9.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (9.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csinh (-9.0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (-9.0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csinh (NaN + 0.0 i) == NaN + 0.0 i",  FUNC(csinh) (BUILD_COMPLEX (nan_value, 0.0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, 0);
  check_complex ("csinh (NaN - 0 i) == NaN - 0 i",  FUNC(csinh) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, minus_zero), 0, 0, 0);

  check_complex ("csinh (NaN + 10.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (nan_value, 10.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csinh (NaN - 10.0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (nan_value, -10.0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csinh (NaN + inf i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csinh (NaN - inf i) == NaN + NaN i plus invalid exception allowed",  FUNC(csinh) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csinh (NaN + NaN i) == NaN + NaN i",  FUNC(csinh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("csinh (0.75 + 1.25 i) == 0.259294854551162779153349830618433028 + 1.22863452409509552219214606515777594 i",  FUNC(csinh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.259294854551162779153349830618433028L, 1.22863452409509552219214606515777594L), 0, 0, 0);
  check_complex ("csinh (-2 - 3 i) == 3.59056458998577995201256544779481679 - 0.530921086248519805267040090660676560 i",  FUNC(csinh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (3.59056458998577995201256544779481679L, -0.530921086248519805267040090660676560L), 0, 0, 0);

  print_complex_max_error ("csinh", 0, 0);
}


static void
csqrt_test (void)
{
  errno = 0;
  FUNC(csqrt) (BUILD_COMPLEX (-1, 0));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("csqrt (0 + 0 i) == 0.0 + 0.0 i",  FUNC(csqrt) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("csqrt (0 - 0 i) == 0 - 0 i",  FUNC(csqrt) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0, minus_zero), 0, 0, 0);
  check_complex ("csqrt (-0 + 0 i) == 0.0 + 0.0 i",  FUNC(csqrt) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("csqrt (-0 - 0 i) == 0.0 - 0 i",  FUNC(csqrt) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);

  check_complex ("csqrt (-inf + 0 i) == 0.0 + inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("csqrt (-inf + 6 i) == 0.0 + inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, 6)), BUILD_COMPLEX (0.0, plus_infty), 0, 0, 0);
  check_complex ("csqrt (-inf - 0 i) == 0.0 - inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);
  check_complex ("csqrt (-inf - 6 i) == 0.0 - inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, -6)), BUILD_COMPLEX (0.0, minus_infty), 0, 0, 0);

  check_complex ("csqrt (inf + 0 i) == inf + 0.0 i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("csqrt (inf + 6 i) == inf + 0.0 i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, 6)), BUILD_COMPLEX (plus_infty, 0.0), 0, 0, 0);
  check_complex ("csqrt (inf - 0 i) == inf - 0 i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);
  check_complex ("csqrt (inf - 6 i) == inf - 0 i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, -6)), BUILD_COMPLEX (plus_infty, minus_zero), 0, 0, 0);

  check_complex ("csqrt (0 + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (4 + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (4, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (inf + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (-0 + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (-4 + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (-4, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (-inf + inf i) == inf + inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, plus_infty)), BUILD_COMPLEX (plus_infty, plus_infty), 0, 0, 0);
  check_complex ("csqrt (0 - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csqrt (4 - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (4, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csqrt (inf - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csqrt (-0 - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csqrt (-4 - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (-4, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);
  check_complex ("csqrt (-inf - inf i) == inf - inf i",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, minus_infty)), BUILD_COMPLEX (plus_infty, minus_infty), 0, 0, 0);

  check_complex ("csqrt (-inf + NaN i) == NaN + inf i plus sign of zero/inf not specified",  FUNC(csqrt) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (nan_value, plus_infty), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("csqrt (inf + NaN i) == inf + NaN i",  FUNC(csqrt) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (plus_infty, nan_value), 0, 0, 0);

  check_complex ("csqrt (0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (1 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (1, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (-0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (-1 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (-1, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csqrt (NaN + 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (NaN + 8 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (nan_value, 8)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (NaN - 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("csqrt (NaN - 8 i) == NaN + NaN i plus invalid exception allowed",  FUNC(csqrt) (BUILD_COMPLEX (nan_value, -8)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("csqrt (NaN + NaN i) == NaN + NaN i",  FUNC(csqrt) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("csqrt (16.0 - 30.0 i) == 5.0 - 3.0 i",  FUNC(csqrt) (BUILD_COMPLEX (16.0, -30.0)), BUILD_COMPLEX (5.0, -3.0), 0, 0, 0);
  check_complex ("csqrt (-1 + 0 i) == 0.0 + 1.0 i",  FUNC(csqrt) (BUILD_COMPLEX (-1, 0)), BUILD_COMPLEX (0.0, 1.0), 0, 0, 0);
  check_complex ("csqrt (0 + 2 i) == 1.0 + 1.0 i",  FUNC(csqrt) (BUILD_COMPLEX (0, 2)), BUILD_COMPLEX (1.0, 1.0), 0, 0, 0);
  check_complex ("csqrt (119 + 120 i) == 12.0 + 5.0 i",  FUNC(csqrt) (BUILD_COMPLEX (119, 120)), BUILD_COMPLEX (12.0, 5.0), 0, 0, 0);
  check_complex ("csqrt (0.75 + 1.25 i) == 1.05065169626078392338656675760808326 + 0.594868882070379067881984030639932657 i",  FUNC(csqrt) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.05065169626078392338656675760808326L, 0.594868882070379067881984030639932657L), 0, 0, 0);
  check_complex ("csqrt (-2 - 3 i) == 0.89597747612983812471573375529004348 - 1.6741492280355400404480393008490519 i",  FUNC(csqrt) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (0.89597747612983812471573375529004348L, -1.6741492280355400404480393008490519L), 0, 0, 0);
  check_complex ("csqrt (-2 + 3 i) == 0.89597747612983812471573375529004348 + 1.6741492280355400404480393008490519 i",  FUNC(csqrt) (BUILD_COMPLEX (-2, 3)), BUILD_COMPLEX (0.89597747612983812471573375529004348L, 1.6741492280355400404480393008490519L), 0, 0, 0);
  /* Principal square root should be returned (i.e., non-negative real
     part).  */
  check_complex ("csqrt (0 - 1 i) == M_SQRT_2_2 - M_SQRT_2_2 i",  FUNC(csqrt) (BUILD_COMPLEX (0, -1)), BUILD_COMPLEX (M_SQRT_2_2, -M_SQRT_2_2), 0, 0, 0);

  print_complex_max_error ("csqrt", 0, 0);
}

static void
ctan_test (void)
{
  errno = 0;
  FUNC(ctan) (BUILD_COMPLEX (0.7L, 1.2L));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("ctan (0 + 0 i) == 0.0 + 0.0 i",  FUNC(ctan) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("ctan (0 - 0 i) == 0.0 - 0 i",  FUNC(ctan) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("ctan (-0 + 0 i) == -0 + 0.0 i",  FUNC(ctan) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("ctan (-0 - 0 i) == -0 - 0 i",  FUNC(ctan) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("ctan (0 + inf i) == 0.0 + 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (0.0, 1.0), 0, 0, 0);
  check_complex ("ctan (1 + inf i) == 0.0 + 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (1, plus_infty)), BUILD_COMPLEX (0.0, 1.0), 0, 0, 0);
  check_complex ("ctan (-0 + inf i) == -0 + 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (minus_zero, 1.0), 0, 0, 0);
  check_complex ("ctan (-1 + inf i) == -0 + 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (-1, plus_infty)), BUILD_COMPLEX (minus_zero, 1.0), 0, 0, 0);

  check_complex ("ctan (0 - inf i) == 0.0 - 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (0.0, -1.0), 0, 0, 0);
  check_complex ("ctan (1 - inf i) == 0.0 - 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (1, minus_infty)), BUILD_COMPLEX (0.0, -1.0), 0, 0, 0);
  check_complex ("ctan (-0 - inf i) == -0 - 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (minus_zero, -1.0), 0, 0, 0);
  check_complex ("ctan (-1 - inf i) == -0 - 1.0 i",  FUNC(ctan) (BUILD_COMPLEX (-1, minus_infty)), BUILD_COMPLEX (minus_zero, -1.0), 0, 0, 0);

  check_complex ("ctan (inf + 0 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (inf + 2 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (plus_infty, 2)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (-inf + 0 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (-inf + 2 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (minus_infty, 2)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (inf - 0 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (inf - 2 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (plus_infty, -2)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (-inf - 0 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctan (-inf - 2 i) == NaN + NaN i plus invalid exception",  FUNC(ctan) (BUILD_COMPLEX (minus_infty, -2)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ctan (NaN + inf i) == 0.0 + 1.0 i plus sign of zero/inf not specified",  FUNC(ctan) (BUILD_COMPLEX (nan_value, plus_infty)), BUILD_COMPLEX (0.0, 1.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ctan (NaN - inf i) == 0.0 - 1.0 i plus sign of zero/inf not specified",  FUNC(ctan) (BUILD_COMPLEX (nan_value, minus_infty)), BUILD_COMPLEX (0.0, -1.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ctan (0 + NaN i) == 0.0 + NaN i",  FUNC(ctan) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (0.0, nan_value), 0, 0, 0);
  check_complex ("ctan (-0 + NaN i) == -0 + NaN i",  FUNC(ctan) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (minus_zero, nan_value), 0, 0, 0);

  check_complex ("ctan (0.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (0.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctan (-4.5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (-4.5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ctan (NaN + 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctan (NaN + 5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (nan_value, 5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctan (NaN - 0 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctan (NaN - 0.25 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctan) (BUILD_COMPLEX (nan_value, -0.25)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ctan (NaN + NaN i) == NaN + NaN i",  FUNC(ctan) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("ctan (0.75 + 1.25 i) == 0.160807785916206426725166058173438663 + 0.975363285031235646193581759755216379 i",  FUNC(ctan) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (0.160807785916206426725166058173438663L, 0.975363285031235646193581759755216379L), 0, 0, 0);
  check_complex ("ctan (-2 - 3 i) == 0.376402564150424829275122113032269084e-2 - 1.00323862735360980144635859782192726 i",  FUNC(ctan) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (0.376402564150424829275122113032269084e-2L, -1.00323862735360980144635859782192726L), 0, 0, 0);

  print_complex_max_error ("ctan", 0, 0);
}


static void
ctanh_test (void)
{
  errno = 0;
  FUNC(ctanh) (BUILD_COMPLEX (0, 0));
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_complex ("ctanh (0 + 0 i) == 0.0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (0, 0)), BUILD_COMPLEX (0.0, 0.0), 0, 0, 0);
  check_complex ("ctanh (0 - 0 i) == 0.0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (0, minus_zero)), BUILD_COMPLEX (0.0, minus_zero), 0, 0, 0);
  check_complex ("ctanh (-0 + 0 i) == -0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_zero, 0)), BUILD_COMPLEX (minus_zero, 0.0), 0, 0, 0);
  check_complex ("ctanh (-0 - 0 i) == -0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_zero, minus_zero)), BUILD_COMPLEX (minus_zero, minus_zero), 0, 0, 0);

  check_complex ("ctanh (inf + 0 i) == 1.0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (plus_infty, 0)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("ctanh (inf + 1 i) == 1.0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (plus_infty, 1)), BUILD_COMPLEX (1.0, 0.0), 0, 0, 0);
  check_complex ("ctanh (inf - 0 i) == 1.0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (plus_infty, minus_zero)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);
  check_complex ("ctanh (inf - 1 i) == 1.0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (plus_infty, -1)), BUILD_COMPLEX (1.0, minus_zero), 0, 0, 0);
  check_complex ("ctanh (-inf + 0 i) == -1.0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_infty, 0)), BUILD_COMPLEX (-1.0, 0.0), 0, 0, 0);
  check_complex ("ctanh (-inf + 1 i) == -1.0 + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_infty, 1)), BUILD_COMPLEX (-1.0, 0.0), 0, 0, 0);
  check_complex ("ctanh (-inf - 0 i) == -1.0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_infty, minus_zero)), BUILD_COMPLEX (-1.0, minus_zero), 0, 0, 0);
  check_complex ("ctanh (-inf - 1 i) == -1.0 - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (minus_infty, -1)), BUILD_COMPLEX (-1.0, minus_zero), 0, 0, 0);

  check_complex ("ctanh (0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (0, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (2 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (2, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (0, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (2 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (2, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (-0 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (minus_zero, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (-2 + inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (-2, plus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (-0 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (minus_zero, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);
  check_complex ("ctanh (-2 - inf i) == NaN + NaN i plus invalid exception",  FUNC(ctanh) (BUILD_COMPLEX (-2, minus_infty)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION);

  check_complex ("ctanh (inf + NaN i) == 1.0 + 0.0 i plus sign of zero/inf not specified",  FUNC(ctanh) (BUILD_COMPLEX (plus_infty, nan_value)), BUILD_COMPLEX (1.0, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);
  check_complex ("ctanh (-inf + NaN i) == -1.0 + 0.0 i plus sign of zero/inf not specified",  FUNC(ctanh) (BUILD_COMPLEX (minus_infty, nan_value)), BUILD_COMPLEX (-1.0, 0.0), 0, 0, IGNORE_ZERO_INF_SIGN);

  check_complex ("ctanh (NaN + 0 i) == NaN + 0.0 i",  FUNC(ctanh) (BUILD_COMPLEX (nan_value, 0)), BUILD_COMPLEX (nan_value, 0.0), 0, 0, 0);
  check_complex ("ctanh (NaN - 0 i) == NaN - 0 i",  FUNC(ctanh) (BUILD_COMPLEX (nan_value, minus_zero)), BUILD_COMPLEX (nan_value, minus_zero), 0, 0, 0);

  check_complex ("ctanh (NaN + 0.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (nan_value, 0.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctanh (NaN - 4.5 i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (nan_value, -4.5)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ctanh (0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (0, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctanh (5 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (5, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctanh (-0 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (minus_zero, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);
  check_complex ("ctanh (-0.25 + NaN i) == NaN + NaN i plus invalid exception allowed",  FUNC(ctanh) (BUILD_COMPLEX (-0.25, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, INVALID_EXCEPTION_OK);

  check_complex ("ctanh (NaN + NaN i) == NaN + NaN i",  FUNC(ctanh) (BUILD_COMPLEX (nan_value, nan_value)), BUILD_COMPLEX (nan_value, nan_value), 0, 0, 0);

  check_complex ("ctanh (0 + pi/4 i) == 0.0 + 1.0 i",  FUNC(ctanh) (BUILD_COMPLEX (0, M_PI_4l)), BUILD_COMPLEX (0.0, 1.0), 0, 0, 0);

  check_complex ("ctanh (0.75 + 1.25 i) == 1.37260757053378320258048606571226857 + 0.385795952609750664177596760720790220 i",  FUNC(ctanh) (BUILD_COMPLEX (0.75L, 1.25L)), BUILD_COMPLEX (1.37260757053378320258048606571226857L, 0.385795952609750664177596760720790220L), 0, 0, 0);
  check_complex ("ctanh (-2 - 3 i) == -0.965385879022133124278480269394560686 + 0.988437503832249372031403430350121098e-2 i",  FUNC(ctanh) (BUILD_COMPLEX (-2, -3)), BUILD_COMPLEX (-0.965385879022133124278480269394560686L, 0.988437503832249372031403430350121098e-2L), 0, 0, 0);

  print_complex_max_error ("ctanh", 0, 0);
}
#endif

static void
erf_test (void)
{
  errno = 0;
  FUNC(erf) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("erf (0) == 0",  FUNC(erf) (0), 0, 0, 0, 0);
  check_float ("erf (-0) == -0",  FUNC(erf) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("erf (inf) == 1",  FUNC(erf) (plus_infty), 1, 0, 0, 0);
  check_float ("erf (-inf) == -1",  FUNC(erf) (minus_infty), -1, 0, 0, 0);
  check_float ("erf (NaN) == NaN",  FUNC(erf) (nan_value), nan_value, 0, 0, 0);

  check_float ("erf (0.125) == 0.140316204801333817393029446521623398",  FUNC(erf) (0.125L), 0.140316204801333817393029446521623398L, 0, 0, 0);
  check_float ("erf (0.75) == 0.711155633653515131598937834591410777",  FUNC(erf) (0.75L), 0.711155633653515131598937834591410777L, 0, 0, 0);
  check_float ("erf (1.25) == 0.922900128256458230136523481197281140",  FUNC(erf) (1.25L), 0.922900128256458230136523481197281140L, 0, 0, 0);
  check_float ("erf (2.0) == 0.995322265018952734162069256367252929",  FUNC(erf) (2.0L), 0.995322265018952734162069256367252929L, 0, 0, 0);
  check_float ("erf (4.125) == 0.999999994576599200434933994687765914",  FUNC(erf) (4.125L), 0.999999994576599200434933994687765914L, 0, 0, 0);
  check_float ("erf (27.0) == 1.0",  FUNC(erf) (27.0L), 1.0L, 0, 0, 0);

  print_max_error ("erf", 0, 0);
}


static void
erfc_test (void)
{
  errno = 0;
  FUNC(erfc) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("erfc (inf) == 0.0",  FUNC(erfc) (plus_infty), 0.0, 0, 0, 0);
  check_float ("erfc (-inf) == 2.0",  FUNC(erfc) (minus_infty), 2.0, 0, 0, 0);
  check_float ("erfc (0.0) == 1.0",  FUNC(erfc) (0.0), 1.0, 0, 0, 0);
  check_float ("erfc (-0) == 1.0",  FUNC(erfc) (minus_zero), 1.0, 0, 0, 0);
  check_float ("erfc (NaN) == NaN",  FUNC(erfc) (nan_value), nan_value, 0, 0, 0);

  check_float ("erfc (0.125) == 0.859683795198666182606970553478376602",  FUNC(erfc) (0.125L), 0.859683795198666182606970553478376602L, 0, 0, 0);
  check_float ("erfc (0.75) == 0.288844366346484868401062165408589223",  FUNC(erfc) (0.75L), 0.288844366346484868401062165408589223L, 0, 0, 0);
  check_float ("erfc (1.25) == 0.0770998717435417698634765188027188596",  FUNC(erfc) (1.25L), 0.0770998717435417698634765188027188596L, 0, 0, 0);
  check_float ("erfc (2.0) == 0.00467773498104726583793074363274707139",  FUNC(erfc) (2.0L), 0.00467773498104726583793074363274707139L, 0, 0, 0);
  check_float ("erfc (4.125) == 0.542340079956506600531223408575531062e-8",  FUNC(erfc) (4.125L), 0.542340079956506600531223408575531062e-8L, 0, 0, 0);
#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
# if LDBL_MIN_10_EXP < -319
  check_float ("erfc (27.0) == 0.523704892378925568501606768284954709e-318",  FUNC(erfc) (27.0L), 0.523704892378925568501606768284954709e-318L, 0, 0, 0);
# endif
#endif

  print_max_error ("erfc", 0, 0);
}


static void
exp_test (void)
{
  errno = 0;
  FUNC(exp) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("exp (0) == 1",  FUNC(exp) (0), 1, 0, 0, 0);
  check_float ("exp (-0) == 1",  FUNC(exp) (minus_zero), 1, 0, 0, 0);

#ifndef TEST_INLINE
  check_float ("exp (inf) == inf",  FUNC(exp) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("exp (-inf) == 0",  FUNC(exp) (minus_infty), 0, 0, 0, 0);
#endif
  check_float ("exp (NaN) == NaN",  FUNC(exp) (nan_value), nan_value, 0, 0, 0);
  check_float ("exp (1) == e",  FUNC(exp) (1), M_El, 0, 0, 0);

  check_float ("exp (2) == e^2",  FUNC(exp) (2), M_E2l, 0, 0, 0);
  check_float ("exp (3) == e^3",  FUNC(exp) (3), M_E3l, 0, 0, 0);
  check_float ("exp (0.75) == 2.11700001661267466854536981983709561",  FUNC(exp) (0.75L), 2.11700001661267466854536981983709561L, 0, 0, 0);
  check_float ("exp (50.0) == 5184705528587072464087.45332293348538",  FUNC(exp) (50.0L), 5184705528587072464087.45332293348538L, 0, 0, 0);
  check_float ("exp (88.72269439697265625) == 3.40233126623160774937554134772290447915e38",  FUNC(exp) (88.72269439697265625L), 3.40233126623160774937554134772290447915e38L, 0, 0, 0);
#if defined TEST_LDOUBLE && __LDBL_MAX_EXP__ > 1024
  /* The result can only be represented in sane long double.  */
  check_float ("exp (1000.0) == 0.197007111401704699388887935224332313e435",  FUNC(exp) (1000.0L), 0.197007111401704699388887935224332313e435L, 0, 0, 0);
#endif

  print_max_error ("exp", 0, 0);
}


static void
exp10_test (void)
{
  errno = 0;
  FUNC(exp10) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("exp10 (0) == 1",  FUNC(exp10) (0), 1, 0, 0, 0);
  check_float ("exp10 (-0) == 1",  FUNC(exp10) (minus_zero), 1, 0, 0, 0);

  check_float ("exp10 (inf) == inf",  FUNC(exp10) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("exp10 (-inf) == 0",  FUNC(exp10) (minus_infty), 0, 0, 0, 0);
  check_float ("exp10 (NaN) == NaN",  FUNC(exp10) (nan_value), nan_value, 0, 0, 0);
  check_float ("exp10 (3) == 1000",  FUNC(exp10) (3), 1000, 0, 0, 0);
  check_float ("exp10 (-1) == 0.1",  FUNC(exp10) (-1), 0.1L, 0, 0, 0);
  check_float ("exp10 (1e6) == inf",  FUNC(exp10) (1e6), plus_infty, 0, 0, 0);
  check_float ("exp10 (-1e6) == 0",  FUNC(exp10) (-1e6), 0, 0, 0, 0);
  check_float ("exp10 (0.75) == 5.62341325190349080394951039776481231",  FUNC(exp10) (0.75L), 5.62341325190349080394951039776481231L, 0, 0, 0);

  print_max_error ("exp10", 0, 0);
}


static void
exp2_test (void)
{
  errno = 0;
  FUNC(exp2) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("exp2 (0) == 1",  FUNC(exp2) (0), 1, 0, 0, 0);
  check_float ("exp2 (-0) == 1",  FUNC(exp2) (minus_zero), 1, 0, 0, 0);
  check_float ("exp2 (inf) == inf",  FUNC(exp2) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("exp2 (-inf) == 0",  FUNC(exp2) (minus_infty), 0, 0, 0, 0);
  check_float ("exp2 (NaN) == NaN",  FUNC(exp2) (nan_value), nan_value, 0, 0, 0);

  check_float ("exp2 (10) == 1024",  FUNC(exp2) (10), 1024, 0, 0, 0);
  check_float ("exp2 (-1) == 0.5",  FUNC(exp2) (-1), 0.5, 0, 0, 0);
  check_float ("exp2 (1e6) == inf",  FUNC(exp2) (1e6), plus_infty, 0, 0, 0);
  check_float ("exp2 (-1e6) == 0",  FUNC(exp2) (-1e6), 0, 0, 0, 0);
  check_float ("exp2 (0.75) == 1.68179283050742908606225095246642979",  FUNC(exp2) (0.75L), 1.68179283050742908606225095246642979L, 0, 0, 0);

  print_max_error ("exp2", 0, 0);
}


static void
expm1_test (void)
{
  errno = 0;
  FUNC(expm1) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("expm1 (0) == 0",  FUNC(expm1) (0), 0, 0, 0, 0);
  check_float ("expm1 (-0) == -0",  FUNC(expm1) (minus_zero), minus_zero, 0, 0, 0);

#ifndef TEST_INLINE
  check_float ("expm1 (inf) == inf",  FUNC(expm1) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("expm1 (-inf) == -1",  FUNC(expm1) (minus_infty), -1, 0, 0, 0);
#endif
  check_float ("expm1 (NaN) == NaN",  FUNC(expm1) (nan_value), nan_value, 0, 0, 0);

  check_float ("expm1 (1) == M_El - 1.0",  FUNC(expm1) (1), M_El - 1.0, 0, 0, 0);
  check_float ("expm1 (0.75) == 1.11700001661267466854536981983709561",  FUNC(expm1) (0.75L), 1.11700001661267466854536981983709561L, 0, 0, 0);

  errno = 0;
  check_float ("expm1 (100000.0) == inf",  FUNC(expm1) (100000.0), plus_infty, 0, 0, 0);
  check_int ("errno for expm1(large) == ERANGE", errno, ERANGE, 0, 0, 0);

  print_max_error ("expm1", 0, 0);
}


static void
fabs_test (void)
{
  init_max_error ();

  check_float ("fabs (0) == 0",  FUNC(fabs) (0), 0, 0, 0, 0);
  check_float ("fabs (-0) == 0",  FUNC(fabs) (minus_zero), 0, 0, 0, 0);

  check_float ("fabs (inf) == inf",  FUNC(fabs) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("fabs (-inf) == inf",  FUNC(fabs) (minus_infty), plus_infty, 0, 0, 0);
  check_float ("fabs (NaN) == NaN",  FUNC(fabs) (nan_value), nan_value, 0, 0, 0);

  check_float ("fabs (38.0) == 38.0",  FUNC(fabs) (38.0), 38.0, 0, 0, 0);
  check_float ("fabs (-e) == e",  FUNC(fabs) (-M_El), M_El, 0, 0, 0);

  print_max_error ("fabs", 0, 0);
}


static void
fdim_test (void)
{
  init_max_error ();

  check_float ("fdim (0, 0) == 0",  FUNC(fdim) (0, 0), 0, 0, 0, 0);
  check_float ("fdim (9, 0) == 9",  FUNC(fdim) (9, 0), 9, 0, 0, 0);
  check_float ("fdim (0, 9) == 0",  FUNC(fdim) (0, 9), 0, 0, 0, 0);
  check_float ("fdim (-9, 0) == 0",  FUNC(fdim) (-9, 0), 0, 0, 0, 0);
  check_float ("fdim (0, -9) == 9",  FUNC(fdim) (0, -9), 9, 0, 0, 0);

  check_float ("fdim (inf, 9) == inf",  FUNC(fdim) (plus_infty, 9), plus_infty, 0, 0, 0);
  check_float ("fdim (inf, -9) == inf",  FUNC(fdim) (plus_infty, -9), plus_infty, 0, 0, 0);
  check_float ("fdim (-inf, 9) == 0",  FUNC(fdim) (minus_infty, 9), 0, 0, 0, 0);
  check_float ("fdim (-inf, -9) == 0",  FUNC(fdim) (minus_infty, -9), 0, 0, 0, 0);
  check_float ("fdim (9, -inf) == inf",  FUNC(fdim) (9, minus_infty), plus_infty, 0, 0, 0);
  check_float ("fdim (-9, -inf) == inf",  FUNC(fdim) (-9, minus_infty), plus_infty, 0, 0, 0);
  check_float ("fdim (9, inf) == 0",  FUNC(fdim) (9, plus_infty), 0, 0, 0, 0);
  check_float ("fdim (-9, inf) == 0",  FUNC(fdim) (-9, plus_infty), 0, 0, 0, 0);

  check_float ("fdim (0, NaN) == NaN",  FUNC(fdim) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("fdim (9, NaN) == NaN",  FUNC(fdim) (9, nan_value), nan_value, 0, 0, 0);
  check_float ("fdim (-9, NaN) == NaN",  FUNC(fdim) (-9, nan_value), nan_value, 0, 0, 0);
  check_float ("fdim (NaN, 9) == NaN",  FUNC(fdim) (nan_value, 9), nan_value, 0, 0, 0);
  check_float ("fdim (NaN, -9) == NaN",  FUNC(fdim) (nan_value, -9), nan_value, 0, 0, 0);
  check_float ("fdim (inf, NaN) == NaN",  FUNC(fdim) (plus_infty, nan_value), nan_value, 0, 0, 0);
  check_float ("fdim (-inf, NaN) == NaN",  FUNC(fdim) (minus_infty, nan_value), nan_value, 0, 0, 0);
  check_float ("fdim (NaN, inf) == NaN",  FUNC(fdim) (nan_value, plus_infty), nan_value, 0, 0, 0);
  check_float ("fdim (NaN, -inf) == NaN",  FUNC(fdim) (nan_value, minus_infty), nan_value, 0, 0, 0);
  check_float ("fdim (NaN, NaN) == NaN",  FUNC(fdim) (nan_value, nan_value), nan_value, 0, 0, 0);

  check_float ("fdim (inf, inf) == 0",  FUNC(fdim) (plus_infty, plus_infty), 0, 0, 0, 0);

  print_max_error ("fdim", 0, 0);
}


static void
floor_test (void)
{
  init_max_error ();

  check_float ("floor (0.0) == 0.0",  FUNC(floor) (0.0), 0.0, 0, 0, 0);
  check_float ("floor (-0) == -0",  FUNC(floor) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("floor (inf) == inf",  FUNC(floor) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("floor (-inf) == -inf",  FUNC(floor) (minus_infty), minus_infty, 0, 0, 0);
  check_float ("floor (NaN) == NaN",  FUNC(floor) (nan_value), nan_value, 0, 0, 0);

  check_float ("floor (pi) == 3.0",  FUNC(floor) (M_PIl), 3.0, 0, 0, 0);
  check_float ("floor (-pi) == -4.0",  FUNC(floor) (-M_PIl), -4.0, 0, 0, 0);

  check_float ("floor (0.1) == 0.0",  FUNC(floor) (0.1), 0.0, 0, 0, 0);
  check_float ("floor (0.25) == 0.0",  FUNC(floor) (0.25), 0.0, 0, 0, 0);
  check_float ("floor (0.625) == 0.0",  FUNC(floor) (0.625), 0.0, 0, 0, 0);
  check_float ("floor (-0.1) == -1.0",  FUNC(floor) (-0.1), -1.0, 0, 0, 0);
  check_float ("floor (-0.25) == -1.0",  FUNC(floor) (-0.25), -1.0, 0, 0, 0);
  check_float ("floor (-0.625) == -1.0",  FUNC(floor) (-0.625), -1.0, 0, 0, 0);

#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
  check_float ("floor (4503599627370495.5) == 4503599627370495.0",  FUNC(floor) (4503599627370495.5L), 4503599627370495.0L, 0, 0, 0);
  check_float ("floor (4503599627370496.25) == 4503599627370496.0",  FUNC(floor) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("floor (4503599627370496.5) == 4503599627370496.0",  FUNC(floor) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("floor (4503599627370496.75) == 4503599627370496.0",  FUNC(floor) (4503599627370496.75L), 4503599627370496.0L, 0, 0, 0);
  check_float ("floor (4503599627370497.5) == 4503599627370497.0",  FUNC(floor) (4503599627370497.5L), 4503599627370497.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("floor (4503599627370494.5000000000001) == 4503599627370494.0",  FUNC(floor) (4503599627370494.5000000000001L), 4503599627370494.0L, 0, 0, 0);
  check_float ("floor (4503599627370495.5000000000001) == 4503599627370495.0",  FUNC(floor) (4503599627370495.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("floor (4503599627370496.5000000000001) == 4503599627370496.0",  FUNC(floor) (4503599627370496.5000000000001L), 4503599627370496.0L, 0, 0, 0);
# endif

  check_float ("floor (-4503599627370495.5) == -4503599627370496.0",  FUNC(floor) (-4503599627370495.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("floor (-4503599627370496.25) == -4503599627370497.0",  FUNC(floor) (-4503599627370496.25L), -4503599627370497.0L, 0, 0, 0);
  check_float ("floor (-4503599627370496.5) == -4503599627370497.0",  FUNC(floor) (-4503599627370496.5L), -4503599627370497.0L, 0, 0, 0);
  check_float ("floor (-4503599627370496.75) == -4503599627370497.0",  FUNC(floor) (-4503599627370496.75L), -4503599627370497.0L, 0, 0, 0);
  check_float ("floor (-4503599627370497.5) == -4503599627370498.0",  FUNC(floor) (-4503599627370497.5L), -4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("floor (-4503599627370494.5000000000001) == -4503599627370495.0",  FUNC(floor) (-4503599627370494.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("floor (-4503599627370495.5000000000001) == -4503599627370496.0",  FUNC(floor) (-4503599627370495.5000000000001L), -4503599627370496.0L, 0, 0, 0);
  check_float ("floor (-4503599627370496.5000000000001) == -4503599627370497.0",  FUNC(floor) (-4503599627370496.5000000000001L), -4503599627370497.0L, 0, 0, 0);
# endif

  check_float ("floor (9007199254740991.5) == 9007199254740991.0",  FUNC(floor) (9007199254740991.5L), 9007199254740991.0L, 0, 0, 0);
  check_float ("floor (9007199254740992.25) == 9007199254740992.0",  FUNC(floor) (9007199254740992.25L), 9007199254740992.0L, 0, 0, 0);
  check_float ("floor (9007199254740992.5) == 9007199254740992.0",  FUNC(floor) (9007199254740992.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("floor (9007199254740992.75) == 9007199254740992.0",  FUNC(floor) (9007199254740992.75L), 9007199254740992.0L, 0, 0, 0);
  check_float ("floor (9007199254740993.5) == 9007199254740993.0",  FUNC(floor) (9007199254740993.5L), 9007199254740993.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("floor (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(floor) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("floor (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(floor) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("floor (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(floor) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("floor (9007199254740991.5000000000001) == 9007199254740991.0",  FUNC(floor) (9007199254740991.5000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("floor (9007199254740992.5000000000001) == 9007199254740992.0",  FUNC(floor) (9007199254740992.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("floor (9007199254740993.5000000000001) == 9007199254740993.0",  FUNC(floor) (9007199254740993.5000000000001L), 9007199254740993.0L, 0, 0, 0);
# endif

  check_float ("floor (-9007199254740991.5) == -9007199254740992.0",  FUNC(floor) (-9007199254740991.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("floor (-9007199254740992.25) == -9007199254740993.0",  FUNC(floor) (-9007199254740992.25L), -9007199254740993.0L, 0, 0, 0);
  check_float ("floor (-9007199254740992.5) == -9007199254740993.0",  FUNC(floor) (-9007199254740992.5L), -9007199254740993.0L, 0, 0, 0);
  check_float ("floor (-9007199254740992.75) == -9007199254740993.0",  FUNC(floor) (-9007199254740992.75L), -9007199254740993.0L, 0, 0, 0);
  check_float ("floor (-9007199254740993.5) == -9007199254740994.0",  FUNC(floor) (-9007199254740993.5L), -9007199254740994.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("floor (-9007199254740991.0000000000001) == -9007199254740992.0",  FUNC(floor) (-9007199254740991.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("floor (-9007199254740992.0000000000001) == -9007199254740993.0",  FUNC(floor) (-9007199254740992.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("floor (-9007199254740993.0000000000001) == -9007199254740994.0",  FUNC(floor) (-9007199254740993.0000000000001L), -9007199254740994.0L, 0, 0, 0);
  check_float ("floor (-9007199254740991.5000000000001) == -9007199254740992.0",  FUNC(floor) (-9007199254740991.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("floor (-9007199254740992.5000000000001) == -9007199254740993.0",  FUNC(floor) (-9007199254740992.5000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("floor (-9007199254740993.5000000000001) == -9007199254740994.0",  FUNC(floor) (-9007199254740993.5000000000001L), -9007199254740994.0L, 0, 0, 0);
# endif

  check_float ("floor (72057594037927935.5) == 72057594037927935.0",  FUNC(floor) (72057594037927935.5L), 72057594037927935.0L, 0, 0, 0);
  check_float ("floor (72057594037927936.25) == 72057594037927936.0",  FUNC(floor) (72057594037927936.25L), 72057594037927936.0L, 0, 0, 0);
  check_float ("floor (72057594037927936.5) == 72057594037927936.0",  FUNC(floor) (72057594037927936.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("floor (72057594037927936.75) == 72057594037927936.0",  FUNC(floor) (72057594037927936.75L), 72057594037927936.0L, 0, 0, 0);
  check_float ("floor (72057594037927937.5) == 72057594037927937.0",  FUNC(floor) (72057594037927937.5L), 72057594037927937.0L, 0, 0, 0);

  check_float ("floor (-72057594037927935.5) == -72057594037927936.0",  FUNC(floor) (-72057594037927935.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("floor (-72057594037927936.25) == -72057594037927937.0",  FUNC(floor) (-72057594037927936.25L), -72057594037927937.0L, 0, 0, 0);
  check_float ("floor (-72057594037927936.5) == -72057594037927937.0",  FUNC(floor) (-72057594037927936.5L), -72057594037927937.0L, 0, 0, 0);
  check_float ("floor (-72057594037927936.75) == -72057594037927937.0",  FUNC(floor) (-72057594037927936.75L), -72057594037927937.0L, 0, 0, 0);
  check_float ("floor (-72057594037927937.5) == -72057594037927938.0",  FUNC(floor) (-72057594037927937.5L), -72057594037927938.0L, 0, 0, 0);

  check_float ("floor (10141204801825835211973625643007.5) == 10141204801825835211973625643007.0",  FUNC(floor) (10141204801825835211973625643007.5L), 10141204801825835211973625643007.0L, 0, 0, 0);
  check_float ("floor (10141204801825835211973625643008.25) == 10141204801825835211973625643008.0",  FUNC(floor) (10141204801825835211973625643008.25L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("floor (10141204801825835211973625643008.5) == 10141204801825835211973625643008.0",  FUNC(floor) (10141204801825835211973625643008.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("floor (10141204801825835211973625643008.75) == 10141204801825835211973625643008.0",  FUNC(floor) (10141204801825835211973625643008.75L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("floor (10141204801825835211973625643009.5) == 10141204801825835211973625643009.0",  FUNC(floor) (10141204801825835211973625643009.5L), 10141204801825835211973625643009.0L, 0, 0, 0);
#endif

  print_max_error ("floor", 0, 0);
}


static void
fma_test (void)
{
  init_max_error ();

  check_float ("fma (1.0, 2.0, 3.0) == 5.0",  FUNC(fma) (1.0, 2.0, 3.0), 5.0, 0, 0, 0);
  check_float ("fma (NaN, 2.0, 3.0) == NaN",  FUNC(fma) (nan_value, 2.0, 3.0), nan_value, 0, 0, 0);
  check_float ("fma (1.0, NaN, 3.0) == NaN",  FUNC(fma) (1.0, nan_value, 3.0), nan_value, 0, 0, 0);
  check_float ("fma (1.0, 2.0, NaN) == NaN plus invalid exception allowed",  FUNC(fma) (1.0, 2.0, nan_value), nan_value, 0, 0, INVALID_EXCEPTION_OK);
  check_float ("fma (inf, 0.0, NaN) == NaN plus invalid exception allowed",  FUNC(fma) (plus_infty, 0.0, nan_value), nan_value, 0, 0, INVALID_EXCEPTION_OK);
  check_float ("fma (-inf, 0.0, NaN) == NaN plus invalid exception allowed",  FUNC(fma) (minus_infty, 0.0, nan_value), nan_value, 0, 0, INVALID_EXCEPTION_OK);
  check_float ("fma (0.0, inf, NaN) == NaN plus invalid exception allowed",  FUNC(fma) (0.0, plus_infty, nan_value), nan_value, 0, 0, INVALID_EXCEPTION_OK);
  check_float ("fma (0.0, -inf, NaN) == NaN plus invalid exception allowed",  FUNC(fma) (0.0, minus_infty, nan_value), nan_value, 0, 0, INVALID_EXCEPTION_OK);
  check_float ("fma (inf, 0.0, 1.0) == NaN plus invalid exception",  FUNC(fma) (plus_infty, 0.0, 1.0), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-inf, 0.0, 1.0) == NaN plus invalid exception",  FUNC(fma) (minus_infty, 0.0, 1.0), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (0.0, inf, 1.0) == NaN plus invalid exception",  FUNC(fma) (0.0, plus_infty, 1.0), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (0.0, -inf, 1.0) == NaN plus invalid exception",  FUNC(fma) (0.0, minus_infty, 1.0), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("fma (inf, inf, -inf) == NaN plus invalid exception",  FUNC(fma) (plus_infty, plus_infty, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-inf, inf, inf) == NaN plus invalid exception",  FUNC(fma) (minus_infty, plus_infty, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (inf, -inf, inf) == NaN plus invalid exception",  FUNC(fma) (plus_infty, minus_infty, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-inf, -inf, -inf) == NaN plus invalid exception",  FUNC(fma) (minus_infty, minus_infty, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (inf, 3.5, -inf) == NaN plus invalid exception",  FUNC(fma) (plus_infty, 3.5L, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-inf, -7.5, -inf) == NaN plus invalid exception",  FUNC(fma) (minus_infty, -7.5L, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-13.5, inf, inf) == NaN plus invalid exception",  FUNC(fma) (-13.5L, plus_infty, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("fma (-inf, 7.5, inf) == NaN plus invalid exception",  FUNC(fma) (minus_infty, 7.5L, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("fma (1.25, 0.75, 0.0625) == 1.0",  FUNC(fma) (1.25L, 0.75L, 0.0625L), 1.0L, 0, 0, 0);

  FLOAT fltmax = CHOOSE (LDBL_MAX, DBL_MAX, FLT_MAX,
			 LDBL_MAX, DBL_MAX, FLT_MAX);
  check_float ("fma (-fltmax, -fltmax, -inf) == -inf",  FUNC(fma) (-fltmax, -fltmax, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fma (fltmax / 2, fltmax / 2, -inf) == -inf",  FUNC(fma) (fltmax / 2, fltmax / 2, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fma (-fltmax, fltmax, inf) == inf",  FUNC(fma) (-fltmax, fltmax, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fma (fltmax / 2, -fltmax / 4, inf) == inf",  FUNC(fma) (fltmax / 2, -fltmax / 4, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fma (inf, 4, inf) == inf",  FUNC(fma) (plus_infty, 4, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fma (2, -inf, -inf) == -inf",  FUNC(fma) (2, minus_infty, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fma (-inf, -inf, inf) == inf",  FUNC(fma) (minus_infty, minus_infty, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fma (inf, -inf, -inf) == -inf",  FUNC(fma) (plus_infty, minus_infty, minus_infty), minus_infty, 0, 0, 0);

#if defined (TEST_FLOAT) && FLT_MANT_DIG == 24
  check_float ("fma (0x1.7ff8p+13, 0x1.000002p+0, 0x1.ffffp-24) == 0x1.7ff802p+13",  FUNC(fma) (0x1.7ff8p+13, 0x1.000002p+0, 0x1.ffffp-24), 0x1.7ff802p+13, 0, 0, 0);
  check_float ("fma (0x1.fffp+0, 0x1.00001p+0, -0x1.fffp+0) == 0x1.fffp-20",  FUNC(fma) (0x1.fffp+0, 0x1.00001p+0, -0x1.fffp+0), 0x1.fffp-20, 0, 0, 0);
  check_float ("fma (0x1.9abcdep+127, 0x0.9abcdep-126, -0x1.f08948p+0) == 0x1.bb421p-25",  FUNC(fma) (0x1.9abcdep+127, 0x0.9abcdep-126, -0x1.f08948p+0), 0x1.bb421p-25, 0, 0, 0);
  check_float ("fma (0x1.9abcdep+100, 0x0.9abcdep-126, -0x1.f08948p-27) == 0x1.bb421p-52",  FUNC(fma) (0x1.9abcdep+100, 0x0.9abcdep-126, -0x1.f08948p-27), 0x1.bb421p-52, 0, 0, 0);
  check_float ("fma (0x1.fffffep+127, 0x1.001p+0, -0x1.fffffep+127) == 0x1.fffffep+115",  FUNC(fma) (0x1.fffffep+127, 0x1.001p+0, -0x1.fffffep+127), 0x1.fffffep+115, 0, 0, 0);
  check_float ("fma (-0x1.fffffep+127, 0x1.fffffep+0, 0x1.fffffep+127) == -0x1.fffffap+127",  FUNC(fma) (-0x1.fffffep+127, 0x1.fffffep+0, 0x1.fffffep+127), -0x1.fffffap+127, 0, 0, 0);
  check_float ("fma (0x1.fffffep+127, 2.0, -0x1.fffffep+127) == 0x1.fffffep+127",  FUNC(fma) (0x1.fffffep+127, 2.0, -0x1.fffffep+127), 0x1.fffffep+127, 0, 0, 0);
#endif
#if defined (TEST_DOUBLE) && DBL_MANT_DIG == 53
  check_float ("fma (0x1.7fp+13, 0x1.0000000000001p+0, 0x1.ffep-48) == 0x1.7f00000000001p+13",  FUNC(fma) (0x1.7fp+13, 0x1.0000000000001p+0, 0x1.ffep-48), 0x1.7f00000000001p+13, 0, 0, 0);
  check_float ("fma (0x1.fffp+0, 0x1.0000000000001p+0, -0x1.fffp+0) == 0x1.fffp-52",  FUNC(fma) (0x1.fffp+0, 0x1.0000000000001p+0, -0x1.fffp+0), 0x1.fffp-52, 0, 0, 0);
  check_float ("fma (0x1.0000002p+0, 0x1.ffffffcp-1, 0x1p-300) == 1.0",  FUNC(fma) (0x1.0000002p+0, 0x1.ffffffcp-1, 0x1p-300), 1.0, 0, 0, 0);
  check_float ("fma (0x1.0000002p+0, 0x1.ffffffcp-1, -0x1p-300) == 0x1.fffffffffffffp-1",  FUNC(fma) (0x1.0000002p+0, 0x1.ffffffcp-1, -0x1p-300), 0x1.fffffffffffffp-1, 0, 0, 0);
  check_float ("fma (0x1.deadbeef2feedp+1023, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp+1) == 0x1.0989687bc9da4p-53",  FUNC(fma) (0x1.deadbeef2feedp+1023, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp+1), 0x1.0989687bc9da4p-53, 0, 0, 0);
  check_float ("fma (0x1.deadbeef2feedp+900, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp-122) == 0x1.0989687bc9da4p-176",  FUNC(fma) (0x1.deadbeef2feedp+900, 0x0.deadbeef2feedp-1022, -0x1.a05f8c01a4bfbp-122), 0x1.0989687bc9da4p-176, 0, 0, 0);
  check_float ("fma (0x1.fffffffffffffp+1023, 0x1.001p+0, -0x1.fffffffffffffp+1023) == 0x1.fffffffffffffp+1011",  FUNC(fma) (0x1.fffffffffffffp+1023, 0x1.001p+0, -0x1.fffffffffffffp+1023), 0x1.fffffffffffffp+1011, 0, 0, 0);
  check_float ("fma (-0x1.fffffffffffffp+1023, 0x1.fffffffffffffp+0, 0x1.fffffffffffffp+1023) == -0x1.ffffffffffffdp+1023",  FUNC(fma) (-0x1.fffffffffffffp+1023, 0x1.fffffffffffffp+0, 0x1.fffffffffffffp+1023), -0x1.ffffffffffffdp+1023, 0, 0, 0);
  check_float ("fma (0x1.fffffffffffffp+1023, 2.0, -0x1.fffffffffffffp+1023) == 0x1.fffffffffffffp+1023",  FUNC(fma) (0x1.fffffffffffffp+1023, 2.0, -0x1.fffffffffffffp+1023), 0x1.fffffffffffffp+1023, 0, 0, 0);
  check_float ("fma (0x1.6a09e667f3bccp-538, 0x1.6a09e667f3bccp-538, 0.0) == 0.0",  FUNC(fma) (0x1.6a09e667f3bccp-538, 0x1.6a09e667f3bccp-538, 0.0), 0.0, 0, 0, 0);
  check_float ("fma (0x1.deadbeef2feedp-495, 0x1.deadbeef2feedp-495, -0x1.bf86a5786a574p-989) == 0x0.0000042625a1fp-1022",  FUNC(fma) (0x1.deadbeef2feedp-495, 0x1.deadbeef2feedp-495, -0x1.bf86a5786a574p-989), 0x0.0000042625a1fp-1022, 0, 0, 0);
  check_float ("fma (0x1.deadbeef2feedp-503, 0x1.deadbeef2feedp-503, -0x1.bf86a5786a574p-1005) == 0x0.0000000004262p-1022",  FUNC(fma) (0x1.deadbeef2feedp-503, 0x1.deadbeef2feedp-503, -0x1.bf86a5786a574p-1005), 0x0.0000000004262p-1022, 0, 0, 0);
  check_float ("fma (0x1p-537, 0x1p-538, 0x1p-1074) == 0x0.0000000000002p-1022",  FUNC(fma) (0x1p-537, 0x1p-538, 0x1p-1074), 0x0.0000000000002p-1022, 0, 0, 0);
  check_float ("fma (0x1.7fffff8p-968, 0x1p-106, 0x0.000001p-1022) == 0x0.0000010000001p-1022",  FUNC(fma) (0x1.7fffff8p-968, 0x1p-106, 0x0.000001p-1022), 0x0.0000010000001p-1022, 0, 0, 0);
  check_float ("fma (0x1.4000004p-967, 0x1p-106, 0x0.000001p-1022) == 0x0.0000010000003p-1022",  FUNC(fma) (0x1.4000004p-967, 0x1p-106, 0x0.000001p-1022), 0x0.0000010000003p-1022, 0, 0, 0);
  check_float ("fma (0x1.4p-967, -0x1p-106, -0x0.000001p-1022) == -0x0.0000010000002p-1022",  FUNC(fma) (0x1.4p-967, -0x1p-106, -0x0.000001p-1022), -0x0.0000010000002p-1022, 0, 0, 0);
  check_float ("fma (-0x1.19cab66d73e17p-959, 0x1.c7108a8c5ff51p-107, -0x0.80b0ad65d9b64p-1022) == -0x0.80b0ad65d9d59p-1022",  FUNC(fma) (-0x1.19cab66d73e17p-959, 0x1.c7108a8c5ff51p-107, -0x0.80b0ad65d9b64p-1022), -0x0.80b0ad65d9d59p-1022, 0, 0, 0);
  check_float ("fma (-0x1.d2eaed6e8e9d3p-979, -0x1.4e066c62ac9ddp-63, -0x0.9245e6b003454p-1022) == -0x0.9245c09c5fb5dp-1022",  FUNC(fma) (-0x1.d2eaed6e8e9d3p-979, -0x1.4e066c62ac9ddp-63, -0x0.9245e6b003454p-1022), -0x0.9245c09c5fb5dp-1022, 0, 0, 0);
  check_float ("fma (0x1.153d650bb9f06p-907, 0x1.2d01230d48407p-125, -0x0.b278d5acfc3cp-1022) == -0x0.b22757123bbe9p-1022",  FUNC(fma) (0x1.153d650bb9f06p-907, 0x1.2d01230d48407p-125, -0x0.b278d5acfc3cp-1022), -0x0.b22757123bbe9p-1022, 0, 0, 0);
  check_float ("fma (-0x1.fffffffffffffp-711, 0x1.fffffffffffffp-275, 0x1.fffffe00007ffp-983) == 0x1.7ffffe00007ffp-983",  FUNC(fma) (-0x1.fffffffffffffp-711, 0x1.fffffffffffffp-275, 0x1.fffffe00007ffp-983), 0x1.7ffffe00007ffp-983, 0, 0, 0);
#endif
#if defined (TEST_LDOUBLE) && LDBL_MANT_DIG == 64
  check_float ("fma (-0x8.03fcp+3696, 0xf.fffffffffffffffp-6140, 0x8.3ffffffffffffffp-2450) == -0x8.01ecp-2440",  FUNC(fma) (-0x8.03fcp+3696L, 0xf.fffffffffffffffp-6140L, 0x8.3ffffffffffffffp-2450L), -0x8.01ecp-2440L, 0, 0, 0);
  check_float ("fma (0x9.fcp+2033, -0x8.000e1f000ff800fp-3613, -0xf.fffffffffffc0ffp-1579) == -0xd.fc119fb093ed092p-1577",  FUNC(fma) (0x9.fcp+2033L, -0x8.000e1f000ff800fp-3613L, -0xf.fffffffffffc0ffp-1579L), -0xd.fc119fb093ed092p-1577L, 0, 0, 0);
  check_float ("fma (0xc.7fc000003ffffffp-1194, 0x8.1e0003fffffffffp+15327, -0x8.fffep+14072) == 0xc.ae9f164020effffp+14136",  FUNC(fma) (0xc.7fc000003ffffffp-1194L, 0x8.1e0003fffffffffp+15327L, -0x8.fffep+14072L), 0xc.ae9f164020effffp+14136L, 0, 0, 0);
  check_float ("fma (-0x8.0001fc000000003p+1798, 0xcp-2230, 0x8.f7e000000000007p-468) == -0xc.0002f9ffee10404p-429",  FUNC(fma) (-0x8.0001fc000000003p+1798L, 0xcp-2230L, 0x8.f7e000000000007p-468L), -0xc.0002f9ffee10404p-429L, 0, 0, 0);
  check_float ("fma (0xc.0000000000007ffp+10130, -0x8.000000000000001p+4430, 0xc.07000000001ffffp+14513) == -0xb.fffffffffffd7e4p+14563",  FUNC(fma) (0xc.0000000000007ffp+10130L, -0x8.000000000000001p+4430L, 0xc.07000000001ffffp+14513L), -0xb.fffffffffffd7e4p+14563L, 0, 0, 0);
  check_float ("fma (0xb.ffffp-4777, 0x8.000000fffffffffp-11612, -0x0.3800fff8p-16385) == 0x5.c7fe80c7ffeffffp-16385",  FUNC(fma) (0xb.ffffp-4777L, 0x8.000000fffffffffp-11612L, -0x0.3800fff8p-16385L), 0x5.c7fe80c7ffeffffp-16385L, 0, 0, 0);
#endif
#if defined (TEST_LDOUBLE) && LDBL_MANT_DIG == 113
  check_float ("fma (0x1.bb2de33e02ccbbfa6e245a7c1f71p-2584, -0x1.6b500daf0580d987f1bc0cadfcddp-13777, 0x1.613cd91d9fed34b33820e5ab9d8dp-16378) == -0x1.3a79fb50eb9ce887cffa0f09bd9fp-16360",  FUNC(fma) (0x1.bb2de33e02ccbbfa6e245a7c1f71p-2584L, -0x1.6b500daf0580d987f1bc0cadfcddp-13777L, 0x1.613cd91d9fed34b33820e5ab9d8dp-16378L), -0x1.3a79fb50eb9ce887cffa0f09bd9fp-16360L, 0, 0, 0);
  check_float ("fma (-0x1.f949b880cacb0f0c61540105321dp-5954, -0x1.3876cec84b4140f3bd6198731b7ep-10525, -0x0.a5dc1c6cfbc498c54fb0b504bf19p-16382) == -0x0.a5dc1c6cfbc498c54fb0b5038abbp-16382",  FUNC(fma) (-0x1.f949b880cacb0f0c61540105321dp-5954L, -0x1.3876cec84b4140f3bd6198731b7ep-10525L, -0x0.a5dc1c6cfbc498c54fb0b504bf19p-16382L), -0x0.a5dc1c6cfbc498c54fb0b5038abbp-16382L, 0, 0, 0);
  check_float ("fma (-0x1.0000fffffffffp-16221, 0x1.0000001fffff8007fep-239, 0x0.ff87ffffffffffffe000003fffffp-16382) == 0x0.ff87ffffffffffffdffc003bff7fp-16382",  FUNC(fma) (-0x1.0000fffffffffp-16221L, 0x1.0000001fffff8007fep-239L, 0x0.ff87ffffffffffffe000003fffffp-16382L), 0x0.ff87ffffffffffffdffc003bff7fp-16382L, 0, 0, 0);
  check_float ("fma (-0x1.ac79c9376ef447f3827c9e9de008p-2228, -0x1.5ba830022b6139e21fbe7270cad8p-6314, 0x1.e8282b6a26bb6a9daf5c8e73e9f9p-8616) == 0x1.22f14a0253878a730cd1aee373adp-8541",  FUNC(fma) (-0x1.ac79c9376ef447f3827c9e9de008p-2228L, -0x1.5ba830022b6139e21fbe7270cad8p-6314L, 0x1.e8282b6a26bb6a9daf5c8e73e9f9p-8616L), 0x1.22f14a0253878a730cd1aee373adp-8541L, 0, 0, 0);
  check_float ("fma (-0x1.c69749ec574caaa2ab8e97ddb9f3p+2652, 0x1.f34235ff9d095449c29b4831b62dp+3311, 0x1.fbe4302df23354dbd0c4d3cfe606p+5879) == -0x1.bb473bfdfb7a6e18886ce6e57eafp+5964",  FUNC(fma) (-0x1.c69749ec574caaa2ab8e97ddb9f3p+2652L, 0x1.f34235ff9d095449c29b4831b62dp+3311L, 0x1.fbe4302df23354dbd0c4d3cfe606p+5879L), -0x1.bb473bfdfb7a6e18886ce6e57eafp+5964L, 0, 0, 0);
  check_float ("fma (-0x1.ca8835fc6ecfb5398625fc891be5p-1686, 0x1.621e1972bbe2180e5be9dd7d8df5p-7671, -0x1.7d2d21b73b52cf20dec2a83902a4p-9395) == -0x1.3d2322191c9c88bc68a62ab8042cp-9356",  FUNC(fma) (-0x1.ca8835fc6ecfb5398625fc891be5p-1686L, 0x1.621e1972bbe2180e5be9dd7d8df5p-7671L, -0x1.7d2d21b73b52cf20dec2a83902a4p-9395L), -0x1.3d2322191c9c88bc68a62ab8042cp-9356L, 0, 0, 0);
  check_float ("fma (-0x1.55cff679ec49c2541fab41fc843ep-11819, 0x1.e60e9f464f9e8df0509647c7c971p+12325, 0x1.eaa2a7649d765c2f564f7a5beca7p+454) == -0x1.447e29fa7e406a285f4e350fcf86p+507",  FUNC(fma) (-0x1.55cff679ec49c2541fab41fc843ep-11819L, 0x1.e60e9f464f9e8df0509647c7c971p+12325L, 0x1.eaa2a7649d765c2f564f7a5beca7p+454L), -0x1.447e29fa7e406a285f4e350fcf86p+507L, 0, 0, 0);
  check_float ("fma (0x1.f0e7b1454908576f2537d863cf9bp+11432, 0x1.cdce52f09d4ca76e68706f34b5d5p-1417, -0x1.2e986187c70f146235ea2066e486p+9979) == 0x1.c030dad3cc5643f3dd0f5619f661p+10016",  FUNC(fma) (0x1.f0e7b1454908576f2537d863cf9bp+11432L, 0x1.cdce52f09d4ca76e68706f34b5d5p-1417L, -0x1.2e986187c70f146235ea2066e486p+9979L), 0x1.c030dad3cc5643f3dd0f5619f661p+10016L, 0, 0, 0);
  check_float ("fma (0x1.f102f7da4a57a3a4aab620e29452p-3098, -0x1.cc06a4ff40248f9e2dcc4b6afd84p-11727, 0x1.d512a11126b5ac8ed8973b8580c8p-14849) == -0x1.be8f1cf737ab4d1c31c54f5ec23bp-14824",  FUNC(fma) (0x1.f102f7da4a57a3a4aab620e29452p-3098L, -0x1.cc06a4ff40248f9e2dcc4b6afd84p-11727L, 0x1.d512a11126b5ac8ed8973b8580c8p-14849L), -0x1.be8f1cf737ab4d1c31c54f5ec23bp-14824L, 0, 0, 0);
  check_float ("fma (-0x1.fc47ac7434b993cd8dcb2b431f25p-3816, 0x1.fbc9750da8468852d84558e1db6dp-5773, -0x1.00a98abf783f75c40fe5b7a37d86p-9607) == -0x1.f81917b166f45e763cfcc057e2adp-9588",  FUNC(fma) (-0x1.fc47ac7434b993cd8dcb2b431f25p-3816L, 0x1.fbc9750da8468852d84558e1db6dp-5773L, -0x1.00a98abf783f75c40fe5b7a37d86p-9607L), -0x1.f81917b166f45e763cfcc057e2adp-9588L, 0, 0, 0);
  check_float ("fma (0x1.00000000000007ffffffffffffffp-9045, -0x1.ffffffffffff80000001ffffffffp+4773, -0x1.f8p-4316) == -0x1.00000000000f88000000fffffdffp-4271",  FUNC(fma) (0x1.00000000000007ffffffffffffffp-9045L, -0x1.ffffffffffff80000001ffffffffp+4773L, -0x1.f8p-4316L), -0x1.00000000000f88000000fffffdffp-4271L, 0, 0, 0);
  check_float ("fma (0x1.4e922764c90701d4a2f21d01893dp-8683, -0x1.955a12e2d7c9447c27fa022fc865p+212, -0x1.e9634462eaef96528b90b6944578p-8521) == -0x1.08e1783184a371943d3598e10865p-8470",  FUNC(fma) (0x1.4e922764c90701d4a2f21d01893dp-8683L, -0x1.955a12e2d7c9447c27fa022fc865p+212L, -0x1.e9634462eaef96528b90b6944578p-8521L), -0x1.08e1783184a371943d3598e10865p-8470L, 0, 0, 0);
  check_float ("fma (0x1.801181509c03bdbef10d6165588cp-15131, 0x1.ad86f8e57d3d40bfa8007780af63p-368, -0x1.6e9df0dab1c9f1d7a6043c390741p-15507) == 0x1.417c9b2b15e2ad57dc9e0e920844p-15498",  FUNC(fma) (0x1.801181509c03bdbef10d6165588cp-15131L, 0x1.ad86f8e57d3d40bfa8007780af63p-368L, -0x1.6e9df0dab1c9f1d7a6043c390741p-15507L), 0x1.417c9b2b15e2ad57dc9e0e920844p-15498L, 0, 0, 0);
#endif

  print_max_error ("fma", 0, 0);
}


static void
fmax_test (void)
{
  init_max_error ();

  check_float ("fmax (0, 0) == 0",  FUNC(fmax) (0, 0), 0, 0, 0, 0);
  check_float ("fmax (-0, -0) == -0",  FUNC(fmax) (minus_zero, minus_zero), minus_zero, 0, 0, 0);
  check_float ("fmax (9, 0) == 9",  FUNC(fmax) (9, 0), 9, 0, 0, 0);
  check_float ("fmax (0, 9) == 9",  FUNC(fmax) (0, 9), 9, 0, 0, 0);
  check_float ("fmax (-9, 0) == 0",  FUNC(fmax) (-9, 0), 0, 0, 0, 0);
  check_float ("fmax (0, -9) == 0",  FUNC(fmax) (0, -9), 0, 0, 0, 0);

  check_float ("fmax (inf, 9) == inf",  FUNC(fmax) (plus_infty, 9), plus_infty, 0, 0, 0);
  check_float ("fmax (0, inf) == inf",  FUNC(fmax) (0, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fmax (-9, inf) == inf",  FUNC(fmax) (-9, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fmax (inf, -9) == inf",  FUNC(fmax) (plus_infty, -9), plus_infty, 0, 0, 0);

  check_float ("fmax (-inf, 9) == 9",  FUNC(fmax) (minus_infty, 9), 9, 0, 0, 0);
  check_float ("fmax (-inf, -9) == -9",  FUNC(fmax) (minus_infty, -9), -9, 0, 0, 0);
  check_float ("fmax (9, -inf) == 9",  FUNC(fmax) (9, minus_infty), 9, 0, 0, 0);
  check_float ("fmax (-9, -inf) == -9",  FUNC(fmax) (-9, minus_infty), -9, 0, 0, 0);

  check_float ("fmax (0, NaN) == 0",  FUNC(fmax) (0, nan_value), 0, 0, 0, 0);
  check_float ("fmax (9, NaN) == 9",  FUNC(fmax) (9, nan_value), 9, 0, 0, 0);
  check_float ("fmax (-9, NaN) == -9",  FUNC(fmax) (-9, nan_value), -9, 0, 0, 0);
  check_float ("fmax (NaN, 0) == 0",  FUNC(fmax) (nan_value, 0), 0, 0, 0, 0);
  check_float ("fmax (NaN, 9) == 9",  FUNC(fmax) (nan_value, 9), 9, 0, 0, 0);
  check_float ("fmax (NaN, -9) == -9",  FUNC(fmax) (nan_value, -9), -9, 0, 0, 0);
  check_float ("fmax (inf, NaN) == inf",  FUNC(fmax) (plus_infty, nan_value), plus_infty, 0, 0, 0);
  check_float ("fmax (-inf, NaN) == -inf",  FUNC(fmax) (minus_infty, nan_value), minus_infty, 0, 0, 0);
  check_float ("fmax (NaN, inf) == inf",  FUNC(fmax) (nan_value, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fmax (NaN, -inf) == -inf",  FUNC(fmax) (nan_value, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fmax (NaN, NaN) == NaN",  FUNC(fmax) (nan_value, nan_value), nan_value, 0, 0, 0);

  print_max_error ("fmax", 0, 0);
}


static void
fmin_test (void)
{
  init_max_error ();

  check_float ("fmin (0, 0) == 0",  FUNC(fmin) (0, 0), 0, 0, 0, 0);
  check_float ("fmin (-0, -0) == -0",  FUNC(fmin) (minus_zero, minus_zero), minus_zero, 0, 0, 0);
  check_float ("fmin (9, 0) == 0",  FUNC(fmin) (9, 0), 0, 0, 0, 0);
  check_float ("fmin (0, 9) == 0",  FUNC(fmin) (0, 9), 0, 0, 0, 0);
  check_float ("fmin (-9, 0) == -9",  FUNC(fmin) (-9, 0), -9, 0, 0, 0);
  check_float ("fmin (0, -9) == -9",  FUNC(fmin) (0, -9), -9, 0, 0, 0);

  check_float ("fmin (inf, 9) == 9",  FUNC(fmin) (plus_infty, 9), 9, 0, 0, 0);
  check_float ("fmin (9, inf) == 9",  FUNC(fmin) (9, plus_infty), 9, 0, 0, 0);
  check_float ("fmin (inf, -9) == -9",  FUNC(fmin) (plus_infty, -9), -9, 0, 0, 0);
  check_float ("fmin (-9, inf) == -9",  FUNC(fmin) (-9, plus_infty), -9, 0, 0, 0);
  check_float ("fmin (-inf, 9) == -inf",  FUNC(fmin) (minus_infty, 9), minus_infty, 0, 0, 0);
  check_float ("fmin (-inf, -9) == -inf",  FUNC(fmin) (minus_infty, -9), minus_infty, 0, 0, 0);
  check_float ("fmin (9, -inf) == -inf",  FUNC(fmin) (9, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fmin (-9, -inf) == -inf",  FUNC(fmin) (-9, minus_infty), minus_infty, 0, 0, 0);

  check_float ("fmin (0, NaN) == 0",  FUNC(fmin) (0, nan_value), 0, 0, 0, 0);
  check_float ("fmin (9, NaN) == 9",  FUNC(fmin) (9, nan_value), 9, 0, 0, 0);
  check_float ("fmin (-9, NaN) == -9",  FUNC(fmin) (-9, nan_value), -9, 0, 0, 0);
  check_float ("fmin (NaN, 0) == 0",  FUNC(fmin) (nan_value, 0), 0, 0, 0, 0);
  check_float ("fmin (NaN, 9) == 9",  FUNC(fmin) (nan_value, 9), 9, 0, 0, 0);
  check_float ("fmin (NaN, -9) == -9",  FUNC(fmin) (nan_value, -9), -9, 0, 0, 0);
  check_float ("fmin (inf, NaN) == inf",  FUNC(fmin) (plus_infty, nan_value), plus_infty, 0, 0, 0);
  check_float ("fmin (-inf, NaN) == -inf",  FUNC(fmin) (minus_infty, nan_value), minus_infty, 0, 0, 0);
  check_float ("fmin (NaN, inf) == inf",  FUNC(fmin) (nan_value, plus_infty), plus_infty, 0, 0, 0);
  check_float ("fmin (NaN, -inf) == -inf",  FUNC(fmin) (nan_value, minus_infty), minus_infty, 0, 0, 0);
  check_float ("fmin (NaN, NaN) == NaN",  FUNC(fmin) (nan_value, nan_value), nan_value, 0, 0, 0);

  print_max_error ("fmin", 0, 0);
}


static void
fmod_test (void)
{
  errno = 0;
  FUNC(fmod) (6.5, 2.3L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  /* fmod (+0, y) == +0 for y != 0.  */
  check_float ("fmod (0, 3) == 0",  FUNC(fmod) (0, 3), 0, 0, 0, 0);

  /* fmod (-0, y) == -0 for y != 0.  */
  check_float ("fmod (-0, 3) == -0",  FUNC(fmod) (minus_zero, 3), minus_zero, 0, 0, 0);

  /* fmod (+inf, y) == NaN plus invalid exception.  */
  errno = 0;
  check_float ("fmod (inf, 3) == NaN plus invalid exception",  FUNC(fmod) (plus_infty, 3), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for fmod(Inf,3) unchanged", errno, EDOM, 0, 0, 0);
  /* fmod (-inf, y) == NaN plus invalid exception.  */
  errno = 0;
  check_float ("fmod (-inf, 3) == NaN plus invalid exception",  FUNC(fmod) (minus_infty, 3), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for fmod(-Inf,3) unchanged", errno, EDOM, 0, 0, 0);
  /* fmod (x, +0) == NaN plus invalid exception.  */
  errno = 0;
  check_float ("fmod (3, 0) == NaN plus invalid exception",  FUNC(fmod) (3, 0), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for fmod(3,0) unchanged", errno, EDOM, 0, 0, 0);
  /* fmod (x, -0) == NaN plus invalid exception.  */
  check_float ("fmod (3, -0) == NaN plus invalid exception",  FUNC(fmod) (3, minus_zero), nan_value, 0, 0, INVALID_EXCEPTION);

  /* fmod (x, +inf) == x for x not infinite.  */
  check_float ("fmod (3.0, inf) == 3.0",  FUNC(fmod) (3.0, plus_infty), 3.0, 0, 0, 0);
  /* fmod (x, -inf) == x for x not infinite.  */
  check_float ("fmod (3.0, -inf) == 3.0",  FUNC(fmod) (3.0, minus_infty), 3.0, 0, 0, 0);

  check_float ("fmod (NaN, NaN) == NaN",  FUNC(fmod) (nan_value, nan_value), nan_value, 0, 0, 0);

  check_float ("fmod (6.5, 2.25) == 2.0",  FUNC(fmod) (6.5, 2.25L), 2.0L, 0, 0, 0);
  check_float ("fmod (-6.5, 2.25) == -2.0",  FUNC(fmod) (-6.5, 2.25L), -2.0L, 0, 0, 0);
  check_float ("fmod (6.5, -2.25) == 2.0",  FUNC(fmod) (6.5, -2.25L), 2.0L, 0, 0, 0);
  check_float ("fmod (-6.5, -2.25) == -2.0",  FUNC(fmod) (-6.5, -2.25L), -2.0L, 0, 0, 0);

  print_max_error ("fmod", 0, 0);
}


static void
fpclassify_test (void)
{
  init_max_error ();

  check_int ("fpclassify (NaN) == FP_NAN", fpclassify (nan_value), FP_NAN, 0, 0, 0);
  check_int ("fpclassify (inf) == FP_INFINITE", fpclassify (plus_infty), FP_INFINITE, 0, 0, 0);
  check_int ("fpclassify (-inf) == FP_INFINITE", fpclassify (minus_infty), FP_INFINITE, 0, 0, 0);
  check_int ("fpclassify (+0) == FP_ZERO", fpclassify (plus_zero), FP_ZERO, 0, 0, 0);
  check_int ("fpclassify (-0) == FP_ZERO", fpclassify (minus_zero), FP_ZERO, 0, 0, 0);
  check_int ("fpclassify (1000) == FP_NORMAL", fpclassify (1000), FP_NORMAL, 0, 0, 0);

  print_max_error ("fpclassify", 0, 0);
}


static void
frexp_test (void)
{
  int x;

  init_max_error ();

  check_float ("frexp (inf, &x) == inf",  FUNC(frexp) (plus_infty, &x), plus_infty, 0, 0, 0);
  check_float ("frexp (-inf, &x) == -inf",  FUNC(frexp) (minus_infty, &x), minus_infty, 0, 0, 0);
  check_float ("frexp (NaN, &x) == NaN",  FUNC(frexp) (nan_value, &x), nan_value, 0, 0, 0);

  check_float ("frexp (0.0, &x) == 0.0",  FUNC(frexp) (0.0, &x), 0.0, 0, 0, 0);
  check_int ("frexp (0.0, &x) sets x to 0.0", x, 0.0, 0, 0, 0);
  check_float ("frexp (-0, &x) == -0",  FUNC(frexp) (minus_zero, &x), minus_zero, 0, 0, 0);
  check_int ("frexp (-0, &x) sets x to 0.0", x, 0.0, 0, 0, 0);

  check_float ("frexp (12.8, &x) == 0.8",  FUNC(frexp) (12.8L, &x), 0.8L, 0, 0, 0);
  check_int ("frexp (12.8, &x) sets x to 4", x, 4, 0, 0, 0);
  check_float ("frexp (-27.34, &x) == -0.854375",  FUNC(frexp) (-27.34L, &x), -0.854375L, 0, 0, 0);
  check_int ("frexp (-27.34, &x) sets x to 5", x, 5, 0, 0, 0);

  print_max_error ("frexp", 0, 0);
}


static void
gamma_test (void)
{
  errno = 0;
  FUNC(gamma) (1);

  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;
  feclearexcept (FE_ALL_EXCEPT);

  init_max_error ();

  signgam = 0;
  check_float ("gamma (inf) == inf",  FUNC(gamma) (plus_infty), plus_infty, 0, 0, 0);
  signgam = 0;
  check_float ("gamma (0) == inf plus division by zero exception",  FUNC(gamma) (0), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  signgam = 0;
  check_float ("gamma (-3) == inf plus division by zero exception",  FUNC(gamma) (-3), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  signgam = 0;
  check_float ("gamma (-inf) == inf",  FUNC(gamma) (minus_infty), plus_infty, 0, 0, 0);
  signgam = 0;
  check_float ("gamma (NaN) == NaN",  FUNC(gamma) (nan_value), nan_value, 0, 0, 0);

  signgam = 0;
  check_float ("gamma (1) == 0",  FUNC(gamma) (1), 0, 0, 0, 0);
  check_int ("gamma (1) sets signgam to 1", signgam, 1, 0, 0, 0);
  signgam = 0;
  check_float ("gamma (3) == M_LN2l",  FUNC(gamma) (3), M_LN2l, 0, 0, 0);
  check_int ("gamma (3) sets signgam to 1", signgam, 1, 0, 0, 0);

  signgam = 0;
  check_float ("gamma (0.5) == log(sqrt(pi))",  FUNC(gamma) (0.5), M_LOG_SQRT_PIl, 0, 0, 0);
  check_int ("gamma (0.5) sets signgam to 1", signgam, 1, 0, 0, 0);
  signgam = 0;
  check_float ("gamma (-0.5) == log(2*sqrt(pi))",  FUNC(gamma) (-0.5), M_LOG_2_SQRT_PIl, 0, 0, 0);
  check_int ("gamma (-0.5) sets signgam to -1", signgam, -1, 0, 0, 0);

  print_max_error ("gamma", 0, 0);
}

static void
hypot_test (void)
{
  errno = 0;
  FUNC(hypot) (0.7L, 12.4L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("hypot (inf, 1) == inf plus sign of zero/inf not specified",  FUNC(hypot) (plus_infty, 1), plus_infty, 0, 0, IGNORE_ZERO_INF_SIGN);
  check_float ("hypot (-inf, 1) == inf plus sign of zero/inf not specified",  FUNC(hypot) (minus_infty, 1), plus_infty, 0, 0, IGNORE_ZERO_INF_SIGN);

#ifndef TEST_INLINE
  check_float ("hypot (inf, NaN) == inf",  FUNC(hypot) (plus_infty, nan_value), plus_infty, 0, 0, 0);
  check_float ("hypot (-inf, NaN) == inf",  FUNC(hypot) (minus_infty, nan_value), plus_infty, 0, 0, 0);
  check_float ("hypot (NaN, inf) == inf",  FUNC(hypot) (nan_value, plus_infty), plus_infty, 0, 0, 0);
  check_float ("hypot (NaN, -inf) == inf",  FUNC(hypot) (nan_value, minus_infty), plus_infty, 0, 0, 0);
#endif

  check_float ("hypot (NaN, NaN) == NaN",  FUNC(hypot) (nan_value, nan_value), nan_value, 0, 0, 0);

  /* hypot (x,y) == hypot (+-x, +-y)  */
  check_float ("hypot (0.7, 12.4) == 12.419742348374220601176836866763271",  FUNC(hypot) (0.7L, 12.4L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (-0.7, 12.4) == 12.419742348374220601176836866763271",  FUNC(hypot) (-0.7L, 12.4L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (0.7, -12.4) == 12.419742348374220601176836866763271",  FUNC(hypot) (0.7L, -12.4L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (-0.7, -12.4) == 12.419742348374220601176836866763271",  FUNC(hypot) (-0.7L, -12.4L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (12.4, 0.7) == 12.419742348374220601176836866763271",  FUNC(hypot) (12.4L, 0.7L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (-12.4, 0.7) == 12.419742348374220601176836866763271",  FUNC(hypot) (-12.4L, 0.7L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (12.4, -0.7) == 12.419742348374220601176836866763271",  FUNC(hypot) (12.4L, -0.7L), 12.419742348374220601176836866763271L, 0, 0, 0);
  check_float ("hypot (-12.4, -0.7) == 12.419742348374220601176836866763271",  FUNC(hypot) (-12.4L, -0.7L), 12.419742348374220601176836866763271L, 0, 0, 0);

  /*  hypot (x,0) == fabs (x)  */
  check_float ("hypot (0.75, 0) == 0.75",  FUNC(hypot) (0.75L, 0), 0.75L, 0, 0, 0);
  check_float ("hypot (-0.75, 0) == 0.75",  FUNC(hypot) (-0.75L, 0), 0.75L, 0, 0, 0);
  check_float ("hypot (-5.7e7, 0) == 5.7e7",  FUNC(hypot) (-5.7e7, 0), 5.7e7L, 0, 0, 0);

  check_float ("hypot (0.75, 1.25) == 1.45773797371132511771853821938639577",  FUNC(hypot) (0.75L, 1.25L), 1.45773797371132511771853821938639577L, 0, 0, 0);

  print_max_error ("hypot", 0, 0);
}


static void
ilogb_test (void)
{
  init_max_error ();

  check_int ("ilogb (1) == 0",  FUNC(ilogb) (1), 0, 0, 0, 0);
  check_int ("ilogb (e) == 1",  FUNC(ilogb) (M_El), 1, 0, 0, 0);
  check_int ("ilogb (1024) == 10",  FUNC(ilogb) (1024), 10, 0, 0, 0);
  check_int ("ilogb (-2000) == 10",  FUNC(ilogb) (-2000), 10, 0, 0, 0);

  /* XXX We have a problem here: the standard does not tell us whether
     exceptions are allowed/required.  ignore them for now.  */

  check_int ("ilogb (0.0) == FP_ILOGB0 plus exceptions allowed",  FUNC(ilogb) (0.0), FP_ILOGB0, 0, 0, EXCEPTIONS_OK);
  check_int ("ilogb (NaN) == FP_ILOGBNAN plus exceptions allowed",  FUNC(ilogb) (nan_value), FP_ILOGBNAN, 0, 0, EXCEPTIONS_OK);
  check_int ("ilogb (inf) == INT_MAX plus exceptions allowed",  FUNC(ilogb) (plus_infty), INT_MAX, 0, 0, EXCEPTIONS_OK);
  check_int ("ilogb (-inf) == INT_MAX plus exceptions allowed",  FUNC(ilogb) (minus_infty), INT_MAX, 0, 0, EXCEPTIONS_OK);

  print_max_error ("ilogb", 0, 0);
}

static void
isfinite_test (void)
{
  init_max_error ();

  check_bool ("isfinite (0) == true", isfinite (0), 1, 0, 0, 0);
  check_bool ("isfinite (-0) == true", isfinite (minus_zero), 1, 0, 0, 0);
  check_bool ("isfinite (10) == true", isfinite (10), 1, 0, 0, 0);
  check_bool ("isfinite (inf) == false", isfinite (plus_infty), 0, 0, 0, 0);
  check_bool ("isfinite (-inf) == false", isfinite (minus_infty), 0, 0, 0, 0);
  check_bool ("isfinite (NaN) == false", isfinite (nan_value), 0, 0, 0, 0);

  print_max_error ("isfinite", 0, 0);
}

static void
isnormal_test (void)
{
  init_max_error ();

  check_bool ("isnormal (0) == false", isnormal (0), 0, 0, 0, 0);
  check_bool ("isnormal (-0) == false", isnormal (minus_zero), 0, 0, 0, 0);
  check_bool ("isnormal (10) == true", isnormal (10), 1, 0, 0, 0);
  check_bool ("isnormal (inf) == false", isnormal (plus_infty), 0, 0, 0, 0);
  check_bool ("isnormal (-inf) == false", isnormal (minus_infty), 0, 0, 0, 0);
  check_bool ("isnormal (NaN) == false", isnormal (nan_value), 0, 0, 0, 0);

  print_max_error ("isnormal", 0, 0);
}

static void
j0_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(j0) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  /* j0 is the Bessel function of the first kind of order 0 */
  check_float ("j0 (NaN) == NaN",  FUNC(j0) (nan_value), nan_value, 0, 0, 0);
  check_float ("j0 (inf) == 0",  FUNC(j0) (plus_infty), 0, 0, 0, 0);
  check_float ("j0 (-1.0) == 0.765197686557966551449717526102663221",  FUNC(j0) (-1.0), 0.765197686557966551449717526102663221L, 0, 0, 0);
  check_float ("j0 (0.0) == 1.0",  FUNC(j0) (0.0), 1.0, 0, 0, 0);
  check_float ("j0 (0.125) == 0.996097563041985204620768999453174712",  FUNC(j0) (0.125L), 0.996097563041985204620768999453174712L, 0, 0, 0);
  check_float ("j0 (0.75) == 0.864242275166648623555731103820923211",  FUNC(j0) (0.75L), 0.864242275166648623555731103820923211L, 0, 0, 0);
  check_float ("j0 (1.0) == 0.765197686557966551449717526102663221",  FUNC(j0) (1.0), 0.765197686557966551449717526102663221L, 0, 0, 0);
  check_float ("j0 (1.5) == 0.511827671735918128749051744283411720",  FUNC(j0) (1.5), 0.511827671735918128749051744283411720L, 0, 0, 0);
  check_float ("j0 (2.0) == 0.223890779141235668051827454649948626",  FUNC(j0) (2.0), 0.223890779141235668051827454649948626L, 0, 0, 0);
  check_float ("j0 (8.0) == 0.171650807137553906090869407851972001",  FUNC(j0) (8.0), 0.171650807137553906090869407851972001L, 0, 0, 0);
  check_float ("j0 (10.0) == -0.245935764451348335197760862485328754",  FUNC(j0) (10.0), -0.245935764451348335197760862485328754L, 0, 0, 0);
  check_float ("j0 (4.0) == -3.9714980986384737228659076845169804197562E-1",  FUNC(j0) (4.0), -3.9714980986384737228659076845169804197562E-1L, 0, 0, 0);
  check_float ("j0 (-4.0) == -3.9714980986384737228659076845169804197562E-1",  FUNC(j0) (-4.0), -3.9714980986384737228659076845169804197562E-1L, 0, 0, 0);

  print_max_error ("j0", 0, 0);
}


static void
j1_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(j1) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  /* j1 is the Bessel function of the first kind of order 1 */

  init_max_error ();

  check_float ("j1 (NaN) == NaN",  FUNC(j1) (nan_value), nan_value, 0, 0, 0);
  check_float ("j1 (inf) == 0",  FUNC(j1) (plus_infty), 0, 0, 0, 0);

  check_float ("j1 (-1.0) == -0.440050585744933515959682203718914913",  FUNC(j1) (-1.0), -0.440050585744933515959682203718914913L, 0, 0, 0);
  check_float ("j1 (0.0) == 0.0",  FUNC(j1) (0.0), 0.0, 0, 0, 0);
  check_float ("j1 (0.125) == 0.0623780091344946810942311355879361177",  FUNC(j1) (0.125L), 0.0623780091344946810942311355879361177L, 0, 0, 0);
  check_float ("j1 (0.75) == 0.349243602174862192523281016426251335",  FUNC(j1) (0.75L), 0.349243602174862192523281016426251335L, 0, 0, 0);
  check_float ("j1 (1.0) == 0.440050585744933515959682203718914913",  FUNC(j1) (1.0), 0.440050585744933515959682203718914913L, 0, 0, 0);
  check_float ("j1 (1.5) == 0.557936507910099641990121213156089400",  FUNC(j1) (1.5), 0.557936507910099641990121213156089400L, 0, 0, 0);
  check_float ("j1 (2.0) == 0.576724807756873387202448242269137087",  FUNC(j1) (2.0), 0.576724807756873387202448242269137087L, 0, 0, 0);
  check_float ("j1 (8.0) == 0.234636346853914624381276651590454612",  FUNC(j1) (8.0), 0.234636346853914624381276651590454612L, 0, 0, 0);
  check_float ("j1 (10.0) == 0.0434727461688614366697487680258592883",  FUNC(j1) (10.0), 0.0434727461688614366697487680258592883L, 0, 0, 0);

  print_max_error ("j1", 0, 0);
}

static void
jn_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(jn) (1, 1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  /* jn is the Bessel function of the first kind of order n.  */
  init_max_error ();

  /* jn (0, x) == j0 (x)  */
  check_float ("jn (0, NaN) == NaN",  FUNC(jn) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("jn (0, inf) == 0",  FUNC(jn) (0, plus_infty), 0, 0, 0, 0);
  check_float ("jn (0, -1.0) == 0.765197686557966551449717526102663221",  FUNC(jn) (0, -1.0), 0.765197686557966551449717526102663221L, 0, 0, 0);
  check_float ("jn (0, 0.0) == 1.0",  FUNC(jn) (0, 0.0), 1.0, 0, 0, 0);
  check_float ("jn (0, 0.125) == 0.996097563041985204620768999453174712",  FUNC(jn) (0, 0.125L), 0.996097563041985204620768999453174712L, 0, 0, 0);
  check_float ("jn (0, 0.75) == 0.864242275166648623555731103820923211",  FUNC(jn) (0, 0.75L), 0.864242275166648623555731103820923211L, 0, 0, 0);
  check_float ("jn (0, 1.0) == 0.765197686557966551449717526102663221",  FUNC(jn) (0, 1.0), 0.765197686557966551449717526102663221L, 0, 0, 0);
  check_float ("jn (0, 1.5) == 0.511827671735918128749051744283411720",  FUNC(jn) (0, 1.5), 0.511827671735918128749051744283411720L, 0, 0, 0);
  check_float ("jn (0, 2.0) == 0.223890779141235668051827454649948626",  FUNC(jn) (0, 2.0), 0.223890779141235668051827454649948626L, 0, 0, 0);
  check_float ("jn (0, 8.0) == 0.171650807137553906090869407851972001",  FUNC(jn) (0, 8.0), 0.171650807137553906090869407851972001L, 0, 0, 0);
  check_float ("jn (0, 10.0) == -0.245935764451348335197760862485328754",  FUNC(jn) (0, 10.0), -0.245935764451348335197760862485328754L, 0, 0, 0);
  check_float ("jn (0, 4.0) == -3.9714980986384737228659076845169804197562E-1",  FUNC(jn) (0, 4.0), -3.9714980986384737228659076845169804197562E-1L, 0, 0, 0);
  check_float ("jn (0, -4.0) == -3.9714980986384737228659076845169804197562E-1",  FUNC(jn) (0, -4.0), -3.9714980986384737228659076845169804197562E-1L, 0, 0, 0);

  /* jn (1, x) == j1 (x)  */
  check_float ("jn (1, NaN) == NaN",  FUNC(jn) (1, nan_value), nan_value, 0, 0, 0);
  check_float ("jn (1, inf) == 0",  FUNC(jn) (1, plus_infty), 0, 0, 0, 0);
  check_float ("jn (1, -1.0) == -0.440050585744933515959682203718914913",  FUNC(jn) (1, -1.0), -0.440050585744933515959682203718914913L, 0, 0, 0);
  check_float ("jn (1, 0.0) == 0.0",  FUNC(jn) (1, 0.0), 0.0, 0, 0, 0);
  check_float ("jn (1, 0.125) == 0.0623780091344946810942311355879361177",  FUNC(jn) (1, 0.125L), 0.0623780091344946810942311355879361177L, 0, 0, 0);
  check_float ("jn (1, 0.75) == 0.349243602174862192523281016426251335",  FUNC(jn) (1, 0.75L), 0.349243602174862192523281016426251335L, 0, 0, 0);
  check_float ("jn (1, 1.0) == 0.440050585744933515959682203718914913",  FUNC(jn) (1, 1.0), 0.440050585744933515959682203718914913L, 0, 0, 0);
  check_float ("jn (1, 1.5) == 0.557936507910099641990121213156089400",  FUNC(jn) (1, 1.5), 0.557936507910099641990121213156089400L, 0, 0, 0);
  check_float ("jn (1, 2.0) == 0.576724807756873387202448242269137087",  FUNC(jn) (1, 2.0), 0.576724807756873387202448242269137087L, 0, 0, 0);
  check_float ("jn (1, 8.0) == 0.234636346853914624381276651590454612",  FUNC(jn) (1, 8.0), 0.234636346853914624381276651590454612L, 0, 0, 0);
  check_float ("jn (1, 10.0) == 0.0434727461688614366697487680258592883",  FUNC(jn) (1, 10.0), 0.0434727461688614366697487680258592883L, 0, 0, 0);

  /* jn (3, x)  */
  check_float ("jn (3, NaN) == NaN",  FUNC(jn) (3, nan_value), nan_value, 0, 0, 0);
  check_float ("jn (3, inf) == 0",  FUNC(jn) (3, plus_infty), 0, 0, 0, 0);

  check_float ("jn (3, -1.0) == -0.0195633539826684059189053216217515083",  FUNC(jn) (3, -1.0), -0.0195633539826684059189053216217515083L, 0, 0, 0);
  check_float ("jn (3, 0.0) == 0.0",  FUNC(jn) (3, 0.0), 0.0, 0, 0, 0);
  check_float ("jn (3, 0.125) == 0.406503832554912875023029337653442868e-4",  FUNC(jn) (3, 0.125L), 0.406503832554912875023029337653442868e-4L, 0, 0, 0);
  check_float ("jn (3, 0.75) == 0.848438342327410884392755236884386804e-2",  FUNC(jn) (3, 0.75L), 0.848438342327410884392755236884386804e-2L, 0, 0, 0);
  check_float ("jn (3, 1.0) == 0.0195633539826684059189053216217515083",  FUNC(jn) (3, 1.0), 0.0195633539826684059189053216217515083L, 0, 0, 0);
  check_float ("jn (3, 2.0) == 0.128943249474402051098793332969239835",  FUNC(jn) (3, 2.0), 0.128943249474402051098793332969239835L, 0, 0, 0);
  check_float ("jn (3, 10.0) == 0.0583793793051868123429354784103409563",  FUNC(jn) (3, 10.0), 0.0583793793051868123429354784103409563L, 0, 0, 0);

  /*  jn (10, x)  */
  check_float ("jn (10, NaN) == NaN",  FUNC(jn) (10, nan_value), nan_value, 0, 0, 0);
  check_float ("jn (10, inf) == 0",  FUNC(jn) (10, plus_infty), 0, 0, 0, 0);

  check_float ("jn (10, -1.0) == 0.263061512368745320699785368779050294e-9",  FUNC(jn) (10, -1.0), 0.263061512368745320699785368779050294e-9L, 0, 0, 0);
  check_float ("jn (10, 0.0) == 0.0",  FUNC(jn) (10, 0.0), 0.0, 0, 0, 0);
  check_float ("jn (10, 0.125) == 0.250543369809369890173993791865771547e-18",  FUNC(jn) (10, 0.125L), 0.250543369809369890173993791865771547e-18L, 0, 0, 0);
  check_float ("jn (10, 0.75) == 0.149621713117596814698712483621682835e-10",  FUNC(jn) (10, 0.75L), 0.149621713117596814698712483621682835e-10L, 0, 0, 0);
  check_float ("jn (10, 1.0) == 0.263061512368745320699785368779050294e-9",  FUNC(jn) (10, 1.0), 0.263061512368745320699785368779050294e-9L, 0, 0, 0);
  check_float ("jn (10, 2.0) == 0.251538628271673670963516093751820639e-6",  FUNC(jn) (10, 2.0), 0.251538628271673670963516093751820639e-6L, 0, 0, 0);
  check_float ("jn (10, 10.0) == 0.207486106633358857697278723518753428",  FUNC(jn) (10, 10.0), 0.207486106633358857697278723518753428L, 0, 0, 0);

  print_max_error ("jn", 0, 0);
}


static void
ldexp_test (void)
{
  check_float ("jn (0, 0) == 0",  FUNC(ldexp) (0, 0), 0, 0, 0, 0);
  check_float ("jn (-0, 0) == -0",  FUNC(ldexp) (minus_zero, 0), minus_zero, 0, 0, 0);

  check_float ("jn (inf, 1) == inf",  FUNC(ldexp) (plus_infty, 1), plus_infty, 0, 0, 0);
  check_float ("jn (-inf, 1) == -inf",  FUNC(ldexp) (minus_infty, 1), minus_infty, 0, 0, 0);
  check_float ("jn (NaN, 1) == NaN",  FUNC(ldexp) (nan_value, 1), nan_value, 0, 0, 0);

  check_float ("jn (0.8, 4) == 12.8",  FUNC(ldexp) (0.8L, 4), 12.8L, 0, 0, 0);
  check_float ("jn (-0.854375, 5) == -27.34",  FUNC(ldexp) (-0.854375L, 5), -27.34L, 0, 0, 0);

  /* ldexp (x, 0) == x.  */
  check_float ("jn (1.0, 0) == 1.0",  FUNC(ldexp) (1.0L, 0L), 1.0L, 0, 0, 0);
}


static void
lgamma_test (void)
{
  errno = 0;
  FUNC(lgamma) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;
  feclearexcept (FE_ALL_EXCEPT);

  init_max_error ();

  signgam = 0;
  check_float ("lgamma (inf) == inf",  FUNC(lgamma) (plus_infty), plus_infty, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (0) == inf plus division by zero exception",  FUNC(lgamma) (0), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("signgam for lgamma(0) == 1", signgam, 1, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (-0) == inf plus division by zero exception",  FUNC(lgamma) (minus_zero), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("signgam for lgamma(-0) == -1", signgam, -1, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (NaN) == NaN",  FUNC(lgamma) (nan_value), nan_value, 0, 0, 0);

  /* lgamma (x) == +inf plus divide by zero exception for integer x <= 0.  */
  errno = 0;
  signgam = 0;
  check_float ("lgamma (-3) == inf plus division by zero exception",  FUNC(lgamma) (-3), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for lgamma(-integer) == ERANGE", errno, ERANGE, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (-inf) == inf",  FUNC(lgamma) (minus_infty), plus_infty, 0, 0, 0);

  signgam = 0;
  check_float ("lgamma (1) == 0",  FUNC(lgamma) (1), 0, 0, 0, 0);
  check_int ("lgamma (1) sets signgam to 1", signgam, 1, 0, 0, 0);

  signgam = 0;
  check_float ("lgamma (3) == M_LN2l",  FUNC(lgamma) (3), M_LN2l, 0, 0, 0);
  check_int ("lgamma (3) sets signgam to 1", signgam, 1, 0, 0, 0);

  signgam = 0;
  check_float ("lgamma (0.5) == log(sqrt(pi))",  FUNC(lgamma) (0.5), M_LOG_SQRT_PIl, 0, 0, 0);
  check_int ("lgamma (0.5) sets signgam to 1", signgam, 1, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (-0.5) == log(2*sqrt(pi))",  FUNC(lgamma) (-0.5), M_LOG_2_SQRT_PIl, 0, 0, 0);
  check_int ("lgamma (-0.5) sets signgam to -1", signgam, -1, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (0.7) == 0.260867246531666514385732417016759578",  FUNC(lgamma) (0.7L), 0.260867246531666514385732417016759578L, 0, 0, 0);
  check_int ("lgamma (0.7) sets signgam to 1", signgam, 1, 0, 0, 0);
  signgam = 0;
  check_float ("lgamma (1.2) == -0.853740900033158497197028392998854470e-1",  FUNC(lgamma) (1.2L), -0.853740900033158497197028392998854470e-1L, 0, 0, 0);
  check_int ("lgamma (1.2) sets signgam to 1", signgam, 1, 0, 0, 0);

  print_max_error ("lgamma", 0, 0);
}


static void
lrint_test (void)
{
  /* XXX this test is incomplete.  We need to have a way to specifiy
     the rounding method and test the critical cases.  So far, only
     unproblematic numbers are tested.  */

  init_max_error ();

  check_long ("lrint (0.0) == 0",  FUNC(lrint) (0.0), 0, 0, 0, 0);
  check_long ("lrint (-0) == 0",  FUNC(lrint) (minus_zero), 0, 0, 0, 0);
  check_long ("lrint (0.2) == 0",  FUNC(lrint) (0.2L), 0, 0, 0, 0);
  check_long ("lrint (-0.2) == 0",  FUNC(lrint) (-0.2L), 0, 0, 0, 0);

  check_long ("lrint (1.4) == 1",  FUNC(lrint) (1.4L), 1, 0, 0, 0);
  check_long ("lrint (-1.4) == -1",  FUNC(lrint) (-1.4L), -1, 0, 0, 0);

  check_long ("lrint (8388600.3) == 8388600",  FUNC(lrint) (8388600.3L), 8388600, 0, 0, 0);
  check_long ("lrint (-8388600.3) == -8388600",  FUNC(lrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("lrint (1071930.0008) == 1071930",  FUNC(lrint) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("lrint (1073741824.01) == 1073741824",  FUNC(lrint) (1073741824.01), 1073741824, 0, 0, 0);
# if LONG_MAX > 281474976710656
  check_long ("lrint (281474976710656.025) == 281474976710656",  FUNC(lrint) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
#endif

  print_max_error ("lrint", 0, 0);
}

#if 0
static void
lrint_test_tonearest (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TONEAREST))
    {
  check_long ("lrint_tonearest (0.0) == 0",  FUNC(lrint) (0.0), 0, 0, 0, 0);
  check_long ("lrint_tonearest (-0) == 0",  FUNC(lrint) (minus_zero), 0, 0, 0, 0);
  check_long ("lrint_tonearest (0.2) == 0",  FUNC(lrint) (0.2L), 0, 0, 0, 0);
  check_long ("lrint_tonearest (-0.2) == 0",  FUNC(lrint) (-0.2L), 0, 0, 0, 0);
  check_long ("lrint_tonearest (0.5) == 0",  FUNC(lrint) (0.5L), 0, 0, 0, 0);
  check_long ("lrint_tonearest (-0.5) == 0",  FUNC(lrint) (-0.5L), 0, 0, 0, 0);
  check_long ("lrint_tonearest (0.8) == 1",  FUNC(lrint) (0.8L), 1, 0, 0, 0);
  check_long ("lrint_tonearest (-0.8) == -1",  FUNC(lrint) (-0.8L), -1, 0, 0, 0);

  check_long ("lrint_tonearest (1.4) == 1",  FUNC(lrint) (1.4L), 1, 0, 0, 0);
  check_long ("lrint_tonearest (-1.4) == -1",  FUNC(lrint) (-1.4L), -1, 0, 0, 0);

  check_long ("lrint_tonearest (8388600.3) == 8388600",  FUNC(lrint) (8388600.3L), 8388600, 0, 0, 0);
  check_long ("lrint_tonearest (-8388600.3) == -8388600",  FUNC(lrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("lrint_tonearest (1071930.0008) == 1071930",  FUNC(lrint) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("lrint_tonearest (1073741824.01) == 1073741824",  FUNC(lrint) (1073741824.01), 1073741824, 0, 0, 0);
# if LONG_MAX > 281474976710656
  check_long ("lrint_tonearest (281474976710656.025) == 281474976710656",  FUNC(lrint) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("lrint_tonearest", 0, 0);
}


static void
lrint_test_towardzero (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TOWARDZERO))
    {
  check_long ("lrint_towardzero (0.0) == 0",  FUNC(lrint) (0.0), 0, 0, 0, 0);
  check_long ("lrint_towardzero (-0) == 0",  FUNC(lrint) (minus_zero), 0, 0, 0, 0);
  check_long ("lrint_towardzero (0.2) == 0",  FUNC(lrint) (0.2L), 0, 0, 0, 0);
  check_long ("lrint_towardzero (-0.2) == 0",  FUNC(lrint) (-0.2L), 0, 0, 0, 0);
  check_long ("lrint_towardzero (0.5) == 0",  FUNC(lrint) (0.5L), 0, 0, 0, 0);
  check_long ("lrint_towardzero (-0.5) == 0",  FUNC(lrint) (-0.5L), 0, 0, 0, 0);
  check_long ("lrint_towardzero (0.8) == 0",  FUNC(lrint) (0.8L), 0, 0, 0, 0);
  check_long ("lrint_towardzero (-0.8) == 0",  FUNC(lrint) (-0.8L), 0, 0, 0, 0);

  check_long ("lrint_towardzero (1.4) == 1",  FUNC(lrint) (1.4L), 1, 0, 0, 0);
  check_long ("lrint_towardzero (-1.4) == -1",  FUNC(lrint) (-1.4L), -1, 0, 0, 0);

  check_long ("lrint_towardzero (8388600.3) == 8388600",  FUNC(lrint) (8388600.3L), 8388600, 0, 0, 0);
  check_long ("lrint_towardzero (-8388600.3) == -8388600",  FUNC(lrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("lrint_towardzero (1071930.0008) == 1071930",  FUNC(lrint) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("lrint_towardzero (1073741824.01) == 1073741824",  FUNC(lrint) (1073741824.01), 1073741824, 0, 0, 0);
# if LONG_MAX > 281474976710656
  check_long ("lrint_towardzero (281474976710656.025) == 281474976710656",  FUNC(lrint) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("lrint_towardzero", 0, 0);
}


static void
lrint_test_downward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_DOWNWARD))
    {
  check_long ("lrint_downward (0.0) == 0",  FUNC(lrint) (0.0), 0, 0, 0, 0);
  check_long ("lrint_downward (-0) == 0",  FUNC(lrint) (minus_zero), 0, 0, 0, 0);
  check_long ("lrint_downward (0.2) == 0",  FUNC(lrint) (0.2L), 0, 0, 0, 0);
  check_long ("lrint_downward (-0.2) == -1",  FUNC(lrint) (-0.2L), -1, 0, 0, 0);
  check_long ("lrint_downward (0.5) == 0",  FUNC(lrint) (0.5L), 0, 0, 0, 0);
  check_long ("lrint_downward (-0.5) == -1",  FUNC(lrint) (-0.5L), -1, 0, 0, 0);
  check_long ("lrint_downward (0.8) == 0",  FUNC(lrint) (0.8L), 0, 0, 0, 0);
  check_long ("lrint_downward (-0.8) == -1",  FUNC(lrint) (-0.8L), -1, 0, 0, 0);

  check_long ("lrint_downward (1.4) == 1",  FUNC(lrint) (1.4L), 1, 0, 0, 0);
  check_long ("lrint_downward (-1.4) == -2",  FUNC(lrint) (-1.4L), -2, 0, 0, 0);

  check_long ("lrint_downward (8388600.3) == 8388600",  FUNC(lrint) (8388600.3L), 8388600, 0, 0, 0);
  check_long ("lrint_downward (-8388600.3) == -8388601",  FUNC(lrint) (-8388600.3L), -8388601, 0, 0, 0);

  check_long ("lrint_downward (1071930.0008) == 1071930",  FUNC(lrint) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("lrint_downward (1073741824.01) == 1073741824",  FUNC(lrint) (1073741824.01), 1073741824, 0, 0, 0);
# if LONG_MAX > 281474976710656
  check_long ("lrint_downward (281474976710656.025) == 281474976710656",  FUNC(lrint) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("lrint_downward", 0, 0);
}


static void
lrint_test_upward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_UPWARD))
    {
  check_long ("lrint_upward (0.0) == 0",  FUNC(lrint) (0.0), 0, 0, 0, 0);
  check_long ("lrint_upward (-0) == 0",  FUNC(lrint) (minus_zero), 0, 0, 0, 0);
  check_long ("lrint_upward (0.2) == 1",  FUNC(lrint) (0.2L), 1, 0, 0, 0);
  check_long ("lrint_upward (-0.2) == 0",  FUNC(lrint) (-0.2L), 0, 0, 0, 0);
  check_long ("lrint_upward (0.5) == 1",  FUNC(lrint) (0.5L), 1, 0, 0, 0);
  check_long ("lrint_upward (-0.5) == 0",  FUNC(lrint) (-0.5L), 0, 0, 0, 0);
  check_long ("lrint_upward (0.8) == 1",  FUNC(lrint) (0.8L), 1, 0, 0, 0);
  check_long ("lrint_upward (-0.8) == 0",  FUNC(lrint) (-0.8L), 0, 0, 0, 0);

  check_long ("lrint_upward (1.4) == 2",  FUNC(lrint) (1.4L), 2, 0, 0, 0);
  check_long ("lrint_upward (-1.4) == -1",  FUNC(lrint) (-1.4L), -1, 0, 0, 0);

  check_long ("lrint_upward (8388600.3) == 8388601",  FUNC(lrint) (8388600.3L), 8388601, 0, 0, 0);
  check_long ("lrint_upward (-8388600.3) == -8388600",  FUNC(lrint) (-8388600.3L), -8388600, 0, 0, 0);

#ifndef TEST_FLOAT
  check_long ("lrint_upward (1071930.0008) == 1071931",  FUNC(lrint) (1071930.0008), 1071931, 0, 0, 0);
  check_long ("lrint_upward (1073741824.01) == 1073741825",  FUNC(lrint) (1073741824.01), 1073741825, 0, 0, 0);
# if LONG_MAX > 281474976710656 && defined (TEST_LDOUBLE)
  check_long ("lrint_upward (281474976710656.025) == 281474976710656",  FUNC(lrint) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("lrint_upward", 0, 0);
}
#endif

static void
llrint_test (void)
{
  /* XXX this test is incomplete.  We need to have a way to specifiy
     the rounding method and test the critical cases.  So far, only
     unproblematic numbers are tested.  */

  init_max_error ();

  check_longlong ("llrint (0.0) == 0",  FUNC(llrint) (0.0), 0, 0, 0, 0);
  check_longlong ("llrint (-0) == 0",  FUNC(llrint) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llrint (0.2) == 0",  FUNC(llrint) (0.2L), 0, 0, 0, 0);
  check_longlong ("llrint (-0.2) == 0",  FUNC(llrint) (-0.2L), 0, 0, 0, 0);

  check_longlong ("llrint (1.4) == 1",  FUNC(llrint) (1.4L), 1, 0, 0, 0);
  check_longlong ("llrint (-1.4) == -1",  FUNC(llrint) (-1.4L), -1, 0, 0, 0);

  check_longlong ("llrint (8388600.3) == 8388600",  FUNC(llrint) (8388600.3L), 8388600, 0, 0, 0);
  check_longlong ("llrint (-8388600.3) == -8388600",  FUNC(llrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("llrint (1071930.0008) == 1071930",  FUNC(llrint) (1071930.0008), 1071930, 0, 0, 0);

  /* Test boundary conditions.  */
  /* 0x1FFFFF */
  check_longlong ("llrint (2097151.0) == 2097151LL",  FUNC(llrint) (2097151.0), 2097151LL, 0, 0, 0);
  /* 0x800000 */
  check_longlong ("llrint (8388608.0) == 8388608LL",  FUNC(llrint) (8388608.0), 8388608LL, 0, 0, 0);
  /* 0x1000000 */
  check_longlong ("llrint (16777216.0) == 16777216LL",  FUNC(llrint) (16777216.0), 16777216LL, 0, 0, 0);
  /* 0x20000000000 */
  check_longlong ("llrint (2199023255552.0) == 2199023255552LL",  FUNC(llrint) (2199023255552.0), 2199023255552LL, 0, 0, 0);
  /* 0x40000000000 */
  check_longlong ("llrint (4398046511104.0) == 4398046511104LL",  FUNC(llrint) (4398046511104.0), 4398046511104LL, 0, 0, 0);
  /* 0x1000000000000 */
  check_longlong ("llrint (281474976710656.0) == 281474976710656LL",  FUNC(llrint) (281474976710656.0), 281474976710656LL, 0, 0, 0);
  /* 0x10000000000000 */
  check_longlong ("llrint (4503599627370496.0) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
  /* 0x10000080000000 */
  check_longlong ("llrint (4503601774854144.0) == 4503601774854144LL",  FUNC(llrint) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
  /* 0x20000000000000 */
  check_longlong ("llrint (9007199254740992.0) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
  /* 0x80000000000000 */
  check_longlong ("llrint (36028797018963968.0) == 36028797018963968LL",  FUNC(llrint) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
  /* 0x100000000000000 */
  check_longlong ("llrint (72057594037927936.0) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);
#ifdef TEST_LDOUBLE
  /* The input can only be represented in long double.  */
  check_longlong ("llrint (4503599627370495.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370496.25) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.25L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370496.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370496.75) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.75L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370497.5) == 4503599627370498LL",  FUNC(llrint) (4503599627370497.5L), 4503599627370498LL, 0, 0, 0);

  check_longlong ("llrint (-4503599627370495.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370496.25) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.25L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370496.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370496.75) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.75L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370497.5) == -4503599627370498LL",  FUNC(llrint) (-4503599627370497.5L), -4503599627370498LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint (4503599627370495.4999999999999) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.4999999999999L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370496.4999999999999) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370497.4999999999999) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370494.5000000000001) == 4503599627370495LL",  FUNC(llrint) (4503599627370494.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370495.5000000000001) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5000000000001L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (4503599627370496.5000000000001) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.5000000000001L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint (-4503599627370495.4999999999999) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.4999999999999L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370496.4999999999999) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370497.4999999999999) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370494.5000000000001) == -4503599627370495LL",  FUNC(llrint) (-4503599627370494.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370495.5000000000001) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5000000000001L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint (-4503599627370496.5000000000001) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.5000000000001L), -4503599627370497LL, 0, 0, 0);
#endif

  check_longlong ("llrint (9007199254740991.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740992.25) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.25L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740992.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740992.75) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.75L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740993.5) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint (-9007199254740991.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740992.25) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.25L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740992.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740992.75) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.75L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740993.5) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5L), -9007199254740994LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint (9007199254740991.4999999999999) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.4999999999999L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740992.4999999999999) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740993.4999999999999) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740991.5000000000001) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740992.5000000000001) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.5000000000001L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (9007199254740993.5000000000001) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5000000000001L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint (-9007199254740991.4999999999999) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.4999999999999L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740992.4999999999999) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740993.4999999999999) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740991.5000000000001) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740992.5000000000001) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.5000000000001L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint (-9007199254740993.5000000000001) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5000000000001L), -9007199254740994LL, 0, 0, 0);
#endif

  check_longlong ("llrint (72057594037927935.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927935.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (72057594037927936.25) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.25L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (72057594037927936.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (72057594037927936.75) == 72057594037927937LL",  FUNC(llrint) (72057594037927936.75L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint (72057594037927937.5) == 72057594037927938LL",  FUNC(llrint) (72057594037927937.5L), 72057594037927938LL, 0, 0, 0);

  check_longlong ("llrint (-72057594037927935.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927935.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (-72057594037927936.25) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.25L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (-72057594037927936.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint (-72057594037927936.75) == -72057594037927937LL",  FUNC(llrint) (-72057594037927936.75L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint (-72057594037927937.5) == -72057594037927938LL",  FUNC(llrint) (-72057594037927937.5L), -72057594037927938LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint (9223372036854775805.5) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775805.5L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775805.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775805.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (9223372036854775806.0) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.0L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775806.0) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.0L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (9223372036854775806.25) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.25L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775806.25) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.25L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (9223372036854775806.5) == 9223372036854775806",  FUNC(llrint) (9223372036854775806.5L), 9223372036854775806L, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775806.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint (9223372036854775806.75) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775806.75L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775806.75) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775806.75L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
# endif
#endif

  print_max_error ("llrint", 0, 0);
}

#if 0
static void
llrint_test_tonearest (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TONEAREST))
    {
  check_longlong ("llrint_tonearest (0.0) == 0",  FUNC(llrint) (0.0), 0, 0, 0, 0);
  check_longlong ("llrint_tonearest (-0) == 0",  FUNC(llrint) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llrint_tonearest (0.2) == 0",  FUNC(llrint) (0.2L), 0, 0, 0, 0);
  check_longlong ("llrint_tonearest (-0.2) == 0",  FUNC(llrint) (-0.2L), 0, 0, 0, 0);

  check_longlong ("llrint_tonearest (1.4) == 1",  FUNC(llrint) (1.4L), 1, 0, 0, 0);
  check_longlong ("llrint_tonearest (-1.4) == -1",  FUNC(llrint) (-1.4L), -1, 0, 0, 0);

  check_longlong ("llrint_tonearest (8388600.3) == 8388600",  FUNC(llrint) (8388600.3L), 8388600, 0, 0, 0);
  check_longlong ("llrint_tonearest (-8388600.3) == -8388600",  FUNC(llrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("llrint_tonearest (1071930.0008) == 1071930",  FUNC(llrint) (1071930.0008), 1071930, 0, 0, 0);

      /* Test boundary conditions.  */
      /* 0x1FFFFF */
  check_longlong ("llrint_tonearest (2097151.0) == 2097151LL",  FUNC(llrint) (2097151.0), 2097151LL, 0, 0, 0);
      /* 0x800000 */
  check_longlong ("llrint_tonearest (8388608.0) == 8388608LL",  FUNC(llrint) (8388608.0), 8388608LL, 0, 0, 0);
      /* 0x1000000 */
  check_longlong ("llrint_tonearest (16777216.0) == 16777216LL",  FUNC(llrint) (16777216.0), 16777216LL, 0, 0, 0);
      /* 0x20000000000 */
  check_longlong ("llrint_tonearest (2199023255552.0) == 2199023255552LL",  FUNC(llrint) (2199023255552.0), 2199023255552LL, 0, 0, 0);
      /* 0x40000000000 */
  check_longlong ("llrint_tonearest (4398046511104.0) == 4398046511104LL",  FUNC(llrint) (4398046511104.0), 4398046511104LL, 0, 0, 0);
      /* 0x1000000000000 */
  check_longlong ("llrint_tonearest (281474976710656.0) == 281474976710656LL",  FUNC(llrint) (281474976710656.0), 281474976710656LL, 0, 0, 0);
      /* 0x10000000000000 */
  check_longlong ("llrint_tonearest (4503599627370496.0) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
      /* 0x10000080000000 */
  check_longlong ("llrint_tonearest (4503601774854144.0) == 4503601774854144LL",  FUNC(llrint) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
      /* 0x20000000000000 */
  check_longlong ("llrint_tonearest (9007199254740992.0) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
      /* 0x80000000000000 */
  check_longlong ("llrint_tonearest (36028797018963968.0) == 36028797018963968LL",  FUNC(llrint) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
      /* 0x100000000000000 */
  check_longlong ("llrint_tonearest (72057594037927936.0) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The input can only be represented in long double.  */
  check_longlong ("llrint_tonearest (4503599627370495.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370496.25) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.25L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370496.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370496.75) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.75L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370497.5) == 4503599627370498LL",  FUNC(llrint) (4503599627370497.5L), 4503599627370498LL, 0, 0, 0);

  check_longlong ("llrint_tonearest (-4503599627370495.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370496.25) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.25L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370496.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370496.75) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.75L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370497.5) == -4503599627370498LL",  FUNC(llrint) (-4503599627370497.5L), -4503599627370498LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_tonearest (4503599627370495.4999999999999) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.4999999999999L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370496.4999999999999) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370497.4999999999999) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370494.5000000000001) == 4503599627370495LL",  FUNC(llrint) (4503599627370494.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370495.5000000000001) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5000000000001L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (4503599627370496.5000000000001) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.5000000000001L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_tonearest (-4503599627370495.4999999999999) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.4999999999999L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370496.4999999999999) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370497.4999999999999) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370494.5000000000001) == -4503599627370495LL",  FUNC(llrint) (-4503599627370494.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370495.5000000000001) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5000000000001L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-4503599627370496.5000000000001) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.5000000000001L), -4503599627370497LL, 0, 0, 0);
#endif

  check_longlong ("llrint_tonearest (9007199254740991.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740992.25) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.25L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740992.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740992.75) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.75L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740993.5) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_tonearest (-9007199254740991.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740992.25) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.25L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740992.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740992.75) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.75L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740993.5) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5L), -9007199254740994LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_tonearest (9007199254740991.4999999999999) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.4999999999999L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740992.4999999999999) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740993.4999999999999) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740991.5000000000001) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740992.5000000000001) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.5000000000001L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9007199254740993.5000000000001) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5000000000001L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_tonearest (-9007199254740991.4999999999999) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.4999999999999L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740992.4999999999999) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740993.4999999999999) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740991.5000000000001) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740992.5000000000001) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.5000000000001L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9007199254740993.5000000000001) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5000000000001L), -9007199254740994LL, 0, 0, 0);
#endif

  check_longlong ("llrint_tonearest (72057594037927935.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927935.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (72057594037927936.25) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.25L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (72057594037927936.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (72057594037927936.75) == 72057594037927937LL",  FUNC(llrint) (72057594037927936.75L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (72057594037927937.5) == 72057594037927938LL",  FUNC(llrint) (72057594037927937.5L), 72057594037927938LL, 0, 0, 0);

  check_longlong ("llrint_tonearest (-72057594037927935.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927935.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-72057594037927936.25) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.25L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-72057594037927936.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-72057594037927936.75) == -72057594037927937LL",  FUNC(llrint) (-72057594037927936.75L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-72057594037927937.5) == -72057594037927938LL",  FUNC(llrint) (-72057594037927937.5L), -72057594037927938LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_tonearest (9223372036854775805.5) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775805.5L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775805.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775805.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9223372036854775806.0) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.0L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775806.0) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.0L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9223372036854775806.25) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.25L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775806.25) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.25L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9223372036854775806.5) == 9223372036854775806",  FUNC(llrint) (9223372036854775806.5L), 9223372036854775806L, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775806.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9223372036854775806.75) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775806.75L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775806.75) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775806.75L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_tonearest (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("llrint_tonearest", 0, 0);
}

static void
llrint_test_towardzero (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TOWARDZERO))
    {
  check_longlong ("llrint_towardzero (0.0) == 0",  FUNC(llrint) (0.0), 0, 0, 0, 0);
  check_longlong ("llrint_towardzero (-0) == 0",  FUNC(llrint) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llrint_towardzero (0.2) == 0",  FUNC(llrint) (0.2L), 0, 0, 0, 0);
  check_longlong ("llrint_towardzero (-0.2) == 0",  FUNC(llrint) (-0.2L), 0, 0, 0, 0);

  check_longlong ("llrint_towardzero (1.4) == 1",  FUNC(llrint) (1.4L), 1, 0, 0, 0);
  check_longlong ("llrint_towardzero (-1.4) == -1",  FUNC(llrint) (-1.4L), -1, 0, 0, 0);

  check_longlong ("llrint_towardzero (8388600.3) == 8388600",  FUNC(llrint) (8388600.3L), 8388600, 0, 0, 0);
  check_longlong ("llrint_towardzero (-8388600.3) == -8388600",  FUNC(llrint) (-8388600.3L), -8388600, 0, 0, 0);

  check_long ("llrint_towardzero (1071930.0008) == 1071930",  FUNC(llrint) (1071930.0008), 1071930, 0, 0, 0);

      /* Test boundary conditions.  */
      /* 0x1FFFFF */
  check_longlong ("llrint_towardzero (2097151.0) == 2097151LL",  FUNC(llrint) (2097151.0), 2097151LL, 0, 0, 0);
      /* 0x800000 */
  check_longlong ("llrint_towardzero (8388608.0) == 8388608LL",  FUNC(llrint) (8388608.0), 8388608LL, 0, 0, 0);
      /* 0x1000000 */
  check_longlong ("llrint_towardzero (16777216.0) == 16777216LL",  FUNC(llrint) (16777216.0), 16777216LL, 0, 0, 0);
      /* 0x20000000000 */
  check_longlong ("llrint_towardzero (2199023255552.0) == 2199023255552LL",  FUNC(llrint) (2199023255552.0), 2199023255552LL, 0, 0, 0);
      /* 0x40000000000 */
  check_longlong ("llrint_towardzero (4398046511104.0) == 4398046511104LL",  FUNC(llrint) (4398046511104.0), 4398046511104LL, 0, 0, 0);
      /* 0x1000000000000 */
  check_longlong ("llrint_towardzero (281474976710656.0) == 281474976710656LL",  FUNC(llrint) (281474976710656.0), 281474976710656LL, 0, 0, 0);
      /* 0x10000000000000 */
  check_longlong ("llrint_towardzero (4503599627370496.0) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
      /* 0x10000080000000 */
  check_longlong ("llrint_towardzero (4503601774854144.0) == 4503601774854144LL",  FUNC(llrint) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
      /* 0x20000000000000 */
  check_longlong ("llrint_towardzero (9007199254740992.0) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
      /* 0x80000000000000 */
  check_longlong ("llrint_towardzero (36028797018963968.0) == 36028797018963968LL",  FUNC(llrint) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
      /* 0x100000000000000 */
  check_longlong ("llrint_towardzero (72057594037927936.0) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The input can only be represented in long double.  */
  check_longlong ("llrint_towardzero (4503599627370495.5) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.5L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370496.25) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.25L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370496.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370496.75) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.75L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370497.5) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.5L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_towardzero (-4503599627370495.5) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.5L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370496.25) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.25L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370496.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370496.75) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.75L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370497.5) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.5L), -4503599627370497LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_towardzero (4503599627370495.4999999999999) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.4999999999999L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370496.4999999999999) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370497.4999999999999) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370494.5000000000001) == 4503599627370494LL",  FUNC(llrint) (4503599627370494.5000000000001L), 4503599627370494LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370495.5000000000001) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (4503599627370496.5000000000001) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5000000000001L), 4503599627370496LL, 0, 0, 0);

  check_longlong ("llrint_towardzero (-4503599627370495.4999999999999) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.4999999999999L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370496.4999999999999) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370497.4999999999999) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370494.5000000000001) == -4503599627370494LL",  FUNC(llrint) (-4503599627370494.5000000000001L), -4503599627370494LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370495.5000000000001) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-4503599627370496.5000000000001) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5000000000001L), -4503599627370496LL, 0, 0, 0);
#endif

  check_longlong ("llrint_towardzero (9007199254740991.5) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.5L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740992.25) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.25L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740992.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740992.75) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.75L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740993.5) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.5L), 9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_towardzero (-9007199254740991.5) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.5L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740992.25) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.25L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740992.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740992.75) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.75L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740993.5) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.5L), -9007199254740993LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_towardzero (9007199254740991.4999999999999) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.4999999999999L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740992.4999999999999) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740993.4999999999999) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740991.5000000000001) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.5000000000001L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740992.5000000000001) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9007199254740993.5000000000001) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.5000000000001L), 9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_towardzero (-9007199254740991.4999999999999) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.4999999999999L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740992.4999999999999) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740993.4999999999999) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740991.5000000000001) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.5000000000001L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740992.5000000000001) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9007199254740993.5000000000001) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.5000000000001L), -9007199254740993LL, 0, 0, 0);
#endif

  check_longlong ("llrint_towardzero (72057594037927935.5) == 72057594037927935LL",  FUNC(llrint) (72057594037927935.5L), 72057594037927935LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (72057594037927936.25) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.25L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (72057594037927936.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (72057594037927936.75) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.75L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (72057594037927937.5) == 72057594037927937LL",  FUNC(llrint) (72057594037927937.5L), 72057594037927937LL, 0, 0, 0);

  check_longlong ("llrint_towardzero (-72057594037927935.5) == -72057594037927935LL",  FUNC(llrint) (-72057594037927935.5L), -72057594037927935LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-72057594037927936.25) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.25L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-72057594037927936.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-72057594037927936.75) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.75L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-72057594037927937.5) == -72057594037927937LL",  FUNC(llrint) (-72057594037927937.5L), -72057594037927937LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_towardzero (9223372036854775805.5) == 9223372036854775805LL",  FUNC(llrint) (9223372036854775805.5L), 9223372036854775805LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775805.5) == -9223372036854775805LL",  FUNC(llrint) (-9223372036854775805.5L), -9223372036854775805LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9223372036854775806.0) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.0L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775806.0) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.0L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9223372036854775806.25) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.25L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775806.25) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.25L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9223372036854775806.5) == 9223372036854775806",  FUNC(llrint) (9223372036854775806.5L), 9223372036854775806L, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775806.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9223372036854775806.75) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.75L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775806.75) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.75L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_towardzero (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("llrint_towardzero", 0, 0);
}

static void
llrint_test_downward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_DOWNWARD))
    {
  check_longlong ("llrint_downward (0.0) == 0",  FUNC(llrint) (0.0), 0, 0, 0, 0);
  check_longlong ("llrint_downward (-0) == 0",  FUNC(llrint) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llrint_downward (0.2) == 0",  FUNC(llrint) (0.2L), 0, 0, 0, 0);
  check_longlong ("llrint_downward (-0.2) == -1",  FUNC(llrint) (-0.2L), -1, 0, 0, 0);

  check_longlong ("llrint_downward (1.4) == 1",  FUNC(llrint) (1.4L), 1, 0, 0, 0);
  check_longlong ("llrint_downward (-1.4) == -2",  FUNC(llrint) (-1.4L), -2, 0, 0, 0);

  check_longlong ("llrint_downward (8388600.3) == 8388600",  FUNC(llrint) (8388600.3L), 8388600, 0, 0, 0);
  check_longlong ("llrint_downward (-8388600.3) == -8388601",  FUNC(llrint) (-8388600.3L), -8388601, 0, 0, 0);

  check_long ("llrint_downward (1071930.0008) == 1071930",  FUNC(llrint) (1071930.0008), 1071930, 0, 0, 0);

      /* Test boundary conditions.  */
      /* 0x1FFFFF */
  check_longlong ("llrint_downward (2097151.0) == 2097151LL",  FUNC(llrint) (2097151.0), 2097151LL, 0, 0, 0);
      /* 0x800000 */
  check_longlong ("llrint_downward (8388608.0) == 8388608LL",  FUNC(llrint) (8388608.0), 8388608LL, 0, 0, 0);
      /* 0x1000000 */
  check_longlong ("llrint_downward (16777216.0) == 16777216LL",  FUNC(llrint) (16777216.0), 16777216LL, 0, 0, 0);
      /* 0x20000000000 */
  check_longlong ("llrint_downward (2199023255552.0) == 2199023255552LL",  FUNC(llrint) (2199023255552.0), 2199023255552LL, 0, 0, 0);
      /* 0x40000000000 */
  check_longlong ("llrint_downward (4398046511104.0) == 4398046511104LL",  FUNC(llrint) (4398046511104.0), 4398046511104LL, 0, 0, 0);
      /* 0x1000000000000 */
  check_longlong ("llrint_downward (281474976710656.0) == 281474976710656LL",  FUNC(llrint) (281474976710656.0), 281474976710656LL, 0, 0, 0);
      /* 0x10000000000000 */
  check_longlong ("llrint_downward (4503599627370496.0) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
      /* 0x10000080000000 */
  check_longlong ("llrint_downward (4503601774854144.0) == 4503601774854144LL",  FUNC(llrint) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
      /* 0x20000000000000 */
  check_longlong ("llrint_downward (9007199254740992.0) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
      /* 0x80000000000000 */
  check_longlong ("llrint_downward (36028797018963968.0) == 36028797018963968LL",  FUNC(llrint) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
      /* 0x100000000000000 */
  check_longlong ("llrint_downward (72057594037927936.0) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The input can only be represented in long double.  */
  check_longlong ("llrint_downward (4503599627370495.5) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.5L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370496.25) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.25L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370496.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370496.75) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.75L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370497.5) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.5L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_downward (4503599627370495.4999999999999) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.4999999999999L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370496.4999999999999) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370497.4999999999999) == 4503599627370497LL",  FUNC(llrint) (4503599627370497.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370494.5000000000001) == 4503599627370494LL",  FUNC(llrint) (4503599627370494.5000000000001L), 4503599627370494LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370495.5000000000001) == 4503599627370495LL",  FUNC(llrint) (4503599627370495.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_downward (4503599627370496.5000000000001) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.5000000000001L), 4503599627370496LL, 0, 0, 0);

  check_longlong ("llrint_downward (-4503599627370495.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370496.25) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.25L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370496.5) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.5L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370496.75) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.75L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370497.5) == -4503599627370498LL",  FUNC(llrint) (-4503599627370497.5L), -4503599627370498LL, 0, 0, 0);

  check_longlong ("llrint_downward (-4503599627370495.4999999999999) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370496.4999999999999) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370497.4999999999999) == -4503599627370498LL",  FUNC(llrint) (-4503599627370497.4999999999999L), -4503599627370498LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370494.5000000000001) == -4503599627370495LL",  FUNC(llrint) (-4503599627370494.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370495.5000000000001) == -4503599627370496LL",  FUNC(llrint) (-4503599627370495.5000000000001L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_downward (-4503599627370496.5000000000001) == -4503599627370497LL",  FUNC(llrint) (-4503599627370496.5000000000001L), -4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_downward (9007199254740991.5) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.5L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740992.25) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.25L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740992.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740992.75) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.75L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740993.5) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.5L), 9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_downward (9007199254740991.4999999999999) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.4999999999999L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740992.4999999999999) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740993.4999999999999) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740991.5000000000001) == 9007199254740991LL",  FUNC(llrint) (9007199254740991.5000000000001L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740992.5000000000001) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (9007199254740993.5000000000001) == 9007199254740993LL",  FUNC(llrint) (9007199254740993.5000000000001L), 9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_downward (-9007199254740991.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740992.25) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.25L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740992.5) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.5L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740992.75) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.75L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740993.5) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5L), -9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_downward (-9007199254740991.4999999999999) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740992.4999999999999) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740993.4999999999999) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.4999999999999L), -9007199254740994LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740991.5000000000001) == -9007199254740992LL",  FUNC(llrint) (-9007199254740991.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740992.5000000000001) == -9007199254740993LL",  FUNC(llrint) (-9007199254740992.5000000000001L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9007199254740993.5000000000001) == -9007199254740994LL",  FUNC(llrint) (-9007199254740993.5000000000001L), -9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_downward (72057594037927935.5) == 72057594037927935LL",  FUNC(llrint) (72057594037927935.5L), 72057594037927935LL, 0, 0, 0);
  check_longlong ("llrint_downward (72057594037927936.25) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.25L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_downward (72057594037927936.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_downward (72057594037927936.75) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.75L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_downward (72057594037927937.5) == 72057594037927937LL",  FUNC(llrint) (72057594037927937.5L), 72057594037927937LL, 0, 0, 0);

  check_longlong ("llrint_downward (-72057594037927935.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927935.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_downward (-72057594037927936.25) == -72057594037927937LL",  FUNC(llrint) (-72057594037927936.25L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_downward (-72057594037927936.5) == -72057594037927937LL",  FUNC(llrint) (-72057594037927936.5L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_downward (-72057594037927936.75) == -72057594037927937LL",  FUNC(llrint) (-72057594037927936.75L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_downward (-72057594037927937.5) == -72057594037927938LL",  FUNC(llrint) (-72057594037927937.5L), -72057594037927938LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_downward (9223372036854775805.5) == 9223372036854775805LL",  FUNC(llrint) (9223372036854775805.5L), 9223372036854775805LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775805.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775805.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_downward (9223372036854775806.0) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.0L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775806.0) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.0L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_downward (9223372036854775806.25) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.25L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775806.25) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775806.25L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_downward (9223372036854775806.5) == 9223372036854775806",  FUNC(llrint) (9223372036854775806.5L), 9223372036854775806L, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775806.5) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775806.5L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_downward (9223372036854775806.75) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.75L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775806.75) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775806.75L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_downward (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_downward (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("llrint_downward", 0, 0);
}

static void
llrint_test_upward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_UPWARD))
    {
  check_longlong ("llrint_upward (0.0) == 0",  FUNC(llrint) (0.0), 0, 0, 0, 0);
  check_longlong ("llrint_upward (-0) == 0",  FUNC(llrint) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llrint_upward (0.2) == 1",  FUNC(llrint) (0.2L), 1, 0, 0, 0);
  check_longlong ("llrint_upward (-0.2) == 0",  FUNC(llrint) (-0.2L), 0, 0, 0, 0);

  check_longlong ("llrint_upward (1.4) == 2",  FUNC(llrint) (1.4L), 2, 0, 0, 0);
  check_longlong ("llrint_upward (-1.4) == -1",  FUNC(llrint) (-1.4L), -1, 0, 0, 0);

  check_longlong ("llrint_upward (8388600.3) == 8388601",  FUNC(llrint) (8388600.3L), 8388601, 0, 0, 0);
  check_longlong ("llrint_upward (-8388600.3) == -8388600",  FUNC(llrint) (-8388600.3L), -8388600, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("llrint_upward (1071930.0008) == 1071931",  FUNC(llrint) (1071930.0008), 1071931, 0, 0, 0);
#endif
      /* Test boundary conditions.  */
      /* 0x1FFFFF */
  check_longlong ("llrint_upward (2097151.0) == 2097151LL",  FUNC(llrint) (2097151.0), 2097151LL, 0, 0, 0);
      /* 0x800000 */
  check_longlong ("llrint_upward (8388608.0) == 8388608LL",  FUNC(llrint) (8388608.0), 8388608LL, 0, 0, 0);
      /* 0x1000000 */
  check_longlong ("llrint_upward (16777216.0) == 16777216LL",  FUNC(llrint) (16777216.0), 16777216LL, 0, 0, 0);
      /* 0x20000000000 */
  check_longlong ("llrint_upward (2199023255552.0) == 2199023255552LL",  FUNC(llrint) (2199023255552.0), 2199023255552LL, 0, 0, 0);
      /* 0x40000000000 */
  check_longlong ("llrint_upward (4398046511104.0) == 4398046511104LL",  FUNC(llrint) (4398046511104.0), 4398046511104LL, 0, 0, 0);
      /* 0x1000000000000 */
  check_longlong ("llrint_upward (281474976710656.0) == 281474976710656LL",  FUNC(llrint) (281474976710656.0), 281474976710656LL, 0, 0, 0);
      /* 0x10000000000000 */
  check_longlong ("llrint_upward (4503599627370496.0) == 4503599627370496LL",  FUNC(llrint) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
      /* 0x10000080000000 */
  check_longlong ("llrint_upward (4503601774854144.0) == 4503601774854144LL",  FUNC(llrint) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
      /* 0x20000000000000 */
  check_longlong ("llrint_upward (9007199254740992.0) == 9007199254740992LL",  FUNC(llrint) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
      /* 0x80000000000000 */
  check_longlong ("llrint_upward (36028797018963968.0) == 36028797018963968LL",  FUNC(llrint) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
      /* 0x100000000000000 */
  check_longlong ("llrint_upward (72057594037927936.0) == 72057594037927936LL",  FUNC(llrint) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The input can only be represented in long double.  */
  check_longlong ("llrint_upward (4503599627370495.5) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370496.25) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.25L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370496.5) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.5L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370496.75) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.75L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370497.5) == 4503599627370498LL",  FUNC(llrint) (4503599627370497.5L), 4503599627370498LL, 0, 0, 0);

  check_longlong ("llrint_upward (4503599627370495.4999999999999) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370496.4999999999999) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370497.4999999999999) == 4503599627370498LL",  FUNC(llrint) (4503599627370497.4999999999999L), 4503599627370498LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370494.5000000000001) == 4503599627370495LL",  FUNC(llrint) (4503599627370494.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370495.5000000000001) == 4503599627370496LL",  FUNC(llrint) (4503599627370495.5000000000001L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (4503599627370496.5000000000001) == 4503599627370497LL",  FUNC(llrint) (4503599627370496.5000000000001L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_upward (-4503599627370495.5) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.5L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370496.25) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.25L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370496.5) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370496.75) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.75L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370497.5) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.5L), -4503599627370497LL, 0, 0, 0);

  check_longlong ("llrint_upward (-4503599627370495.4999999999999) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.4999999999999L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370496.4999999999999) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370497.4999999999999) == -4503599627370497LL",  FUNC(llrint) (-4503599627370497.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370494.5000000000001) == -4503599627370494LL",  FUNC(llrint) (-4503599627370494.5000000000001L), -4503599627370494LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370495.5000000000001) == -4503599627370495LL",  FUNC(llrint) (-4503599627370495.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llrint_upward (-4503599627370496.5000000000001) == -4503599627370496LL",  FUNC(llrint) (-4503599627370496.5000000000001L), -4503599627370496LL, 0, 0, 0);

  check_longlong ("llrint_upward (9007199254740991.5) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740992.25) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.25L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740992.5) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.5L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740992.75) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.75L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740993.5) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_upward (9007199254740991.4999999999999) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740992.4999999999999) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740993.4999999999999) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.4999999999999L), 9007199254740994LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740991.5000000000001) == 9007199254740992LL",  FUNC(llrint) (9007199254740991.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740992.5000000000001) == 9007199254740993LL",  FUNC(llrint) (9007199254740992.5000000000001L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (9007199254740993.5000000000001) == 9007199254740994LL",  FUNC(llrint) (9007199254740993.5000000000001L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llrint_upward (-9007199254740991.5) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.5L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740992.25) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.25L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740992.5) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740992.75) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.75L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740993.5) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.5L), -9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_upward (-9007199254740991.4999999999999) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.4999999999999L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740992.4999999999999) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740993.4999999999999) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740991.5000000000001) == -9007199254740991LL",  FUNC(llrint) (-9007199254740991.5000000000001L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740992.5000000000001) == -9007199254740992LL",  FUNC(llrint) (-9007199254740992.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9007199254740993.5000000000001) == -9007199254740993LL",  FUNC(llrint) (-9007199254740993.5000000000001L), -9007199254740993LL, 0, 0, 0);

  check_longlong ("llrint_upward (72057594037927935.5) == 72057594037927936LL",  FUNC(llrint) (72057594037927935.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_upward (72057594037927936.25) == 72057594037927937LL",  FUNC(llrint) (72057594037927936.25L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_upward (72057594037927936.5) == 72057594037927937LL",  FUNC(llrint) (72057594037927936.5L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_upward (72057594037927936.75) == 72057594037927937LL",  FUNC(llrint) (72057594037927936.75L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llrint_upward (72057594037927937.5) == 72057594037927938LL",  FUNC(llrint) (72057594037927937.5L), 72057594037927938LL, 0, 0, 0);

  check_longlong ("llrint_upward (-72057594037927935.5) == -72057594037927935LL",  FUNC(llrint) (-72057594037927935.5L), -72057594037927935LL, 0, 0, 0);
  check_longlong ("llrint_upward (-72057594037927936.25) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.25L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_upward (-72057594037927936.5) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_upward (-72057594037927936.75) == -72057594037927936LL",  FUNC(llrint) (-72057594037927936.75L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llrint_upward (-72057594037927937.5) == -72057594037927937LL",  FUNC(llrint) (-72057594037927937.5L), -72057594037927937LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llrint_upward (9223372036854775805.5) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775805.5L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775805.5) == -9223372036854775805LL",  FUNC(llrint) (-9223372036854775805.5L), -9223372036854775805LL, 0, 0, 0);
  check_longlong ("llrint_upward (9223372036854775806.0) == 9223372036854775806LL",  FUNC(llrint) (9223372036854775806.0L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775806.0) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.0L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (9223372036854775806.25) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775806.25L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775806.25) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.25L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (9223372036854775806.5) == 9223372036854775807",  FUNC(llrint) (9223372036854775806.5L), 9223372036854775807L, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775806.5) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.5L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (9223372036854775806.75) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775806.75L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775806.75) == -9223372036854775806LL",  FUNC(llrint) (-9223372036854775806.75L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llrint_upward (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llrint) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llrint_upward (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llrint) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("llrint_upward", 0, 0);
}
#endif

static void
log_test (void)
{
  errno = 0;
  FUNC(log) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;
  init_max_error ();

  check_float ("log (0) == -inf plus division by zero exception",  FUNC(log) (0), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("log (-0) == -inf plus division by zero exception",  FUNC(log) (minus_zero), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_float ("log (1) == 0",  FUNC(log) (1), 0, 0, 0, 0);

  check_float ("log (-1) == NaN plus invalid exception",  FUNC(log) (-1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("log (-inf) == NaN plus invalid exception",  FUNC(log) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("log (inf) == inf",  FUNC(log) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("log (NaN) == NaN",  FUNC(log) (nan_value), nan_value, 0, 0, 0);

  check_float ("log (e) == 1",  FUNC(log) (M_El), 1, 0, 0, 0);
  check_float ("log (1.0 / M_El) == -1",  FUNC(log) (1.0 / M_El), -1, 0, 0, 0);
  check_float ("log (2) == M_LN2l",  FUNC(log) (2), M_LN2l, 0, 0, 0);
  check_float ("log (10) == M_LN10l",  FUNC(log) (10), M_LN10l, 0, 0, 0);
  check_float ("log (0.75) == -0.287682072451780927439219005993827432",  FUNC(log) (0.75L), -0.287682072451780927439219005993827432L, 0, 0, 0);

  print_max_error ("log", 0, 0);
}


static void
log10_test (void)
{
  errno = 0;
  FUNC(log10) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("log10 (0) == -inf plus division by zero exception",  FUNC(log10) (0), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("log10 (-0) == -inf plus division by zero exception",  FUNC(log10) (minus_zero), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_float ("log10 (1) == 0",  FUNC(log10) (1), 0, 0, 0, 0);

  /* log10 (x) == NaN plus invalid exception if x < 0.  */
  check_float ("log10 (-1) == NaN plus invalid exception",  FUNC(log10) (-1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("log10 (-inf) == NaN plus invalid exception",  FUNC(log10) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("log10 (inf) == inf",  FUNC(log10) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("log10 (NaN) == NaN",  FUNC(log10) (nan_value), nan_value, 0, 0, 0);

  check_float ("log10 (0.1) == -1",  FUNC(log10) (0.1L), -1, 0, 0, 0);
  check_float ("log10 (10.0) == 1",  FUNC(log10) (10.0), 1, 0, 0, 0);
  check_float ("log10 (100.0) == 2",  FUNC(log10) (100.0), 2, 0, 0, 0);
  check_float ("log10 (10000.0) == 4",  FUNC(log10) (10000.0), 4, 0, 0, 0);
  check_float ("log10 (e) == log10(e)",  FUNC(log10) (M_El), M_LOG10El, 0, 0, 0);
  check_float ("log10 (0.75) == -0.124938736608299953132449886193870744",  FUNC(log10) (0.75L), -0.124938736608299953132449886193870744L, 0, 0, 0);

  print_max_error ("log10", 0, 0);
}


static void
log1p_test (void)
{
  errno = 0;
  FUNC(log1p) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("log1p (0) == 0",  FUNC(log1p) (0), 0, 0, 0, 0);
  check_float ("log1p (-0) == -0",  FUNC(log1p) (minus_zero), minus_zero, 0, 0, 0);

  check_float ("log1p (-1) == -inf plus division by zero exception",  FUNC(log1p) (-1), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("log1p (-2) == NaN plus invalid exception",  FUNC(log1p) (-2), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("log1p (-inf) == NaN plus invalid exception",  FUNC(log1p) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("log1p (inf) == inf",  FUNC(log1p) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("log1p (NaN) == NaN",  FUNC(log1p) (nan_value), nan_value, 0, 0, 0);

  check_float ("log1p (M_El - 1.0) == 1",  FUNC(log1p) (M_El - 1.0), 1, 0, 0, 0);

  check_float ("log1p (-0.25) == -0.287682072451780927439219005993827432",  FUNC(log1p) (-0.25L), -0.287682072451780927439219005993827432L, 0, 0, 0);
  check_float ("log1p (-0.875) == -2.07944154167983592825169636437452970",  FUNC(log1p) (-0.875), -2.07944154167983592825169636437452970L, 0, 0, 0);

  print_max_error ("log1p", 0, 0);
}


static void
log2_test (void)
{
  errno = 0;
  FUNC(log2) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("log2 (0) == -inf plus division by zero exception",  FUNC(log2) (0), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("log2 (-0) == -inf plus division by zero exception",  FUNC(log2) (minus_zero), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_float ("log2 (1) == 0",  FUNC(log2) (1), 0, 0, 0, 0);

  check_float ("log2 (-1) == NaN plus invalid exception",  FUNC(log2) (-1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("log2 (-inf) == NaN plus invalid exception",  FUNC(log2) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("log2 (inf) == inf",  FUNC(log2) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("log2 (NaN) == NaN",  FUNC(log2) (nan_value), nan_value, 0, 0, 0);

  check_float ("log2 (e) == M_LOG2El",  FUNC(log2) (M_El), M_LOG2El, 0, 0, 0);
  check_float ("log2 (2.0) == 1",  FUNC(log2) (2.0), 1, 0, 0, 0);
  check_float ("log2 (16.0) == 4",  FUNC(log2) (16.0), 4, 0, 0, 0);
  check_float ("log2 (256.0) == 8",  FUNC(log2) (256.0), 8, 0, 0, 0);
  check_float ("log2 (0.75) == -.415037499278843818546261056052183492",  FUNC(log2) (0.75L), -.415037499278843818546261056052183492L, 0, 0, 0);

  print_max_error ("log2", 0, 0);
}


static void
logb_test (void)
{
  init_max_error ();

  check_float ("logb (inf) == inf",  FUNC(logb) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("logb (-inf) == inf",  FUNC(logb) (minus_infty), plus_infty, 0, 0, 0);

  check_float ("logb (0) == -inf plus division by zero exception",  FUNC(logb) (0), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);

  check_float ("logb (-0) == -inf plus division by zero exception",  FUNC(logb) (minus_zero), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("logb (NaN) == NaN",  FUNC(logb) (nan_value), nan_value, 0, 0, 0);

  check_float ("logb (1) == 0",  FUNC(logb) (1), 0, 0, 0, 0);
  check_float ("logb (e) == 1",  FUNC(logb) (M_El), 1, 0, 0, 0);
  check_float ("logb (1024) == 10",  FUNC(logb) (1024), 10, 0, 0, 0);
  check_float ("logb (-2000) == 10",  FUNC(logb) (-2000), 10, 0, 0, 0);

  print_max_error ("logb", 0, 0);
}


static void
lround_test (void)
{
  init_max_error ();

  check_long ("lround (0) == 0",  FUNC(lround) (0), 0, 0, 0, 0);
  check_long ("lround (-0) == 0",  FUNC(lround) (minus_zero), 0, 0, 0, 0);
  check_long ("lround (0.2) == 0.0",  FUNC(lround) (0.2L), 0.0, 0, 0, 0);
  check_long ("lround (-0.2) == 0",  FUNC(lround) (-0.2L), 0, 0, 0, 0);
  check_long ("lround (0.5) == 1",  FUNC(lround) (0.5), 1, 0, 0, 0);
  check_long ("lround (-0.5) == -1",  FUNC(lround) (-0.5), -1, 0, 0, 0);
  check_long ("lround (0.8) == 1",  FUNC(lround) (0.8L), 1, 0, 0, 0);
  check_long ("lround (-0.8) == -1",  FUNC(lround) (-0.8L), -1, 0, 0, 0);
  check_long ("lround (1.5) == 2",  FUNC(lround) (1.5), 2, 0, 0, 0);
  check_long ("lround (-1.5) == -2",  FUNC(lround) (-1.5), -2, 0, 0, 0);
  check_long ("lround (22514.5) == 22515",  FUNC(lround) (22514.5), 22515, 0, 0, 0);
  check_long ("lround (-22514.5) == -22515",  FUNC(lround) (-22514.5), -22515, 0, 0, 0);
  check_long ("lround (1071930.0008) == 1071930",  FUNC(lround) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_long ("lround (1073741824.01) == 1073741824",  FUNC(lround) (1073741824.01), 1073741824, 0, 0, 0);
# if LONG_MAX > 281474976710656
  check_long ("lround (281474976710656.025) == 281474976710656",  FUNC(lround) (281474976710656.025), 281474976710656, 0, 0, 0);
# endif
  check_long ("lround (2097152.5) == 2097153",  FUNC(lround) (2097152.5), 2097153, 0, 0, 0);
  check_long ("lround (-2097152.5) == -2097153",  FUNC(lround) (-2097152.5), -2097153, 0, 0, 0);
  /* nextafter(0.5,-1)  */
  check_long ("lround (0x1.fffffffffffffp-2) == 0",  FUNC(lround) (0x1.fffffffffffffp-2), 0, 0, 0, 0);
  /* nextafter(-0.5,1)  */
  check_long ("lround (-0x1.fffffffffffffp-2) == 0",  FUNC(lround) (-0x1.fffffffffffffp-2), 0, 0, 0, 0);
#else
  /* nextafter(0.5,-1)  */
  check_long ("lround (0x1.fffffp-2) == 0",  FUNC(lround) (0x1.fffffp-2), 0, 0, 0, 0);
  /* nextafter(-0.5,1)  */
  check_long ("lround (-0x1.fffffp-2) == 0",  FUNC(lround) (-0x1.fffffp-2), 0, 0, 0, 0);
  check_long ("lround (0x1.fffffep+23) == 16777215",  FUNC(lround) (0x1.fffffep+23), 16777215, 0, 0, 0);
  check_long ("lround (-0x1.fffffep+23) == -16777215",  FUNC(lround) (-0x1.fffffep+23), -16777215, 0, 0, 0);
#endif
  print_max_error ("lround", 0, 0);
}


static void
llround_test (void)
{
  init_max_error ();

  check_longlong ("llround (0) == 0",  FUNC(llround) (0), 0, 0, 0, 0);
  check_longlong ("llround (-0) == 0",  FUNC(llround) (minus_zero), 0, 0, 0, 0);
  check_longlong ("llround (0.2) == 0.0",  FUNC(llround) (0.2L), 0.0, 0, 0, 0);
  check_longlong ("llround (-0.2) == 0",  FUNC(llround) (-0.2L), 0, 0, 0, 0);
  check_longlong ("llround (0.5) == 1",  FUNC(llround) (0.5), 1, 0, 0, 0);
  check_longlong ("llround (-0.5) == -1",  FUNC(llround) (-0.5), -1, 0, 0, 0);
  check_longlong ("llround (0.8) == 1",  FUNC(llround) (0.8L), 1, 0, 0, 0);
  check_longlong ("llround (-0.8) == -1",  FUNC(llround) (-0.8L), -1, 0, 0, 0);
  check_longlong ("llround (1.5) == 2",  FUNC(llround) (1.5), 2, 0, 0, 0);
  check_longlong ("llround (-1.5) == -2",  FUNC(llround) (-1.5), -2, 0, 0, 0);
  check_longlong ("llround (22514.5) == 22515",  FUNC(llround) (22514.5), 22515, 0, 0, 0);
  check_longlong ("llround (-22514.5) == -22515",  FUNC(llround) (-22514.5), -22515, 0, 0, 0);
  check_long ("llround (1071930.0008) == 1071930",  FUNC(llround) (1071930.0008), 1071930, 0, 0, 0);
#ifndef TEST_FLOAT
  check_longlong ("llround (2097152.5) == 2097153",  FUNC(llround) (2097152.5), 2097153, 0, 0, 0);
  check_longlong ("llround (-2097152.5) == -2097153",  FUNC(llround) (-2097152.5), -2097153, 0, 0, 0);
  check_longlong ("llround (34359738368.5) == 34359738369ll",  FUNC(llround) (34359738368.5), 34359738369ll, 0, 0, 0);
  check_longlong ("llround (-34359738368.5) == -34359738369ll",  FUNC(llround) (-34359738368.5), -34359738369ll, 0, 0, 0);
#endif

  /* Test boundary conditions.  */
  /* 0x1FFFFF */
  check_longlong ("llround (2097151.0) == 2097151LL",  FUNC(llround) (2097151.0), 2097151LL, 0, 0, 0);
  /* 0x800000 */
  check_longlong ("llround (8388608.0) == 8388608LL",  FUNC(llround) (8388608.0), 8388608LL, 0, 0, 0);
  /* 0x1000000 */
  check_longlong ("llround (16777216.0) == 16777216LL",  FUNC(llround) (16777216.0), 16777216LL, 0, 0, 0);
  /* 0x20000000000 */
  check_longlong ("llround (2199023255552.0) == 2199023255552LL",  FUNC(llround) (2199023255552.0), 2199023255552LL, 0, 0, 0);
  /* 0x40000000000 */
  check_longlong ("llround (4398046511104.0) == 4398046511104LL",  FUNC(llround) (4398046511104.0), 4398046511104LL, 0, 0, 0);
  /* 0x1000000000000 */
  check_longlong ("llround (281474976710656.0) == 281474976710656LL",  FUNC(llround) (281474976710656.0), 281474976710656LL, 0, 0, 0);
  /* 0x10000000000000 */
  check_longlong ("llround (4503599627370496.0) == 4503599627370496LL",  FUNC(llround) (4503599627370496.0), 4503599627370496LL, 0, 0, 0);
  /* 0x10000080000000 */
  check_longlong ("llround (4503601774854144.0) == 4503601774854144LL",  FUNC(llround) (4503601774854144.0), 4503601774854144LL, 0, 0, 0);
  /* 0x20000000000000 */
  check_longlong ("llround (9007199254740992.0) == 9007199254740992LL",  FUNC(llround) (9007199254740992.0), 9007199254740992LL, 0, 0, 0);
  /* 0x80000000000000 */
  check_longlong ("llround (36028797018963968.0) == 36028797018963968LL",  FUNC(llround) (36028797018963968.0), 36028797018963968LL, 0, 0, 0);
  /* 0x100000000000000 */
  check_longlong ("llround (72057594037927936.0) == 72057594037927936LL",  FUNC(llround) (72057594037927936.0), 72057594037927936LL, 0, 0, 0);

#ifndef TEST_FLOAT
  /* 0x100000000 */
  check_longlong ("llround (4294967295.5) == 4294967296LL",  FUNC(llround) (4294967295.5), 4294967296LL, 0, 0, 0);
  /* 0x200000000 */
  check_longlong ("llround (8589934591.5) == 8589934592LL",  FUNC(llround) (8589934591.5), 8589934592LL, 0, 0, 0);

  /* nextafter(0.5,-1)  */
  check_longlong ("llround (0x1.fffffffffffffp-2) == 0",  FUNC(llround) (0x1.fffffffffffffp-2), 0, 0, 0, 0);
  /* nextafter(-0.5,1)  */
  check_longlong ("llround (-0x1.fffffffffffffp-2) == 0",  FUNC(llround) (-0x1.fffffffffffffp-2), 0, 0, 0, 0);
  /* On PowerPC an exponent of '52' is the largest incrementally
   * representable sequence of whole-numbers in the 'double' range.  We test
   * lround to make sure that a guard bit set during the lround operation
   * hasn't forced an erroneous shift giving us an incorrect result.  The odd
   * numbers between +-(2^52+1 and 2^53-1) are affected since they have the
   * rightmost bit set.  */
  /* +-(2^52+1)  */
  check_longlong ("llround (0x1.0000000000001p+52) == 4503599627370497LL",  FUNC(llround) (0x1.0000000000001p+52), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (-0x1.0000000000001p+52) == -4503599627370497LL",  FUNC(llround) (-0x1.0000000000001p+52), -4503599627370497LL, 0, 0, 0);
  /* +-(2^53-1): Input is the last (positive and negative) incrementally
   * representable whole-number in the 'double' range that might round
   * erroneously.  */
  check_longlong ("llround (0x1.fffffffffffffp+52) == 9007199254740991LL",  FUNC(llround) (0x1.fffffffffffffp+52), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llround (-0x1.fffffffffffffp+52) == -9007199254740991LL",  FUNC(llround) (-0x1.fffffffffffffp+52), -9007199254740991LL, 0, 0, 0);
#else
  /* nextafter(0.5,-1)  */
  check_longlong ("llround (0x1.fffffep-2) == 0",  FUNC(llround) (0x1.fffffep-2), 0, 0, 0, 0);
  /* nextafter(-0.5,1)  */
  check_longlong ("llround (-0x1.fffffep-2) == 0",  FUNC(llround) (-0x1.fffffep-2), 0, 0, 0, 0);
  /* As above, on PowerPC an exponent of '23' is the largest incrementally
   * representable sequence of whole-numbers in the 'float' range.
   * Likewise, numbers between +-(2^23+1 and 2^24-1) are affected.  */
  check_longlong ("llround (0x1.000002p+23) == 8388609",  FUNC(llround) (0x1.000002p+23), 8388609, 0, 0, 0);
  check_longlong ("llround (-0x1.000002p+23) == -8388609",  FUNC(llround) (-0x1.000002p+23), -8388609, 0, 0, 0);
  check_longlong ("llround (0x1.fffffep+23) == 16777215",  FUNC(llround) (0x1.fffffep+23), 16777215, 0, 0, 0);
  check_longlong ("llround (-0x1.fffffep+23) == -16777215",  FUNC(llround) (-0x1.fffffep+23), -16777215, 0, 0, 0);
#endif


#ifdef TEST_LDOUBLE
  /* The input can only be represented in long double.  */
  check_longlong ("llround (4503599627370495.5) == 4503599627370496LL",  FUNC(llround) (4503599627370495.5L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (4503599627370496.25) == 4503599627370496LL",  FUNC(llround) (4503599627370496.25L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (4503599627370496.5) == 4503599627370497LL",  FUNC(llround) (4503599627370496.5L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (4503599627370496.75) == 4503599627370497LL",  FUNC(llround) (4503599627370496.75L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (4503599627370497.5) == 4503599627370498LL",  FUNC(llround) (4503599627370497.5L), 4503599627370498LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llround (4503599627370495.4999999999999) == 4503599627370495LL",  FUNC(llround) (4503599627370495.4999999999999L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llround (4503599627370496.4999999999999) == 4503599627370496LL",  FUNC(llround) (4503599627370496.4999999999999L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (4503599627370497.4999999999999) == 4503599627370497LL",  FUNC(llround) (4503599627370497.4999999999999L), 4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (4503599627370494.5000000000001) == 4503599627370495LL",  FUNC(llround) (4503599627370494.5000000000001L), 4503599627370495LL, 0, 0, 0);
  check_longlong ("llround (4503599627370495.5000000000001) == 4503599627370496LL",  FUNC(llround) (4503599627370495.5000000000001L), 4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (4503599627370496.5000000000001) == 4503599627370497LL",  FUNC(llround) (4503599627370496.5000000000001L), 4503599627370497LL, 0, 0, 0);

  check_longlong ("llround (-4503599627370495.4999999999999) == -4503599627370495LL",  FUNC(llround) (-4503599627370495.4999999999999L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370496.4999999999999) == -4503599627370496LL",  FUNC(llround) (-4503599627370496.4999999999999L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370497.4999999999999) == -4503599627370497LL",  FUNC(llround) (-4503599627370497.4999999999999L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370494.5000000000001) == -4503599627370495LL",  FUNC(llround) (-4503599627370494.5000000000001L), -4503599627370495LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370495.5000000000001) == -4503599627370496LL",  FUNC(llround) (-4503599627370495.5000000000001L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370496.5000000000001) == -4503599627370497LL",  FUNC(llround) (-4503599627370496.5000000000001L), -4503599627370497LL, 0, 0, 0);
# endif

  check_longlong ("llround (-4503599627370495.5) == -4503599627370496LL",  FUNC(llround) (-4503599627370495.5L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370496.25) == -4503599627370496LL",  FUNC(llround) (-4503599627370496.25L), -4503599627370496LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370496.5) == -4503599627370497LL",  FUNC(llround) (-4503599627370496.5L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370496.75) == -4503599627370497LL",  FUNC(llround) (-4503599627370496.75L), -4503599627370497LL, 0, 0, 0);
  check_longlong ("llround (-4503599627370497.5) == -4503599627370498LL",  FUNC(llround) (-4503599627370497.5L), -4503599627370498LL, 0, 0, 0);

  check_longlong ("llround (9007199254740991.5) == 9007199254740992LL",  FUNC(llround) (9007199254740991.5L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (9007199254740992.25) == 9007199254740992LL",  FUNC(llround) (9007199254740992.25L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (9007199254740992.5) == 9007199254740993LL",  FUNC(llround) (9007199254740992.5L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (9007199254740992.75) == 9007199254740993LL",  FUNC(llround) (9007199254740992.75L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (9007199254740993.5) == 9007199254740994LL",  FUNC(llround) (9007199254740993.5L), 9007199254740994LL, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_longlong ("llround (9007199254740991.4999999999999) == 9007199254740991LL",  FUNC(llround) (9007199254740991.4999999999999L), 9007199254740991LL, 0, 0, 0);
  check_longlong ("llround (9007199254740992.4999999999999) == 9007199254740992LL",  FUNC(llround) (9007199254740992.4999999999999L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (9007199254740993.4999999999999) == 9007199254740993LL",  FUNC(llround) (9007199254740993.4999999999999L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (9007199254740991.5000000000001) == 9007199254740992LL",  FUNC(llround) (9007199254740991.5000000000001L), 9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (9007199254740992.5000000000001) == 9007199254740993LL",  FUNC(llround) (9007199254740992.5000000000001L), 9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (9007199254740993.5000000000001) == 9007199254740994LL",  FUNC(llround) (9007199254740993.5000000000001L), 9007199254740994LL, 0, 0, 0);

  check_longlong ("llround (-9007199254740991.4999999999999) == -9007199254740991LL",  FUNC(llround) (-9007199254740991.4999999999999L), -9007199254740991LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740992.4999999999999) == -9007199254740992LL",  FUNC(llround) (-9007199254740992.4999999999999L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740993.4999999999999) == -9007199254740993LL",  FUNC(llround) (-9007199254740993.4999999999999L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740991.5000000000001) == -9007199254740992LL",  FUNC(llround) (-9007199254740991.5000000000001L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740992.5000000000001) == -9007199254740993LL",  FUNC(llround) (-9007199254740992.5000000000001L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740993.5000000000001) == -9007199254740994LL",  FUNC(llround) (-9007199254740993.5000000000001L), -9007199254740994LL, 0, 0, 0);
# endif

  check_longlong ("llround (-9007199254740991.5) == -9007199254740992LL",  FUNC(llround) (-9007199254740991.5L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740992.25) == -9007199254740992LL",  FUNC(llround) (-9007199254740992.25L), -9007199254740992LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740992.5) == -9007199254740993LL",  FUNC(llround) (-9007199254740992.5L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740992.75) == -9007199254740993LL",  FUNC(llround) (-9007199254740992.75L), -9007199254740993LL, 0, 0, 0);
  check_longlong ("llround (-9007199254740993.5) == -9007199254740994LL",  FUNC(llround) (-9007199254740993.5L), -9007199254740994LL, 0, 0, 0);

  check_longlong ("llround (72057594037927935.5) == 72057594037927936LL",  FUNC(llround) (72057594037927935.5L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llround (72057594037927936.25) == 72057594037927936LL",  FUNC(llround) (72057594037927936.25L), 72057594037927936LL, 0, 0, 0);
  check_longlong ("llround (72057594037927936.5) == 72057594037927937LL",  FUNC(llround) (72057594037927936.5L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llround (72057594037927936.75) == 72057594037927937LL",  FUNC(llround) (72057594037927936.75L), 72057594037927937LL, 0, 0, 0);
  check_longlong ("llround (72057594037927937.5) == 72057594037927938LL",  FUNC(llround) (72057594037927937.5L), 72057594037927938LL, 0, 0, 0);

  check_longlong ("llround (-72057594037927935.5) == -72057594037927936LL",  FUNC(llround) (-72057594037927935.5L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llround (-72057594037927936.25) == -72057594037927936LL",  FUNC(llround) (-72057594037927936.25L), -72057594037927936LL, 0, 0, 0);
  check_longlong ("llround (-72057594037927936.5) == -72057594037927937LL",  FUNC(llround) (-72057594037927936.5L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llround (-72057594037927936.75) == -72057594037927937LL",  FUNC(llround) (-72057594037927936.75L), -72057594037927937LL, 0, 0, 0);
  check_longlong ("llround (-72057594037927937.5) == -72057594037927938LL",  FUNC(llround) (-72057594037927937.5L), -72057594037927938LL, 0, 0, 0);

  check_longlong ("llround (9223372036854775806.25) == 9223372036854775806LL",  FUNC(llround) (9223372036854775806.25L), 9223372036854775806LL, 0, 0, 0);
  check_longlong ("llround (-9223372036854775806.25) == -9223372036854775806LL",  FUNC(llround) (-9223372036854775806.25L), -9223372036854775806LL, 0, 0, 0);
  check_longlong ("llround (9223372036854775806.5) == 9223372036854775807LL",  FUNC(llround) (9223372036854775806.5L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llround (-9223372036854775806.5) == -9223372036854775807LL",  FUNC(llround) (-9223372036854775806.5L), -9223372036854775807LL, 0, 0, 0);
  check_longlong ("llround (9223372036854775807.0) == 9223372036854775807LL",  FUNC(llround) (9223372036854775807.0L), 9223372036854775807LL, 0, 0, 0);
  check_longlong ("llround (-9223372036854775807.0) == -9223372036854775807LL",  FUNC(llround) (-9223372036854775807.0L), -9223372036854775807LL, 0, 0, 0);
#endif

  print_max_error ("llround", 0, 0);
}

static void
modf_test (void)
{
  FLOAT x;

  init_max_error ();

  check_float ("modf (inf, &x) == 0",  FUNC(modf) (plus_infty, &x), 0, 0, 0, 0);
  check_float ("modf (inf, &x) sets x to plus_infty", x, plus_infty, 0, 0, 0);
  check_float ("modf (-inf, &x) == -0",  FUNC(modf) (minus_infty, &x), minus_zero, 0, 0, 0);
  check_float ("modf (-inf, &x) sets x to minus_infty", x, minus_infty, 0, 0, 0);
  check_float ("modf (NaN, &x) == NaN",  FUNC(modf) (nan_value, &x), nan_value, 0, 0, 0);
  check_float ("modf (NaN, &x) sets x to nan_value", x, nan_value, 0, 0, 0);
  check_float ("modf (0, &x) == 0",  FUNC(modf) (0, &x), 0, 0, 0, 0);
  check_float ("modf (0, &x) sets x to 0", x, 0, 0, 0, 0);
  check_float ("modf (1.5, &x) == 0.5",  FUNC(modf) (1.5, &x), 0.5, 0, 0, 0);
  check_float ("modf (1.5, &x) sets x to 1", x, 1, 0, 0, 0);
  check_float ("modf (2.5, &x) == 0.5",  FUNC(modf) (2.5, &x), 0.5, 0, 0, 0);
  check_float ("modf (2.5, &x) sets x to 2", x, 2, 0, 0, 0);
  check_float ("modf (-2.5, &x) == -0.5",  FUNC(modf) (-2.5, &x), -0.5, 0, 0, 0);
  check_float ("modf (-2.5, &x) sets x to -2", x, -2, 0, 0, 0);
  check_float ("modf (20, &x) == 0",  FUNC(modf) (20, &x), 0, 0, 0, 0);
  check_float ("modf (20, &x) sets x to 20", x, 20, 0, 0, 0);
  check_float ("modf (21, &x) == 0",  FUNC(modf) (21, &x), 0, 0, 0, 0);
  check_float ("modf (21, &x) sets x to 21", x, 21, 0, 0, 0);
  check_float ("modf (89.5, &x) == 0.5",  FUNC(modf) (89.5, &x), 0.5, 0, 0, 0);
  check_float ("modf (89.5, &x) sets x to 89", x, 89, 0, 0, 0);

  print_max_error ("modf", 0, 0);
}


static void
nearbyint_test (void)
{
  init_max_error ();

  check_float ("nearbyint (0.0) == 0.0",  FUNC(nearbyint) (0.0), 0.0, 0, 0, 0);
  check_float ("nearbyint (-0) == -0",  FUNC(nearbyint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("nearbyint (inf) == inf",  FUNC(nearbyint) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("nearbyint (-inf) == -inf",  FUNC(nearbyint) (minus_infty), minus_infty, 0, 0, 0);
  check_float ("nearbyint (NaN) == NaN",  FUNC(nearbyint) (nan_value), nan_value, 0, 0, 0);

  /* Default rounding mode is round to nearest.  */
  check_float ("nearbyint (0.5) == 0.0",  FUNC(nearbyint) (0.5), 0.0, 0, 0, 0);
  check_float ("nearbyint (1.5) == 2.0",  FUNC(nearbyint) (1.5), 2.0, 0, 0, 0);
  check_float ("nearbyint (-0.5) == -0",  FUNC(nearbyint) (-0.5), minus_zero, 0, 0, 0);
  check_float ("nearbyint (-1.5) == -2.0",  FUNC(nearbyint) (-1.5), -2.0, 0, 0, 0);

  check_float ("nearbyint (262144.75) == 262145.0",  FUNC(nearbyint) (262144.75), 262145.0, 0, 0, 0);
  check_float ("nearbyint (262142.75) == 262143.0",  FUNC(nearbyint) (262142.75), 262143.0, 0, 0, 0);
  check_float ("nearbyint (524286.75) == 524287.0",  FUNC(nearbyint) (524286.75), 524287.0, 0, 0, 0);
  check_float ("nearbyint (524288.75) == 524289.0",  FUNC(nearbyint) (524288.75), 524289.0, 0, 0, 0);

  print_max_error ("nearbyint", 0, 0);
}

static void
nextafter_test (void)
{

  init_max_error ();

  check_float ("nextafter (0, 0) == 0",  FUNC(nextafter) (0, 0), 0, 0, 0, 0);
  check_float ("nextafter (-0, 0) == 0",  FUNC(nextafter) (minus_zero, 0), 0, 0, 0, 0);
  check_float ("nextafter (0, -0) == -0",  FUNC(nextafter) (0, minus_zero), minus_zero, 0, 0, 0);
  check_float ("nextafter (-0, -0) == -0",  FUNC(nextafter) (minus_zero, minus_zero), minus_zero, 0, 0, 0);

  check_float ("nextafter (9, 9) == 9",  FUNC(nextafter) (9, 9), 9, 0, 0, 0);
  check_float ("nextafter (-9, -9) == -9",  FUNC(nextafter) (-9, -9), -9, 0, 0, 0);
  check_float ("nextafter (inf, inf) == inf",  FUNC(nextafter) (plus_infty, plus_infty), plus_infty, 0, 0, 0);
  check_float ("nextafter (-inf, -inf) == -inf",  FUNC(nextafter) (minus_infty, minus_infty), minus_infty, 0, 0, 0);

  check_float ("nextafter (NaN, 1.1) == NaN",  FUNC(nextafter) (nan_value, 1.1L), nan_value, 0, 0, 0);
  check_float ("nextafter (1.1, NaN) == NaN",  FUNC(nextafter) (1.1L, nan_value), nan_value, 0, 0, 0);
  check_float ("nextafter (NaN, NaN) == NaN",  FUNC(nextafter) (nan_value, nan_value), nan_value, 0, 0, 0);

  FLOAT fltmax = CHOOSE (LDBL_MAX, DBL_MAX, FLT_MAX,
			 LDBL_MAX, DBL_MAX, FLT_MAX);
  check_float ("nextafter (fltmax, inf) == inf",  FUNC(nextafter) (fltmax, plus_infty), plus_infty, 0, 0, 0);
  check_float ("nextafter (-fltmax, -inf) == -inf",  FUNC(nextafter) (-fltmax, minus_infty), minus_infty, 0, 0, 0);

#ifdef TEST_LDOUBLE
  // XXX Enable once gcc is fixed.
  //TEST_ff_f (nextafter, 0x0.00000040000000000000p-16385L, -0.1L, 0x0.0000003ffffffff00000p-16385L);
#endif

  /* XXX We need the hexadecimal FP number representation here for further
     tests.  */

  print_max_error ("nextafter", 0, 0);
}


static void
nexttoward_test (void)
{
  init_max_error ();
  check_float ("nexttoward (0, 0) == 0",  FUNC(nexttoward) (0, 0), 0, 0, 0, 0);
  check_float ("nexttoward (-0, 0) == 0",  FUNC(nexttoward) (minus_zero, 0), 0, 0, 0, 0);
  check_float ("nexttoward (0, -0) == -0",  FUNC(nexttoward) (0, minus_zero), minus_zero, 0, 0, 0);
  check_float ("nexttoward (-0, -0) == -0",  FUNC(nexttoward) (minus_zero, minus_zero), minus_zero, 0, 0, 0);

  check_float ("nexttoward (9, 9) == 9",  FUNC(nexttoward) (9, 9), 9, 0, 0, 0);
  check_float ("nexttoward (-9, -9) == -9",  FUNC(nexttoward) (-9, -9), -9, 0, 0, 0);
  check_float ("nexttoward (inf, inf) == inf",  FUNC(nexttoward) (plus_infty, plus_infty), plus_infty, 0, 0, 0);
  check_float ("nexttoward (-inf, -inf) == -inf",  FUNC(nexttoward) (minus_infty, minus_infty), minus_infty, 0, 0, 0);

  check_float ("nexttoward (NaN, 1.1) == NaN",  FUNC(nexttoward) (nan_value, 1.1L), nan_value, 0, 0, 0);
  check_float ("nexttoward (1.1, NaN) == NaN",  FUNC(nexttoward) (1.1L, nan_value), nan_value, 0, 0, 0);
  check_float ("nexttoward (NaN, NaN) == NaN",  FUNC(nexttoward) (nan_value, nan_value), nan_value, 0, 0, 0);

  /* XXX We need the hexadecimal FP number representation here for further
     tests.  */

  print_max_error ("nexttoward", 0, 0);
}


static void
pow_test (void)
{

  errno = 0;
  FUNC(pow) (0, 0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("pow (0, 0) == 1",  FUNC(pow) (0, 0), 1, 0, 0, 0);
  check_float ("pow (0, -0) == 1",  FUNC(pow) (0, minus_zero), 1, 0, 0, 0);
  check_float ("pow (-0, 0) == 1",  FUNC(pow) (minus_zero, 0), 1, 0, 0, 0);
  check_float ("pow (-0, -0) == 1",  FUNC(pow) (minus_zero, minus_zero), 1, 0, 0, 0);

  check_float ("pow (10, 0) == 1",  FUNC(pow) (10, 0), 1, 0, 0, 0);
  check_float ("pow (10, -0) == 1",  FUNC(pow) (10, minus_zero), 1, 0, 0, 0);
  check_float ("pow (-10, 0) == 1",  FUNC(pow) (-10, 0), 1, 0, 0, 0);
  check_float ("pow (-10, -0) == 1",  FUNC(pow) (-10, minus_zero), 1, 0, 0, 0);

  check_float ("pow (NaN, 0) == 1",  FUNC(pow) (nan_value, 0), 1, 0, 0, 0);
  check_float ("pow (NaN, -0) == 1",  FUNC(pow) (nan_value, minus_zero), 1, 0, 0, 0);


#ifndef TEST_INLINE
  check_float ("pow (1.1, inf) == inf",  FUNC(pow) (1.1L, plus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (inf, inf) == inf",  FUNC(pow) (plus_infty, plus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (-1.1, inf) == inf",  FUNC(pow) (-1.1L, plus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, inf) == inf",  FUNC(pow) (minus_infty, plus_infty), plus_infty, 0, 0, 0);

  check_float ("pow (0.9, inf) == 0",  FUNC(pow) (0.9L, plus_infty), 0, 0, 0, 0);
  check_float ("pow (1e-7, inf) == 0",  FUNC(pow) (1e-7L, plus_infty), 0, 0, 0, 0);
  check_float ("pow (-0.9, inf) == 0",  FUNC(pow) (-0.9L, plus_infty), 0, 0, 0, 0);
  check_float ("pow (-1e-7, inf) == 0",  FUNC(pow) (-1e-7L, plus_infty), 0, 0, 0, 0);

  check_float ("pow (1.1, -inf) == 0",  FUNC(pow) (1.1L, minus_infty), 0, 0, 0, 0);
  check_float ("pow (inf, -inf) == 0",  FUNC(pow) (plus_infty, minus_infty), 0, 0, 0, 0);
  check_float ("pow (-1.1, -inf) == 0",  FUNC(pow) (-1.1L, minus_infty), 0, 0, 0, 0);
  check_float ("pow (-inf, -inf) == 0",  FUNC(pow) (minus_infty, minus_infty), 0, 0, 0, 0);

  check_float ("pow (0.9, -inf) == inf",  FUNC(pow) (0.9L, minus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (1e-7, -inf) == inf",  FUNC(pow) (1e-7L, minus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (-0.9, -inf) == inf",  FUNC(pow) (-0.9L, minus_infty), plus_infty, 0, 0, 0);
  check_float ("pow (-1e-7, -inf) == inf",  FUNC(pow) (-1e-7L, minus_infty), plus_infty, 0, 0, 0);

  check_float ("pow (inf, 1e-7) == inf",  FUNC(pow) (plus_infty, 1e-7L), plus_infty, 0, 0, 0);
  check_float ("pow (inf, 1) == inf",  FUNC(pow) (plus_infty, 1), plus_infty, 0, 0, 0);
  check_float ("pow (inf, 1e7) == inf",  FUNC(pow) (plus_infty, 1e7L), plus_infty, 0, 0, 0);

  check_float ("pow (inf, -1e-7) == 0",  FUNC(pow) (plus_infty, -1e-7L), 0, 0, 0, 0);
  check_float ("pow (inf, -1) == 0",  FUNC(pow) (plus_infty, -1), 0, 0, 0, 0);
  check_float ("pow (inf, -1e7) == 0",  FUNC(pow) (plus_infty, -1e7L), 0, 0, 0, 0);

  check_float ("pow (-inf, 1) == -inf",  FUNC(pow) (minus_infty, 1), minus_infty, 0, 0, 0);
  check_float ("pow (-inf, 11) == -inf",  FUNC(pow) (minus_infty, 11), minus_infty, 0, 0, 0);
  check_float ("pow (-inf, 1001) == -inf",  FUNC(pow) (minus_infty, 1001), minus_infty, 0, 0, 0);

  check_float ("pow (-inf, 2) == inf",  FUNC(pow) (minus_infty, 2), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 12) == inf",  FUNC(pow) (minus_infty, 12), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 1002) == inf",  FUNC(pow) (minus_infty, 1002), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 0.1) == inf",  FUNC(pow) (minus_infty, 0.1L), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 1.1) == inf",  FUNC(pow) (minus_infty, 1.1L), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 11.1) == inf",  FUNC(pow) (minus_infty, 11.1L), plus_infty, 0, 0, 0);
  check_float ("pow (-inf, 1001.1) == inf",  FUNC(pow) (minus_infty, 1001.1L), plus_infty, 0, 0, 0);

  check_float ("pow (-inf, -1) == -0",  FUNC(pow) (minus_infty, -1), minus_zero, 0, 0, 0);
  check_float ("pow (-inf, -11) == -0",  FUNC(pow) (minus_infty, -11), minus_zero, 0, 0, 0);
  check_float ("pow (-inf, -1001) == -0",  FUNC(pow) (minus_infty, -1001), minus_zero, 0, 0, 0);

  check_float ("pow (-inf, -2) == 0",  FUNC(pow) (minus_infty, -2), 0, 0, 0, 0);
  check_float ("pow (-inf, -12) == 0",  FUNC(pow) (minus_infty, -12), 0, 0, 0, 0);
  check_float ("pow (-inf, -1002) == 0",  FUNC(pow) (minus_infty, -1002), 0, 0, 0, 0);
  check_float ("pow (-inf, -0.1) == 0",  FUNC(pow) (minus_infty, -0.1L), 0, 0, 0, 0);
  check_float ("pow (-inf, -1.1) == 0",  FUNC(pow) (minus_infty, -1.1L), 0, 0, 0, 0);
  check_float ("pow (-inf, -11.1) == 0",  FUNC(pow) (minus_infty, -11.1L), 0, 0, 0, 0);
  check_float ("pow (-inf, -1001.1) == 0",  FUNC(pow) (minus_infty, -1001.1L), 0, 0, 0, 0);
#endif

  check_float ("pow (NaN, NaN) == NaN",  FUNC(pow) (nan_value, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (0, NaN) == NaN",  FUNC(pow) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (1, NaN) == 1",  FUNC(pow) (1, nan_value), 1, 0, 0, 0);
  check_float ("pow (-1, NaN) == NaN",  FUNC(pow) (-1, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (NaN, 1) == NaN",  FUNC(pow) (nan_value, 1), nan_value, 0, 0, 0);
  check_float ("pow (NaN, -1) == NaN",  FUNC(pow) (nan_value, -1), nan_value, 0, 0, 0);

  /* pow (x, NaN) == NaN.  */
  check_float ("pow (3.0, NaN) == NaN",  FUNC(pow) (3.0, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (-0, NaN) == NaN",  FUNC(pow) (minus_zero, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (inf, NaN) == NaN",  FUNC(pow) (plus_infty, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (-3.0, NaN) == NaN",  FUNC(pow) (-3.0, nan_value), nan_value, 0, 0, 0);
  check_float ("pow (-inf, NaN) == NaN",  FUNC(pow) (minus_infty, nan_value), nan_value, 0, 0, 0);

  check_float ("pow (NaN, 3.0) == NaN",  FUNC(pow) (nan_value, 3.0), nan_value, 0, 0, 0);
  check_float ("pow (NaN, -3.0) == NaN",  FUNC(pow) (nan_value, -3.0), nan_value, 0, 0, 0);
  check_float ("pow (NaN, inf) == NaN",  FUNC(pow) (nan_value, plus_infty), nan_value, 0, 0, 0);
  check_float ("pow (NaN, -inf) == NaN",  FUNC(pow) (nan_value, minus_infty), nan_value, 0, 0, 0);
  check_float ("pow (NaN, 2.5) == NaN",  FUNC(pow) (nan_value, 2.5), nan_value, 0, 0, 0);
  check_float ("pow (NaN, -2.5) == NaN",  FUNC(pow) (nan_value, -2.5), nan_value, 0, 0, 0);

  check_float ("pow (1, inf) == 1",  FUNC(pow) (1, plus_infty), 1, 0, 0, 0);
  check_float ("pow (-1, inf) == 1",  FUNC(pow) (-1, plus_infty), 1, 0, 0, 0);
  check_float ("pow (1, -inf) == 1",  FUNC(pow) (1, minus_infty), 1, 0, 0, 0);
  check_float ("pow (-1, -inf) == 1",  FUNC(pow) (-1, minus_infty), 1, 0, 0, 0);
  check_float ("pow (1, 1) == 1",  FUNC(pow) (1, 1), 1, 0, 0, 0);
  check_float ("pow (1, -1) == 1",  FUNC(pow) (1, -1), 1, 0, 0, 0);
  check_float ("pow (1, 1.25) == 1",  FUNC(pow) (1, 1.25), 1, 0, 0, 0);
  check_float ("pow (1, -1.25) == 1",  FUNC(pow) (1, -1.25), 1, 0, 0, 0);
  check_float ("pow (1, 0x1p62) == 1",  FUNC(pow) (1, 0x1p62L), 1, 0, 0, 0);
  check_float ("pow (1, 0x1p63) == 1",  FUNC(pow) (1, 0x1p63L), 1, 0, 0, 0);
  check_float ("pow (1, 0x1p64) == 1",  FUNC(pow) (1, 0x1p64L), 1, 0, 0, 0);
  check_float ("pow (1, 0x1p72) == 1",  FUNC(pow) (1, 0x1p72L), 1, 0, 0, 0);

  /* pow (x, +-0) == 1.  */
  check_float ("pow (inf, 0) == 1",  FUNC(pow) (plus_infty, 0), 1, 0, 0, 0);
  check_float ("pow (inf, -0) == 1",  FUNC(pow) (plus_infty, minus_zero), 1, 0, 0, 0);
  check_float ("pow (-inf, 0) == 1",  FUNC(pow) (minus_infty, 0), 1, 0, 0, 0);
  check_float ("pow (-inf, -0) == 1",  FUNC(pow) (minus_infty, minus_zero), 1, 0, 0, 0);
  check_float ("pow (32.75, 0) == 1",  FUNC(pow) (32.75L, 0), 1, 0, 0, 0);
  check_float ("pow (32.75, -0) == 1",  FUNC(pow) (32.75L, minus_zero), 1, 0, 0, 0);
  check_float ("pow (-32.75, 0) == 1",  FUNC(pow) (-32.75L, 0), 1, 0, 0, 0);
  check_float ("pow (-32.75, -0) == 1",  FUNC(pow) (-32.75L, minus_zero), 1, 0, 0, 0);
  check_float ("pow (0x1p72, 0) == 1",  FUNC(pow) (0x1p72L, 0), 1, 0, 0, 0);
  check_float ("pow (0x1p72, -0) == 1",  FUNC(pow) (0x1p72L, minus_zero), 1, 0, 0, 0);
  check_float ("pow (0x1p-72, 0) == 1",  FUNC(pow) (0x1p-72L, 0), 1, 0, 0, 0);
  check_float ("pow (0x1p-72, -0) == 1",  FUNC(pow) (0x1p-72L, minus_zero), 1, 0, 0, 0);

  check_float ("pow (-0.1, 1.1) == NaN plus invalid exception",  FUNC(pow) (-0.1L, 1.1L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("pow (-0.1, -1.1) == NaN plus invalid exception",  FUNC(pow) (-0.1L, -1.1L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("pow (-10.1, 1.1) == NaN plus invalid exception",  FUNC(pow) (-10.1L, 1.1L), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("pow (-10.1, -1.1) == NaN plus invalid exception",  FUNC(pow) (-10.1L, -1.1L), nan_value, 0, 0, INVALID_EXCEPTION);

  errno = 0;
  check_float ("pow (0, -1) == inf plus division by zero exception",  FUNC(pow) (0, -1), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(0,-odd) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (0, -11) == inf plus division by zero exception",  FUNC(pow) (0, -11), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(0,-odd) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (-0, -1) == -inf plus division by zero exception",  FUNC(pow) (minus_zero, -1), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(-0,-odd) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (-0, -11) == -inf plus division by zero exception",  FUNC(pow) (minus_zero, -11L), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(-0,-odd) == ERANGE", errno, ERANGE, 0, 0, 0);

  errno = 0;
  check_float ("pow (0, -2) == inf plus division by zero exception",  FUNC(pow) (0, -2), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(0,-even) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (0, -11.1) == inf plus division by zero exception",  FUNC(pow) (0, -11.1L), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(0,-num) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (-0, -2) == inf plus division by zero exception",  FUNC(pow) (minus_zero, -2), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(-0,-even) == ERANGE", errno, ERANGE, 0, 0, 0);
  errno = 0;
  check_float ("pow (-0, -11.1) == inf plus division by zero exception",  FUNC(pow) (minus_zero, -11.1L), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_int ("errno for pow(-0,-num) == ERANGE", errno, ERANGE, 0, 0, 0);

  check_float ("pow (0x1p72, 0x1p72) == inf",  FUNC(pow) (0x1p72L, 0x1p72L), plus_infty, 0, 0, 0);
  check_float ("pow (10, -0x1p72) == 0",  FUNC(pow) (10, -0x1p72L), 0, 0, 0, 0);
  check_float ("pow (max_value, max_value) == inf",  FUNC(pow) (max_value, max_value), plus_infty, 0, 0, 0);
  check_float ("pow (10, -max_value) == 0",  FUNC(pow) (10, -max_value), 0, 0, 0, 0);

  check_float ("pow (0, 1) == 0",  FUNC(pow) (0, 1), 0, 0, 0, 0);
  check_float ("pow (0, 11) == 0",  FUNC(pow) (0, 11), 0, 0, 0, 0);

  check_float ("pow (-0, 1) == -0",  FUNC(pow) (minus_zero, 1), minus_zero, 0, 0, 0);
  check_float ("pow (-0, 11) == -0",  FUNC(pow) (minus_zero, 11), minus_zero, 0, 0, 0);

  check_float ("pow (0, 2) == 0",  FUNC(pow) (0, 2), 0, 0, 0, 0);
  check_float ("pow (0, 11.1) == 0",  FUNC(pow) (0, 11.1L), 0, 0, 0, 0);

  check_float ("pow (-0, 2) == 0",  FUNC(pow) (minus_zero, 2), 0, 0, 0, 0);
  check_float ("pow (-0, 11.1) == 0",  FUNC(pow) (minus_zero, 11.1L), 0, 0, 0, 0);
  check_float ("pow (0, inf) == 0",  FUNC(pow) (0, plus_infty), 0, 0, 0, 0);
  check_float ("pow (-0, inf) == 0",  FUNC(pow) (minus_zero, plus_infty), 0, 0, 0, 0);

#ifndef TEST_INLINE
  /* pow (x, +inf) == +inf for |x| > 1.  */
  check_float ("pow (1.5, inf) == inf",  FUNC(pow) (1.5, plus_infty), plus_infty, 0, 0, 0);

  /* pow (x, +inf) == +0 for |x| < 1.  */
  check_float ("pow (0.5, inf) == 0.0",  FUNC(pow) (0.5, plus_infty), 0.0, 0, 0, 0);

  /* pow (x, -inf) == +0 for |x| > 1.  */
  check_float ("pow (1.5, -inf) == 0.0",  FUNC(pow) (1.5, minus_infty), 0.0, 0, 0, 0);

  /* pow (x, -inf) == +inf for |x| < 1.  */
  check_float ("pow (0.5, -inf) == inf",  FUNC(pow) (0.5, minus_infty), plus_infty, 0, 0, 0);
#endif

  /* pow (+inf, y) == +inf for y > 0.  */
  check_float ("pow (inf, 2) == inf",  FUNC(pow) (plus_infty, 2), plus_infty, 0, 0, 0);

  /* pow (+inf, y) == +0 for y < 0.  */
  check_float ("pow (inf, -1) == 0.0",  FUNC(pow) (plus_infty, -1), 0.0, 0, 0, 0);

  /* pow (-inf, y) == -inf for y an odd integer > 0.  */
  check_float ("pow (-inf, 27) == -inf",  FUNC(pow) (minus_infty, 27), minus_infty, 0, 0, 0);

  /* pow (-inf, y) == +inf for y > 0 and not an odd integer.  */
  check_float ("pow (-inf, 28) == inf",  FUNC(pow) (minus_infty, 28), plus_infty, 0, 0, 0);

  /* pow (-inf, y) == -0 for y an odd integer < 0. */
  check_float ("pow (-inf, -3) == -0",  FUNC(pow) (minus_infty, -3), minus_zero, 0, 0, 0);
  /* pow (-inf, y) == +0 for y < 0 and not an odd integer.  */
  check_float ("pow (-inf, -2.0) == 0.0",  FUNC(pow) (minus_infty, -2.0), 0.0, 0, 0, 0);

  /* pow (+0, y) == +0 for y an odd integer > 0.  */
  check_float ("pow (0.0, 27) == 0.0",  FUNC(pow) (0.0, 27), 0.0, 0, 0, 0);

  /* pow (-0, y) == -0 for y an odd integer > 0.  */
  check_float ("pow (-0, 27) == -0",  FUNC(pow) (minus_zero, 27), minus_zero, 0, 0, 0);

  /* pow (+0, y) == +0 for y > 0 and not an odd integer.  */
  check_float ("pow (0.0, 4) == 0.0",  FUNC(pow) (0.0, 4), 0.0, 0, 0, 0);

  /* pow (-0, y) == +0 for y > 0 and not an odd integer.  */
  check_float ("pow (-0, 4) == 0.0",  FUNC(pow) (minus_zero, 4), 0.0, 0, 0, 0);

  check_float ("pow (16, 0.25) == 2",  FUNC(pow) (16, 0.25L), 2, 0, 0, 0);
  check_float ("pow (0x1p64, 0.125) == 256",  FUNC(pow) (0x1p64L, 0.125L), 256, 0, 0, 0);
  check_float ("pow (2, 4) == 16",  FUNC(pow) (2, 4), 16, 0, 0, 0);
  check_float ("pow (256, 8) == 0x1p64",  FUNC(pow) (256, 8), 0x1p64L, 0, 0, 0);

  check_float ("pow (0.75, 1.25) == 0.697953644326574699205914060237425566",  FUNC(pow) (0.75L, 1.25L), 0.697953644326574699205914060237425566L, 0, 0, 0);

#if defined TEST_DOUBLE || defined TEST_LDOUBLE
  check_float ("pow (-7.49321e+133, -9.80818e+16) == 0",  FUNC(pow) (-7.49321e+133, -9.80818e+16), 0, 0, 0, 0);
#endif

  print_max_error ("pow", 0, 0);
}

static void
remainder_test (void)
{
  errno = 0;
  FUNC(remainder) (1.625, 1.0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("remainder (1, 0) == NaN plus invalid exception",  FUNC(remainder) (1, 0), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remainder (1, -0) == NaN plus invalid exception",  FUNC(remainder) (1, minus_zero), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remainder (inf, 1) == NaN plus invalid exception",  FUNC(remainder) (plus_infty, 1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remainder (-inf, 1) == NaN plus invalid exception",  FUNC(remainder) (minus_infty, 1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remainder (NaN, NaN) == NaN",  FUNC(remainder) (nan_value, nan_value), nan_value, 0, 0, 0);

  check_float ("remainder (1.625, 1.0) == -0.375",  FUNC(remainder) (1.625, 1.0), -0.375, 0, 0, 0);
  check_float ("remainder (-1.625, 1.0) == 0.375",  FUNC(remainder) (-1.625, 1.0), 0.375, 0, 0, 0);
  check_float ("remainder (1.625, -1.0) == -0.375",  FUNC(remainder) (1.625, -1.0), -0.375, 0, 0, 0);
  check_float ("remainder (-1.625, -1.0) == 0.375",  FUNC(remainder) (-1.625, -1.0), 0.375, 0, 0, 0);
  check_float ("remainder (5.0, 2.0) == 1.0",  FUNC(remainder) (5.0, 2.0), 1.0, 0, 0, 0);
  check_float ("remainder (3.0, 2.0) == -1.0",  FUNC(remainder) (3.0, 2.0), -1.0, 0, 0, 0);

  print_max_error ("remainder", 0, 0);
}

static void
remquo_test (void)
{
  /* x is needed.  */
  int x;

  errno = 0;
  FUNC(remquo) (1.625, 1.0, &x);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("remquo (1, 0, &x) == NaN plus invalid exception",  FUNC(remquo) (1, 0, &x), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remquo (1, -0, &x) == NaN plus invalid exception",  FUNC(remquo) (1, minus_zero, &x), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remquo (inf, 1, &x) == NaN plus invalid exception",  FUNC(remquo) (plus_infty, 1, &x), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remquo (-inf, 1, &x) == NaN plus invalid exception",  FUNC(remquo) (minus_infty, 1, &x), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("remquo (NaN, NaN, &x) == NaN",  FUNC(remquo) (nan_value, nan_value, &x), nan_value, 0, 0, 0);

  check_float ("remquo (1.625, 1.0, &x) == -0.375",  FUNC(remquo) (1.625, 1.0, &x), -0.375, 0, 0, 0);
  check_int ("remquo (1.625, 1.0, &x) sets x to 2", x, 2, 0, 0, 0);
  check_float ("remquo (-1.625, 1.0, &x) == 0.375",  FUNC(remquo) (-1.625, 1.0, &x), 0.375, 0, 0, 0);
  check_int ("remquo (-1.625, 1.0, &x) sets x to -2", x, -2, 0, 0, 0);
  check_float ("remquo (1.625, -1.0, &x) == -0.375",  FUNC(remquo) (1.625, -1.0, &x), -0.375, 0, 0, 0);
  check_int ("remquo (1.625, -1.0, &x) sets x to -2", x, -2, 0, 0, 0);
  check_float ("remquo (-1.625, -1.0, &x) == 0.375",  FUNC(remquo) (-1.625, -1.0, &x), 0.375, 0, 0, 0);
  check_int ("remquo (-1.625, -1.0, &x) sets x to 2", x, 2, 0, 0, 0);

  check_float ("remquo (5, 2, &x) == 1",  FUNC(remquo) (5, 2, &x), 1, 0, 0, 0);
  check_int ("remquo (5, 2, &x) sets x to 2", x, 2, 0, 0, 0);
  check_float ("remquo (3, 2, &x) == -1",  FUNC(remquo) (3, 2, &x), -1, 0, 0, 0);
  check_int ("remquo (3, 2, &x) sets x to 2", x, 2, 0, 0, 0);

  print_max_error ("remquo", 0, 0);
}

static void
rint_test (void)
{
  init_max_error ();

  check_float ("rint (0.0) == 0.0",  FUNC(rint) (0.0), 0.0, 0, 0, 0);
  check_float ("rint (-0) == -0",  FUNC(rint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("rint (inf) == inf",  FUNC(rint) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("rint (-inf) == -inf",  FUNC(rint) (minus_infty), minus_infty, 0, 0, 0);

  /* Default rounding mode is round to even.  */
  check_float ("rint (0.5) == 0.0",  FUNC(rint) (0.5), 0.0, 0, 0, 0);
  check_float ("rint (1.5) == 2.0",  FUNC(rint) (1.5), 2.0, 0, 0, 0);
  check_float ("rint (2.5) == 2.0",  FUNC(rint) (2.5), 2.0, 0, 0, 0);
  check_float ("rint (3.5) == 4.0",  FUNC(rint) (3.5), 4.0, 0, 0, 0);
  check_float ("rint (4.5) == 4.0",  FUNC(rint) (4.5), 4.0, 0, 0, 0);
  check_float ("rint (-0.5) == -0.0",  FUNC(rint) (-0.5), -0.0, 0, 0, 0);
  check_float ("rint (-1.5) == -2.0",  FUNC(rint) (-1.5), -2.0, 0, 0, 0);
  check_float ("rint (-2.5) == -2.0",  FUNC(rint) (-2.5), -2.0, 0, 0, 0);
  check_float ("rint (-3.5) == -4.0",  FUNC(rint) (-3.5), -4.0, 0, 0, 0);
  check_float ("rint (-4.5) == -4.0",  FUNC(rint) (-4.5), -4.0, 0, 0, 0);
  check_float ("rint (0.1) == 0.0",  FUNC(rint) (0.1), 0.0, 0, 0, 0);
  check_float ("rint (0.25) == 0.0",  FUNC(rint) (0.25), 0.0, 0, 0, 0);
  check_float ("rint (0.625) == 1.0",  FUNC(rint) (0.625), 1.0, 0, 0, 0);
  check_float ("rint (-0.1) == -0.0",  FUNC(rint) (-0.1), -0.0, 0, 0, 0);
  check_float ("rint (-0.25) == -0.0",  FUNC(rint) (-0.25), -0.0, 0, 0, 0);
  check_float ("rint (-0.625) == -1.0",  FUNC(rint) (-0.625), -1.0, 0, 0, 0);
  check_float ("rint (262144.75) == 262145.0",  FUNC(rint) (262144.75), 262145.0, 0, 0, 0);
  check_float ("rint (262142.75) == 262143.0",  FUNC(rint) (262142.75), 262143.0, 0, 0, 0);
  check_float ("rint (524286.75) == 524287.0",  FUNC(rint) (524286.75), 524287.0, 0, 0, 0);
  check_float ("rint (524288.75) == 524289.0",  FUNC(rint) (524288.75), 524289.0, 0, 0, 0);
#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
  check_float ("rint (4503599627370495.5) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint (4503599627370496.25) == 4503599627370496.0",  FUNC(rint) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint (4503599627370496.5) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint (4503599627370496.75) == 4503599627370497.0",  FUNC(rint) (4503599627370496.75L), 4503599627370497.0L, 0, 0, 0);
  check_float ("rint (4503599627370497.5) == 4503599627370498.0",  FUNC(rint) (4503599627370497.5L), 4503599627370498.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("rint (4503599627370494.5000000000001) == 4503599627370495.0",  FUNC(rint) (4503599627370494.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint (4503599627370495.5000000000001) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5000000000001L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint (4503599627370496.5000000000001) == 4503599627370497.0",  FUNC(rint) (4503599627370496.5000000000001L), 4503599627370497.0L, 0, 0, 0);
# endif

  check_float ("rint (-4503599627370495.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint (-4503599627370496.25) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint (-4503599627370496.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint (-4503599627370496.75) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.75L), -4503599627370497.0L, 0, 0, 0);
  check_float ("rint (-4503599627370497.5) == -4503599627370498.0",  FUNC(rint) (-4503599627370497.5L), -4503599627370498.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("rint (-4503599627370494.5000000000001) == -4503599627370495.0",  FUNC(rint) (-4503599627370494.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint (-4503599627370495.5000000000001) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5000000000001L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint (-4503599627370496.5000000000001) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.5000000000001L), -4503599627370497.0L, 0, 0, 0);

  check_float ("rint (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint (9007199254740991.5000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740991.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint (9007199254740992.5000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740992.5000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint (9007199254740993.5000000000001) == 9007199254740994.0",  FUNC(rint) (9007199254740993.5000000000001L), 9007199254740994.0L, 0, 0, 0);

  check_float ("rint (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint (-9007199254740991.5000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740991.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint (-9007199254740992.5000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740992.5000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint (-9007199254740993.5000000000001) == -9007199254740994.0",  FUNC(rint) (-9007199254740993.5000000000001L), -9007199254740994.0L, 0, 0, 0);
# endif

  check_float ("rint (9007199254740991.5) == 9007199254740992.0",  FUNC(rint) (9007199254740991.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint (9007199254740992.25) == 9007199254740992.0",  FUNC(rint) (9007199254740992.25L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint (9007199254740992.5) == 9007199254740992.0",  FUNC(rint) (9007199254740992.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint (9007199254740992.75) == 9007199254740993.0",  FUNC(rint) (9007199254740992.75L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint (9007199254740993.5) == 9007199254740994.0",  FUNC(rint) (9007199254740993.5L), 9007199254740994.0L, 0, 0, 0);

  check_float ("rint (-9007199254740991.5) == -9007199254740992.0",  FUNC(rint) (-9007199254740991.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint (-9007199254740992.25) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.25L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint (-9007199254740992.5) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint (-9007199254740992.75) == -9007199254740993.0",  FUNC(rint) (-9007199254740992.75L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint (-9007199254740993.5) == -9007199254740994.0",  FUNC(rint) (-9007199254740993.5L), -9007199254740994.0L, 0, 0, 0);

  check_float ("rint (72057594037927935.5) == 72057594037927936.0",  FUNC(rint) (72057594037927935.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("rint (72057594037927936.25) == 72057594037927936.0",  FUNC(rint) (72057594037927936.25L), 72057594037927936.0L, 0, 0, 0);
  check_float ("rint (72057594037927936.5) == 72057594037927936.0",  FUNC(rint) (72057594037927936.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("rint (72057594037927936.75) == 72057594037927937.0",  FUNC(rint) (72057594037927936.75L), 72057594037927937.0L, 0, 0, 0);
  check_float ("rint (72057594037927937.5) == 72057594037927938.0",  FUNC(rint) (72057594037927937.5L), 72057594037927938.0L, 0, 0, 0);

  check_float ("rint (-72057594037927935.5) == -72057594037927936.0",  FUNC(rint) (-72057594037927935.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("rint (-72057594037927936.25) == -72057594037927936.0",  FUNC(rint) (-72057594037927936.25L), -72057594037927936.0L, 0, 0, 0);
  check_float ("rint (-72057594037927936.5) == -72057594037927936.0",  FUNC(rint) (-72057594037927936.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("rint (-72057594037927936.75) == -72057594037927937.0",  FUNC(rint) (-72057594037927936.75L), -72057594037927937.0L, 0, 0, 0);
  check_float ("rint (-72057594037927937.5) == -72057594037927938.0",  FUNC(rint) (-72057594037927937.5L), -72057594037927938.0L, 0, 0, 0);

  check_float ("rint (10141204801825835211973625643007.5) == 10141204801825835211973625643008.0",  FUNC(rint) (10141204801825835211973625643007.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("rint (10141204801825835211973625643008.25) == 10141204801825835211973625643008.0",  FUNC(rint) (10141204801825835211973625643008.25L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("rint (10141204801825835211973625643008.5) == 10141204801825835211973625643008.0",  FUNC(rint) (10141204801825835211973625643008.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("rint (10141204801825835211973625643008.75) == 10141204801825835211973625643009.0",  FUNC(rint) (10141204801825835211973625643008.75L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("rint (10141204801825835211973625643009.5) == 10141204801825835211973625643010.0",  FUNC(rint) (10141204801825835211973625643009.5L), 10141204801825835211973625643010.0L, 0, 0, 0);
#endif

  print_max_error ("rint", 0, 0);
}

#if 0
static void
rint_test_tonearest (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TONEAREST))
    {
  check_float ("rint_tonearest (2.0) == 2.0",  FUNC(rint) (2.0), 2.0, 0, 0, 0);
  check_float ("rint_tonearest (1.5) == 2.0",  FUNC(rint) (1.5), 2.0, 0, 0, 0);
  check_float ("rint_tonearest (1.0) == 1.0",  FUNC(rint) (1.0), 1.0, 0, 0, 0);
  check_float ("rint_tonearest (0.5) == 0.0",  FUNC(rint) (0.5), 0.0, 0, 0, 0);
  check_float ("rint_tonearest (0.0) == 0.0",  FUNC(rint) (0.0), 0.0, 0, 0, 0);
  check_float ("rint_tonearest (-0) == -0",  FUNC(rint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("rint_tonearest (-0.5) == -0.0",  FUNC(rint) (-0.5), -0.0, 0, 0, 0);
  check_float ("rint_tonearest (-1.0) == -1.0",  FUNC(rint) (-1.0), -1.0, 0, 0, 0);
  check_float ("rint_tonearest (-1.5) == -2.0",  FUNC(rint) (-1.5), -2.0, 0, 0, 0);
  check_float ("rint_tonearest (-2.0) == -2.0",  FUNC(rint) (-2.0), -2.0, 0, 0, 0);
  check_float ("rint_tonearest (0.1) == 0.0",  FUNC(rint) (0.1), 0.0, 0, 0, 0);
  check_float ("rint_tonearest (0.25) == 0.0",  FUNC(rint) (0.25), 0.0, 0, 0, 0);
  check_float ("rint_tonearest (0.625) == 1.0",  FUNC(rint) (0.625), 1.0, 0, 0, 0);
  check_float ("rint_tonearest (-0.1) == -0.0",  FUNC(rint) (-0.1), -0.0, 0, 0, 0);
  check_float ("rint_tonearest (-0.25) == -0.0",  FUNC(rint) (-0.25), -0.0, 0, 0, 0);
  check_float ("rint_tonearest (-0.625) == -1.0",  FUNC(rint) (-0.625), -1.0, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The result can only be represented in long double.  */
  check_float ("rint_tonearest (4503599627370495.5) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370496.25) == 4503599627370496.0",  FUNC(rint) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370496.5) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370496.75) == 4503599627370497.0",  FUNC(rint) (4503599627370496.75L), 4503599627370497.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370497.5) == 4503599627370498.0",  FUNC(rint) (4503599627370497.5L), 4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_tonearest (4503599627370494.5000000000001) == 4503599627370495.0",  FUNC(rint) (4503599627370494.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370495.5000000000001) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5000000000001L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (4503599627370496.5000000000001) == 4503599627370497.0",  FUNC(rint) (4503599627370496.5000000000001L), 4503599627370497.0L, 0, 0, 0);
# endif
  check_float ("rint_tonearest (-4503599627370495.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370496.25) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370496.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370496.75) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.75L), -4503599627370497.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370497.5) == -4503599627370498.0",  FUNC(rint) (-4503599627370497.5L), -4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_tonearest (-4503599627370494.5000000000001) == -4503599627370495.0",  FUNC(rint) (-4503599627370494.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370495.5000000000001) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5000000000001L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_tonearest (-4503599627370496.5000000000001) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.5000000000001L), -4503599627370497.0L, 0, 0, 0);

  check_float ("rint_tonearest (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint_tonearest (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_tonearest (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_tonearest (9007199254740991.5000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740991.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_tonearest (9007199254740992.5000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740992.5000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_tonearest (9007199254740993.5000000000001) == 9007199254740994.0",  FUNC(rint) (9007199254740993.5000000000001L), 9007199254740994.0L, 0, 0, 0);

  check_float ("rint_tonearest (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint_tonearest (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_tonearest (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_tonearest (-9007199254740991.5000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740991.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_tonearest (-9007199254740992.5000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740992.5000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_tonearest (-9007199254740993.5000000000001) == -9007199254740994.0",  FUNC(rint) (-9007199254740993.5000000000001L), -9007199254740994.0L, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("rint_tonearest", 0, 0);
}

static void
rint_test_towardzero (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_TOWARDZERO))
    {
  check_float ("rint_towardzero (2.0) == 2.0",  FUNC(rint) (2.0), 2.0, 0, 0, 0);
  check_float ("rint_towardzero (1.5) == 1.0",  FUNC(rint) (1.5), 1.0, 0, 0, 0);
  check_float ("rint_towardzero (1.0) == 1.0",  FUNC(rint) (1.0), 1.0, 0, 0, 0);
  check_float ("rint_towardzero (0.5) == 0.0",  FUNC(rint) (0.5), 0.0, 0, 0, 0);
  check_float ("rint_towardzero (0.0) == 0.0",  FUNC(rint) (0.0), 0.0, 0, 0, 0);
  check_float ("rint_towardzero (-0) == -0",  FUNC(rint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("rint_towardzero (-0.5) == -0.0",  FUNC(rint) (-0.5), -0.0, 0, 0, 0);
  check_float ("rint_towardzero (-1.0) == -1.0",  FUNC(rint) (-1.0), -1.0, 0, 0, 0);
  check_float ("rint_towardzero (-1.5) == -1.0",  FUNC(rint) (-1.5), -1.0, 0, 0, 0);
  check_float ("rint_towardzero (-2.0) == -2.0",  FUNC(rint) (-2.0), -2.0, 0, 0, 0);
  check_float ("rint_towardzero (0.1) == 0.0",  FUNC(rint) (0.1), 0.0, 0, 0, 0);
  check_float ("rint_towardzero (0.25) == 0.0",  FUNC(rint) (0.25), 0.0, 0, 0, 0);
  check_float ("rint_towardzero (0.625) == 0.0",  FUNC(rint) (0.625), 0.0, 0, 0, 0);
  check_float ("rint_towardzero (-0.1) == -0.0",  FUNC(rint) (-0.1), -0.0, 0, 0, 0);
  check_float ("rint_towardzero (-0.25) == -0.0",  FUNC(rint) (-0.25), -0.0, 0, 0, 0);
  check_float ("rint_towardzero (-0.625) == -0.0",  FUNC(rint) (-0.625), -0.0, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The result can only be represented in long double.  */
  check_float ("rint_towardzero (4503599627370495.5) == 4503599627370495.0",  FUNC(rint) (4503599627370495.5L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370496.25) == 4503599627370496.0",  FUNC(rint) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370496.5) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370496.75) == 4503599627370496.0",  FUNC(rint) (4503599627370496.75L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370497.5) == 4503599627370497.0",  FUNC(rint) (4503599627370497.5L), 4503599627370497.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_towardzero (4503599627370494.5000000000001) == 4503599627370494.0",  FUNC(rint) (4503599627370494.5000000000001L), 4503599627370494.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370495.5000000000001) == 4503599627370495.0",  FUNC(rint) (4503599627370495.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_towardzero (4503599627370496.5000000000001) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5000000000001L), 4503599627370496.0L, 0, 0, 0);
# endif
  check_float ("rint_towardzero (-4503599627370495.5) == -4503599627370495.0",  FUNC(rint) (-4503599627370495.5L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370496.25) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370496.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370496.75) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.75L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370497.5) == -4503599627370497.0",  FUNC(rint) (-4503599627370497.5L), -4503599627370497.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_towardzero (-4503599627370494.5000000000001) == -4503599627370494.0",  FUNC(rint) (-4503599627370494.5000000000001L), -4503599627370494.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370495.5000000000001) == -4503599627370495.0",  FUNC(rint) (-4503599627370495.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_towardzero (-4503599627370496.5000000000001) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5000000000001L), -4503599627370496.0L, 0, 0, 0);

  check_float ("rint_towardzero (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint_towardzero (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_towardzero (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_towardzero (9007199254740991.5000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.5000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint_towardzero (9007199254740992.5000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_towardzero (9007199254740993.5000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.5000000000001L), 9007199254740993.0L, 0, 0, 0);

  check_float ("rint_towardzero (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint_towardzero (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_towardzero (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_towardzero (-9007199254740991.5000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.5000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint_towardzero (-9007199254740992.5000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_towardzero (-9007199254740993.5000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.5000000000001L), -9007199254740993.0L, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("rint_towardzero", 0, 0);
}

static void
rint_test_downward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_DOWNWARD))
    {
  check_float ("rint_downward (2.0) == 2.0",  FUNC(rint) (2.0), 2.0, 0, 0, 0);
  check_float ("rint_downward (1.5) == 1.0",  FUNC(rint) (1.5), 1.0, 0, 0, 0);
  check_float ("rint_downward (1.0) == 1.0",  FUNC(rint) (1.0), 1.0, 0, 0, 0);
  check_float ("rint_downward (0.5) == 0.0",  FUNC(rint) (0.5), 0.0, 0, 0, 0);
  check_float ("rint_downward (0.0) == 0.0",  FUNC(rint) (0.0), 0.0, 0, 0, 0);
  check_float ("rint_downward (-0) == -0",  FUNC(rint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("rint_downward (-0.5) == -1.0",  FUNC(rint) (-0.5), -1.0, 0, 0, 0);
  check_float ("rint_downward (-1.0) == -1.0",  FUNC(rint) (-1.0), -1.0, 0, 0, 0);
  check_float ("rint_downward (-1.5) == -2.0",  FUNC(rint) (-1.5), -2.0, 0, 0, 0);
  check_float ("rint_downward (-2.0) == -2.0",  FUNC(rint) (-2.0), -2.0, 0, 0, 0);
  check_float ("rint_downward (0.1) == 0.0",  FUNC(rint) (0.1), 0.0, 0, 0, 0);
  check_float ("rint_downward (0.25) == 0.0",  FUNC(rint) (0.25), 0.0, 0, 0, 0);
  check_float ("rint_downward (0.625) == 0.0",  FUNC(rint) (0.625), 0.0, 0, 0, 0);
  check_float ("rint_downward (-0.1) == -1.0",  FUNC(rint) (-0.1), -1.0, 0, 0, 0);
  check_float ("rint_downward (-0.25) == -1.0",  FUNC(rint) (-0.25), -1.0, 0, 0, 0);
  check_float ("rint_downward (-0.625) == -1.0",  FUNC(rint) (-0.625), -1.0, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The result can only be represented in long double.  */
  check_float ("rint_downward (4503599627370495.5) == 4503599627370495.0",  FUNC(rint) (4503599627370495.5L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370496.25) == 4503599627370496.0",  FUNC(rint) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370496.5) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370496.75) == 4503599627370496.0",  FUNC(rint) (4503599627370496.75L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370497.5) == 4503599627370497.0",  FUNC(rint) (4503599627370497.5L), 4503599627370497.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_downward (4503599627370494.5000000000001) == 4503599627370494.0",  FUNC(rint) (4503599627370494.5000000000001L), 4503599627370494.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370495.5000000000001) == 4503599627370495.0",  FUNC(rint) (4503599627370495.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_downward (4503599627370496.5000000000001) == 4503599627370496.0",  FUNC(rint) (4503599627370496.5000000000001L), 4503599627370496.0L, 0, 0, 0);
# endif
  check_float ("rint_downward (-4503599627370495.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370496.25) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.25L), -4503599627370497.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370496.5) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.5L), -4503599627370497.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370496.75) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.75L), -4503599627370497.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370497.5) == -4503599627370498.0",  FUNC(rint) (-4503599627370497.5L), -4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_downward (-4503599627370494.5000000000001) == -4503599627370495.0",  FUNC(rint) (-4503599627370494.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370495.5000000000001) == -4503599627370496.0",  FUNC(rint) (-4503599627370495.5000000000001L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_downward (-4503599627370496.5000000000001) == -4503599627370497.0",  FUNC(rint) (-4503599627370496.5000000000001L), -4503599627370497.0L, 0, 0, 0);

  check_float ("rint_downward (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint_downward (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_downward (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_downward (9007199254740991.5000000000001) == 9007199254740991.0",  FUNC(rint) (9007199254740991.5000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("rint_downward (9007199254740992.5000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740992.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_downward (9007199254740993.5000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740993.5000000000001L), 9007199254740993.0L, 0, 0, 0);

  check_float ("rint_downward (-9007199254740991.0000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740991.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_downward (-9007199254740992.0000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740992.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_downward (-9007199254740993.0000000000001) == -9007199254740994.0",  FUNC(rint) (-9007199254740993.0000000000001L), -9007199254740994.0L, 0, 0, 0);
  check_float ("rint_downward (-9007199254740991.5000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740991.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_downward (-9007199254740992.5000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740992.5000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_downward (-9007199254740993.5000000000001) == -9007199254740994.0",  FUNC(rint) (-9007199254740993.5000000000001L), -9007199254740994.0L, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("rint_downward", 0, 0);
}

static void
rint_test_upward (void)
{
  int save_round_mode;
  init_max_error ();

  save_round_mode = fegetround ();

  if (!fesetround (FE_UPWARD))
    {
  check_float ("rint_upward (2.0) == 2.0",  FUNC(rint) (2.0), 2.0, 0, 0, 0);
  check_float ("rint_upward (1.5) == 2.0",  FUNC(rint) (1.5), 2.0, 0, 0, 0);
  check_float ("rint_upward (1.0) == 1.0",  FUNC(rint) (1.0), 1.0, 0, 0, 0);
  check_float ("rint_upward (0.5) == 1.0",  FUNC(rint) (0.5), 1.0, 0, 0, 0);
  check_float ("rint_upward (0.0) == 0.0",  FUNC(rint) (0.0), 0.0, 0, 0, 0);
  check_float ("rint_upward (-0) == -0",  FUNC(rint) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("rint_upward (-0.5) == -0.0",  FUNC(rint) (-0.5), -0.0, 0, 0, 0);
  check_float ("rint_upward (-1.0) == -1.0",  FUNC(rint) (-1.0), -1.0, 0, 0, 0);
  check_float ("rint_upward (-1.5) == -1.0",  FUNC(rint) (-1.5), -1.0, 0, 0, 0);
  check_float ("rint_upward (-2.0) == -2.0",  FUNC(rint) (-2.0), -2.0, 0, 0, 0);
  check_float ("rint_upward (0.1) == 1.0",  FUNC(rint) (0.1), 1.0, 0, 0, 0);
  check_float ("rint_upward (0.25) == 1.0",  FUNC(rint) (0.25), 1.0, 0, 0, 0);
  check_float ("rint_upward (0.625) == 1.0",  FUNC(rint) (0.625), 1.0, 0, 0, 0);
  check_float ("rint_upward (-0.1) == -0.0",  FUNC(rint) (-0.1), -0.0, 0, 0, 0);
  check_float ("rint_upward (-0.25) == -0.0",  FUNC(rint) (-0.25), -0.0, 0, 0, 0);
  check_float ("rint_upward (-0.625) == -0.0",  FUNC(rint) (-0.625), -0.0, 0, 0, 0);
#ifdef TEST_LDOUBLE
      /* The result can only be represented in long double.  */
  check_float ("rint_upward (4503599627370495.5) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370496.25) == 4503599627370497.0",  FUNC(rint) (4503599627370496.25L), 4503599627370497.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370496.5) == 4503599627370497.0",  FUNC(rint) (4503599627370496.5L), 4503599627370497.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370496.75) == 4503599627370497.0",  FUNC(rint) (4503599627370496.75L), 4503599627370497.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370497.5) == 4503599627370498.0",  FUNC(rint) (4503599627370497.5L), 4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_upward (4503599627370494.5000000000001) == 4503599627370495.0",  FUNC(rint) (4503599627370494.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370495.5000000000001) == 4503599627370496.0",  FUNC(rint) (4503599627370495.5000000000001L), 4503599627370496.0L, 0, 0, 0);
  check_float ("rint_upward (4503599627370496.5000000000001) == 4503599627370497.0",  FUNC(rint) (4503599627370496.5000000000001L), 4503599627370497.0L, 0, 0, 0);
# endif
  check_float ("rint_upward (-4503599627370495.5) == -4503599627370495.0",  FUNC(rint) (-4503599627370495.5L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370496.25) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370496.5) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370496.75) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.75L), -4503599627370496.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370497.5) == -4503599627370497.0",  FUNC(rint) (-4503599627370497.5L), -4503599627370497.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("rint_upward (-4503599627370494.5000000000001) == -4503599627370494.0",  FUNC(rint) (-4503599627370494.5000000000001L), -4503599627370494.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370495.5000000000001) == -4503599627370495.0",  FUNC(rint) (-4503599627370495.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("rint_upward (-4503599627370496.5000000000001) == -4503599627370496.0",  FUNC(rint) (-4503599627370496.5000000000001L), -4503599627370496.0L, 0, 0, 0);

  check_float ("rint_upward (9007199254740991.0000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740991.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_upward (9007199254740992.0000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740992.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_upward (9007199254740993.0000000000001) == 9007199254740994.0",  FUNC(rint) (9007199254740993.0000000000001L), 9007199254740994.0L, 0, 0, 0);
  check_float ("rint_upward (9007199254740991.5000000000001) == 9007199254740992.0",  FUNC(rint) (9007199254740991.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("rint_upward (9007199254740992.5000000000001) == 9007199254740993.0",  FUNC(rint) (9007199254740992.5000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("rint_upward (9007199254740993.5000000000001) == 9007199254740994.0",  FUNC(rint) (9007199254740993.5000000000001L), 9007199254740994.0L, 0, 0, 0);

  check_float ("rint_upward (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint_upward (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_upward (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("rint_upward (-9007199254740991.5000000000001) == -9007199254740991.0",  FUNC(rint) (-9007199254740991.5000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("rint_upward (-9007199254740992.5000000000001) == -9007199254740992.0",  FUNC(rint) (-9007199254740992.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("rint_upward (-9007199254740993.5000000000001) == -9007199254740993.0",  FUNC(rint) (-9007199254740993.5000000000001L), -9007199254740993.0L, 0, 0, 0);
# endif
#endif
    }

  fesetround (save_round_mode);

  print_max_error ("rint_upward", 0, 0);
}
#endif

static void
round_test (void)
{
  init_max_error ();

  check_float ("round (0) == 0",  FUNC(round) (0), 0, 0, 0, 0);
  check_float ("round (-0) == -0",  FUNC(round) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("round (0.2) == 0.0",  FUNC(round) (0.2L), 0.0, 0, 0, 0);
  check_float ("round (-0.2) == -0",  FUNC(round) (-0.2L), minus_zero, 0, 0, 0);
  check_float ("round (0.5) == 1.0",  FUNC(round) (0.5), 1.0, 0, 0, 0);
  check_float ("round (-0.5) == -1.0",  FUNC(round) (-0.5), -1.0, 0, 0, 0);
  check_float ("round (0.8) == 1.0",  FUNC(round) (0.8L), 1.0, 0, 0, 0);
  check_float ("round (-0.8) == -1.0",  FUNC(round) (-0.8L), -1.0, 0, 0, 0);
  check_float ("round (1.5) == 2.0",  FUNC(round) (1.5), 2.0, 0, 0, 0);
  check_float ("round (-1.5) == -2.0",  FUNC(round) (-1.5), -2.0, 0, 0, 0);
  check_float ("round (0.1) == 0.0",  FUNC(round) (0.1), 0.0, 0, 0, 0);
  check_float ("round (0.25) == 0.0",  FUNC(round) (0.25), 0.0, 0, 0, 0);
  check_float ("round (0.625) == 1.0",  FUNC(round) (0.625), 1.0, 0, 0, 0);
  check_float ("round (-0.1) == -0.0",  FUNC(round) (-0.1), -0.0, 0, 0, 0);
  check_float ("round (-0.25) == -0.0",  FUNC(round) (-0.25), -0.0, 0, 0, 0);
  check_float ("round (-0.625) == -1.0",  FUNC(round) (-0.625), -1.0, 0, 0, 0);
  check_float ("round (2097152.5) == 2097153",  FUNC(round) (2097152.5), 2097153, 0, 0, 0);
  check_float ("round (-2097152.5) == -2097153",  FUNC(round) (-2097152.5), -2097153, 0, 0, 0);

#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
  check_float ("round (4503599627370495.5) == 4503599627370496.0",  FUNC(round) (4503599627370495.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("round (4503599627370496.25) == 4503599627370496.0",  FUNC(round) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("round (4503599627370496.5) == 4503599627370497.0",  FUNC(round) (4503599627370496.5L), 4503599627370497.0L, 0, 0, 0);
  check_float ("round (4503599627370496.75) == 4503599627370497.0",  FUNC(round) (4503599627370496.75L), 4503599627370497.0L, 0, 0, 0);
  check_float ("round (4503599627370497.5) == 4503599627370498.0",  FUNC(round) (4503599627370497.5L), 4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("round (4503599627370494.5000000000001) == 4503599627370495.0",  FUNC(round) (4503599627370494.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("round (4503599627370495.5000000000001) == 4503599627370496.0",  FUNC(round) (4503599627370495.5000000000001L), 4503599627370496.0L, 0, 0, 0);
  check_float ("round (4503599627370496.5000000000001) == 4503599627370497.0",  FUNC(round) (4503599627370496.5000000000001L), 4503599627370497.0L, 0, 0, 0);
# endif

  check_float ("round (-4503599627370495.5) == -4503599627370496.0",  FUNC(round) (-4503599627370495.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("round (-4503599627370496.25) == -4503599627370496.0",  FUNC(round) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("round (-4503599627370496.5) == -4503599627370497.0",  FUNC(round) (-4503599627370496.5L), -4503599627370497.0L, 0, 0, 0);
  check_float ("round (-4503599627370496.75) == -4503599627370497.0",  FUNC(round) (-4503599627370496.75L), -4503599627370497.0L, 0, 0, 0);
  check_float ("round (-4503599627370497.5) == -4503599627370498.0",  FUNC(round) (-4503599627370497.5L), -4503599627370498.0L, 0, 0, 0);
# if LDBL_MANT_DIG > 100
  check_float ("round (-4503599627370494.5000000000001) == -4503599627370495.0",  FUNC(round) (-4503599627370494.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("round (-4503599627370495.5000000000001) == -4503599627370496.0",  FUNC(round) (-4503599627370495.5000000000001L), -4503599627370496.0L, 0, 0, 0);
  check_float ("round (-4503599627370496.5000000000001) == -4503599627370497.0",  FUNC(round) (-4503599627370496.5000000000001L), -4503599627370497.0L, 0, 0, 0);
# endif

  check_float ("round (9007199254740991.5) == 9007199254740992.0",  FUNC(round) (9007199254740991.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("round (9007199254740992.25) == 9007199254740992.0",  FUNC(round) (9007199254740992.25L), 9007199254740992.0L, 0, 0, 0);
  check_float ("round (9007199254740992.5) == 9007199254740993.0",  FUNC(round) (9007199254740992.5L), 9007199254740993.0L, 0, 0, 0);
  check_float ("round (9007199254740992.75) == 9007199254740993.0",  FUNC(round) (9007199254740992.75L), 9007199254740993.0L, 0, 0, 0);
  check_float ("round (9007199254740993.5) == 9007199254740994.0",  FUNC(round) (9007199254740993.5L), 9007199254740994.0L, 0, 0, 0);

  check_float ("round (-9007199254740991.5) == -9007199254740992.0",  FUNC(round) (-9007199254740991.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("round (-9007199254740992.25) == -9007199254740992.0",  FUNC(round) (-9007199254740992.25L), -9007199254740992.0L, 0, 0, 0);
  check_float ("round (-9007199254740992.5) == -9007199254740993.0",  FUNC(round) (-9007199254740992.5L), -9007199254740993.0L, 0, 0, 0);
  check_float ("round (-9007199254740992.75) == -9007199254740993.0",  FUNC(round) (-9007199254740992.75L), -9007199254740993.0L, 0, 0, 0);
  check_float ("round (-9007199254740993.5) == -9007199254740994.0",  FUNC(round) (-9007199254740993.5L), -9007199254740994.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("round (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(round) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("round (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(round) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("round (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(round) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("round (9007199254740991.5000000000001) == 9007199254740992.0",  FUNC(round) (9007199254740991.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("round (9007199254740992.5000000000001) == 9007199254740993.0",  FUNC(round) (9007199254740992.5000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("round (9007199254740993.5000000000001) == 9007199254740994.0",  FUNC(round) (9007199254740993.5000000000001L), 9007199254740994.0L, 0, 0, 0);

  check_float ("round (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(round) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("round (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(round) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("round (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(round) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("round (-9007199254740991.5000000000001) == -9007199254740992.0",  FUNC(round) (-9007199254740991.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("round (-9007199254740992.5000000000001) == -9007199254740993.0",  FUNC(round) (-9007199254740992.5000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("round (-9007199254740993.5000000000001) == -9007199254740994.0",  FUNC(round) (-9007199254740993.5000000000001L), -9007199254740994.0L, 0, 0, 0);
# endif

  check_float ("round (72057594037927935.5) == 72057594037927936.0",  FUNC(round) (72057594037927935.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("round (72057594037927936.25) == 72057594037927936.0",  FUNC(round) (72057594037927936.25L), 72057594037927936.0L, 0, 0, 0);
  check_float ("round (72057594037927936.5) == 72057594037927937.0",  FUNC(round) (72057594037927936.5L), 72057594037927937.0L, 0, 0, 0);
  check_float ("round (72057594037927936.75) == 72057594037927937.0",  FUNC(round) (72057594037927936.75L), 72057594037927937.0L, 0, 0, 0);
  check_float ("round (72057594037927937.5) == 72057594037927938.0",  FUNC(round) (72057594037927937.5L), 72057594037927938.0L, 0, 0, 0);

  check_float ("round (-72057594037927935.5) == -72057594037927936.0",  FUNC(round) (-72057594037927935.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("round (-72057594037927936.25) == -72057594037927936.0",  FUNC(round) (-72057594037927936.25L), -72057594037927936.0L, 0, 0, 0);
  check_float ("round (-72057594037927936.5) == -72057594037927937.0",  FUNC(round) (-72057594037927936.5L), -72057594037927937.0L, 0, 0, 0);
  check_float ("round (-72057594037927936.75) == -72057594037927937.0",  FUNC(round) (-72057594037927936.75L), -72057594037927937.0L, 0, 0, 0);
  check_float ("round (-72057594037927937.5) == -72057594037927938.0",  FUNC(round) (-72057594037927937.5L), -72057594037927938.0L, 0, 0, 0);

  check_float ("round (10141204801825835211973625643007.5) == 10141204801825835211973625643008.0",  FUNC(round) (10141204801825835211973625643007.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("round (10141204801825835211973625643008.25) == 10141204801825835211973625643008.0",  FUNC(round) (10141204801825835211973625643008.25L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("round (10141204801825835211973625643008.5) == 10141204801825835211973625643009.0",  FUNC(round) (10141204801825835211973625643008.5L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("round (10141204801825835211973625643008.75) == 10141204801825835211973625643009.0",  FUNC(round) (10141204801825835211973625643008.75L), 10141204801825835211973625643009.0L, 0, 0, 0);
  check_float ("round (10141204801825835211973625643009.5) == 10141204801825835211973625643010.0",  FUNC(round) (10141204801825835211973625643009.5L), 10141204801825835211973625643010.0L, 0, 0, 0);
#endif

  print_max_error ("round", 0, 0);
}


static void
scalb_test (void)
{

  init_max_error ();

  check_float ("scalb (2.0, 0.5) == NaN plus invalid exception",  FUNC(scalb) (2.0, 0.5), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("scalb (3.0, -2.5) == NaN plus invalid exception",  FUNC(scalb) (3.0, -2.5), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("scalb (0, NaN) == NaN",  FUNC(scalb) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("scalb (1, NaN) == NaN",  FUNC(scalb) (1, nan_value), nan_value, 0, 0, 0);

  check_float ("scalb (1, 0) == 1",  FUNC(scalb) (1, 0), 1, 0, 0, 0);
  check_float ("scalb (-1, 0) == -1",  FUNC(scalb) (-1, 0), -1, 0, 0, 0);

  check_float ("scalb (0, inf) == NaN plus invalid exception",  FUNC(scalb) (0, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("scalb (-0, inf) == NaN plus invalid exception",  FUNC(scalb) (minus_zero, plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("scalb (0, 2) == 0",  FUNC(scalb) (0, 2), 0, 0, 0, 0);
  check_float ("scalb (-0, -4) == -0",  FUNC(scalb) (minus_zero, -4), minus_zero, 0, 0, 0);
  check_float ("scalb (0, 0) == 0",  FUNC(scalb) (0, 0), 0, 0, 0, 0);
  check_float ("scalb (-0, 0) == -0",  FUNC(scalb) (minus_zero, 0), minus_zero, 0, 0, 0);
  check_float ("scalb (0, -1) == 0",  FUNC(scalb) (0, -1), 0, 0, 0, 0);
  check_float ("scalb (-0, -10) == -0",  FUNC(scalb) (minus_zero, -10), minus_zero, 0, 0, 0);
  check_float ("scalb (0, -inf) == 0",  FUNC(scalb) (0, minus_infty), 0, 0, 0, 0);
  check_float ("scalb (-0, -inf) == -0",  FUNC(scalb) (minus_zero, minus_infty), minus_zero, 0, 0, 0);

  check_float ("scalb (inf, -1) == inf",  FUNC(scalb) (plus_infty, -1), plus_infty, 0, 0, 0);
  check_float ("scalb (-inf, -10) == -inf",  FUNC(scalb) (minus_infty, -10), minus_infty, 0, 0, 0);
  check_float ("scalb (inf, 0) == inf",  FUNC(scalb) (plus_infty, 0), plus_infty, 0, 0, 0);
  check_float ("scalb (-inf, 0) == -inf",  FUNC(scalb) (minus_infty, 0), minus_infty, 0, 0, 0);
  check_float ("scalb (inf, 2) == inf",  FUNC(scalb) (plus_infty, 2), plus_infty, 0, 0, 0);
  check_float ("scalb (-inf, 100) == -inf",  FUNC(scalb) (minus_infty, 100), minus_infty, 0, 0, 0);

  check_float ("scalb (0.1, -inf) == 0.0",  FUNC(scalb) (0.1L, minus_infty), 0.0, 0, 0, 0);
  check_float ("scalb (-0.1, -inf) == -0",  FUNC(scalb) (-0.1L, minus_infty), minus_zero, 0, 0, 0);

  check_float ("scalb (1, inf) == inf",  FUNC(scalb) (1, plus_infty), plus_infty, 0, 0, 0);
  check_float ("scalb (-1, inf) == -inf",  FUNC(scalb) (-1, plus_infty), minus_infty, 0, 0, 0);
  check_float ("scalb (inf, inf) == inf",  FUNC(scalb) (plus_infty, plus_infty), plus_infty, 0, 0, 0);
  check_float ("scalb (-inf, inf) == -inf",  FUNC(scalb) (minus_infty, plus_infty), minus_infty, 0, 0, 0);

  check_float ("scalb (inf, -inf) == NaN plus invalid exception",  FUNC(scalb) (plus_infty, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("scalb (-inf, -inf) == NaN plus invalid exception",  FUNC(scalb) (minus_infty, minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);

  check_float ("scalb (NaN, 1) == NaN",  FUNC(scalb) (nan_value, 1), nan_value, 0, 0, 0);
  check_float ("scalb (1, NaN) == NaN",  FUNC(scalb) (1, nan_value), nan_value, 0, 0, 0);
  check_float ("scalb (NaN, 0) == NaN",  FUNC(scalb) (nan_value, 0), nan_value, 0, 0, 0);
  check_float ("scalb (0, NaN) == NaN",  FUNC(scalb) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("scalb (NaN, inf) == NaN",  FUNC(scalb) (nan_value, plus_infty), nan_value, 0, 0, 0);
  check_float ("scalb (inf, NaN) == NaN",  FUNC(scalb) (plus_infty, nan_value), nan_value, 0, 0, 0);
  check_float ("scalb (NaN, NaN) == NaN",  FUNC(scalb) (nan_value, nan_value), nan_value, 0, 0, 0);

  check_float ("scalb (0.8, 4) == 12.8",  FUNC(scalb) (0.8L, 4), 12.8L, 0, 0, 0);
  check_float ("scalb (-0.854375, 5) == -27.34",  FUNC(scalb) (-0.854375L, 5), -27.34L, 0, 0, 0);

  print_max_error ("scalb", 0, 0);
}


static void
scalbn_test (void)
{

  init_max_error ();

  check_float ("scalbn (0, 0) == 0",  FUNC(scalbn) (0, 0), 0, 0, 0, 0);
  check_float ("scalbn (-0, 0) == -0",  FUNC(scalbn) (minus_zero, 0), minus_zero, 0, 0, 0);

  check_float ("scalbn (inf, 1) == inf",  FUNC(scalbn) (plus_infty, 1), plus_infty, 0, 0, 0);
  check_float ("scalbn (-inf, 1) == -inf",  FUNC(scalbn) (minus_infty, 1), minus_infty, 0, 0, 0);
  check_float ("scalbn (NaN, 1) == NaN",  FUNC(scalbn) (nan_value, 1), nan_value, 0, 0, 0);

  check_float ("scalbn (0.8, 4) == 12.8",  FUNC(scalbn) (0.8L, 4), 12.8L, 0, 0, 0);
  check_float ("scalbn (-0.854375, 5) == -27.34",  FUNC(scalbn) (-0.854375L, 5), -27.34L, 0, 0, 0);

  check_float ("scalbn (1, 0) == 1",  FUNC(scalbn) (1, 0L), 1, 0, 0, 0);

  print_max_error ("scalbn", 0, 0);
}


static void
scalbln_test (void)
{

  init_max_error ();

  check_float ("scalbln (0, 0) == 0",  FUNC(scalbln) (0, 0), 0, 0, 0, 0);
  check_float ("scalbln (-0, 0) == -0",  FUNC(scalbln) (minus_zero, 0), minus_zero, 0, 0, 0);

  check_float ("scalbln (inf, 1) == inf",  FUNC(scalbln) (plus_infty, 1), plus_infty, 0, 0, 0);
  check_float ("scalbln (-inf, 1) == -inf",  FUNC(scalbln) (minus_infty, 1), minus_infty, 0, 0, 0);
  check_float ("scalbln (NaN, 1) == NaN",  FUNC(scalbln) (nan_value, 1), nan_value, 0, 0, 0);

  check_float ("scalbln (0.8, 4) == 12.8",  FUNC(scalbln) (0.8L, 4), 12.8L, 0, 0, 0);
  check_float ("scalbln (-0.854375, 5) == -27.34",  FUNC(scalbln) (-0.854375L, 5), -27.34L, 0, 0, 0);

  check_float ("scalbln (1, 0) == 1",  FUNC(scalbln) (1, 0L), 1, 0, 0, 0);

  print_max_error ("scalbn", 0, 0);
}


static void
signbit_test (void)
{

  init_max_error ();

  check_bool ("signbit (0) == false", signbit (0), 0, 0, 0, 0);
  check_bool ("signbit (-0) == true", signbit (minus_zero), 1, 0, 0, 0);
  check_bool ("signbit (inf) == false", signbit (plus_infty), 0, 0, 0, 0);
  check_bool ("signbit (-inf) == true", signbit (minus_infty), 1, 0, 0, 0);

  /* signbit (x) != 0 for x < 0.  */
  check_bool ("signbit (-1) == true", signbit (-1), 1, 0, 0, 0);
  /* signbit (x) == 0 for x >= 0.  */
  check_bool ("signbit (1) == false", signbit (1), 0, 0, 0, 0);

  print_max_error ("signbit", 0, 0);
}


static void
sin_test (void)
{
  errno = 0;
  FUNC(sin) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("sin (0) == 0",  FUNC(sin) (0), 0, 0, 0, 0);
  check_float ("sin (-0) == -0",  FUNC(sin) (minus_zero), minus_zero, 0, 0, 0);
  errno = 0;
  check_float ("sin (inf) == NaN plus invalid exception",  FUNC(sin) (plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for sin(+inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("sin (-inf) == NaN plus invalid exception",  FUNC(sin) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for sin(-inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("sin (NaN) == NaN",  FUNC(sin) (nan_value), nan_value, 0, 0, 0);
  check_int ("errno for sin(NaN) unchanged", errno, 0, 0, 0, 0);

  check_float ("sin (pi/6) == 0.5",  FUNC(sin) (M_PI_6l), 0.5, 0, 0, 0);
  check_float ("sin (-pi/6) == -0.5",  FUNC(sin) (-M_PI_6l), -0.5, 0, 0, 0);
  check_float ("sin (pi/2) == 1",  FUNC(sin) (M_PI_2l), 1, 0, 0, 0);
  check_float ("sin (-pi/2) == -1",  FUNC(sin) (-M_PI_2l), -1, 0, 0, 0);
  check_float ("sin (0.75) == 0.681638760023334166733241952779893935",  FUNC(sin) (0.75L), 0.681638760023334166733241952779893935L, 0, 0, 0);

#ifdef TEST_DOUBLE
  check_float ("sin (0.80190127184058835) == 0.71867942238767868",  FUNC(sin) (0.80190127184058835), 0.71867942238767868, 0, 0, 0);
#endif

  print_max_error ("sin", 0, 0);

}


static void
sincos_test (void)
{
  FLOAT sin_res, cos_res;

  errno = 0;
  FUNC(sincos) (0, &sin_res, &cos_res);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  /* sincos is treated differently because it returns void.  */
  FUNC (sincos) (0, &sin_res, &cos_res);
  check_float ("sincos (0, &sin_res, &cos_res) puts 0 in sin_res", sin_res, 0, 0, 0, 0);
  check_float ("sincos (0, &sin_res, &cos_res) puts 1 in cos_res", cos_res, 1, 0, 0, 0);

  FUNC (sincos) (minus_zero, &sin_res, &cos_res);
  check_float ("sincos (-0, &sin_res, &cos_res) puts -0 in sin_res", sin_res, minus_zero, 0, 0, 0);
  check_float ("sincos (-0, &sin_res, &cos_res) puts 1 in cos_res", cos_res, 1, 0, 0, 0);
  FUNC (sincos) (plus_infty, &sin_res, &cos_res);
  check_float ("sincos (inf, &sin_res, &cos_res) puts NaN in sin_res plus invalid exception", sin_res, nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("sincos (inf, &sin_res, &cos_res) puts NaN in cos_res", cos_res, nan_value, 0, 0, 0);
  FUNC (sincos) (minus_infty, &sin_res, &cos_res);
  check_float ("sincos (-inf, &sin_res, &cos_res) puts NaN in sin_res plus invalid exception", sin_res, nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("sincos (-inf, &sin_res, &cos_res) puts NaN in cos_res", cos_res, nan_value, 0, 0, 0);
  FUNC (sincos) (nan_value, &sin_res, &cos_res);
  check_float ("sincos (NaN, &sin_res, &cos_res) puts NaN in sin_res", sin_res, nan_value, 0, 0, 0);
  check_float ("sincos (NaN, &sin_res, &cos_res) puts NaN in cos_res", cos_res, nan_value, 0, 0, 0);

  FUNC (sincos) (M_PI_2l, &sin_res, &cos_res);
  check_float ("sincos (pi/2, &sin_res, &cos_res) puts 1 in sin_res", sin_res, 1, 0, 0, 0);
  check_float ("sincos (pi/2, &sin_res, &cos_res) puts 0 in cos_res", cos_res, 0, 0, 0, 0);
  FUNC (sincos) (M_PI_6l, &sin_res, &cos_res);
  check_float ("sincos (pi/6, &sin_res, &cos_res) puts 0.5 in sin_res", sin_res, 0.5, 0, 0, 0);
  check_float ("sincos (pi/6, &sin_res, &cos_res) puts 0.86602540378443864676372317075293616 in cos_res", cos_res, 0.86602540378443864676372317075293616L, 0, 0, 0);
  FUNC (sincos) (M_PI_6l*2.0, &sin_res, &cos_res);
  check_float ("sincos (M_PI_6l*2.0, &sin_res, &cos_res) puts 0.86602540378443864676372317075293616 in sin_res", sin_res, 0.86602540378443864676372317075293616L, 0, 0, 0);
  check_float ("sincos (M_PI_6l*2.0, &sin_res, &cos_res) puts 0.5 in cos_res", cos_res, 0.5, 0, 0, 0);
  FUNC (sincos) (0.75L, &sin_res, &cos_res);
  check_float ("sincos (0.75, &sin_res, &cos_res) puts 0.681638760023334166733241952779893935 in sin_res", sin_res, 0.681638760023334166733241952779893935L, 0, 0, 0);
  check_float ("sincos (0.75, &sin_res, &cos_res) puts 0.731688868873820886311838753000084544 in cos_res", cos_res, 0.731688868873820886311838753000084544L, 0, 0, 0);

#ifdef TEST_DOUBLE
  FUNC (sincos) (0.80190127184058835, &sin_res, &cos_res);
  check_float ("sincos (0.80190127184058835, &sin_res, &cos_res) puts 0.71867942238767868 in sin_res", sin_res, 0.71867942238767868, 0, 0, 0);
  check_float ("sincos (0.80190127184058835, &sin_res, &cos_res) puts 0.69534156199418473 in cos_res", cos_res, 0.69534156199418473, 0, 0, 0);
#endif

  print_max_error ("sincos", 0, 0);
}

static void
sinh_test (void)
{
  errno = 0;
  FUNC(sinh) (0.7L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();
  check_float ("sinh (0) == 0",  FUNC(sinh) (0), 0, 0, 0, 0);
  check_float ("sinh (-0) == -0",  FUNC(sinh) (minus_zero), minus_zero, 0, 0, 0);

#ifndef TEST_INLINE
  check_float ("sinh (inf) == inf",  FUNC(sinh) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("sinh (-inf) == -inf",  FUNC(sinh) (minus_infty), minus_infty, 0, 0, 0);
#endif
  check_float ("sinh (NaN) == NaN",  FUNC(sinh) (nan_value), nan_value, 0, 0, 0);

  check_float ("sinh (0.75) == 0.822316731935829980703661634446913849",  FUNC(sinh) (0.75L), 0.822316731935829980703661634446913849L, 0, 0, 0);
  check_float ("sinh (0x8p-32) == 1.86264514923095703232705808926175479e-9",  FUNC(sinh) (0x8p-32L), 1.86264514923095703232705808926175479e-9L, 0, 0, 0);

  print_max_error ("sinh", 0, 0);
}

static void
sqrt_test (void)
{
  errno = 0;
  FUNC(sqrt) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("sqrt (0) == 0",  FUNC(sqrt) (0), 0, 0, 0, 0);
  check_float ("sqrt (NaN) == NaN",  FUNC(sqrt) (nan_value), nan_value, 0, 0, 0);
  check_float ("sqrt (inf) == inf",  FUNC(sqrt) (plus_infty), plus_infty, 0, 0, 0);

  check_float ("sqrt (-0) == -0",  FUNC(sqrt) (minus_zero), minus_zero, 0, 0, 0);

  /* sqrt (x) == NaN plus invalid exception for x < 0.  */
  check_float ("sqrt (-1) == NaN plus invalid exception",  FUNC(sqrt) (-1), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("sqrt (-inf) == NaN plus invalid exception",  FUNC(sqrt) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("sqrt (NaN) == NaN",  FUNC(sqrt) (nan_value), nan_value, 0, 0, 0);

  check_float ("sqrt (2209) == 47",  FUNC(sqrt) (2209), 47, 0, 0, 0);
  check_float ("sqrt (4) == 2",  FUNC(sqrt) (4), 2, 0, 0, 0);
  check_float ("sqrt (2) == M_SQRT2l",  FUNC(sqrt) (2), M_SQRT2l, 0, 0, 0);
  check_float ("sqrt (0.25) == 0.5",  FUNC(sqrt) (0.25), 0.5, 0, 0, 0);
  check_float ("sqrt (6642.25) == 81.5",  FUNC(sqrt) (6642.25), 81.5, 0, 0, 0);
  check_float ("sqrt (15190.5625) == 123.25",  FUNC(sqrt) (15190.5625L), 123.25L, 0, 0, 0);
  check_float ("sqrt (0.75) == 0.866025403784438646763723170752936183",  FUNC(sqrt) (0.75L), 0.866025403784438646763723170752936183L, 0, 0, 0);

  print_max_error ("sqrt", 0, 0);
}


static void
tan_test (void)
{
  errno = 0;
  FUNC(tan) (0);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("tan (0) == 0",  FUNC(tan) (0), 0, 0, 0, 0);
  check_float ("tan (-0) == -0",  FUNC(tan) (minus_zero), minus_zero, 0, 0, 0);
  errno = 0;
  check_float ("tan (inf) == NaN plus invalid exception",  FUNC(tan) (plus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for tan(Inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("tan (-inf) == NaN plus invalid exception",  FUNC(tan) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_int ("errno for tan(-Inf) == EDOM", errno, EDOM, 0, 0, 0);
  errno = 0;
  check_float ("tan (NaN) == NaN",  FUNC(tan) (nan_value), nan_value, 0, 0, 0);
  check_int ("errno for tan(NaN) == 0", errno, 0, 0, 0, 0);

  check_float ("tan (pi/4) == 1",  FUNC(tan) (M_PI_4l), 1, 0, 0, 0);
  check_float ("tan (0.75) == 0.931596459944072461165202756573936428",  FUNC(tan) (0.75L), 0.931596459944072461165202756573936428L, 0, 0, 0);

  print_max_error ("tan", 0, 0);
}

static void
tanh_test (void)
{
  errno = 0;
  FUNC(tanh) (0.7L);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  init_max_error ();

  check_float ("tanh (0) == 0",  FUNC(tanh) (0), 0, 0, 0, 0);
  check_float ("tanh (-0) == -0",  FUNC(tanh) (minus_zero), minus_zero, 0, 0, 0);

#ifndef TEST_INLINE
  check_float ("tanh (inf) == 1",  FUNC(tanh) (plus_infty), 1, 0, 0, 0);
  check_float ("tanh (-inf) == -1",  FUNC(tanh) (minus_infty), -1, 0, 0, 0);
#endif
  check_float ("tanh (NaN) == NaN",  FUNC(tanh) (nan_value), nan_value, 0, 0, 0);

  check_float ("tanh (0.75) == 0.635148952387287319214434357312496495",  FUNC(tanh) (0.75L), 0.635148952387287319214434357312496495L, 0, 0, 0);
  check_float ("tanh (-0.75) == -0.635148952387287319214434357312496495",  FUNC(tanh) (-0.75L), -0.635148952387287319214434357312496495L, 0, 0, 0);

  check_float ("tanh (1.0) == 0.7615941559557648881194582826047935904",  FUNC(tanh) (1.0L), 0.7615941559557648881194582826047935904L, 0, 0, 0);
  check_float ("tanh (-1.0) == -0.7615941559557648881194582826047935904",  FUNC(tanh) (-1.0L), -0.7615941559557648881194582826047935904L, 0, 0, 0);

  /* 2^-57  */
  check_float ("tanh (0x1p-57) == 6.938893903907228377647697925567626953125e-18",  FUNC(tanh) (0x1p-57L), 6.938893903907228377647697925567626953125e-18L, 0, 0, 0);

  print_max_error ("tanh", 0, 0);
}

static void
tgamma_test (void)
{
  errno = 0;
  FUNC(tgamma) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;
  feclearexcept (FE_ALL_EXCEPT);

  init_max_error ();

  check_float ("tgamma (inf) == inf",  FUNC(tgamma) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("tgamma (0) == inf plus division by zero exception",  FUNC(tgamma) (0), plus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  check_float ("tgamma (-0) == -inf plus division by zero exception",  FUNC(tgamma) (minus_zero), minus_infty, 0, 0, DIVIDE_BY_ZERO_EXCEPTION);
  /* tgamma (x) == NaN plus invalid exception for integer x <= 0.  */
  check_float ("tgamma (-2) == NaN plus invalid exception",  FUNC(tgamma) (-2), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("tgamma (-inf) == NaN plus invalid exception",  FUNC(tgamma) (minus_infty), nan_value, 0, 0, INVALID_EXCEPTION);
  check_float ("tgamma (NaN) == NaN",  FUNC(tgamma) (nan_value), nan_value, 0, 0, 0);

  check_float ("tgamma (0.5) == sqrt (pi)",  FUNC(tgamma) (0.5), M_SQRT_PIl, 0, 0, 0);
  check_float ("tgamma (-0.5) == -2 sqrt (pi)",  FUNC(tgamma) (-0.5), -M_2_SQRT_PIl, 0, 0, 0);

  check_float ("tgamma (1) == 1",  FUNC(tgamma) (1), 1, 0, 0, 0);
  check_float ("tgamma (4) == 6",  FUNC(tgamma) (4), 6, 0, 0, 0);

  check_float ("tgamma (0.7) == 1.29805533264755778568117117915281162",  FUNC(tgamma) (0.7L), 1.29805533264755778568117117915281162L, 0, 0, 0);
  check_float ("tgamma (1.2) == 0.918168742399760610640951655185830401",  FUNC(tgamma) (1.2L), 0.918168742399760610640951655185830401L, 0, 0, 0);

  print_max_error ("tgamma", 0, 0);
}


static void
trunc_test (void)
{
  init_max_error ();

  check_float ("trunc (inf) == inf",  FUNC(trunc) (plus_infty), plus_infty, 0, 0, 0);
  check_float ("trunc (-inf) == -inf",  FUNC(trunc) (minus_infty), minus_infty, 0, 0, 0);
  check_float ("trunc (NaN) == NaN",  FUNC(trunc) (nan_value), nan_value, 0, 0, 0);

  check_float ("trunc (0) == 0",  FUNC(trunc) (0), 0, 0, 0, 0);
  check_float ("trunc (-0) == -0",  FUNC(trunc) (minus_zero), minus_zero, 0, 0, 0);
  check_float ("trunc (0.1) == 0",  FUNC(trunc) (0.1), 0, 0, 0, 0);
  check_float ("trunc (0.25) == 0",  FUNC(trunc) (0.25), 0, 0, 0, 0);
  check_float ("trunc (0.625) == 0",  FUNC(trunc) (0.625), 0, 0, 0, 0);
  check_float ("trunc (-0.1) == -0",  FUNC(trunc) (-0.1), minus_zero, 0, 0, 0);
  check_float ("trunc (-0.25) == -0",  FUNC(trunc) (-0.25), minus_zero, 0, 0, 0);
  check_float ("trunc (-0.625) == -0",  FUNC(trunc) (-0.625), minus_zero, 0, 0, 0);
  check_float ("trunc (1) == 1",  FUNC(trunc) (1), 1, 0, 0, 0);
  check_float ("trunc (-1) == -1",  FUNC(trunc) (-1), -1, 0, 0, 0);
  check_float ("trunc (1.625) == 1",  FUNC(trunc) (1.625), 1, 0, 0, 0);
  check_float ("trunc (-1.625) == -1",  FUNC(trunc) (-1.625), -1, 0, 0, 0);

  check_float ("trunc (1048580.625) == 1048580",  FUNC(trunc) (1048580.625L), 1048580L, 0, 0, 0);
  check_float ("trunc (-1048580.625) == -1048580",  FUNC(trunc) (-1048580.625L), -1048580L, 0, 0, 0);

  check_float ("trunc (8388610.125) == 8388610.0",  FUNC(trunc) (8388610.125L), 8388610.0L, 0, 0, 0);
  check_float ("trunc (-8388610.125) == -8388610.0",  FUNC(trunc) (-8388610.125L), -8388610.0L, 0, 0, 0);

  check_float ("trunc (4294967296.625) == 4294967296.0",  FUNC(trunc) (4294967296.625L), 4294967296.0L, 0, 0, 0);
  check_float ("trunc (-4294967296.625) == -4294967296.0",  FUNC(trunc) (-4294967296.625L), -4294967296.0L, 0, 0, 0);

#ifdef TEST_LDOUBLE
  /* The result can only be represented in long double.  */
  check_float ("trunc (4503599627370495.5) == 4503599627370495.0",  FUNC(trunc) (4503599627370495.5L), 4503599627370495.0L, 0, 0, 0);
  check_float ("trunc (4503599627370496.25) == 4503599627370496.0",  FUNC(trunc) (4503599627370496.25L), 4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (4503599627370496.5) == 4503599627370496.0",  FUNC(trunc) (4503599627370496.5L), 4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (4503599627370496.75) == 4503599627370496.0",  FUNC(trunc) (4503599627370496.75L), 4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (4503599627370497.5) == 4503599627370497.0",  FUNC(trunc) (4503599627370497.5L), 4503599627370497.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("trunc (4503599627370494.5000000000001) == 4503599627370494.0",  FUNC(trunc) (4503599627370494.5000000000001L), 4503599627370494.0L, 0, 0, 0);
  check_float ("trunc (4503599627370495.5000000000001) == 4503599627370495.0",  FUNC(trunc) (4503599627370495.5000000000001L), 4503599627370495.0L, 0, 0, 0);
  check_float ("trunc (4503599627370496.5000000000001) == 4503599627370496.0",  FUNC(trunc) (4503599627370496.5000000000001L), 4503599627370496.0L, 0, 0, 0);
# endif

  check_float ("trunc (-4503599627370495.5) == -4503599627370495.0",  FUNC(trunc) (-4503599627370495.5L), -4503599627370495.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370496.25) == -4503599627370496.0",  FUNC(trunc) (-4503599627370496.25L), -4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370496.5) == -4503599627370496.0",  FUNC(trunc) (-4503599627370496.5L), -4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370496.75) == -4503599627370496.0",  FUNC(trunc) (-4503599627370496.75L), -4503599627370496.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370497.5) == -4503599627370497.0",  FUNC(trunc) (-4503599627370497.5L), -4503599627370497.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("trunc (-4503599627370494.5000000000001) == -4503599627370494.0",  FUNC(trunc) (-4503599627370494.5000000000001L), -4503599627370494.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370495.5000000000001) == -4503599627370495.0",  FUNC(trunc) (-4503599627370495.5000000000001L), -4503599627370495.0L, 0, 0, 0);
  check_float ("trunc (-4503599627370496.5000000000001) == -4503599627370496.0",  FUNC(trunc) (-4503599627370496.5000000000001L), -4503599627370496.0L, 0, 0, 0);
# endif

  check_float ("trunc (9007199254740991.5) == 9007199254740991.0",  FUNC(trunc) (9007199254740991.5L), 9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (9007199254740992.25) == 9007199254740992.0",  FUNC(trunc) (9007199254740992.25L), 9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (9007199254740992.5) == 9007199254740992.0",  FUNC(trunc) (9007199254740992.5L), 9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (9007199254740992.75) == 9007199254740992.0",  FUNC(trunc) (9007199254740992.75L), 9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (9007199254740993.5) == 9007199254740993.0",  FUNC(trunc) (9007199254740993.5L), 9007199254740993.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("trunc (9007199254740991.0000000000001) == 9007199254740991.0",  FUNC(trunc) (9007199254740991.0000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (9007199254740992.0000000000001) == 9007199254740992.0",  FUNC(trunc) (9007199254740992.0000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (9007199254740993.0000000000001) == 9007199254740993.0",  FUNC(trunc) (9007199254740993.0000000000001L), 9007199254740993.0L, 0, 0, 0);
  check_float ("trunc (9007199254740991.5000000000001) == 9007199254740991.0",  FUNC(trunc) (9007199254740991.5000000000001L), 9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (9007199254740992.5000000000001) == 9007199254740992.0",  FUNC(trunc) (9007199254740992.5000000000001L), 9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (9007199254740993.5000000000001) == 9007199254740993.0",  FUNC(trunc) (9007199254740993.5000000000001L), 9007199254740993.0L, 0, 0, 0);
# endif

  check_float ("trunc (-9007199254740991.5) == -9007199254740991.0",  FUNC(trunc) (-9007199254740991.5L), -9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740992.25) == -9007199254740992.0",  FUNC(trunc) (-9007199254740992.25L), -9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740992.5) == -9007199254740992.0",  FUNC(trunc) (-9007199254740992.5L), -9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740992.75) == -9007199254740992.0",  FUNC(trunc) (-9007199254740992.75L), -9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740993.5) == -9007199254740993.0",  FUNC(trunc) (-9007199254740993.5L), -9007199254740993.0L, 0, 0, 0);

# if LDBL_MANT_DIG > 100
  check_float ("trunc (-9007199254740991.0000000000001) == -9007199254740991.0",  FUNC(trunc) (-9007199254740991.0000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740992.0000000000001) == -9007199254740992.0",  FUNC(trunc) (-9007199254740992.0000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740993.0000000000001) == -9007199254740993.0",  FUNC(trunc) (-9007199254740993.0000000000001L), -9007199254740993.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740991.5000000000001) == -9007199254740991.0",  FUNC(trunc) (-9007199254740991.5000000000001L), -9007199254740991.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740992.5000000000001) == -9007199254740992.0",  FUNC(trunc) (-9007199254740992.5000000000001L), -9007199254740992.0L, 0, 0, 0);
  check_float ("trunc (-9007199254740993.5000000000001) == -9007199254740993.0",  FUNC(trunc) (-9007199254740993.5000000000001L), -9007199254740993.0L, 0, 0, 0);
# endif

  check_float ("trunc (72057594037927935.5) == 72057594037927935.0",  FUNC(trunc) (72057594037927935.5L), 72057594037927935.0L, 0, 0, 0);
  check_float ("trunc (72057594037927936.25) == 72057594037927936.0",  FUNC(trunc) (72057594037927936.25L), 72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (72057594037927936.5) == 72057594037927936.0",  FUNC(trunc) (72057594037927936.5L), 72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (72057594037927936.75) == 72057594037927936.0",  FUNC(trunc) (72057594037927936.75L), 72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (72057594037927937.5) == 72057594037927937.0",  FUNC(trunc) (72057594037927937.5L), 72057594037927937.0L, 0, 0, 0);

  check_float ("trunc (-72057594037927935.5) == -72057594037927935.0",  FUNC(trunc) (-72057594037927935.5L), -72057594037927935.0L, 0, 0, 0);
  check_float ("trunc (-72057594037927936.25) == -72057594037927936.0",  FUNC(trunc) (-72057594037927936.25L), -72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (-72057594037927936.5) == -72057594037927936.0",  FUNC(trunc) (-72057594037927936.5L), -72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (-72057594037927936.75) == -72057594037927936.0",  FUNC(trunc) (-72057594037927936.75L), -72057594037927936.0L, 0, 0, 0);
  check_float ("trunc (-72057594037927937.5) == -72057594037927937.0",  FUNC(trunc) (-72057594037927937.5L), -72057594037927937.0L, 0, 0, 0);

  check_float ("trunc (10141204801825835211973625643007.5) == 10141204801825835211973625643007.0",  FUNC(trunc) (10141204801825835211973625643007.5L), 10141204801825835211973625643007.0L, 0, 0, 0);
  check_float ("trunc (10141204801825835211973625643008.25) == 10141204801825835211973625643008.0",  FUNC(trunc) (10141204801825835211973625643008.25L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("trunc (10141204801825835211973625643008.5) == 10141204801825835211973625643008.0",  FUNC(trunc) (10141204801825835211973625643008.5L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("trunc (10141204801825835211973625643008.75) == 10141204801825835211973625643008.0",  FUNC(trunc) (10141204801825835211973625643008.75L), 10141204801825835211973625643008.0L, 0, 0, 0);
  check_float ("trunc (10141204801825835211973625643009.5) == 10141204801825835211973625643009.0",  FUNC(trunc) (10141204801825835211973625643009.5L), 10141204801825835211973625643009.0L, 0, 0, 0);
#endif

  print_max_error ("trunc", 0, 0);
}

static void
y0_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(y0) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  /* y0 is the Bessel function of the second kind of order 0 */
  init_max_error ();

  check_float ("y0 (-1.0) == -inf plus invalid exception",  FUNC(y0) (-1.0), minus_infty, 0, 0, INVALID_EXCEPTION);
  check_float ("y0 (0.0) == -inf",  FUNC(y0) (0.0), minus_infty, 0, 0, 0);
  check_float ("y0 (NaN) == NaN",  FUNC(y0) (nan_value), nan_value, 0, 0, 0);
  check_float ("y0 (inf) == 0",  FUNC(y0) (plus_infty), 0, 0, 0, 0);

  check_float ("y0 (0.125) == -1.38968062514384052915582277745018693",  FUNC(y0) (0.125L), -1.38968062514384052915582277745018693L, 0, 0, 0);
  check_float ("y0 (0.75) == -0.137172769385772397522814379396581855",  FUNC(y0) (0.75L), -0.137172769385772397522814379396581855L, 0, 0, 0);
  check_float ("y0 (1.0) == 0.0882569642156769579829267660235151628",  FUNC(y0) (1.0), 0.0882569642156769579829267660235151628L, 0, 0, 0);
  check_float ("y0 (1.5) == 0.382448923797758843955068554978089862",  FUNC(y0) (1.5), 0.382448923797758843955068554978089862L, 0, 0, 0);
  check_float ("y0 (2.0) == 0.510375672649745119596606592727157873",  FUNC(y0) (2.0), 0.510375672649745119596606592727157873L, 0, 0, 0);
  check_float ("y0 (8.0) == 0.223521489387566220527323400498620359",  FUNC(y0) (8.0), 0.223521489387566220527323400498620359L, 0, 0, 0);
  check_float ("y0 (10.0) == 0.0556711672835993914244598774101900481",  FUNC(y0) (10.0), 0.0556711672835993914244598774101900481L, 0, 0, 0);

  print_max_error ("y0", 0, 0);
}


static void
y1_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(y1) (1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  /* y1 is the Bessel function of the second kind of order 1 */
  init_max_error ();

  check_float ("y1 (-1.0) == -inf plus invalid exception",  FUNC(y1) (-1.0), minus_infty, 0, 0, INVALID_EXCEPTION);
  check_float ("y1 (0.0) == -inf",  FUNC(y1) (0.0), minus_infty, 0, 0, 0);
  check_float ("y1 (inf) == 0",  FUNC(y1) (plus_infty), 0, 0, 0, 0);
  check_float ("y1 (NaN) == NaN",  FUNC(y1) (nan_value), nan_value, 0, 0, 0);

  check_float ("y1 (0.125) == -5.19993611253477499595928744876579921",  FUNC(y1) (0.125L), -5.19993611253477499595928744876579921L, 0, 0, 0);
  check_float ("y1 (0.75) == -1.03759455076928541973767132140642198",  FUNC(y1) (0.75L), -1.03759455076928541973767132140642198L, 0, 0, 0);
  check_float ("y1 (1.0) == -0.781212821300288716547150000047964821",  FUNC(y1) (1.0), -0.781212821300288716547150000047964821L, 0, 0, 0);
  check_float ("y1 (1.5) == -0.412308626973911295952829820633445323",  FUNC(y1) (1.5), -0.412308626973911295952829820633445323L, 0, 0, 0);
  check_float ("y1 (2.0) == -0.107032431540937546888370772277476637",  FUNC(y1) (2.0), -0.107032431540937546888370772277476637L, 0, 0, 0);
  check_float ("y1 (8.0) == -0.158060461731247494255555266187483550",  FUNC(y1) (8.0), -0.158060461731247494255555266187483550L, 0, 0, 0);
  check_float ("y1 (10.0) == 0.249015424206953883923283474663222803",  FUNC(y1) (10.0), 0.249015424206953883923283474663222803L, 0, 0, 0);

  print_max_error ("y1", 0, 0);
}


static void
yn_test (void)
{
  FLOAT s, c;
  errno = 0;
  FUNC (sincos) (0, &s, &c);
  if (errno == ENOSYS)
    /* Required function not implemented.  */
    return;
  FUNC(yn) (1, 1);
  if (errno == ENOSYS)
    /* Function not implemented.  */
    return;

  /* yn is the Bessel function of the second kind of order n */
  init_max_error ();

  /* yn (0, x) == y0 (x)  */
  check_float ("yn (0, -1.0) == -inf plus invalid exception",  FUNC(yn) (0, -1.0), minus_infty, 0, 0, INVALID_EXCEPTION);
  check_float ("yn (0, 0.0) == -inf",  FUNC(yn) (0, 0.0), minus_infty, 0, 0, 0);
  check_float ("yn (0, NaN) == NaN",  FUNC(yn) (0, nan_value), nan_value, 0, 0, 0);
  check_float ("yn (0, inf) == 0",  FUNC(yn) (0, plus_infty), 0, 0, 0, 0);

  check_float ("yn (0, 0.125) == -1.38968062514384052915582277745018693",  FUNC(yn) (0, 0.125L), -1.38968062514384052915582277745018693L, 0, 0, 0);
  check_float ("yn (0, 0.75) == -0.137172769385772397522814379396581855",  FUNC(yn) (0, 0.75L), -0.137172769385772397522814379396581855L, 0, 0, 0);
  check_float ("yn (0, 1.0) == 0.0882569642156769579829267660235151628",  FUNC(yn) (0, 1.0), 0.0882569642156769579829267660235151628L, 0, 0, 0);
  check_float ("yn (0, 1.5) == 0.382448923797758843955068554978089862",  FUNC(yn) (0, 1.5), 0.382448923797758843955068554978089862L, 0, 0, 0);
  check_float ("yn (0, 2.0) == 0.510375672649745119596606592727157873",  FUNC(yn) (0, 2.0), 0.510375672649745119596606592727157873L, 0, 0, 0);
  check_float ("yn (0, 8.0) == 0.223521489387566220527323400498620359",  FUNC(yn) (0, 8.0), 0.223521489387566220527323400498620359L, 0, 0, 0);
  check_float ("yn (0, 10.0) == 0.0556711672835993914244598774101900481",  FUNC(yn) (0, 10.0), 0.0556711672835993914244598774101900481L, 0, 0, 0);

  /* yn (1, x) == y1 (x)  */
  check_float ("yn (1, -1.0) == -inf plus invalid exception",  FUNC(yn) (1, -1.0), minus_infty, 0, 0, INVALID_EXCEPTION);
  check_float ("yn (1, 0.0) == -inf",  FUNC(yn) (1, 0.0), minus_infty, 0, 0, 0);
  check_float ("yn (1, inf) == 0",  FUNC(yn) (1, plus_infty), 0, 0, 0, 0);
  check_float ("yn (1, NaN) == NaN",  FUNC(yn) (1, nan_value), nan_value, 0, 0, 0);

  check_float ("yn (1, 0.125) == -5.19993611253477499595928744876579921",  FUNC(yn) (1, 0.125L), -5.19993611253477499595928744876579921L, 0, 0, 0);
  check_float ("yn (1, 0.75) == -1.03759455076928541973767132140642198",  FUNC(yn) (1, 0.75L), -1.03759455076928541973767132140642198L, 0, 0, 0);
  check_float ("yn (1, 1.0) == -0.781212821300288716547150000047964821",  FUNC(yn) (1, 1.0), -0.781212821300288716547150000047964821L, 0, 0, 0);
  check_float ("yn (1, 1.5) == -0.412308626973911295952829820633445323",  FUNC(yn) (1, 1.5), -0.412308626973911295952829820633445323L, 0, 0, 0);
  check_float ("yn (1, 2.0) == -0.107032431540937546888370772277476637",  FUNC(yn) (1, 2.0), -0.107032431540937546888370772277476637L, 0, 0, 0);
  check_float ("yn (1, 8.0) == -0.158060461731247494255555266187483550",  FUNC(yn) (1, 8.0), -0.158060461731247494255555266187483550L, 0, 0, 0);
  check_float ("yn (1, 10.0) == 0.249015424206953883923283474663222803",  FUNC(yn) (1, 10.0), 0.249015424206953883923283474663222803L, 0, 0, 0);

  /* yn (3, x)  */
  check_float ("yn (3, inf) == 0",  FUNC(yn) (3, plus_infty), 0, 0, 0, 0);
  check_float ("yn (3, NaN) == NaN",  FUNC(yn) (3, nan_value), nan_value, 0, 0, 0);

  check_float ("yn (3, 0.125) == -2612.69757350066712600220955744091741",  FUNC(yn) (3, 0.125L), -2612.69757350066712600220955744091741L, 0, 0, 0);
  check_float ("yn (3, 0.75) == -12.9877176234475433186319774484809207",  FUNC(yn) (3, 0.75L), -12.9877176234475433186319774484809207L, 0, 0, 0);
  check_float ("yn (3, 1.0) == -5.82151760596472884776175706442981440",  FUNC(yn) (3, 1.0), -5.82151760596472884776175706442981440L, 0, 0, 0);
  check_float ("yn (3, 2.0) == -1.12778377684042778608158395773179238",  FUNC(yn) (3, 2.0), -1.12778377684042778608158395773179238L, 0, 0, 0);
  check_float ("yn (3, 10.0) == -0.251362657183837329779204747654240998",  FUNC(yn) (3, 10.0), -0.251362657183837329779204747654240998L, 0, 0, 0);

  /* yn (10, x)  */
  check_float ("yn (10, inf) == 0",  FUNC(yn) (10, plus_infty), 0, 0, 0, 0);
  check_float ("yn (10, NaN) == NaN",  FUNC(yn) (10, nan_value), nan_value, 0, 0, 0);

  check_float ("yn (10, 0.125) == -127057845771019398.252538486899753195",  FUNC(yn) (10, 0.125L), -127057845771019398.252538486899753195L, 0, 0, 0);
  check_float ("yn (10, 0.75) == -2133501638.90573424452445412893839236",  FUNC(yn) (10, 0.75L), -2133501638.90573424452445412893839236L, 0, 0, 0);
  check_float ("yn (10, 1.0) == -121618014.278689189288130426667971145",  FUNC(yn) (10, 1.0), -121618014.278689189288130426667971145L, 0, 0, 0);
  check_float ("yn (10, 2.0) == -129184.542208039282635913145923304214",  FUNC(yn) (10, 2.0), -129184.542208039282635913145923304214L, 0, 0, 0);
  check_float ("yn (10, 10.0) == -0.359814152183402722051986577343560609",  FUNC(yn) (10, 10.0), -0.359814152183402722051986577343560609L, 0, 0, 0);

  print_max_error ("yn", 0, 0);

}


static void
significand_test (void)
{
  /* significand returns the mantissa of the exponential representation.  */
  init_max_error ();

  check_float ("significand (4.0) == 1.0",  FUNC(significand) (4.0), 1.0, 0, 0, 0);
  check_float ("significand (6.0) == 1.5",  FUNC(significand) (6.0), 1.5, 0, 0, 0);
  check_float ("significand (8.0) == 1.0",  FUNC(significand) (8.0), 1.0, 0, 0, 0);

  print_max_error ("significand", 0, 0);
}


static void
initialize (void)
{
  fpstack_test ("start *init*");
  plus_zero = 0.0;
  nan_value = plus_zero / plus_zero;	/* Suppress GCC warning */

  minus_zero = FUNC(copysign) (0.0, -1.0);
  plus_infty = CHOOSE (HUGE_VALL, HUGE_VAL, HUGE_VALF,
		       HUGE_VALL, HUGE_VAL, HUGE_VALF);
  minus_infty = CHOOSE (-HUGE_VALL, -HUGE_VAL, -HUGE_VALF,
			-HUGE_VALL, -HUGE_VAL, -HUGE_VALF);
  max_value = CHOOSE (LDBL_MAX, DBL_MAX, FLT_MAX,
		      LDBL_MAX, DBL_MAX, FLT_MAX);
  min_value = CHOOSE (LDBL_MIN, DBL_MIN, FLT_MIN,
		      LDBL_MIN, DBL_MIN, FLT_MIN);

  (void) &plus_zero;
  (void) &nan_value;
  (void) &minus_zero;
  (void) &plus_infty;
  (void) &minus_infty;
  (void) &max_value;
  (void) &min_value;

  /* Clear all exceptions.  From now on we must not get random exceptions.  */
  feclearexcept (FE_ALL_EXCEPT);

  /* Test to make sure we start correctly.  */
  fpstack_test ("end *init*");
}

#if 0
/* Definitions of arguments for argp functions.  */
static const struct argp_option options[] =
{
  { "verbose", 'v', "NUMBER", 0, "Level of verbosity (0..3)"},
  { "ulps-file", 'u', NULL, 0, "Output ulps to file ULPs"},
  { "no-max-error", 'f', NULL, 0,
    "Don't output maximal errors of functions"},
  { "no-points", 'p', NULL, 0,
    "Don't output results of functions invocations"},
  { "ignore-max-ulp", 'i', "yes/no", 0,
    "Ignore given maximal errors"},
  { NULL, 0, NULL, 0, NULL }
};

/* Short description of program.  */
static const char doc[] = "Math test suite: " TEST_MSG ;

/* Prototype for option handler.  */
static error_t parse_opt (int key, char *arg, struct argp_state *state);

/* Data structure to communicate with argp functions.  */
static struct argp argp =
{
  options, parse_opt, NULL, doc,
};


/* Handle program arguments.  */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case 'f':
      output_max_error = 0;
      break;
    case 'i':
      if (strcmp (arg, "yes") == 0)
	ignore_max_ulp = 1;
      else if (strcmp (arg, "no") == 0)
	ignore_max_ulp = 0;
      break;
    case 'p':
      output_points = 0;
      break;
    case 'u':
      output_ulps = 1;
      break;
    case 'v':
      if (optarg)
	verbose = (unsigned int) strtoul (optarg, NULL, 0);
      else
	verbose = 3;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
#endif

#if 0
/* function to check our ulp calculation.  */
void
check_ulp (void)
{
  int i;

  FLOAT u, diff, ulp;
  /* This gives one ulp.  */
  u = FUNC(nextafter) (10, 20);
  check_equal (10.0, u, 1, &diff, &ulp);
  printf ("One ulp: % .4" PRINTF_NEXPR "\n", ulp);

  /* This gives one more ulp.  */
  u = FUNC(nextafter) (u, 20);
  check_equal (10.0, u, 2, &diff, &ulp);
  printf ("two ulp: % .4" PRINTF_NEXPR "\n", ulp);

  /* And now calculate 100 ulp.  */
  for (i = 2; i < 100; i++)
    u = FUNC(nextafter) (u, 20);
  check_equal (10.0, u, 100, &diff, &ulp);
  printf ("100 ulp: % .4" PRINTF_NEXPR "\n", ulp);
}
#endif

int
main (int argc, char **argv)
{

  int remaining;

  verbose = 1;
  output_ulps = 0;
  output_max_error = 1;
  output_points = 1;
  /* XXX set to 0 for releases.  */
  ignore_max_ulp = 0;

#if 0
  /* Parse and process arguments.  */
  argp_parse (&argp, argc, argv, 0, &remaining, NULL);

  if (remaining != argc)
    {
      fprintf (stderr, "wrong number of arguments");
      argp_help (&argp, stdout, ARGP_HELP_SEE, program_invocation_short_name);
      exit (EXIT_FAILURE);
    }

  if (output_ulps)
    {
      ulps_file = fopen ("ULPs", "a");
      if (ulps_file == NULL)
	{
	  perror ("can't open file `ULPs' for writing: ");
	  exit (1);
	}
    }
#endif

  initialize ();
  printf (TEST_MSG);

#if 0
  check_ulp ();
#endif

  /* Keep the tests a wee bit ordered (according to ISO C99).  */
  /* Classification macros:  */
  fpclassify_test ();
  isfinite_test ();
  isnormal_test ();
  signbit_test ();

  /* Trigonometric functions:  */
  acos_test ();
  asin_test ();
  atan_test ();
  atan2_test ();
  cos_test ();
  sin_test ();
  sincos_test ();
  tan_test ();

  /* Hyperbolic functions:  */
  acosh_test ();
  asinh_test ();
  atanh_test ();
  cosh_test ();
  sinh_test ();
  tanh_test ();

  /* Exponential and logarithmic functions:  */
  exp_test ();
  exp10_test ();
  exp2_test ();
  expm1_test ();
  frexp_test ();
  ldexp_test ();
  log_test ();
  log10_test ();
  log1p_test ();
  log2_test ();
  logb_test ();
  modf_test ();
  ilogb_test ();
  scalb_test ();
  scalbn_test ();
  scalbln_test ();
  significand_test ();

  /* Power and absolute value functions:  */
  cbrt_test ();
  fabs_test ();
  hypot_test ();
  pow_test ();
  sqrt_test ();

  /* Error and gamma functions:  */
  erf_test ();
  erfc_test ();
  gamma_test ();
  lgamma_test ();
  tgamma_test ();

  /* Nearest integer functions:  */
  ceil_test ();
  floor_test ();
  nearbyint_test ();
  rint_test ();
	#if 0
  rint_test_tonearest ();
  rint_test_towardzero ();
  rint_test_downward ();
  rint_test_upward ();
	#endif
  lrint_test ();
	#if 0
  lrint_test_tonearest ();
  lrint_test_towardzero ();
  lrint_test_downward ();
  lrint_test_upward ();
	#endif
  llrint_test ();
	#if 0
  llrint_test_tonearest ();
  llrint_test_towardzero ();
  llrint_test_downward ();
  llrint_test_upward ();
	#endif
  round_test ();
  lround_test ();
  llround_test ();
  trunc_test ();

  /* Remainder functions:  */
  fmod_test ();
  remainder_test ();
  remquo_test ();

  /* Manipulation functions:  */
  copysign_test ();
  nextafter_test ();
  nexttoward_test ();

  /* maximum, minimum and positive difference functions */
  fdim_test ();
  fmax_test ();
  fmin_test ();

  /* Multiply and add:  */
  fma_test ();
#if 0
  /* Complex functions:  */
  cabs_test ();
  cacos_test ();
  cacosh_test ();
  carg_test ();
  casin_test ();
  casinh_test ();
  catan_test ();
  catanh_test ();
  ccos_test ();
  ccosh_test ();
  cexp_test ();
  cimag_test ();
  clog10_test ();
  clog_test ();
  conj_test ();
  cpow_test ();
  cproj_test ();
  creal_test ();
  csin_test ();
  csinh_test ();
  csqrt_test ();
  ctan_test ();
  ctanh_test ();

  /* Bessel functions:  */
  j0_test ();
  j1_test ();
  jn_test ();
  y0_test ();
  y1_test ();
  yn_test ();
#endif
  if (output_ulps)
    fclose (ulps_file);

  printf ("\nTest suite completed:\n");
  printf ("  %d test cases plus %d tests for exception flags executed.\n",
	  noTests, noExcTests);
  if (noXFails)
    printf ("  %d expected failures occurred.\n", noXFails);
  if (noXPasses)
    printf ("  %d unexpected passes occurred.\n", noXPasses);
  if (noErrors)
    {
      printf ("  %d errors occurred.\n", noErrors);
      return 1;
    }
  printf ("  All tests passed successfully.\n");

  return 0;
}

/*
 * Local Variables:
 * mode:c
 * End:
 */
