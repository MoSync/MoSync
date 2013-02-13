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

#ifndef RESDIRECTIVES_H_
#define RESDIRECTIVES_H_

#include <sstream>
#include <string>
#include <vector>

enum LoadType { LoadType_Startup, LoadType_Unloaded };
enum DataType { Byte, Half, Word };

// Copied from compile.h
enum
{
	ResType_null = 0,
	ResType_PlaceHolder = 1,
	ResType_Image = 2,
	ResType_Sprite = 3,
	ResType_Binary = 4,
	ResType_UBinary = 5,
	ResType_Skip = 6,
	ResType_TileSet = 7,
	ResType_TileMap = 8,
	ResType_Label = 9,
//	ResType_Media = 10,
//	ResType_UMedia = 11
};

using namespace std;

const char* findAttr(const char* name, const char** attributes);

class ResourceDirective {
protected:
	string fResType;
	string fId;
	LoadType fLoadType;
	string fFile;
	int fLineNo;
	ResourceDirective* fParent;
	vector<ResourceDirective*> fChildren;
	bool fCanHaveParent;
	bool fCanHaveChildren;
	string fErrorMessage;
public:
	ResourceDirective() : fParent(NULL) { }
	ResourceDirective(const char* resType, bool canHaveParent, bool canHaveChildren) :
		fResType(string(resType)), fParent(NULL), fCanHaveParent(canHaveParent), fCanHaveChildren(canHaveChildren) { }
	virtual ~ResourceDirective() {}
	void setParent(ResourceDirective* parent);
	void setId(string id);
	string getId();
	void setLoadType(LoadType loadType);
	LoadType getLoadType();
	virtual int getResourceTypeAsInt() { return ResType_Binary; }
	virtual void writeDirectives(ostringstream& output, bool asVariant);
	virtual void writeDirectiveIdentifier(ostringstream& output, bool asVariant);
	virtual void writeDirectiveBody(ostringstream& output, bool asVariant);
	virtual void writeDirectiveChildren(ostringstream& output, bool asVariant);
	virtual void initDirectiveFromAttributes(const char **attributes);
	virtual void initDirectiveFromCData(const char* cdata, int length);
	virtual string getUniqueToken();
	virtual string validate();
	void setFile(string file);
	string getFile();
	void setLineNo(int lineNo);
	int getLineNo();
	static void writeByteDirective(ostringstream& output, const char* array, size_t offset, size_t len);
	static void writeWordDirective(ostringstream& output, DataType type, const int* array, size_t offset, size_t len);
};

static string gLastLFileDirective;

class FileResourceDirective : public ResourceDirective {
protected:
	string fResource;
	bool fUseIncludeDirective;
public:
	FileResourceDirective(const char* resType, bool useIncludeDirective)
		: ResourceDirective(resType, false, false) { fUseIncludeDirective = useIncludeDirective; }
	FileResourceDirective(const char* resType, bool useIncludeDirective, bool canHaveParent, bool canHaveChildren)
		: ResourceDirective(resType, canHaveParent, canHaveParent) { fUseIncludeDirective = useIncludeDirective; }
	void setResource(string resource);
	string getUniqueToken();
	virtual void writeDirectives(ostringstream& output, bool asVariant);
	virtual void writeResourceTypeDirective(ostringstream& output);
	virtual void initDirectiveFromAttributes(const char **attributes);
	virtual string validate();
};

class BinaryResourceDirective : public FileResourceDirective {
public:
	BinaryResourceDirective() : FileResourceDirective("bin", true, true, true) { }
	int getResourceTypeAsInt() { return ResType_Binary; }
	string validate();
};

class DataResourceDirective : public ResourceDirective {
private:
	DataType fType;
	vector<int> fInts;
	string fBinaryData;
	bool parseInts();
public:
	DataResourceDirective(DataType type) : ResourceDirective("", true, false), fType(type) { }
	virtual void writeDirectives(ostringstream& output, bool asVariant);
	void initDirectiveFromCData(const char* cdata, int length);
};

class ImageResourceDirective : public FileResourceDirective {
public:
	ImageResourceDirective() : FileResourceDirective("image", false) { }
	int getResourceTypeAsInt() { return ResType_Image; }
};

class MediaResourceDirective : public FileResourceDirective {
protected:
	string fMimeType;
public:
	MediaResourceDirective() : FileResourceDirective("media", false) { }
	void setMimeType(string mimeType);
	virtual string getMimeType();
	void writeResourceTypeDirective(ostringstream& output);
	virtual void initDirectiveFromAttributes(const char **attributes);
	virtual string validate();
};

class AudioResourceDirective : public MediaResourceDirective {
public:
	AudioResourceDirective() : MediaResourceDirective() { }
	virtual string validate();
};

class StringResourceDirective : public ResourceDirective {
public:
	enum StringType { STRING, PSTRING, CSTRING } ;
	StringResourceDirective(StringType type) : ResourceDirective("string", true, false), fType(type) { }
	void writeDirectives(ostringstream& output, bool asVariant);
	void initDirectiveFromCData(const char* cdata, int length);
	string getUniqueToken();
	string validate();
private:
	StringType fType;
	string fStringValue;
	void writeDirectiveAsString(ostringstream& output, size_t offset, size_t len);
};

class PlaceholderDirective : public ResourceDirective {
public:
	PlaceholderDirective() : ResourceDirective("placeholder", false, false) { }
	void writeDirectives(ostringstream& output, bool asVariant);
	int getResourceType() { return ResType_PlaceHolder; }
};

/*class LSTXResourceDirectiveFactory {
private:
	string fId;
public:
	LSTXResourceDirectiveFactory(const char* id) : fId(string(id)) { }
	virtual ResourceDirective* newDirective();
	virtual void startTag(ResourceDirective& directive, ParserState& state, const char *tagName, const char **attributes);
	virtual void text(ResourceDirective& directive, ParserState& state, const char* text);
};*/

#endif /* RESDIRECTIVES_H_ */
