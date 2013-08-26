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

#include <wchar.h>
#include <ma.h>
#include <mavsprintf.h>

#ifndef MAPIP
#include <stdlib.h>
#endif

int MAMain(void) {
	char buf[1024];
	int res;
	const wchar_t* wstr = L"Hello 蝮 UniWorld!\n";
	const char* ustr = "Hello 蝮 UTF-8!\n";
	wchar wbuf[1024];

	res = wcstombs(NULL, wstr, 0);
	lprintfln("len: %i\n", res);
	res = wcstombs(buf, wstr, res);
	lprintfln("res(minimal): %i\n", res);
	res = wcstombs(buf, wstr, sizeof(buf));
	lprintfln("res(big buffer): %i\n", res);
	maWriteLog(buf, res);

	res = mbstowcs(wbuf, ustr, sizeof(wbuf));
	lprintfln("res: %i\n", res);
	maWriteLog(wbuf, res*2);

	return 0;
}
