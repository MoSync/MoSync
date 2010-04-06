/* Copyright (C) 2010 Mobile Sorcery AB

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

#ifndef _BACKEND_H_
#define _BACKEND_H_

#include <fstream>
#include <map>
#include <string>
#include "Base.h"
#include "Internal.h"

class Base;

class Backend {
public:
	typedef std::multimap<std::string, const Base*> BasesMap;
	typedef BasesMap::const_iterator BasesIterator;

	void generate(const BasesMap& bases, const std::string& outputFile) {
		std::fstream stream;
		stream.open(outputFile.c_str(), std::ios::out);
		emit(bases, stream);
		stream.close();
	}
protected:
	virtual void emit(const BasesMap& bases, std::fstream& stream) = 0;


};

#endif // _BACKEND_H_