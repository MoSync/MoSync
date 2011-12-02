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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <fstream>
#include "package.h"
#include "packagers.h"
#include "util.h"
#include "profiledb/profiledb.h"

using namespace std;

static string getModelDir(ProfileType pt, const SETTINGS& s);
static bool getRuntime(ProfileType pt, const SETTINGS& s, RuntimeInfo& ri, string& name);
static bool parseProfileInfo(ProfileType pt, Profile* p, const SETTINGS& s, const char* filename,
		RuntimeInfo& pi);
static bool parseRuntimeLine(string line, string& path, string& name);
static bool parseRuntimeTxt(const char* line, string& path, string& name);
static bool parseIntProp(const string& line, const char* key, int& value);
//static bool parseStringProp(const string& line, const char* key, string& value);

void package(const SETTINGS& s) {
	// Read runtime.txt and maprofile.h to find which runtime to use.
	// Then call the appropriate packager.

	testModel(s);
	testProfileType(s);

	ProfileType profileType =
	        s.profileType && !strcmp("platform", s.profileType) ?
				PLATFORM_BASED :
				DEVICE_BASED;

	// find profile info
	RuntimeInfo ri;

	// parse files
	string runtimeName;
	if (!getRuntime(profileType, s, ri, runtimeName)) {
		printf("runtime parse error\n");
		exit(1);
	}
	toDir(ri.path);

	// parse android version
	ri.androidVersion = 0;
	{
		size_t i = ri.path.find("android_");
		if (i != string::npos)
			sscanf(ri.path.c_str() + i, "android_%i", &ri.androidVersion);
	}

	// select runtime
	if (runtimeName == "JavaME" || runtimeName == "BlackBerry") {
		// The above condition will satisify the device
		// based AND the platform based versions.
		packageJavaME(s, ri);
		if (ri.isBlackberry) {
			packageBlackberry(s, ri);
		}
	} else if (runtimeName == "s60v2") {
		packageS60v2(s, ri);
	} else if (runtimeName == "s60v3") {
		packageS60v3(s, ri);
	} else if (runtimeName == "s60v5") {
		packageS60v3(s, ri);
	} else if (runtimeName == "sp2003") {
		packageWM(s, ri);
	} else if (runtimeName == "wm5") {
		packageWM(s, ri);
	} else if (runtimeName == "wm6") {
		packageWM(s, ri);
	} else if (runtimeName == "wm6pro") {
		packageWM(s, ri);
	} else if (runtimeName == "winphone") {
		packageWindowsPhone(s, ri);
	} else if (runtimeName == "moblin") {
		packageMoblin(s, ri);
	} else if (runtimeName.find("android") == 0) { //begins with
		packageAndroid(s, ri);
	} else if (runtimeName == "iphoneos") {
		packageIOS(s, ri);
	} else {
		printf("Error: unknown runtime '%s'\n", runtimeName.c_str());
		exit(1);
	}
}

static string getModelDir(ProfileType pt, const SETTINGS& s) {
	string modelDir(mosyncdir());
	modelDir += pt == DEVICE_BASED ? "/profiles/vendors/"
	        : "/profiles/platforms/";
	modelDir += s.model;
	return modelDir;
}

static bool getRuntime(ProfileType pt, const SETTINGS& s, RuntimeInfo& ri, string& name) {
	if (pt == DEVICE_BASED) {
		string modelDir = getModelDir(pt, s);
		string runtimeTxtPath = modelDir + "/runtime.txt";
		toSlashes(runtimeTxtPath);
		string headerPath = modelDir + "/maprofile.h";
		toSlashes(headerPath);

		if (parseRuntimeTxt(runtimeTxtPath.c_str(), ri.path, name)) {
			return parseProfileInfo(DEVICE_BASED, NULL, s, headerPath.c_str(), ri);
		}

	} else {
		ProfileDB db;
		Profile* profile = db.findProfile(string(s.model));
		if (profile) {
			string runtime = profile->getRuntime();
			if (parseRuntimeLine("profiles/runtimes/" + runtime, ri.path, name)) {
				return parseProfileInfo(PLATFORM_BASED, profile, s, NULL, ri);
			}
		}
	}
	return false;
}

