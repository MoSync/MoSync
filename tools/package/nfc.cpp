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

#include <expat.h>
#include <stdio.h>
#include <cstring>

#include "nfc.h"

using namespace std;

static NfcInfo* result = 0;
static TechList* currTechList = 0;

static void start(void *data, const char *el, const char **attr) {
	if (!strcmp(el, "nfc")) {
		result = new NfcInfo();
	}

	if (!strcmp(el, "tech-list")) {
		if (currTechList) {
			printf("Nested techlist tags not allowed.");
			exit(1);
		}
		currTechList = new TechList();
	}

	if (!strcmp(el, "tech")) {
		if (!currTechList) {
			printf("Tech tags must be children of techlist tags.");
			exit(1);
		}
		for (int i = 0; attr[i]; i += 2) {
			if (!strcmp(attr[i], "name")) {
				currTechList->technologies.push_back(string(attr[i + 1]));
			} else {
				printf("Tech tags must have a \'name\' attribute.");
				exit(1);
			}
		}
	}
}

static void end(void *data, const char *el) {
	if (!strcmp(el, "tech-list")) {
		result->addTechList(currTechList);
		currTechList = 0;
	}
}

NfcInfo* NfcInfo::parse(const string& filename) {
	char buf[1024];
	size_t len; /* len is the number of bytes in the current bufferful of data */
	int done = 0;
	FILE* f = fopen(filename.c_str(), "r");

	if (!f) {
		printf("Could not open input file: %s", filename.c_str());
		exit(1);
	}

	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, start, end);

	do {
		/*get a piece of input into the buffer
		 done = whether this bufferful is the last bufferful */
		len = fread(buf, 1, 1024, f);
		if (!XML_Parse(parser, buf, (int) len, done)) {
			//handle parse error
			printf("XML parsing failed");
			exit(1);
		}
	} while (!feof(f));
	XML_ParserFree(parser);
	return result;
}

void NfcInfo::addTechList(TechList* techList) {
	mTechLists.push_back(techList);
}

std::vector<TechList*>& NfcInfo::getTechLists() {
	return mTechLists;
}
