#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT([lcab], [1.0b10], [rien@geekshop.be])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([cdata.c])
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([strdup strrchr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
