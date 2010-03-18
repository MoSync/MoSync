#include <mawvsprintf.h>
#include <maapi.h>
#include <maassert.h>

int MAMain() {
	const wchar* str = L"H 蝮 㦩햭Ｑӭܨপ૩ిฐະშኡᎣᑷᾨⅦ⿏タ World!";
	wchar buf[256];
	wsprintf(buf, L"U%s", str);
	wlprintfln(L"%s", buf);
	maSetColor(0xFFFFFF);
	maDrawTextW(10, 10, buf);
	maUpdateScreen();
	FREEZE;
	return 0;
}
