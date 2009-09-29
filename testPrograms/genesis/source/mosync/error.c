/*
    error.c --
    Error logging 
*/

#include "osd.h"

#include <mavsprintf.h>

void error_init(void) {
}

void error_shutdown(void)
{
}

void error(char *format, ...)
{
	if (!log_error) return;
	char temp[1024];
	va_list ap;
  	va_start(ap, format);
  	vsprintf(temp, format, ap);
  	va_end(ap);
	maPanic(0, temp);
}

