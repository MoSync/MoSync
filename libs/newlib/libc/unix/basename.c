#ifndef _NO_BASENAME
/* Copyright 2005 Shaun Jackman
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include <libgen.h>
#include <string.h>

char* 
_DEFUN (basename, (path),
	char *path)
{
	char *p;
	static char dot[] = ".";
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
	return p + 1;
}

#endif /* !_NO_BASENAME  */
