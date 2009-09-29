#include "warn.h"

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void warn(const char *fmt, ...)
{
    fputs("lcab: ", stderr);
    if (fmt != 0)
    {
        va_list ap;	
        va_start(ap, fmt);	
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fputs(": ", stderr);
    }
    fprintf(stderr, "%s\n", strerror(errno));
}
