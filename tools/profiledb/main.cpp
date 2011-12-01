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
#include "profiledb/profiledb.h"

using namespace std;

void printUsage() {
	printf(
		"Usage: profiledb [OPTIONS] COMMAND [PARAMETERS]\n"
		"\n"
		"Commands:\n"
		"-l, --list  [PATTERN]            List all available profiles that\n"
		"                                 matches PATTERN. A PATTERN is a\n"
		"                                 comma-separated list of sub-PATTERNs,\n"
		"                                 that may be either the name of a\n"
		"                                 profile, or a name followed by a\n"
		"                                 wildcard character (*) to match\n"
		"                                 all profiles that starts with a\n"
		"                                 specific string.\n"
		"-f, --list-families              List all available profile families.\n"
		"-g, --get   [PATTERN]            Return a description of all\n"
		"                                 profiles that matches PATTERN.\n"
		"-m, --match PROFILE CAPABILITIES Return a profile matching PROFILE.\n"
		"                                 and CAPABILITIES\n"
		"-c, --list-capabilities [STATE]  List all available capabilities.\n"
		"                                 that matches TYPE, which is a comma-\n"
		"                                 separated list of states (may be\n"
		"                                 SUPPORTED, UNSUPPORTED, NOT_IMPLEMENTED\n"
		"                                 REQUIRES_PERMISSION or\n"
		"                                 REQUIRES_PRIVILEGED_PERMISSION\n"
		"-r, --list-runtime [PROFILE]     List the runtime for a profile\n\n"
		"Options:\n"
		"-x, --exclude PATTERN            Filter out all profiles matching\n"
		"                                 PATTERN.\n"
		"-b, --brief                      List all profiles in a short list\n"
		"                                 instead of using XML.\n"
		"    --no-caps                    Do not include capabilities in\n"
		"                                 XML output"
		"\n"
		"Environment variables used:\n"
		" MOSYNCDIR               Path to the MoSync installation directory.\n"
	);
	exit(1);
}

int main(int argc,char *argv[]) {
	// Parse option(s):
	ProfileDB db = ProfileDB();
	db.setIncludeCapabilities(true);
	int curArg = 0;
	while (curArg < argc) {
		curArg++;
		string cmd = argc > curArg ? string(argv[curArg]) : string("");
		string param = argc > curArg + 1 ? string(argv[curArg + 1]) : string("");
		if (cmd == "--exclude" || cmd == "-x") {
			db.setExcluded(param);
			curArg++;
		} else if (cmd == "--no-caps") {
			db.setIncludeCapabilities(false);
		} else if ((cmd == "--brief" || cmd == "-b")) {
			db.setBrief(true);
		} else if (cmd == "--list-mappings") {
			db.setOutputMappings(true);
		} else if ("--get" == cmd || "-g" == cmd) {
			db.getProfiles(param);
			return 0;
		} else if ("--list" == cmd || "-l" == cmd) {
			string profilePattern = param.length() > 0 ? param : string("*");
			db.listProfiles(profilePattern, false);
			return 0;
		} else if ("--list-families" == cmd || "-f" == cmd) {
			db.listProfiles("*", true);
			return 0;
		} else if ("--match" == cmd || "-m" == cmd) {
			string profilePattern = param;
			vector<Capability> requiredCapabilities;
			vector<Capability> optionalCapabilities;
			vector<Capability>* currentCapabilities = &requiredCapabilities;
			for (int i = curArg + 2; i < argc; i++) {
				string cap = string(argv[i]);
				if ("-o" == cap || "--optional" == cap) {
					currentCapabilities = &optionalCapabilities;
				} else {
					currentCapabilities->push_back(Capability(cap, string(), string(), SUPPORTED, RUNTIME));
				}
			}

			db.matchProfiles(profilePattern, requiredCapabilities, optionalCapabilities);
			return 0;
		} else if ("--list-capabilities" == cmd || "-c" == cmd) {
			string statePattern = param.length() > 0 ? param : string("*");
			db.listCapabilities(statePattern);
			return 0;
		} else if ("--list-runtime" == cmd || "-r" == cmd) {
			db.listRuntime(param);
			return 0;
		}
	}

	printUsage();
	return 1;
}
