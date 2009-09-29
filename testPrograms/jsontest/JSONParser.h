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

#ifndef _SE_MSAB_MAUTIL_JSONPARSER_H_
#define _SE_MSAB_MAUTIL_JSONPARSER_H_

#include <MAUtil/Connection.h>
#include <MAUtil\String.h>
#include <MAUtil/util.h>

namespace MAUtil {
	
	class JSONListener {
	public:
		virtual int null();
		virtual int boolean(bool value);
		virtual int number(const String& number);
		virtual int string(const String& string);
		virtual int startMap();
		virtual int mapKey(const String& key);
		virtual int endMap();
		virtual int startArray();
		virtual int endArray();
	};

	class JSONParser {
	public:
		JSONParser();
	
		void parseFromMemory(const char* jsonText);
		/* Asynchronous - function will return, and callbacks will be invoked later using Environment event handling. */
		void parseFromUrl(const char* url);
	private:
		yajl_handle parser;
		JSONListener* listener;
	};
}

#endif