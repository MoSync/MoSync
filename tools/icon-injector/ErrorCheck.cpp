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

#include "ErrorCheck.h"

//Make sure the result != 0, if not, throw an exception so there's no messy ifs

void checkResult(BOOL result, const char *errorMessage)
{
	if (!result)
	{
		LPVOID msgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &msgBuf,
			0,
			NULL 
		);

		if (errorMessage == NULL)
			errorMessage = "";

		string userErrorMessage(errorMessage);
		string sysErrorMessage((LPTSTR) msgBuf);

		LocalFree(msgBuf);

		string message(userErrorMessage + ": " + sysErrorMessage);
		
		throw message;
	}
}
