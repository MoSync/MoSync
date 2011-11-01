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
#include "xlstcomp.h"
#define LST_EXT "lst"
#define LSTX_EXT "lstx"

using namespace std;

void printUsage() {
	printf("Usage: rescomp platform outputdir [.lst|.lstx files]");
	exit(1);
}

bool isExt(const string& filename, const string& ext) {
	if (filename.find_last_of('.') != std::string::npos) {
		return filename.substr(filename.find_last_of(".") + 1) == ext;
	}
	return false;
}

int main(int argc,char *argv[]) {
	if (argc < 3) {
		printUsage();
	}

	char* platform = argv[1];
	char* outputDir = argv[2];

	VariantResourceSet resources = VariantResourceSet();
	resources.setPlatform(string(platform));

	for (int i = 3; i < argc; i++) {
		string filename = string(argv[i]);
		if (isExt(filename, string(LSTX_EXT))) {
			string lstFile = resources.parseLSTX(filename, outputDir);
			string resFile = resources.parseLST(lstFile, outputDir);
		} else if (isExt(filename, string(LST_EXT))) {
			printf("The rescomp tool does not support .lst files, use pipe-tool instead");
		} else {
			printf("Wrong file extension, must be .lstx\n");
			exit(1);
		}
	}

	return 0;
}
