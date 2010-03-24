#include "mawvsprintf.h"
#include <limits.h>
#include <wchar.h>
#include <stdlib.h>

#define BUFLEN 2048

int wlprintfln(const wchar_t* fmt, ...)
{
	va_list args;
	wchar_t buf[BUFLEN];
	char buf8[BUFLEN * MB_LEN_MAX];
	int len, len8;
	static int lastWLres = 0;
	
	if(lastWLres < 0)
		return lastWLres;

	buf[0] = 0;

	va_start(args, fmt);
	len = vswprintf(buf, sizeof(buf), fmt, args);
	if(len > (int)sizeof(buf) - 2) {
		maPanic(0, "lprintfln buffer overrun!");
	}
	va_end(args);

	if(buf[len-1] != '\n') {
		buf[len++] = '\n';
		buf[len] = 0;
	}
	// convert to utf-8
	len8 = wcstombs(buf8, buf, sizeof(buf8));
	lastWLres = maWriteLog(buf8, len8);
	return len;
}
