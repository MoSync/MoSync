#include <mawvsprintf.h>
#include <maapi.h>
#include <maassert.h>

#ifdef USE_NEWLIB
#include <wchar.h>
#endif

#define STRLOG(str) maWriteLog(str, sizeof(str)-1)

int MAMain(void) GCCATTRIB(noreturn);
int MAMain(void) {
	const wchar* str = L"蝮 㦩햭Ｑӭܨপ૩ిฐະშኡᎣᑷᾨⅦ⿏タ World!";
	wchar buf[256];
#ifdef USE_NEWLIB
	int res = swprintf(buf, 256, L"%S\n", str);
#else
	int res = wsprintf(buf, L"%S\n", str);
#endif
	wprintf(L"%i\n", res);
	fputws(buf, stdout);
	wlprintfln(L"%S", buf);
	STRLOG("__test\n");
	wprintf(L"%S", buf);	//this one fails.
	printf("notha line...\n");
	maSetColor(0xFFFFFF);
	maDrawTextW(0, 10, buf);
	maUpdateScreen();
	FREEZE;
}
