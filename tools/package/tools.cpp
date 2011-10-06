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

#include "util.h"
#include "tools.h"
#include "package.h"
#include <string>
#include <sstream>
#include <ostream>

using namespace std;

void injectIcon(const char* platform, const char* size, const char* srcIcon, const char* dst, bool silent) {
	ostringstream iconInjectCmd;

	// TODO: That "-lenient yes" switch must be removed...
	iconInjectCmd << getBinary("icon-injector") << " -lenient yes -platform " << platform << " -src " <<
		file(srcIcon) << " -size " << size << " -dst " << file(dst);

	sh(iconInjectCmd.str().c_str(), silent);
}

string getBinary(const char* binaryName) {
	return file(mosyncdir() + string("/bin/") + binaryName);
}
