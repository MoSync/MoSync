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

#include <ma.h>
#include <mastring.h>
#include <mastdlib.h>
#include <maassert.h>
#include <mavsprintf.h>
#include "String.h"

namespace MAUtil {

#if defined(MAUTIL_VECTOR_DEBUGGING)
	int nV = 0;
#endif

	StringData::StringData(const char* text, int len) : Vector<char>(len + 1), RefCounted(1) {
		resize(len);
		mData[len] = '\0';
		memcpy(mData, text, len);
	}


	StringData::StringData(const char* text) : Vector<char>(strlen(text) + 1), RefCounted(1) {
		int len = capacity() - 1;
		resize(len);
		memcpy(mData, text, capacity());
	}


	StringData::StringData(int aSize) : Vector<char>(aSize+1), RefCounted(1) {
		resize(aSize);
		mData[aSize] = '\0';
	}



	StringData::StringData(const StringData& other) : Vector<char>(other.mSize+1), RefCounted(1) {
		mSize = other.mSize;
		memcpy(mData, other.mData, mSize+1);
	}

#ifdef HAVE_EMPTY_STRING
	const String* String::EMPTY_STRING = NULL;
#endif

	static StringData* getEmptyData() {
		static StringData* ed = NULL;
		if(ed == NULL) {
			ed = new StringData("");
			MAASSERT(ed);
		}
		ed->addRef();
		return ed;
	}

	String::String() {
		sd = getEmptyData();
	}

	String::String(int aCapacity) {
		sd = new StringData(aCapacity);
		resize(0);
	}

/*
	String::String(const char* text) {
		if(text == NULL) {
			sd = getEmptyData();
		} else if(*text == 0)	{	//empty string
			sd = getEmptyData();
		} else {
			sd = new StringData(text);
			MAASSERT(sd);
		}
	}
*/
	void String::allocStringData(const char *text, int len) {
		if(len < 0) {
			maPanic(0, "String(const char* text, int len), passed a negative length.");
		}

		if(text == NULL || *text == 0 || len == 0) {
			sd = getEmptyData();
		} else {
			sd = new StringData(text, len);
			MAASSERT(sd);
		}		
	}

	String::String(const char* text, int len) {
		if(text == NULL) maPanic(0, "String(const char* text, int len), passed a NULL pointer.");
		allocStringData(text, len);
	}

	String::String(const char* text) {
		if(text == NULL) maPanic(0, "String(const char* text), passed a NULL pointer.");
		allocStringData(text, strlen(text));
	}

	String::String(const String& s) {
		sd = s.sd;
		sd->addRef();
	}

	const char* String::c_str() const {
		return (const char*) sd->mData;
	}

	String& String::operator=(const String& s) {
		sd->release();
		sd = s.sd;
		sd->addRef();
		return *this;
	}

	bool String::operator==(const String& other) const {
		if(this->size() != other.size())
			return false;

		if(this->sd == other.sd)
			return true;

		return strcmp(this->sd->mData, other.sd->mData)==0;
	}

	bool String::operator!=(const String& other) const {
		return !((*this)==other);
	}


	bool String::operator<(const String& other) const {
		return strcmp(c_str(), other.c_str()) < 0;
	}


	bool String::operator>(const String& other) const {
		return strcmp(c_str(), other.c_str()) > 0;
	}


	bool String::operator<=(const String& other) const {
		return strcmp(c_str(), other.c_str()) <= 0;
	}


	bool String::operator>=(const String& other) const {
		return strcmp(c_str(), other.c_str()) >= 0;
	}


	char& String::operator[](int index) {
		//if memory is shared, do copy on write

		if(sd->getRefCount() > 1) {
			StringData* newSd = new StringData(*sd);
			MAASSERT(newSd);
			sd->release();
			sd = newSd;
		}

		return sd->mData[index];
	}

#ifndef NEW_OPERATORS
	String String::operator+(const String& other) const {
#if 0
		String ret = *this;
		//MAASSERT(sd->getRefCount() > 1);

		StringData* newSd = new StringData(&((*sd)[0]));
		MAASSERT(newSd);
		sd->release();
		ret.sd = newSd;
		for(int i = 0; i < other.size(); i++) {
			ret.sd->add(other[i]);
		}
		ret.sd->reserve(ret.size()+1);
		ret[ret.size()] = '\0';
		return ret;
#else
		String s = *this;
		s += other;
		return s;
#endif
	}


	void String::append(const char* other, int len) {
		//order of operations is important here.
		int oldSize = size();
//		int otherSize = other.size();
		int otherSize = len;
		int newSize = oldSize + otherSize;
		if(sd->getRefCount() > 1) {
			StringData* newSd = new StringData(newSize);
			memcpy(newSd->pointer(), sd->pointer(), oldSize);
			sd->release();
			sd = newSd;
		} else {
			sd->reserve(newSize + 1);
		}
		memcpy(sd->pointer() + oldSize, other, otherSize);
		pointer()[newSize] = 0;
		sd->resize(newSize);		
	}

	String& String::operator+=(const String& other) {
		append(other.c_str(), other.length());
		return *this;
		/*
#if 1
		//order of operations is important here.
		int oldSize = size();
		int otherSize = other.size();
		int newSize = oldSize + otherSize;
		if(sd->getRefCount() > 1) {
			StringData* newSd = new StringData(newSize);
			memcpy(newSd->pointer(), sd->pointer(), oldSize);
			sd->release();
			sd = newSd;
		} else {
			sd->reserve(newSize + 1);
		}
		memcpy(sd->pointer() + oldSize, other.sd->pointer(), otherSize + 1);
		sd->resize(newSize);
		return *this;
#else
		*this = *this + other;
		return *this;
#endif
		*/
	}

#if 1
	String String::operator+(char c) const {
		String s = *this;
		s += c;
		return s;
	}

