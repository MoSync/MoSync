/* Copyright (C) 2009 Mobile Sorcery AB

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

#include "wce_helpers.h"
#include <string>
using namespace std;

void convertUnicodeToAscii(char *str, int size, const LPWSTR unicode) {
	int nLenUnicode = lstrlenW( unicode );
	WideCharToMultiByte( CP_ACP, 
	0,
	unicode, 
	nLenUnicode,
	str,
	size,
	NULL, NULL );
	str[nLenUnicode] = 0;
}

void convertAsciiToUnicode(LPWSTR unicode, int size, const char *str) {
	int nLenChar = strlen(str);
	MultiByteToWideChar(CP_ACP, 0, str, nLenChar, unicode, size);
	unicode[nLenChar] = 0;
}

void getWorkingDirectory(char *dst, int maxPath) {
	TCHAR appPath[MAX_PATH+1]; appPath[0] = 0;

	if( GetModuleFileName(NULL, appPath, maxPath) == 0 )
	{
		//failure
	}
	else
	{
		wstring tmpStr = wstring(appPath);
		size_t pos = tmpStr.find_last_of((TCHAR)'\\');
		if( pos == wstring::npos )
		{
			// your EXE is in the root directory
		}
		else
		{
			appPath[pos] = (TCHAR)'\0';  // that's it!
		}
	}

	convertUnicodeToAscii(dst, maxPath, appPath);
}