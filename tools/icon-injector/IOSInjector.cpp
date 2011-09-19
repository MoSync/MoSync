/* Copyright (C) 2011 Mobile Sorcery AB

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

#include "IOSInjector.h"
#include "Icon.h"
#include <fstream>
#include <sstream>
#include "Shared.h"

using namespace std;

namespace MoSync {

void IOSInjector::inject(const Icon* icon, const std::map<std::string, std::string>& params) {
	string size = verifyParameter(params, "size");
	if (size != "default" && size != "72x72" && size != "57x57" && size != "114x114") {
		errorExit("iOS only supports these icon sizes: 57x57, 114x114 and 72x72");
	}
	string dst = verifyParameter(params, "dst");
	const IconInstance* iconInst = icon->findBestInstance(size);
	if(!iconInst)
	{
		map<string, string>::const_iterator i = params.find("lenient");
		if (i == params.end())
		{
			errorExit("Couldn't find any icon instance.");
		} else
		{
			printf("Warning: found no icon with size %s.\n", size.c_str());
		}
	} else {
		if(!convertInstanceToImageFormat(iconInst, dst.c_str(), size, "png"))
		{
			errorExit("IOS icon conversion failed.");
		}
	}
}

}
