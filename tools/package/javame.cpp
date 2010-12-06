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

#include "packagers.h"
#include "util.h"
#include "helpers/mkdir.h"
#include <fstream>
#include <sstream>
#include <errno.h>

using namespace std;

static void writeManifest(const SETTINGS& s, const RuntimeInfo& ri,
	const char* filename, bool isJad, const string& jarFileName);

// if isBlackberry, then s.dst is ignored, and the files are stored in
// the current working directory.
void packageJavaME(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);

	string dstPath = ri.isBlackberry ? "" : s.dst;
	string program, resource;
	program = fullpathString(s.program);
	if(s.resource)
		resource = fullpathString(s.resource);

	int res = _chdir(dstPath.c_str());
	if(res != 0) {
		printf("rename error %i, %i\n", res, errno);
		exit(1);
	}

	string runtimeJarName = ri.path + "MoSyncRuntime" + (s.debug ? "D" : "") + ".jar";
	string appJarName = string(s.name) + ".jar";

	// copy runtime's JAR
	copyFile(appJarName.c_str(), runtimeJarName.c_str());

	// write manifest
	_mkdir("META-INF");
	writeManifest(s, ri, "META-INF/MANIFEST.MF", false, appJarName);

	std::ostringstream cmd;

	// pack program and resource files.
	// done separately to "junk" path names.
	cmd.str("");
	cmd << "zip -9 -j \""<<appJarName<<"\" \""<<program<<"\"";
	if(s.resource)
		cmd << " \""<<resource<<"\"";
	// todo: icon
	sh(cmd.str().c_str());

	// pack manifest
	cmd.str("");
	cmd << "zip -9 -r \""<<appJarName<<"\" META-INF";
	sh(cmd.str().c_str());

	// write JAD
	writeManifest(s, ri, (string(s.name) + ".jad").c_str(), true, appJarName);
}

static void writeManifest(const SETTINGS& s, const RuntimeInfo& ri,
	const char* filename, bool isJad, const string& jarFileName)
{
	ofstream stream(filename);
	if(!isJad) {
		stream << "Manifest-Version: 1.0\n";
	}
	stream << "MIDlet-Vendor: "<<s.vendor<<"\n";
	stream << "MIDlet-Name: "<<s.name<<"\n";
	stream << "MIDlet-1: "<<s.name<<", "<<s.name<<".png, MAMidlet\n";
	stream << "MicroEdition-Profile: MIDP-2.0\n";
	stream << "MicroEdition-Configuration: CLDC-1."<<(ri.isCldc10 ? "0" : "1")<<"\n";
	if(isJad) {
		stream << "MIDlet-Jar-URL: "<<jarFileName<<".jar\n";
		stream << "MIDlet-Jar-Size: "<<getFileSize(jarFileName.c_str())<<"\n";
	}
}
