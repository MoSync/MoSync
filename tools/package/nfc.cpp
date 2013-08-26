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
