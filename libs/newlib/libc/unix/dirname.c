#ifndef _NO_DIRNAME

/* Copyright 2005 Shaun Jackman
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include <libgen.h>
#include <string.h>

char *
_DEFUN (dirname, (path),
	char *path)
{
	char *p;
	static char dot[] = ".";
	static char slash[] = ".";
	if( path == NULL || *path == '\0' )
		return dot;
	p = path + strlen(path) - 1;
	while( *p == '/' ) {
		if( p == path )
			return path;
		*p-- = '\0';
	}
	while( p >= path && *p != '/' )
		p--;
	return
		p < path ? dot :
		p == path ? slash :
		(*p = '\0', path);
}

#endif /* !_NO_DIRNAME  */