static bool parseRuntimeTxt(const char* filename, string& path, string& name) {
	// read file
	ifstream file(filename);
	setName(file, filename);
	beGood(file);
	string line;
	getline(file, line);
	if (line[line.length() - 1] == '\r')
		line = line.erase(line.length() - 1, 1);
	beGood(file);
	return parseRuntimeLine(line, path, name);
}

static bool parseRuntimeLine(string line, string& path, string& name) {
	// swap backslashes
	toSlashes(line);

	// set path
	path = mosyncdir() + ("/" + line);

	// check runtime number
	size_t lastSlash = line.find_last_of('/');
	if (lastSlash == string::npos)
		return false;

	string number = line.substr(lastSlash + 1);
	for (size_t i = 0; i < number.size(); i++) {
		if (!isdigit(number[i]))
			return false;
	}

	// find runtime name
	size_t nameSlash = line.find_last_of('/', lastSlash - 1);
	if (nameSlash == string::npos)
		return false;
	name = line.substr(nameSlash + 1, (lastSlash - nameSlash) - 1);
	if (name.length() == 0)
		return false;

	return true;
}

static const char* findProp(const string& line, const char* key) {
	string k("#define ");
	k += key;
	if (line.find(k) == 0) {
		const char* val = line.c_str() + k.length();
		while (isspace(*val)) {
			val++;
		}
		return val;
	} else {
		return NULL;
	}
}

// changes value and returns true only if prop was found and properly parsed.
static bool parseIntProp(const string& line, const char* key, int& value) {
	const char* val = findProp(line, key);
	if (val) {
		int len;
		int tempval;
		int res = sscanf(val, "%i%n", &tempval, &len);
		if (res == 1 && (int) strlen(val) == len) {
			value = tempval;
			return true;
		}
	}
	return false;
}

#if 0	// unused
static bool parseStringProp(const string& line, const char* key, string& value) {
	const char* val = findProp(line, key);
	if(val) {
		int len = strlen(val);
		if(len < 2)
		return false;
		if(val[0] != '\"' || val[len-1] != '\"')
		return false;
		value = string(val + 1, len - 2);
		return true;
	}
	return false;
}
#endif

static bool parseProfileInfo(ProfileType profileType, Profile* p, const SETTINGS& s, const char* filename,
        RuntimeInfo& pi) {
	bool hasBbMajor = false, hasBbMinor = false;
	pi.isBlackberry = false;
	pi.hasLimitedResourceSize = false;
	pi.isCldc10 = false;
	pi.iconSize = "default";
	int iconX = -1;
	int iconY = -1;
	if (profileType == DEVICE_BASED) {
		ifstream file(filename);
		setName(file, filename);
		if (!file.good())
			return false;
		while (file.good()) {
			string line;
			getline(file, line);
			if (line.find("#define MA_PROF_BUG_RESOURCE_SIZE_LIMITED") == 0) {
				pi.hasLimitedResourceSize = true;
			}
			hasBbMajor |= parseIntProp(line, "MA_PROF_BLACKBERRY_VERSION",
			        pi.blackberryVersion);
			hasBbMinor |= parseIntProp(line,
			        "MA_PROF_BLACKBERRY_VERSION_MINOR", pi.blackberryMinor);
			if (line.find("#define MA_PROF_SUPPORT_CLDC_10") == 0) {
				pi.isCldc10 = true;
			}
			parseIntProp(line, "MA_PROF_CONST_ICONSIZE_X", iconX);
			parseIntProp(line, "MA_PROF_CONST_ICONSIZE_Y", iconY);
		}
		//pi.isBlackberry = (hasBbMajor && hasBbMinor);	//rapc is not available.
	} else {
		pi.isBlackberry = p->getFamily() == "BlackBerry";
		if (pi.isBlackberry) {
			Capability major = p->getCapability("Version/Major");
			Capability minor = p->getCapability("Version/Minor");
			sscanf(major.getValue().c_str(), "%i", &pi.blackberryVersion);
			sscanf(minor.getValue().c_str(), "%i", &pi.blackberryMinor);
			pi.iconSize = "80x80";
		}
	}
	if (iconX > 0 && iconY > 0) {
		char buf[32];
		sprintf(buf, "%ix%i", iconX, iconY);
		pi.iconSize = buf;
	}
	return true;
}

void packageMoblin(const SETTINGS&, const RuntimeInfo& ri) {
	printf("not implemented\n");
	exit(1);
}
