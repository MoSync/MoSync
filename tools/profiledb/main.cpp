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

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "profiledb.h"

using namespace std;

void printUsage() {
	printf(
		"Usage: profiledb <OPTIONS>\n"
		"\n"
		"Options:\n"
		"-l, --list  [PATTERN]            Lists all available profiles that\n"
		"                                 matches PATTERN. A PATTERN is a\n"
		"                                 comma-separated list of sub-PATTERNs,\n"
		"                                 that may be either the name of a\n"
		"                                 profile, or a name followed by a\n"
		"                                 wildcard character (*) to match\n"
		"                                 all profiles that starts with a\n"
		"                                 specific string.\n"
		"-f, --list-families              Lists all available profile families.\n"
		"-g, --get   [PATTERN]            Returns a description of all\n"
		"                                 profiles that matches PATTERN.\n"
		"-m, --match PROFILE CAPABILITIES Returns a profile matching PROFILE.\n"
		"                                 and CAPABILITIES\n"
		"-c, --list-capabilities [STATE]  Lists all available capabilities.\n"
		"                                 that matches TYPE, which is a comma-\n"
		"                                 separated list of states (may be\n"
		"                                 SUPPORTED, UNSUPPORTED, NOT_IMPLEMENTED\n"
		"                                 REQUIRES_PERMISSION or\n"
		"                                 REQUIRES_PRIVILEGED_PERMISSION\n"
		"-x, --exclude PATTERN            Filter out all profiles matching\n"
		"                                 PATTERN."
		"\n"
		""
		"Environment variables used:\n"
		" MOSYNCDIR               Path to the MoSync installation directory.\n"
	);
	exit(1);
}

int main(int argc,char *argv[]) {
	// Parse option(s):
	ProfileDB db = ProfileDB();
	int curArg = 0;
	string cmd = argc > curArg + 1 ? string(argv[curArg + 1]) : string("");
	if ((cmd == "--exclude" || cmd == "-x") && argc > 2) {
		db.setExcluded(string(argv[2]));
		curArg += 2;
	}
	cmd = argc > curArg + 1 ? string(argv[curArg + 1]) : string("");
	if (("--get" == cmd || "-g" == cmd) && argc == curArg + 3) {
		char* profile = argv[curArg + 2];
		db.getProfiles(profile);
	} else if (("--list" == cmd || "-l" == cmd) && argc <= curArg + 3) {
		string profilePattern = argc > curArg + 2 ? argv[curArg + 2] : "*";
		db.listProfiles(profilePattern, false);
	} else if (("--list-families" == cmd || "-f" == cmd) && argc == curArg + 2) {
		db.listProfiles("*", true);
	} else if (("--match" == cmd || "-m" == cmd) && argc >= curArg + 3) {
		string profilePattern = string(argv[curArg + 2]);
		vector<Capability> capabilities;
		for (int i = curArg + 3; i < argc; i++) {
			capabilities.push_back(Capability(string(argv[i]), SUPPORTED));
		}
		db.matchProfiles(profilePattern, capabilities);
	} else if (("--list-capabilities" == cmd || "-c" == cmd) && argc >= curArg + 2) {
		string statePattern = argc > curArg + 2 ? argv[curArg + 2] : "*";
		db.listCapabilities(statePattern);
	} else {
		printUsage();
		return 1;
	}
	return 0;
}
