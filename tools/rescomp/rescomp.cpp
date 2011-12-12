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
#include <string.h>
#include "xlstcomp.h"
#include "File.h"

#define LST_EXT "lst"
#define LSTX_EXT "lstx"

using namespace std;

void printUsage();
bool isExt(const string& filename, const string& ext);

void printUsage() {
	printf("Usage: rescomp [-L lstfile] platform outputdir [.lst|.lstx files]");
	exit(1);
}

bool isExt(const string& filename, const string& ext) {
	if (filename.find_last_of('.') != std::string::npos) {
		return filename.substr(filename.find_last_of(".") + 1) == ext;
	}
	return false;
}

int main(int argc,char *argv[]) {
	// Parse option(s):
	int arg = 1;
	bool generateOnly = false;

	if (argc > 1) {
		// Generate lst file only
		if (!strcmp("-L", argv[1])) {
			generateOnly = true;
			arg += 1;
		}
	}

	if (argc < arg + 2) {
		printUsage();
	}

	char* platform = argv[arg];
	char* outputDir = argv[arg + 1];
	string lstFile = string(outputDir) + "/~tmpres.lst";

	VariantResourceSet resources = VariantResourceSet();
	resources.setPlatform(string(platform));

	arg += 2;

	for (int i = arg; i < argc; i++) {
		string filename = string(argv[i]);
		File file = File(filename);
		if (file.isDirectory()) {
			resources.scanForResources(file.getAbsolutePath());
		} else if (isExt(filename, string(LSTX_EXT))) {
			resources.parseLSTX(filename);
		} else if (isExt(filename, string(LST_EXT))) {
			printf("The rescomp tool does not support .lst files, use pipe-tool instead");
		} else {
			printf("Wrong file extension, must be .lstx\n");
			exit(1);
		}
	}

	resources.writeResources(lstFile);
	if (!generateOnly) {
		string resFile = resources.parseLST(lstFile, outputDir);
	}

	return 0;
}
