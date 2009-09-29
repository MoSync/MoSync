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

#ifndef OBEX_H
#define OBEX_H

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/array.h>
#include <bluetooth/connection.h>

int sendObject(const MABtAddr& address, const Array<char>& file, const Array<u16>& name,
							 int port, int maxPacketSize);

#define CONNERR_OBEX -1001


#define LOG_CODE LOG("code %i\n", __LINE__)

#undef FAIL
//#define FAIL(verb) { LOG("Failure %s, ", verb); LOG_CODE; return false; }

#undef TEST_LEZ
#define TEST_LEZ(a) { int res = (a); if(res <= 0) {\
	LOG("Failure sending, "); LOG_CODE; return res; } }


#endif	//OBEX_H
