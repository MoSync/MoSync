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

#include <stack>
#include <sstream>
#include <string>
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
