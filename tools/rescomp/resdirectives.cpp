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
#include "xlstcomp.h"
#include "File.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void ResourceDirective::setParent(ResourceDirective* parent) {
	fParent = parent;
	if (parent) {
		parent->fChildren.push_back(this);
	}
}

string ResourceDirective::getId() {
	return fId;
}

void ResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	writeDirectiveIdentifier(output, asVariant);
	writeDirectiveBody(output, asVariant);
}

void ResourceDirective::writeDirectiveIdentifier(ostringstream& output, bool asVariant) {
	if (!fParent) {
		output << ".res ";
		if (asVariant) {
			output << "// ID: " << fId << '\n';
		} else {
			output << fId << '\n';
		}
	}
}

void ResourceDirective::writeDirectiveBody(ostringstream& output, bool asVariant) {
	// By default does nothing.
}

void ResourceDirective::writeDirectiveChildren(ostringstream& output, bool asVariant) {
	for (size_t i = 0; i < fChildren.size(); i++) {
		ResourceDirective* child = fChildren[i];
		if (child) {
			child->writeDirectives(output, asVariant);
		}
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
	// Only allow whitespace!
	if (string(cdata, length).find_first_not_of("\n\r\t ") != string::npos) {
		fErrorMessage = "No text allowed within tag";
	}
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

void ResourceDirective::writeWordDirective(ostringstream& output, DataType type, const int* array, size_t offset, size_t len) {
	int mask;
	string directive;
	switch (type) {
	case Byte:
		mask = 0xff;
		directive = ".byte";
		break;
	case Half:
		mask = 0xffff;
		directive = ".half";
		break;
	default:
		mask = 0xffffffff;
		directive = ".word";
		break;
	}

	size_t i = 0;
	while (i < len) {
		output << directive << " ";
		for (size_t j = 0; j < 12 && i < len; j++, i++) {
			if (j > 0) {
				output << ",";
			}
			if (array[i] >= 0) {
				output << "0x" << hex << (array[i] & mask);
			} else {
				output << dec << array[i];
			}
		}
		output << "\n";
	}
}

string ResourceDirective::validate() {
	if (!fErrorMessage.empty()) {
		return fErrorMessage;
	} else if  (fId.empty() && !fParent) {
		return "No id specified for tag";
	} else if (!fId.empty() && fParent) {
		return "Child tags cannot have an id";
	} else if (fParent && !fCanHaveParent) {
		return "This tag cannot be a child of another tag";
	} else if (!fChildren.empty() && !fCanHaveChildren) {
		return "This tag cannot have child tags";
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

string BinaryResourceDirective::validate() {
	return ResourceDirective::validate();
}

void DataResourceDirective::initDirectiveFromCData(const char* cdata, int length) {
	// O(n^2)
	string binaryDataFragment = string(cdata, length);
	fBinaryData += binaryDataFragment;
	// Hm, we now do this at every new text fragment...
	if (!parseInts()) {
		fErrorMessage = "Invalid integer value";
	}
}

bool DataResourceDirective::parseInts() {
	fInts.clear();
	size_t pos = 0;
	while (pos < fBinaryData.size()) {
		size_t intStart = fBinaryData.find_first_not_of("\n\r\t ,", pos);
		if (intStart == string::npos) {
			return true;
		}
		size_t intEnd = fBinaryData.find_first_of("\n\r\t ,", intStart);
		if (intEnd == string::npos) {
			intEnd = fBinaryData.size();
		}
		int value = 0;
		if (sscanf(fBinaryData.substr(intStart, intEnd - intStart).c_str(), "%i", &value) == 1) {
			fInts.push_back(value);
			printf("-> %i\n",  value);
		} else {
			return false;
		}
		pos = intEnd;
	}
	return true;
}

void DataResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	// The data type is .bin, no need to write it.
	ResourceDirective::writeWordDirective(output, fType, &fInts[0], 0, fInts.size());
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
	writeResourceTypeDirective(output);
	ResourceDirective::writeDirectiveChildren(output, asVariant);
}

void FileResourceDirective::writeResourceTypeDirective(ostringstream& output) {
	string resourceStr = string("\"") + fResource + string("\"");
	if (!fParent) {
		output << "." << fResType;
	}
	if (fUseIncludeDirective && !fResource.empty()) {
		output << ".include " << resourceStr;
	} else {
		if (!fResource.empty()) {
			output << " " << resourceStr;
		}
	}
	output << "\n";
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
	return fResource + "|" + fFile;
}

void MediaResourceDirective::writeResourceTypeDirective(ostringstream& output) {
	if (!fUseIncludeDirective) {
		output << ".media \"" << getMimeType() << "\",\"" << fResource << "\"\n";
	} else {
		FileResourceDirective::writeResourceTypeDirective(output);
	}
}

string MediaResourceDirective::getMimeType() {
	if (!fMimeType.empty()) {
		return fMimeType;
	}
	const char* ext = getExtension(fResource).c_str();
	if (equalsIgnoreCase(ext, "mp4") || equalsIgnoreCase(ext, "mp3") || equalsIgnoreCase(ext, "m4a")) {
		return "audio/mpeg";
	} else if (equalsIgnoreCase(ext, "wav")) {
		return "audio/x-wav";
	}
	return string();
}

string MediaResourceDirective::validate() {
	return FileResourceDirective::validate();
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

string StringResourceDirective::validate() {
	if (fType == PSTRING && (int) fStringValue.length() > 255) {
		return "pstrings can be no longer than 255 bytes";
	}
	return ResourceDirective::validate();
}

void StringResourceDirective::writeDirectives(ostringstream& output, bool asVariant) {
	ResourceDirective::writeDirectives(output, asVariant);
	if (!fParent) {
		output << ".bin\n";
	}
	size_t currentPos = 0;
	size_t currentOffset = 0;
	bool isInSpecialString = false;
	bool wasInSpecialString = false;
	bool final = false;
	size_t len = fStringValue.length();
	const char* chArray = fStringValue.c_str();
	if (fType == PSTRING) {
		char lenByte = len & 0xff;
		ResourceDirective::writeByteDirective(output, &lenByte, 0, 1);
	}
	while (!final) {
		char ch = chArray[currentPos];
		isInSpecialString = isSpecialChar(ch);
		if (final || (isInSpecialString ^ wasInSpecialString)) {
			size_t lenToWrite = currentPos - currentOffset;
			if (isInSpecialString) {
				writeDirectiveAsString(output, currentOffset, lenToWrite);
			} else {
				ResourceDirective::writeByteDirective(output, chArray, currentOffset, lenToWrite);
			}
			currentOffset = currentPos;
		}
		wasInSpecialString = isInSpecialString;
		if (!final) {
			currentPos++;
		}
		final = len < currentPos;
	}
	if (fType == CSTRING) {
		char zeroByte = 0x00;
		ResourceDirective::writeByteDirective(output, &zeroByte, 0, 1);
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

string AudioResourceDirective::validate() {
	string mimeType = getMimeType();
	if (mimeType.length() < 5 || mimeType.substr(0, 5) != "audio") {
		return "(" + fResource + ") Audio tags require a mime type that starts with \'audio\', or the audio file must be .mp3, .mp4, .m4a or .wav.";
	}
	return MediaResourceDirective::validate();
}

const char* findAttr(const char* name, const char** attributes) {
	  for (int i = 0; attributes[i]; i += 2) {
		  if (!strcmp(attributes[i], name)) {
			  return attributes[i + 1];
		  }
	  }
	  return NULL;
}
