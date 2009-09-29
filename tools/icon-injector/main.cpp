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

#include "Icon.h"
#include "JavaInjector.h"
#include "WinmobileInjector.h"
#include "MoreInjector.h"
#include "Shared.h"

#include <conio.h>

using namespace std;
using namespace MoSync;

map<string, Injector*> gInjectors;

void initInjectors() {
	gInjectors["j2me"] = new JavaInjector();
	gInjectors["winmobile"] = new WinmobileInjector();
	gInjectors["more"] = new MoreInjector();
}

bool parseCmdLine(map<string, string>& params, int argc, char **argv) {
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
	Icon* icon = Icon::parse(src);
	string platform = Injector::verifyParameter(params, "platform");
	initInjectors();
	map<string, Injector*>::iterator i = gInjectors.find(platform);
	if(i == gInjectors.end()) errorExit("Not an available platform.");
	string dst = Injector::verifyParameter(params, "dst");
	Injector* injector = (*i).second;
	injector->inject(icon, params);

	//getch();
	return 0;
}
