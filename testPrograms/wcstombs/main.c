/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