	//TODO: cleanup
	String& String::operator+=(char c) {
		append(&c, 1); // to avoid code duplication.... 
		return *this;
		/*
		//order of operations is important here.
		int oldSize = size();
		int otherSize = 1;
		int newSize = oldSize + otherSize;
		if(sd->getRefCount() > 1) {
			StringData* newSd = new StringData(newSize);
			memcpy(newSd->pointer(), sd->pointer(), oldSize);
			sd->release();
			sd = newSd;
		} else {
			sd->reserve(newSize + 1);
		}
		sd->pointer()[oldSize] = c;
		sd->pointer()[oldSize + 1] = 0;
		sd->resize(newSize);
		return *this;
		*/
	}
#endif
#endif	//NEW_OPERATORS

	int String::find(const String& s, unsigned int offset) const {
		if (s.length()+offset <= (unsigned int)sd->size()) {
			if (!s.length())
				return ((int) offset);	// Empty string is always found
			const char *str = (const char*) sd->mData + offset;
			const char *search = s.c_str();
			const char *end = sd->mData + sd->size() - s.length() + 1;
			const char *search_end = s.c_str() + s.length();
skipp:
			while (str != end) {
				if (*str++ == *search) {
					register char *i,*j;
					i=(char*) str;
					j=(char*) search+1;
					while (j != search_end)
						if (*i++ != *j++) goto skipp;
					return (int) (str - sd->mData) - 1;
				}
			}
		}
		return npos;
	}

	int String::findLastOf(const char findThis) const {
		for(int i = this->length(); i >= 0; i--) {
			if((*this)[i] == findThis) return i;
		}
		return npos;
	}

	int String::findFirstOf(const char findThis, int position) const {
		for(int i = position; i < this->length(); i++) {
			if((*this)[i] == findThis) return i;
		}
		return npos;
	}

	int String::findFirstNotOf(const char findNotThis, int position) const {
		for(int i = position; i < this->length(); i++) {
			if((*this)[i] != findNotThis) return i;
		}
		return npos;
	}

	void String::insert(int position, const String& other) {
		int otherSize = other.size();
		int newSize = this->size();
		newSize += otherSize;
		this->resize(newSize);
		int endpos = position+otherSize;
		for(int i = newSize-1; i >= endpos; i--) {
			(*sd)[i] = (*sd)[i-otherSize];
		}
		for(int i = 0; i < otherSize; i++) {
			(*sd)[position++] = other[i];
		}

		//String begining = this->substr(0, position);
		//String ending = this->substr(position, this->length() - position);
		//*this = begining + other + ending;
	}

	//TODO: cleanup
	void String::insert(int position, char c) {
		int otherSize = 1;
		int newSize = this->size();
		newSize += otherSize;
		this->resize(newSize);
		int endpos = position+otherSize;
		for(int i = newSize-1; i >= endpos; i--) {
			(*sd)[i] = (*sd)[i-otherSize];
		}
		for(int i = 0; i < otherSize; i++) {
			(*sd)[position++] = c;
		}
	}

	void String::remove(int position, int number) {
		String beginning = this->substr(0, position);
		String ending = this->substr(position+number, this->length() - position - number);
		*this = beginning + ending;
	}

	String String::substr(int startIndex, int len) const {
		ASSERT_MSG(startIndex >= 0 && startIndex <= this->length(), "invalid index");
		if(len == npos)
			len = this->length() - startIndex;
		ASSERT_MSG(len >= 0 && (startIndex+len) <= this->length(), "invalid length");

		String retString;
#if 0
		for(int i = startIndex; i < startIndex + len; i++) {
			retString += (*this)[i];
		}
#else
		if(len > 0) {
			retString.sd->release();
			retString.sd = new StringData(len);
			memcpy(retString.sd->pointer(), sd->pointer() + startIndex, len);
			retString[len] = 0;
		}
#endif
		return retString;
	}


	const char& String::operator[](int index) const {
		return sd->mData[index];
	}

	int String::size() const {
		return sd->size();
	}

	int String::length() const {
		return sd->size();
	}

	int String::capacity() const {
		return sd->capacity() - 1;
	}

	String::~String() {
		sd->release();
	}

	void String::resize(int newSize) {
		reserve(newSize);
		sd->resize(newSize);
		(*sd)[newSize] = 0;
	}

	void String::reserve(int newSize) {
		if(sd->getRefCount() > 1) {
			if(newSize<sd->capacity()) newSize=sd->capacity();
			StringData* temp = new StringData(newSize);
			temp->resize(sd->size());
			memcpy(temp->pointer(), sd->pointer(), sd->size() + 1);
			//memcpy(temp->pointer(), sd->pointer(), sd->size());
			sd->release();
			sd = temp;
		} else {
			sd->reserve(newSize+1);
			(*sd)[sd->size()] = 0;

		}
	}

	void String::clear() {
		if(sd) {
			sd->release();
		}
		sd = getEmptyData();
	}

#ifdef HAVE_EMPTY_STRING
	const String& String::emptyString() {
		return *EMPTY_STRING;
	}
#endif

	void String::setData(StringData* data) {
		sd->release();
		sd = data;
	}

	char* String::pointer() {
		return sd->pointer();
	}

//	int String::hash(const String* self, int /*size*/) {
//		return memHash(self->sd->pointer(), self->size());
//	}

}
