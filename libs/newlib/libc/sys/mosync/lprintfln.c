#include "mavsprintf.h"

int lprintfln(const char* fmt, ...)
{
	va_list args;
	char buf[2048];
	int len;
	static int lastWLres = 0;
	
	if(lastWLres < 0)
		return lastWLres;

	buf[0] = 0;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	if(len > (int)sizeof(buf) - 2) {
		maPanic(0, "lprintfln buffer overrun!");
	}
	va_end(args);

	if(buf[len-1] != '\n') {
		buf[len++] = '\n';
		buf[len] = 0;
	}
	lastWLres = maWriteLog(buf, len);
	return len;
}
