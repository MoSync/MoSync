/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xlstcomp.h"
#include "File.h"
#include "helpers/attribute.h"

#define LST_EXT "lst"
#define LSTX_EXT "lstx"

using namespace std;

void printUsage() GCCATTRIB(noreturn);
bool isExt(const string& filename, const string& ext);

void printUsage() {
	printf("Usage: rescomp [-L] platform outputdir [.lst|.lstx files]\n");
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
	int platformLen = strlen(platform);
	// Must be lower case!
	for (int i = 0; i < platformLen; i++) {
		platform[i] = tolower(platform[i]);
	}
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
