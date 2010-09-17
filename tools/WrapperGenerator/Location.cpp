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

#include "Location.h"
#include "File.h"

using namespace std;

Location::Location(int line, const File* file) : mLine(line), mFile(file) {
}

int Location::getLine() const {
	return mLine;
}

const File* Location::getFile() const {
	return mFile;
}

string Location::toString() const {
	return System::genstr("%d:%s", mLine, mFile->toString().c_str());
}
