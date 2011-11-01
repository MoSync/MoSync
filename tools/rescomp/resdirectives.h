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

enum LoadType { startup, unloaded, explicitLoad };

using namespace std;

const char* findAttr(const char* name, const char** attributes);

class ResourceDirective {
protected:
	string fResType;
	string fId;
public:
	ResourceDirective() { }
	ResourceDirective(const char* resType) : fResType(string(resType)) { }
	void setId(string id);
	string getId();
	virtual void writeDirectives(ostringstream& output, bool asVariant);
	virtual void initDirectiveFromAttributes(const char **attributes);
	virtual void initDirectiveFromCData(const char* cdata, int length);
	virtual string getUniqueToken();
	virtual string validate();
	static void writeByteDirective(ostringstream& output, const char* array, size_t offset, size_t len);
};

class FileResourceDirective : public ResourceDirective {
protected:
	string fResource;
	LoadType fLoadType;
public:
	FileResourceDirective(const char* resType) : ResourceDirective(resType) { }
	void setResource(string resource);
	void setLoadType(LoadType loadType);
	virtual string getSpecialDirective();
	string getUniqueToken();
	virtual void writeDirectives(ostringstream& output, bool asVariant);
	virtual void initDirectiveFromAttributes(const char **attributes);
	virtual string validate();
};

class BinaryResourceDirective : public FileResourceDirective {
public:
	BinaryResourceDirective() : FileResourceDirective("bin") { }
};

class ImageResourceDirective : public FileResourceDirective {
public:
	ImageResourceDirective() : FileResourceDirective("image") { }
};

class MediaResourceDirective : public FileResourceDirective {
private:
	string fMimeType;
public:
	MediaResourceDirective() : FileResourceDirective("media") { }
	void setMimeType(string mimeType);
	void writeDirectives(ostringstream& output, bool asVariant);
	virtual void initDirectiveFromAttributes(const char **attributes);
};

class AudioResourceDirective : public MediaResourceDirective {
public:
	AudioResourceDirective() : MediaResourceDirective() { }
};

class StringResourceDirective : public ResourceDirective {
private:
	string fStringValue;
	void writeDirectiveAsString(ostringstream& output, size_t offset, size_t len);
public:
	StringResourceDirective() : ResourceDirective("string") { }
	void writeDirectives(ostringstream& output, bool asVariant);
	void initDirectiveFromCData(const char* cdata, int length);
	string getUniqueToken();
};

class PlaceholderDirective : public ResourceDirective {
public:
	PlaceholderDirective() : ResourceDirective("placeholder") { }
	void writeDirectives(ostringstream& output, bool asVariant);
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
