/* Copyright (C) 2011 MoSync AB

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

#include <string>
#include <sstream>
#include <stdio.h>
#include "packagers.h"
#include "util.h"
#include "helpers/types.h"

using namespace std;

#ifdef WIN32
#define CABWIZ "cabwiz"
#else
#define CABWIZ "pcab.pl"
#endif

void packageWM(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);

	TemplateMap tm;
	tm["project-name"] = s.name;
	tm["CE1"] = "%CE1%";
	tm["CE11"] = "%CE11%";

#ifdef WIN32
	tm["app-vendor"] = s.vendor;
	tm["app-name"] = s.name;
	tm["D"] = s.debug ? "D" : "";
	tm["AppName"] = "%AppName%";
	tm["InstallDir"] = "%InstallDir%";
	tm["cab-runtime-dir"] = ri.path;
	tm["cab-compile-output-dir"] = getDir(s.program) + "/";

	tm["HAS_RESOURCES"] = s.resource != NULL;
	if(s.resource) {
		tm["res-sdn"] = "3=,\"resources\",,\""+getDir(s.resource)+"\"";
		toBackSlashes(tm["res-sdn"]);
		tm["res-sdf"] = "\"resources\"=3";
		tm["res-fc1"] = "\"resources\",\"resources\",,0";
	} else {
		tm["res-sdn"] = "";
		tm["res-sdf"] = "";
		tm["res-fc1"] = "";
	}

	toBackSlashes(tm["cab-runtime-dir"]);
	toBackSlashes(tm["cab-compile-output-dir"]);


	string templateCabwizInfName = mosyncdir() + string("/bin/cabwiz.inf.template");
	string genCabwizInfName = "cabwiz.inf";

	applyTemplate(genCabwizInfName.c_str(), templateCabwizInfName.c_str(), tm);

	toBackSlashes(genCabwizInfName);

	std::ostringstream cmd;
	cmd << mosyncdir()<<"/bin/"CABWIZ" "<<genCabwizInfName<<" /dest "<<
		s.dst<<" /compress";
	sh(cmd.str().c_str(), s.silent);

	string dstCabName = s.dst + string("/") + string(s.name) + ".cab";
	remove(dstCabName.c_str());
	renameFile(dstCabName, s.dst + string("/cabwiz.cab"));
#else
	printf("Error: not implemented.\n");
	exit(1);
#endif
}
