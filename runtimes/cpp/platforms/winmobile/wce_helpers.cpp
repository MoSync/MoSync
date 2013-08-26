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