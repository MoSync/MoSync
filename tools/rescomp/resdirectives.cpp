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

#include "resdirectives.h"
#include <sstream>

#define ATTR_PLATFORM "platform"
#define ATTR_ID "id"
#define ATTR_LOAD_TYPE "loadType"
#define ATTR_RES_FILE "resource"
#define ATTR_MIME_TYPE "mimeType"

using namespace std;

void ResourceDirective::setId(string id) {
	fId = id;
}

string ResourceDirective::getId() {
	return fId;
}

void ResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	output << ".res ";
	if (asVariant) {
		output << "// ID: " << fId << '\n';
	} else {
		output << fId << '\n';
	}
}

void ResourceDirective::initDirectiveFromAttributes(const char** attributes) {
	const char* id = findAttr(ATTR_ID, attributes);
	if (id) {
		setId(string(id));
	}
	const char* loadType = findAttr(ATTR_LOAD_TYPE, attributes);
	if (loadType && !strcmp("unloaded", loadType)) {
		setLoadType(LoadType_Unloaded);
	} else {
		setLoadType(LoadType_Startup);
	}
}

void ResourceDirective::initDirectiveFromCData(const char* cdata, int length) {
	// Do nothing.
}

void ResourceDirective::writeByteDirective(ostringstream& output, const char* array, size_t offset, size_t len) {
	char HEX_DIGITS[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	size_t i = 0;
	while (i < len) {
		output << ".byte ";
		for (size_t j = 0; j < 12 && i < len; j++, i++) {
			if (j > 0) {
				output << ",";
			}
			output << "0x" << HEX_DIGITS[(array[offset + i] >> 4) & 0xf] << HEX_DIGITS[array[offset + i] & 0xf];
		}
		output << "\n";
	}
}

string ResourceDirective::validate() {
	if (fId.size() == 0) {
		return "No id specified for tag";
	}
	return string();
}

void ResourceDirective::setFile(string file) {
	fFile = file;
}

void ResourceDirective::setLineNo(int lineNo) {
	fLineNo = lineNo;
}

int ResourceDirective::getLineNo() {
	return fLineNo;
}

string ResourceDirective::getFile() {
	return fFile;
}

void ResourceDirective::setLoadType(LoadType loadType) {
	fLoadType = loadType;
}

LoadType ResourceDirective::getLoadType() {
	return fLoadType;
}

string ResourceDirective::getUniqueToken() {
	return string();
}

void FileResourceDirective::setResource(string resource) {
	fResource = resource;
}

void FileResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	// Just get rid of a bunch of .lfile stuff!
	string lfileDirective =  ".lfile \'" + fFile + "\'\n";
	if (lfileDirective != gLastLFileDirective) {
		output << lfileDirective;
		gLastLFileDirective = lfileDirective;
	}
	ResourceDirective::writeDirectives(output, asVariant);
	string resourceStr = string("\"") + fResource + string("\"");
	if (asVariant) {
		output << ".ubin\n";
		output << ".include " << resourceStr << "\n";
	} else {
		output << '.' << fResType << " " << resourceStr << "\n";
	}
}

void FileResourceDirective::initDirectiveFromAttributes(const char** attributes) {
	ResourceDirective::initDirectiveFromAttributes(attributes);
	const char* resFile = findAttr(ATTR_RES_FILE, attributes);
	if (resFile) {
		setResource(string(resFile));
	}
}

string FileResourceDirective::validate() {
	if (fResource.size() == 0) {
		return "Resource tags require the 'resource' attribute";
	}
	return ResourceDirective::validate();
}

string FileResourceDirective::getUniqueToken() {
	return fResource;
}

void MediaResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	if (asVariant) {
		FileResourceDirective::writeDirectives(output, asVariant);
	} else {
		output << ".media \"" << fMimeType << "\",\"" << fResource << "\"\n";
	}
}

void MediaResourceDirective::initDirectiveFromAttributes(const char** attributes) {
	FileResourceDirective::initDirectiveFromAttributes(attributes);
	const char* mimeType = findAttr(ATTR_MIME_TYPE, attributes);
	if (mimeType) {
		setMimeType(string(mimeType));
	}
}

void MediaResourceDirective::setMimeType(string mimeType) {
	fMimeType = mimeType;
}

static bool isSpecialChar(char ch) {
	return ch < 0x20;
}

void StringResourceDirective::writeDirectiveAsString(ostringstream& output, size_t offset, size_t len) {
	output << ".string \"" << fStringValue.substr(offset, len) << "\"\n";
}

void StringResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	ResourceDirective::writeDirectives(output, asVariant);
	string binStr = asVariant ? ".ubin" : ".bin";
	output << binStr << "\n";
	size_t currentPos = 0;
	size_t currentOffset = 0;
	bool isInSpecialString = false;
	bool wasInSpecialString = false;
	size_t len = fStringValue.length();
	const char* chArray = fStringValue.c_str();
	while (currentPos < len) {
		char ch = chArray[currentPos];
		isInSpecialString = isSpecialChar(ch);
		if (isInSpecialString ^ wasInSpecialString) {
			size_t lenToWrite = currentPos - currentOffset;
			if (isInSpecialString) {
				writeDirectiveAsString(output, currentOffset, lenToWrite);
			} else {
				ResourceDirective::writeByteDirective(output, chArray, currentOffset, lenToWrite);
			}
			currentOffset = currentPos;
		}
		wasInSpecialString = isInSpecialString;
		currentPos++;
	}
}

void StringResourceDirective::initDirectiveFromCData(const char* cdata, int length) {
	// O(n^2)
	fStringValue += string(cdata, length);
}

string StringResourceDirective::getUniqueToken() {
	return fStringValue;
}

void PlaceholderDirective::writeDirectives(ostringstream& output, bool asVariant) {
	ResourceDirective::writeDirectives(output, asVariant);
	output << '.' << fResType << '\n';
}

const char* findAttr(const char* name, const char** attributes) {
	  for (int i = 0; attributes[i]; i += 2) {
		  if (!strcmp(attributes[i], name)) {
			  return attributes[i + 1];
		  }
	  }
	  return NULL;
}
