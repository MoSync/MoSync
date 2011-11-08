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
		"Usage: package <OPTIONS>\n"
		"\n"
		"Options:\n"
		"-l, --list               Lists all available profiles\n"
		"-g, --get PROFILE        Returns the capabilities of PROFILE\n"
		"\n"
		"Environment variables used:\n"
		" MOSYNCDIR               Path to the MoSync installation directory.\n"
	);
	exit(1);
}

int main(int argc,char *argv[]) {
	// Parse option(s):
	int arg = 1;
	ProfileDB db = ProfileDB();
	while (arg < argc) {
		if ((!strcmp("--get", argv[arg]) || !strcmp("-g", argv[arg]))  && argc >= arg + 1) {
			char* profile = argv[arg + 1];
			db.getProfile(profile);
			exit(0);
		}
		if (!strcmp("--list", argv[arg]) || !strcmp("-l", argv[arg])) {
			string profilePattern = argc > arg + 1 ? argv[arg + 1] : "*";
			db.listProfiles(profilePattern);
			exit(0);
		}
		arg++;
	}

	printUsage();
	return 0;
}
