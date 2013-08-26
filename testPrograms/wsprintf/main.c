/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
