/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
