#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT(dgles, 0.5, hipo@users.sourceforge.net)
AC_CONFIG_SRCDIR([src/glesapi.c])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

AC_PATH_PROG(SDL_CONFIG, sdl-config)
if test "$SDL_CONFIG" != ""
then
  SDL_CXXFLAGS=`$SDL_CONFIG --cflags`
  SDL_LIBS=`$SDL_CONFIG --libs | sed 's/-framework OpenGL//'`
  AC_SUBST(SDL_CXXFLAGS)
  AC_SUBST(SDL_LIBS)

  dnl AC_CHECK_HEADERS(GL/gl.h, [],[AC_MSG_ERROR([[No gl.h found.]])])
  AC_CHECK_HEADERS(GL/gl.h, [],echo "fdsaFADSFADSFADFADS")
  echo "Niuh, niuh"

  want_tests=yes
fi

AM_CONDITIONAL(BUILD_TESTS, test "$want_tests" == "yes")

dnl want_egl=no
dnl AC_ARG_ENABLE(egl,
dnl [  --enable-egl            enable if you want to compile EGL [default=no]],
dnl   [want_egl="$enableval"])

dnl AM_CONDITIONAL(BUILD_EGL, test "$want_egl" == "yes")

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

if test "$GCC" == "yes"
then
  CFLAGS="$CFLAGS -Wall -W -ansi -pedantic -std=c89 -O"
fi

# Checks for library functions.
AC_OUTPUT([Makefile src/Makefile GLES/Makefile tests/Makefile])
