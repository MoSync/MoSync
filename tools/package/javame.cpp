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

#define CONFIG_H	//hack

#include "packagers.h"
#include "util.h"
#include "permissions.h"
#include "tools.h"
#include "helpers/mkdir.h"
#include "helpers/helpers.h"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <ostream>

using namespace std;

static void writeManifest(const SETTINGS& s, const RuntimeInfo& ri,
	const char* filename, bool isJad, const string& jarFileName);
static void writePermissions(ostream& stream, const SETTINGS& s, const RuntimeInfo& ri, bool isJad);
static void writeToManifest(ostream& stream, const string& line, bool isJad);
static void addMIDletPermission(vector<string>& permissions, bool flag, const char* nativePerm);
static void sign(const SETTINGS& s, const RuntimeInfo& ri, const char* jar, const char* jad);
static void createJadToolCommand(ostringstream& jadToolCmd, const SETTINGS& s,
	const RuntimeInfo& ri, const char* jad, bool hidden);
static void createJarSignCommand(ostringstream& jarSignCmd, const SETTINGS& s,
	const RuntimeInfo& ri, const char* jar, const char* jad, bool hidden);

// splits one file into many.
// size appropriate for BlackBerry.
// names matching SplitResourceStream.
// returns the names of the part-files, formatted for command-line use.
static string split(const string& baseName) {
	static const size_t MAX_SIZE = 63*1024;	// guesswork, based on BB SDK v4
	size_t baseSize;
	byte* data = (byte*)readBinaryFile(baseName.c_str(), baseSize);
	int i = 0;
	size_t pos = 0;
	std::ostringstream names;
	while(pos < baseSize) {
		size_t len = MIN(baseSize - pos, MAX_SIZE);
		std::ostringstream name;
		name << baseName << i;
		if(i > 0)
			names << "\" \"";
		names << name.str();
		writeFile(name.str().c_str(), data + pos, len);
		i++;
		pos += len;
	}
	return names.str();
}

// if isBlackberry, then s.dst is ignored, and the files are stored in
// the current working directory.
void packageJavaME(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);
	testJavaMESigning(s);

	//string dstPath = ri.isBlackberry ? "" : s.dst;
	string dstPath = s.dst;
	string program, resource;
	program = fullpathString(s.program);
	if(s.resource)
		resource = fullpathString(s.resource);

	if(!dstPath.empty()) {
		int res = _chdir(dstPath.c_str());
		if(res != 0) {
			printf("_chdir error %i, %i (%s)\n", res, errno, strerror(errno));
			exit(1);
		}
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
	// done separately from the other package parts in order to "junk" path names.
	cmd.str("");
	cmd << "zip -9 -j "<<file(appJarName)<<" "<<
		file((ri.hasLimitedResourceSize ? split(program) : program));
	if(s.resource)
		cmd << " " << file(ri.hasLimitedResourceSize ? split(resource) : resource);

	sh(cmd.str().c_str());

	// pack manifest
	cmd.str("");
	cmd << "zip -9 -r "<<file(appJarName)<<" META-INF";
	sh(cmd.str().c_str());

	// Inject icon
	std::ostringstream iconInjectCmd;
	string outputIcon = dstPath + "/icon.png";
	if (ri.iconSize.size() != 0 && s.icon) {
		// For java me, the -dst is the JAR!
		injectIcon("j2me", ri.iconSize.c_str(), s.icon, appJarName.c_str(), s.silent);
	}

	// write JAD
	string appJad = string(s.name) + ".jad";
	writeManifest(s, ri, appJad.c_str(), true, appJarName);

	// Sign
	if (s.javameKeystore) {
		sign(s, ri, appJarName.c_str(), appJad.c_str());
	}
}

static void sign(const SETTINGS& s, const RuntimeInfo& ri, const char* jar, const char* jad) {
	std::ostringstream jadToolCmd;
	std::ostringstream hiddenJadToolCmd;

	createJadToolCommand(jadToolCmd, s, ri, jad, false);
	createJadToolCommand(hiddenJadToolCmd, s, ri, jad, true);

	string jadToolCmdStr = jadToolCmd.str();
	sh(jadToolCmdStr.c_str(), false,
		s.showPasswords ? jadToolCmdStr.c_str() : hiddenJadToolCmd.str().c_str());

	std::ostringstream jarSignCmd;
	std::ostringstream hiddenJarSignCmd;

	createJarSignCommand(jarSignCmd, s, ri, jar, jad, false);
	createJarSignCommand(hiddenJarSignCmd, s, ri, jar, jad, true);

	string jarSignCmdStr = jarSignCmd.str();
	sh(jarSignCmdStr.c_str(), false,
		s.showPasswords ? jarSignCmdStr.c_str() : hiddenJarSignCmd.str().c_str());
}

static void createJadToolCommand(ostringstream& jadToolCmd, const SETTINGS& s,
	const RuntimeInfo& ri, const char* jad, bool hidden)
{
	jadToolCmd << "java -jar " << getBinary("javame/JadTool.jar") << " -addcert -alias " <<
		arg(s.javameAlias) << " -keystore " << file(s.javameKeystore) <<
		" -inputjad " << file(jad) << " -outputjad " << file(jad) <<
		" -storepass " << (hidden ? "*** HIDDEN ***" : arg(s.javameStorePass));
}

