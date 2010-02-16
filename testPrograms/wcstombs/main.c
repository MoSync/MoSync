#include <wchar.h>
#include <ma.h>
#include <mavsprintf.h>

#ifndef MAPIP
#include <stdlib.h>
#endif

int MAMain() {
	const wchar_t* wstr = L"Hello 蝮 UniWorld!\n";
	char buf[1024];
	int res = wcstombs(buf, wstr, sizeof(buf));
	lprintfln("res: %i\n", res);
	maWriteLog(buf, res);

	const char* ustr = "Hello 蝮 UTF-8!\n";
	wchar wbuf[1024];
	res = mbstowcs(wbuf, ustr, sizeof(wbuf));
	lprintfln("res: %i\n", res);
	maWriteLog(wbuf, res*2);

	return 0;
}
