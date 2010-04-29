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

#ifndef _IDL_BACKEND_H_
#define _IDL_BACKEND_H_

#include "Backend.h"
#include <string>

class IDLBackend : public Backend {
protected:

	// a bit messy this function...
	std::string getIDLType(const Base* base, const Argument* argument=NULL, bool usePointer=false);
	void emit(const BasesMap& bases, std::fstream& stream);
};

#endif // _IDL_BACKEND_H_