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

#include "ErrorCheck.h"

//Make sure the result != 0, if not, throw an exception so there's no messy ifs

void checkResult(BOOL result, char *errorMessage)
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
