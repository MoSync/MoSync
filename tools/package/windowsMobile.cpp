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

void packageWM(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);

	string dstCabName = s.dst + string("/") + string(s.name) + ".cab";

	TemplateMap tm;
	tm["project-name"] = s.name;
	tm["CE1"] = "%CE1%";
	tm["CE11"] = "%CE11%";
	tm["D"] = s.debug ? "D" : "";

#ifdef WIN32
	tm["app-vendor"] = s.vendor;
	tm["app-name"] = s.name;
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
	cmd << mosyncdir()<<"/bin/cabwiz \""<<genCabwizInfName<<"\" /dest \""<<
		s.dst<<"\" /compress";
	sh(cmd.str().c_str(), s.silent);

	remove(dstCabName.c_str());
	renameFile(dstCabName, s.dst + string("/cabwiz.cab"));
#else
	string templatePcabConfName = mosyncdir() + string("/bin/pcab.conf.template");
	string genPcabConfName = "pcab.conf";
	string runtimeExe = ri.path + "/MoRE-winmobile" + tm["D"] + ".exe";
	string copiedExe = s.name + string(".exe");
	string copiedProgram = "program";
	string copiedResource = "resources";

	applyTemplate(genPcabConfName.c_str(), templatePcabConfName.c_str(), tm);

	// todo: use source files directly, without copying.
	copyFile(copiedExe.c_str(), runtimeExe.c_str());
	copyFile(copiedProgram.c_str(), s.program);
	if(s.resource)
		copyFile(copiedResource.c_str(), s.resource);
	else
		writeFile(copiedResource.c_str(), NULL, 0);

	std::ostringstream cmd;
	cmd << mosyncdir()<<"/bin/pcab.pl -s . -a \""<<s.name<<"\" -p \""<<s.vendor<<
		"\" \""<<genPcabConfName<<"\" "<<" \""<<dstCabName<<"\"";
	sh(cmd.str().c_str(), s.silent);
#endif
}
