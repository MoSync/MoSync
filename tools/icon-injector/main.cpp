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

#include <map>
#include <string>
#include <cstring>

#include "Icon.h"
#include "JavaInjector.h"

#ifdef _WIN32 // for now..
#include "WinmobileInjector.h"
#include "WP7Injector.h"
#endif

#include "MoreInjector.h"
#include "Symbian9Injector.h"
#include "AndroidInjector.h"
#include "IOSInjector.h"
#include "Shared.h"

using namespace std;
using namespace MoSync;

map<string, Injector*> gInjectors;

static void initInjectors() {
	gInjectors["j2me"] = new JavaInjector();
#ifdef WIN32 // for now..
	gInjectors["winmobile"] = new WinmobileInjector();
	gInjectors["WP7"] = new WP7Injector();
#endif
	gInjectors["more"] = new MoreInjector();
	gInjectors["symbian9"] = new Symbian9Injector();
	gInjectors["android"] = new AndroidInjector();
	gInjectors["iOS"] = new IOSInjector();
}

static bool parseCmdLine(map<string, string>& params, int argc, char **argv) {
	for(int i = 1; i < argc; i++) {
		const char *name = argv[i];
		if(strncmp(name, "-", 1)==0) {
			name++;
			i++;
			if(i>=argc) return false;
			params[name] = argv[i];
		} else {
			return false;
		}
	}
	return true;
}

int main(int argc, char **argv) {
	map<string, string> params;
	if(!parseCmdLine(params, argc, argv)) errorExit("Couldn't parse the command-line\n");
	string src = Injector::verifyParameter(params, "src");
	string platform = Injector::verifyParameter(params, "platform");
	Icon* icon = Icon::parse(src, platform);
	initInjectors();
	map<string, Injector*>::iterator i = gInjectors.find(platform);
	if(i == gInjectors.end()) errorExit("Not an available platform.\n");
	string dst = Injector::verifyParameter(params, "dst");
	Injector* injector = (*i).second;
	injector->inject(icon, params);

	return 0;
}
