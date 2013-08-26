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

#include <stack>
#include <sstream>
#include <string>
#include <stdio.h>
#include "XMLWriter.h"

using namespace std;

XMLWriter::XMLWriter(ostringstream* output) {
	fOutput = output;
	fIndent = 0;
	fIsLeaf = false;
}

void XMLWriter::indent() {
	for (int i = 0; i < fIndent; i++) {
		*fOutput << "  ";
	}
}

void XMLWriter::line(string _line) {
	indent();
	*fOutput << _line << '\n';
}

void XMLWriter::closeParentTag() {
	if (fIsLeaf) {
		// Close parent tag.
		*fOutput << ">\n";
	}
}
void XMLWriter::startTag(string name) {
	fAttrCount = 0;
	fElementStack.push(name);
	closeParentTag();
	indent();
	*fOutput << "<" << name;
	fIndent++;
	fIsLeaf = true;
}

void XMLWriter::endTag() {
	string name = fElementStack.top();
	fElementStack.pop();
	fIndent--;
	if (fIsLeaf) {
		*fOutput << "/>\n";
	} else {
		indent();
		closeParentTag();
		*fOutput << "</" << name << ">\n";
	}
	fIsLeaf = false;
}

void XMLWriter::setAttr(string key, string value) {
	*fOutput << " " << key << "=\"" << value << "\"";
	fAttrCount++;
}

void XMLWriter::text(string _text) {
	closeParentTag();
	*fOutput << _text;
	fIsLeaf = false;
}

void XMLWriter::start() {
	line("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
}

void XMLWriter::end() {

}

void XMLWriter::dump() {
	printf("%s", fOutput->str().c_str());
}
