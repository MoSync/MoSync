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

#include "MoreInjector.h"
#include "Icon.h"
#include "Shared.h"
//#include <fcntl.h>
//#include <io.h>
//#include <stdio.h>
//#include <stdlib.h>

using namespace std;

namespace MoSync {
	void MoreInjector::inject(const Icon* icon, const std::map<std::string, std::string>& params) {
		string size = verifyParameter(params, "size");
		string dst = verifyParameter(params, "dst");
		const IconInstance* iconInst = icon->findBestInstance(size);
		if(!iconInst) errorExit("Couldn't find any icon instance.");
		if(!convertInstanceToImageFormat(iconInst, dst.c_str(), size, getExtension(dst))) errorExit("MoRE icon conversion failed.");
	}
}