static void createJarSignCommand(ostringstream& jarSignCmd, const SETTINGS& s,
	const RuntimeInfo& ri, const char* jar, const char* jad, bool hidden)
{
	jarSignCmd << "java -jar " << getBinary("javame/JadTool.jar") << " -addjarsig -jarfile " <<
		file(jar) << " -keystore " << file(s.javameKeystore) << " -storepass " <<
		(hidden ? "*** HIDDEN ***" : arg(s.javameStorePass)) << " -alias " << arg(s.javameAlias) <<
		" -keypass " << (hidden ? "*** HIDDEN ***" : arg(s.javameKeyPass)) << " -inputjad " <<
		file(jad) << " -outputjad " << file(jad);
}

static void writeManifest(const SETTINGS& s, const RuntimeInfo& ri,
	const char* filename, bool isJad, const string& jarFileName)
{
	ofstream stream(filename, ios::binary);
	setName(stream, filename);
	if(!isJad) {
		stream << "Manifest-Version: 1.0\n";
	}
	writeToManifest(stream, string("MIDlet-Vendor: ") + s.vendor + "\n", isJad);
	writeToManifest(stream, string("MIDlet-Name: ") + s.name + "\n", isJad);
	stream << "MIDlet-Version: 1.0\n";
	stream << "Created-By: MoSync package\n";	//todo: add version number and git hash
	writeToManifest(stream, string("MIDlet-1: ") + s.name + ", " + s.name + ".png, MAMidlet\n", isJad);
	writePermissions(stream, s, ri, isJad);
	stream << "MicroEdition-Profile: MIDP-2.0\n";
	stream << "MicroEdition-Configuration: CLDC-1."<<(ri.isCldc10 ? "0" : "1")<<"\n";
	if(isJad) {
		// This line order is crucial for installation on Blackberry 8320
		// (and probably related models).
		// If it is changed, you will get error 904 (Incorrect JAR file size).
		stream << "MIDlet-Jar-Size: "<<getFileSize(jarFileName.c_str())<<"\n";
		stream << "MIDlet-Jar-URL: "<<jarFileName<<"\n";
	}
	beGood(stream);
	stream.flush();
}

static void writeToManifest(ostream& stream, const string& line, bool isJad) {
	if (isJad) {
		stream << line;
	} else {
		write72line(stream, line);
	}
}

static void writePermissions(ostream& stream, const SETTINGS& s, const RuntimeInfo& ri, bool isJad) {
	set<string> permissionSet = set<string>();
	parsePermissions(permissionSet, s.permissions);

	vector<string> outPermissions = vector<string>();
	vector<string> outOptPermissions = vector<string>();

	// Bluetooth
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, BLUETOOTH),
		"javax.microedition.io.Connector.bluetooth.client");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, BLUETOOTH),
		"javax.microedition.io.Connector.bluetooth.server");

	// Calendar
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CALENDAR_READ),
		"javax.microedition.pim.EventList.read");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CALENDAR_READ),
		"javax.microedition.pim.ToDoList.read");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CALENDAR_WRITE),
		"javax.microedition.pim.EventList.write");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CALENDAR_WRITE),
		"javax.microedition.pim.ToDoList.write");

	// Camera -- req. signing
	addMIDletPermission(outOptPermissions, isPermissionSet(permissionSet, CAMERA),
		"javax.microedition.media.control.VideoControl.getSnapshot");

	// Contacts
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CONTACTS_READ),
		"javax.microedition.pim.ContactList.read");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, CONTACTS_WRITE),
		"javax.microedition.pim.ContactList.write");

	// File storage
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, FILE_STORAGE_READ),
		"javax.microedition.io.Connector.file.read");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, FILE_STORAGE_WRITE),
		"javax.microedition.io.Connector.file.write");

	// Internet & networking
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, INTERNET),
		"javax.microedition.io.Connector.http");
	addMIDletPermission(outOptPermissions, isPermissionSet(permissionSet, INTERNET),
		"javax.microedition.io.Connector.socket");

	// Power mgmt - no permissions

	// SMS & Messaging
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, SMS_RECEIVE),
		"javax.microedition.io.Connector.sms");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, SMS_RECEIVE),
		"javax.wireless.messaging.mms.receive");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, SMS_SEND),
		"javax.microedition.io.Connector.sms");
	addMIDletPermission(outPermissions, isPermissionSet(permissionSet, SMS_SEND),
		"javax.wireless.messaging.mms.send");

	// Vibrate - no permissions

	// Location
	bool locationPermission = isPermissionSet(permissionSet, LOCATION_FINE) ||
		isPermissionSet(permissionSet, LOCATION_COARSE);
	addMIDletPermission(outPermissions, locationPermission,
		"javax.microedition.location.Location");
	addMIDletPermission(outPermissions, locationPermission,
		"javax.microedition.location.Orientation");

	// Ehrm... what if line length > max for manifests?
	const string permissionDelim = string(", ");
	if (outPermissions.size() > 0) {
		writeToManifest(stream, string("MIDlet-Permissions: ") + delim(outPermissions, permissionDelim), isJad);
		stream << "\n";
	}
	if (outOptPermissions.size() > 0) {
		writeToManifest(stream, string("MIDlet-Permissions-Opt: ") + delim(outOptPermissions, permissionDelim), isJad);
		stream << "\n";
	}
}

static void addMIDletPermission(vector<string>& permissions, bool flag,
	const char* nativePerm)
{
	if (flag) {
		permissions.push_back(string(nativePerm));
	}
}
