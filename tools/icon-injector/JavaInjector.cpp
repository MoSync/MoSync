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

#include "JavaInjector.h"
#include  "Icon.h"
#include <fstream>
#include <sstream>
#include "Shared.h"
#include <stdio.h>

using namespace std;

namespace MoSync {

void read72line(ifstream& input, char* buf, int bufSize);

static int j_inject(const char *jarFile, const char *pngFile) {
	int res;

	//extract manifest
	char buf[8192];
	sprintf(buf, "unzip -o \"%s\" \"%s\"", jarFile, "META-INF/MANIFEST.MF");
	res = run(buf);
	if(res != 0)
		return res;
	char icon[1024];

	//parse / modify
	{
		ifstream mfIn("META-INF/MANIFEST.MF");
		while(1) {
			if(!mfIn.good())
				break;
			read72line(mfIn, buf, sizeof(buf));
			char name[1024], midlet[1024];
			if(sscanf(buf, "MIDlet-1: %[^,], %[^,], %s", name, icon, midlet) == 3) {
				// Modifying the manifest will lead to deployment problems,
				// hence this fix
				//mfStrOut << "MIDlet-1: " << name << ", " << pngFile << ", " << midlet << "\n";
				printf("Found icon file in manifest: %s\n", icon);
			}
		}
		mfIn.close();
	}

	// Rename our png file
	rename(pngFile, icon);

	//insert modified manifest & icon
	sprintf(buf, "zip \"%s\" \"%s\" \"%s\"", jarFile, "META-INF/MANIFEST.MF", icon);
	res = run(buf);
	if(res != 0)
		return res;

	//done
	return 0;
}

void read72line(ifstream& input, char* buf, int bufSize) {
	bool readNextLine = true;
	int offset = 0;
	while (readNextLine) {
		input.getline(buf + offset, bufSize - offset);
		streamsize read = input.gcount();
		offset += (int)read - 1;
		// Manifests w exactly 72 bytes in a line should continue on the
		// next if there is a space at the first character position on that line.
		bool isSpace = input.peek() == ' ';
		if (isSpace) {
			input.ignore(1, ' ');
		}
		readNextLine = input.good() && offset < bufSize && read >= 72 && read <= 73 && isSpace;
	}
}

void JavaInjector::inject(const Icon* icon, const std::map<std::string, std::string>& params) {
	string size = verifyParameter(params, "size");
	string dst = verifyParameter(params, "dst");
	const IconInstance* iconInst = icon->findBestInstance(size);
	if(!iconInst) errorExit("Couldn't find any icon instance.");

	if(!convertInstanceToImageFormat(iconInst, "temp.png", size, "png"))
		errorExit("Java icon conversion failed.");
	if(j_inject(dst.c_str(), "temp.png") != 0)
		errorExit("Java injector failed.");
}

}
