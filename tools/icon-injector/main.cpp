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
#endif

#include "WP7Injector.h"
#include "MoreInjector.h"
#include "Symbian9Injector.h"
#include "AndroidInjector.h"
#include "IOSInjector.h"
#include "Shared.h"

using namespace std;
using namespace MoSync;

map<string, Injector*> gInjectors;
map<string, string> oldInjectors;

static void initInjectors(bool force) {
	gInjectors["JavaME"] = new JavaInjector();
#ifdef WIN32 // for now..
	gInjectors["Windows Mobile"] = new WinmobileInjector();
#endif
	gInjectors["Windows Phone"] = new WP7Injector();
	gInjectors["more"] = new MoreInjector();
	gInjectors["Symbian"] = new Symbian9Injector();
	gInjectors["Android"] = new AndroidInjector();
	gInjectors["iOS"] = new IOSInjector();

	for (std::map<string, Injector*>::iterator injectors = gInjectors.begin(); injectors != gInjectors.end(); ++injectors) {
		injectors->second->setForce(force);
	}

	oldInjectors["j2me"] = "JavaME";
	oldInjectors["winmobile"] = "Windows Mobile";
	oldInjectors["WP7"] = "Windows Phone";
	oldInjectors["symbian9"] = "Symbian";
	oldInjectors["android"] = "Android";
	// iOS = iOS
}

static bool parseCmdLine(map<string, string>& params, int argc, char **argv) {
	for(int i = 1; i < argc; i++) {
		const char *name = argv[i];
		if(!strcmp(name, "-force")) {
			// Special op.
			params["force"] = "true";
		} else if(strncmp(name, "-", 1)==0) {
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
	initInjectors(!params["force"].empty());
	// We support old, inconsistent platforms as well as the new platform based format
	string newPlatform = oldInjectors[platform];
	if (!newPlatform.empty()) {
		//printf("(Using platform %s instead of %s)\n", newPlatform.c_str(), platform.c_str());
		platform = newPlatform;
	}
	map<string, Injector*>::iterator i = gInjectors.find(platform);
	if(i == gInjectors.end()) errorExit("Not an available platform.\n");
	string dst = Injector::verifyParameter(params, "dst");

	Icon* icon = Icon::parse(src, platform);

	Injector* injector = (*i).second;
	injector->inject(icon, params);

	return 0;
}
