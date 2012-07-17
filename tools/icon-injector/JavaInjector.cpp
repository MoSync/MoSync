/* Copyright (C) 2009 Mobile Sorcery AB

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
