#include <mawvsprintf.h>

int MAMain() {
	const wchar* str = L"Hello World!";
	wchar buf[256];
	wsprintf(buf, L"Unicode %s", str);
	wlprintfln(L"%s", buf);
	return 0;
}
