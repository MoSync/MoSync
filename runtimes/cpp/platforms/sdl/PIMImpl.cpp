/* Copyright (C) 2010 Mobile Sorcery AB

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

#include "config_platform.h"

#ifdef EMULATOR

#include "Syscall.h"
#include "pim.h"
#ifdef WIN32
#include "strptime.h"
#endif
#include "base_errors.h"
#include "sdl_syscall.h"
#include "helpers/CPP_IX_PIM.h"
#include "helpers/smartie.h"
#include "ConfigParser.h"
#include <expat.h>
#include <limits.h>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>

using namespace MoSyncError;
using namespace Base;
using namespace std;

//******************************************************************************
// Config
//******************************************************************************
#define DO_DUMP 0

#if DO_DUMP
#define DUMPLOG LOG
#else
#define DUMPLOG(...)
#endif

//******************************************************************************
// Forwards
//******************************************************************************

struct ContactValue;

// returns 0 on success, error code otherwise.
static int createContactValue(ContactValue*& v, int field, void* buf, int bufSize,
	int attributes);

//******************************************************************************
// Contacts
//******************************************************************************

namespace ContactParser {
	struct FIELD_NAME {
		int id;
		const char* name;
	};

	static void start(void *data, const char *el, const char **attr);
	static void end(void *data, const char *el);
	static void ATTRIBUTE(noreturn, error(const char* msg));
}

struct ContactValue {
	const int type;
	int attr;

	virtual ~ContactValue() {}
	ContactValue(int t) : type(t), attr(0) {}
	virtual int getValue(void* buf, size_t bufSize) const = 0;
	virtual void setValue(void* buf, size_t bufSize) = 0;
	virtual void saveValue(ostream& stream) const = 0;
	void save(ostream& stream) const;
};

template<class T> struct TContactValue : ContactValue {
	TContactValue(int t) : ContactValue(t) {}
	T mValue;

	int getValue(void* buf, size_t bufSize) const;
	void setValue(void* buf, size_t bufSize);
	void saveValue(ostream& stream) const;
};

struct StringArray {
	const ContactParser::FIELD_NAME* fnp;
	// sa.size() is also the number of entries in array pointed to by fnp.
	vector<wstring> sa;
};


template<>
int TContactValue<int>::getValue(void* buf, size_t bufSize) const {
	if(bufSize < sizeof(int))
		return sizeof(int);
	*(int*)buf = mValue;
	return sizeof(int);
}

template<>
void TContactValue<int>::setValue(void* buf, size_t bufSize) {
	MYASSERT(bufSize = sizeof(int), ERR_INVALID_PIM_BUFFER_SIZE);
	mValue = *(int*)buf;
}

static void streamDateTime(ostream& stream, int time) {
	char buf[64];
	time_t t = time;
	strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", localtime(&t));
	stream << buf;
}

template<>
void TContactValue<int>::saveValue(ostream& stream) const {
	stream << " value=\"";
	switch(type) {
	case MA_PIM_TYPE_INT:
		stream << mValue;
		break;
	case MA_PIM_TYPE_DATE:
		streamDateTime(stream, mValue);
		break;
	default:
		DEBIG_PHAT_ERROR;
	}
	
	stream <<"\"";
}

// no bounds check. converts any wchar_t to 16-bit wchar, which MoSync uses.
static void wstringCpy(wchar* dst, const wstring& s) {
	for(size_t i=0; i<s.length(); i++) {
		*dst = (wchar)s[i];
		dst++;
	}
	*dst = 0;
}

template<>
int TContactValue<wstring>::getValue(void* buf, size_t bufSize) const {
	size_t size = (mValue.length() + 1) * sizeof(wchar);
	if(bufSize < size)
		return size;
	wstringCpy((wchar*)buf, mValue);
	return size;
}

template<>
void TContactValue<wstring>::setValue(void* buf, size_t bufSize) {
	MYASSERT(bufSize >= sizeof(wchar), ERR_INVALID_PIM_BUFFER_SIZE);
	MYASSERT((bufSize % sizeof(wchar)) == 0, ERR_INVALID_PIM_BUFFER_SIZE);
	size_t length = bufSize / sizeof(wchar);
	mValue.clear();
	wchar* src = (wchar*)buf;
	size_t i;
	for(i=0; i<length && src[i] != 0; i++) {
		mValue += src[i];
	}
	MYASSERT(src[i] == 0, ERR_PIM_UNTERMINATED_STRING);
}

// converts wide-char to utf-8 on the fly.
static ostream& operator<<(ostream& stream, const wstring& src) {
	for(size_t i=0; i<src.size(); i++) {
		wchar_t w = src[i];
		if(w <= 0x7f) {
			stream << (char)w;
		} else if(w <= 0x7ff) {
			stream << (0xc0 | ((w >> 6) & 0x1f));
			stream << (0x80 | (w & 0x3f));
		} else
#if WCHAR_MAX > 0xffff
			if(w <= 0xffff)
#endif
		{
			stream << (0xe0 | ((w >> 12) & 0x0f));
			stream << (0x80 | ((w >> 6) & 0x3f));
			stream << (0x80 | (w & 0x3f));
#if WCHAR_MAX > 0xffff
		} else if(w <= 0x10ffff) {
			stream << (0xf0 | ((w >> 18) & 0x07));
			stream << (0x80 | ((w >> 12) & 0x3f));
			stream << (0x80 | ((w >> 6) & 0x3f));
			stream << (0x80 | (w & 0x3f));
		} else {
			DEBIG_PHAT_ERROR;
#endif
		}
	}
	return stream;
}

template<>
void TContactValue<wstring>::saveValue(ostream& stream) const {
	stream << " value=\""<<mValue<<"\"";
}

template<>
int TContactValue<StringArray>::getValue(void* buf, size_t bufSize) const {
	const vector<wstring>& sa(mValue.sa);
	size_t size = sizeof(int);
	for(size_t i=0; i<sa.size(); i++) {
		size += (sa[i].length() + 1) * sizeof(wchar);
	}
	if(bufSize < size)
		return (int)size;
	*(int*)buf = sa.size();
	wchar* dst = (wchar*)((byte*)buf + sizeof(int));
	memset(dst, -1, bufSize-4);	//temp
	for(size_t i=0; i<sa.size(); i++) {
		wstringCpy(dst, sa[i]);
		dst += sa[i].length() + 1;
	}
	return size;
}

template<>
void TContactValue<StringArray>::setValue(void* buf, size_t bufSize) {
	MYASSERT(bufSize > sizeof(int), ERR_INVALID_PIM_BUFFER_SIZE);
	int nStrings = *(int*)buf;
	MYASSERT((bufSize % sizeof(wchar)) == 0, ERR_INVALID_PIM_BUFFER_SIZE);
	int nChars = (bufSize - sizeof(int)) / sizeof(wchar);
	MYASSERT(nChars != nStrings, ERR_PIM_EMPTY_STRING_ARRAY);
	MYASSERT(nChars > nStrings, ERR_INVALID_PIM_BUFFER_SIZE);
	wchar* src = (wchar*)((int*)buf + 1);
	wchar* end = src + nChars;
	for(size_t i=0; i<mValue.sa.size(); i++) {
		mValue.sa[i].clear();
		while(true) {
			MYASSERT(src != end, ERR_PIM_UNTERMINATED_STRING);
			wchar c = *src;
			src++;
			if(c == 0)
				break;
			mValue.sa[i] += c;
		}
	}
}

template<>
void TContactValue<StringArray>::saveValue(ostream& stream) const {
	const vector<wstring>& sa(mValue.sa);
	for(size_t i=0; i<sa.size(); i++) {
		if(!sa[i].empty())
			stream << " "<<mValue.fnp[i].name<<"=\""<<sa[i]<<"\"";
	}
}

class ContactItem : public PimItem {
public:
	int count() const {
		return mFields.size();
	}
	int fieldId(int index) const {
		MYASSERT(index >= 0 && index < (int)mFields.size(), ERR_INVALID_PIM_FIELD_INDEX);
		FieldMap::const_iterator itr = mFields.begin();
		for(int i=0; i<index; i++) {
			itr++;
		}
		return itr->first;
	}
	int fieldCount(int field) const {
		FieldMap::const_iterator itr = mFields.find(field);
		if(itr == mFields.end())
			return 0;
		return itr->second.size();
	}

	const vector<ContactValue*>& getVCV(int field) const {
		FieldMap::const_iterator itr = mFields.find(field);
		MYASSERT(itr != mFields.end(), ERR_MISSING_PIM_FIELD);
		return itr->second;
	}

	const ContactValue* getContactValue(int field, int index) const {
		const vector<ContactValue*>& vcv(getVCV(field));
		MYASSERT(index >= 0 && index < (int)vcv.size(), ERR_INVALID_PIM_VALUE_INDEX);
		return vcv[index];
	}

	int attr(int field, int index) const {
		return getContactValue(field, index)->attr;
	}
	int getValue(int field, int index, void* buf, int bufSize) const {
		return getContactValue(field, index)->getValue(buf, bufSize);
	}

	void setValue(int field, int index, void* buf, int bufSize, int attributes) {
		ContactValue* cv = (ContactValue*)getContactValue(field, index);
		cv->setValue(buf, bufSize);
		cv->attr = attributes;
	}
	int addValue(int field, void* buf, int bufSize, int attributes) {
		ContactValue* v;
		int res = createContactValue(v, field, buf, bufSize, attributes);
		if(res < 0)
			return res;
		addValue(field, v);
		return 0;
	}
	void addValue(int field, ContactValue* v) {
		ContactItem::FieldMap::iterator itr = mFields.find(field);
		if(itr == mFields.end()) {
			// add field if item didn't have it already.
			pair<int, vector<ContactValue*> > p;
			p.first = field;
			itr = mFields.insert(p).first;
		}
		vector<ContactValue*>& vcv(itr->second);
		vcv.push_back(v);
	}
	void removeValue(int field, int index) {
		FieldMap::iterator itr = mFields.find(field);
		MYASSERT(itr != mFields.end(), ERR_MISSING_PIM_FIELD);
		vector<ContactValue*>& vcv(itr->second);
		MYASSERT(index >= 0 && index < (int)vcv.size(), ERR_INVALID_PIM_VALUE_INDEX);
		delete vcv[index];
		vcv.erase(vcv.begin() + index);
		if(vcv.empty()) {
			// remove the now-empty field.
			mFields.erase(itr);
		}
	}

	ContactItem(MAHandle pl) : PimItem(pl) {}

	virtual ~ContactItem() {
		FieldMap::iterator itr = mFields.begin();
		while(itr != mFields.end()) {
			vector<ContactValue*>& vcv(itr->second);
			for(size_t i=0; i<vcv.size(); i++) {
				delete vcv[i];
			}
			itr++;
		}
		mFields.clear();
	}

	void save(ostream& stream) const;

	// todo: change to unordered_map when it becomes available across platforms.
	// key: fieldId
	typedef map<int, vector<ContactValue*> > FieldMap;
	FieldMap mFields;
};

class ContactList : public PimList {
private:
	vector<ContactItem*> mItems;
	size_t mPos;
public:
	ContactList() {}
	bool open(const string& fn) {
		try {
			XML_Parser xmlParser = XML_ParserCreate("UTF-8");
			XML_SetUserData(xmlParser, this);
			XML_SetElementHandler(xmlParser, ContactParser::start, ContactParser::end);
			int fileLength;
			char* contents = readFileIntoMem(fn.c_str(), &fileLength);
			if(contents == NULL)
				ContactParser::error("file read fail");
			XML_Parse(xmlParser, contents, fileLength, 1);
			delete contents;
		} catch (exception& e) {
			LOG("ContactParser exception: %s\n", e.what());
			return false;
		}
		mPos = 0;
		return true;
	}
	virtual ~ContactList() {
		for(size_t i=0; i<mItems.size(); i++) {
			delete mItems[i];
		}
	}
	bool save(const string& fn) {
		ofstream stream(fn.c_str());
		stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		stream << "<contacts>\n";
		for(size_t i=0; i<mItems.size(); i++) {
			mItems[i]->save(stream);
		}
		stream << "</contacts>\n";
		return stream.good();
	}
	PimItem* next() {
		if(mPos >= mItems.size())
			return NULL;
		return mItems[mPos++];
	}
	int type(int field) const {
		return pimContactFieldType(field);
	}
	PimItem* createItem(MAHandle list) {
		return new ContactItem(list);
	}
	void removeItem(PimItem* pi) {
		for(size_t i=0; i<mItems.size(); i++) {
			if(mItems[i] == pi) {
				mItems.erase(mItems.begin() + i);
				break;
			}
		}
	}

	void add(ContactItem* ci) {
		mItems.push_back(ci);
	}
};

namespace ContactParser {
	static ContactList* sCL = NULL;
	static ContactItem* sCI = NULL;

	static const FIELD_NAME sAddrFields[] = {
		{ MA_PIM_CONTACT_ADDR_POBOX, "pobox" },
		{ MA_PIM_CONTACT_ADDR_EXTRA, "extra" },
		{ MA_PIM_CONTACT_ADDR_STREET, "street" },
		{ MA_PIM_CONTACT_ADDR_LOCALITY, "locality" },
		{ MA_PIM_CONTACT_ADDR_REGION, "region" },
		{ MA_PIM_CONTACT_ADDR_POSTALCODE, "postalcode" },
		{ MA_PIM_CONTACT_ADDR_COUNTRY, "country" },
	};
	static const int snAddrFields = sizeof(sAddrFields) / sizeof(FIELD_NAME);

	static const FIELD_NAME sNameFields[] = {
		{ MA_PIM_CONTACT_NAME_FAMILY, "family" },
		{ MA_PIM_CONTACT_NAME_GIVEN, "given" },
		{ MA_PIM_CONTACT_NAME_OTHER, "other" },
		{ MA_PIM_CONTACT_NAME_PREFIX, "prefix" },
		{ MA_PIM_CONTACT_NAME_SUFFIX, "suffix" },
	};
	static const int snNameFields = sizeof(sNameFields) / sizeof(FIELD_NAME);

	// We can't handle binaries yet. Class is also useless at this point.
	static const FIELD_NAME sFieldNames[] = {
		{ MA_PIM_FIELD_CONTACT_ADDR, "addr" },
		{ MA_PIM_FIELD_CONTACT_BIRTHDAY, "birthday" },
		//{ MA_PIM_FIELD_CONTACT_CLASS, "class" },
		{ MA_PIM_FIELD_CONTACT_EMAIL, "email" },
		{ MA_PIM_FIELD_CONTACT_FORMATTED_ADDR, "formatted_addr" },
		{ MA_PIM_FIELD_CONTACT_FORMATTED_NAME, "formatted_name" },
		{ MA_PIM_FIELD_CONTACT_NAME, "name" },
		{ MA_PIM_FIELD_CONTACT_NICKNAME, "nickname" },
		{ MA_PIM_FIELD_CONTACT_NOTE, "note" },
		{ MA_PIM_FIELD_CONTACT_ORG, "org" },
		//{ MA_PIM_FIELD_CONTACT_PHOTO, "photo" },
		{ MA_PIM_FIELD_CONTACT_PHOTO_URL, "photo_url" },
		//{ MA_PIM_FIELD_CONTACT_PUBLIC_KEY, "public_key" },
		{ MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING, "public_key_string" },
		{ MA_PIM_FIELD_CONTACT_REVISION, "revision" },
		{ MA_PIM_FIELD_CONTACT_TEL, "tel" },
		{ MA_PIM_FIELD_CONTACT_TITLE, "title" },
		{ MA_PIM_FIELD_CONTACT_UID, "uid" },
		{ MA_PIM_FIELD_CONTACT_URL, "url" },
	};
	static const int snFieldNames = sizeof(sFieldNames) / sizeof(FIELD_NAME);

	static const FIELD_NAME sAttributes[] = {
		{ MA_PIM_ATTR_ASST, "assistant" },
		{ MA_PIM_ATTR_AUTO, "auto" },
		{ MA_PIM_ATTR_FAX, "fax" },
		{ MA_PIM_ATTR_HOME, "home" },
		{ MA_PIM_ATTR_MOBILE, "mobile" },
		{ MA_PIM_ATTR_OTHER, "other" },
		{ MA_PIM_ATTR_PAGER, "pager" },
		{ MA_PIM_ATTR_PREFERRED, "preferred" },
		{ MA_PIM_ATTR_SMS, "sms" },
		{ MA_PIM_ATTR_WORK, "work" },
	};
	static const int snAttributes = sizeof(sAttributes) / sizeof(FIELD_NAME);

	static bool streq(const char* a, const char* b) {
		return strcmp(a, b) == 0;
	}

	static void error(const char* msg) {
		throw logic_error(msg);
	}

	static void parseAttr(bool& attrSet, int& attr, const char* value) {
		if(attrSet)
			error("duplicate attr");
		attrSet = true;
		// value is a comma-separated list.
		// whitespace is not allowed.
		const char* a = value;
		while(*a) {
			const char* c = strchr(a, ',');
			int len;
			if(c) {
				len = c - a;
			} else {
				len = strlen(a);
			}
			int i;
			for(i=0; i<snAttributes; i++) {
				const FIELD_NAME& fn(sAttributes[i]);
				if((int)strlen(fn.name) != len)
					continue;
				if(strncmp(a, fn.name, len) != 0)
					continue;
				attr |= fn.id;
				break;
			}
			if(i == snAttributes) {
				LOG("unknown attribute: %s\n", a);
				error("unknown attribute");
			}
			a += len;
			if(*a == ',')
				a++;
		}
	}

	static void parseString(wstring& w, const char* value) {
		// utf-8 to wstring conversion
		const char* srcp = value;
		int res;
		while(true) {
			wchar_t wc;
			res = mbtowc(&wc, srcp, 4);
			if(res < 0)
				error("utf-8 conversion error");
			if(res == 0)
				break;
			w += wc;
			srcp += res;
		}
	}

	static void parseInt(int& i, const char* value) {
		int res = sscanf(value, "%i", &i);
		if(res != 1)
			error("integer conversion error");
	}

	// examples: 1970-01-23, 2010-04-30T16:04:39Z
	// this is not a proper W3C DTF parser; fractions of a second is not allowed.
	// it's ok, though, because they'd be ignored anyway.
	static void parseDate(int& d, const char* value) {
		struct tm ctime;
		memset(&ctime, 0, sizeof(struct tm));
		const char* res = strptime(value, "%FT%T%z", &ctime);
		if(res == NULL) {
			res = strptime(value, "%F", &ctime);
			if(res == NULL)
				error("date conversion error");
		}
		if(*res != 0)
			error("date conversion incomplete");
		time_t ttime = mktime(&ctime);
		if(ttime > INT_MAX || ttime < 0)
			error("date out of bounds");
		d = (int)ttime;
	}

	template<class T> static ContactValue* attrT(void (*parse)(T&, const char*),
		const char** attr, int type)
	{
		Smartie<TContactValue<T> > v(new TContactValue<T>(type));
		bool valueSet = false, attrSet = false;
		while(*attr != NULL) {
			const char* name = *(attr++);
			const char* value = *(attr++);
			DEBUG_ASSERT(value != NULL);
			if(streq(name, "value")) {
				if(valueSet)
					error("duplicate value");
				valueSet = true;
				parse(v->mValue, value);
			} else if(streq(name, "attr")) {
				parseAttr(attrSet, v->attr, value);
			} else {
				LOG("unknown attribute: %s\n", name);
				error("unknown attribute");
			}
		}
		return v.extract();
	}

	static ContactValue* parseStringArray(const FIELD_NAME* fnp, int nNames, const char** attr) {
		Smartie<TContactValue<StringArray> > v(
			new TContactValue<StringArray>(MA_PIM_TYPE_STRING_ARRAY));
		vector<wstring>& sa(v->mValue.sa);
		vector<bool> valueSet(nNames, false);
		v->mValue.fnp = fnp;
		sa.resize(nNames);
		bool attrSet = false;
		// iterate through an element's attributes
		while(*attr != NULL) {
			const char* name = *(attr++);
			const char* value = *(attr++);
			DEBUG_ASSERT(value != NULL);
			if(streq(name, "attr")) {
				// handle the attr attribute :}
				parseAttr(attrSet, v->attr, value);
			} else {
				// find the index into the string array
				int field = -1;
				for(int i=0; i<nNames; i++) {
					const FIELD_NAME& fn(fnp[i]);
					if(streq(name, fn.name)) {
						field = fn.id;
						break;
					}
				}
				// sanity checks
				if(field == -1)
					error("unknown element");
				DEBUG_ASSERT(field < nNames);
				if(valueSet[field]) {
					LOG("duplicate value: %s\n", name);
					error("duplicate value");
				}
				valueSet[field] = true;
				// convert the string
				parseString(sa[field], value);
			}
		}
		return v.extract();
	}

	static void handleField(const char *el, const char **attr) {
		int field;
		ContactValue* v;
		if(streq(el, "addr")) {
			field = MA_PIM_FIELD_CONTACT_ADDR;
			v = parseStringArray(sAddrFields, snAddrFields, attr);
		} else if(streq(el, "name")) {
			field = MA_PIM_FIELD_CONTACT_NAME;
			v = parseStringArray(sNameFields, snNameFields, attr);
		} else {
			int i;
			for(i=0; i<snFieldNames; i++) {
				const FIELD_NAME& fn(sFieldNames[i]);
				if(streq(el, fn.name)) {
					field = fn.id;
					break;
				}
			}
			if(i == snFieldNames)
				error("unknown element");
			int type = sCL->type(field);
			switch(type) {
			case MA_PIM_TYPE_DATE:
				v = attrT(&parseDate, attr, type);
				break;
			case MA_PIM_TYPE_INT:
				v = attrT(&parseInt, attr, type);
				break;
			case MA_PIM_TYPE_STRING:
				v = attrT(&parseString, attr, type);
				break;
			case MA_PIM_TYPE_STRING_ARRAY:
			case MA_PIM_TYPE_BINARY:
			case MA_PIM_TYPE_BOOLEAN:
			default:
				DEBIG_PHAT_ERROR;
			}
		}
		sCI->addValue(field, v);
	}

	static void start(void *data, const char *el, const char **attr) {
		//LOG("start %s\n", el);
		if(streq(el, "contacts")) {
			if(sCL)
				error("parse error");
			sCL = (ContactList*)data;
		} else if(streq(el, "contact")) {
			if(sCI || !sCL)
				error("parse error");
			sCI = new ContactItem(gSyscall->mPimListNextHandle);
		} else {
			// map element name to (field id + data type)
			handleField(el, attr);
		}
	}

	static void end(void *data, const char *el) {
		//LOG("end %s\n", el);
		if(streq(el, "contacts")) {
			if(!sCL || sCI)
				error("parse error");
			sCL = NULL;
		} else if(streq(el, "contact")) {
			if(!sCI || !sCL)
				error("parse error");
			sCL->add(sCI);
			sCI = NULL;
		}
	}
}



void ContactItem::save(ostream& stream) const {
	stream << "\t<contact>\n";
	FieldMap::const_iterator itr = mFields.begin();
	for(; itr != mFields.end(); itr++) {
		const char* fieldName = NULL;
		for(int i=0; i<ContactParser::snFieldNames; i++) {
			const ContactParser::FIELD_NAME& fn(ContactParser::sFieldNames[i]);
			if(fn.id == itr->first) {
				fieldName = fn.name;
				break;
			}
		}
		// Quietly drop unknown fields; we have no way to represent or parse them yet.
		if(fieldName == NULL)
			continue;

		const vector<ContactValue*>& vcv(itr->second);
		for(size_t j=0; j<vcv.size(); j++) {
			stream << "\t\t<"<<fieldName;
			vcv[j]->save(stream);
			stream << " />\n";
		}
	}
	stream << "\t</contact>\n";
}

void ContactValue::save(ostream& stream) const {
	saveValue(stream);
	if(this->attr != 0) {
		stream << " attr=\"";
		bool first = true;
		for(int i=0; i<ContactParser::snAttributes; i++) {
			if(attr & ContactParser::sAttributes[i].id) {
				if(first) {
					first = false;
				} else {
					stream << ",";
				}
				stream << ContactParser::sAttributes[i].name;
			}
		}
		stream << "\"";
	}
}

// returns 0 on success, error code otherwise.
static int createContactValue(ContactValue*& v, int field, void* buf, int bufSize,
	int attributes)
{
	int type = pimContactFieldType(field);
	switch(type) {
	case MA_PIM_TYPE_BOOLEAN:
	case MA_PIM_TYPE_DATE:
	case MA_PIM_TYPE_INT:
		v = new TContactValue<int>(type);
		break;
	case MA_PIM_TYPE_STRING:
		v = new TContactValue<wstring>(type);
		break;
	case MA_PIM_TYPE_STRING_ARRAY:
		{
			// set mNames here, so setValue() can check for validity.
			TContactValue<StringArray>* sav = new TContactValue<StringArray>(type);
			switch(field) {
			case MA_PIM_FIELD_CONTACT_ADDR:
				sav->mValue.sa.resize(ContactParser::snAddrFields);
				sav->mValue.fnp = ContactParser::sAddrFields;
				break;
			case MA_PIM_FIELD_CONTACT_NAME:
				sav->mValue.sa.resize(ContactParser::snNameFields);
				sav->mValue.fnp = ContactParser::sNameFields;
				break;
			default:
				DEBIG_PHAT_ERROR;
			}
			v = sav;
		}
		break;
	default:
		return MA_PIM_ERR_FIELD_UNSUPPORTED;
	}
	v->setValue(buf, bufSize);
	v->attr = attributes;
	return 0;
}

//******************************************************************************
// Syscalls
//******************************************************************************

MAHandle Syscall::maPimListOpen(int listType) {
	PimList* pl;
	if(listType == MA_PIM_CONTACTS) {
		ContactList* cl = new ContactList();
		string mosyncDir = getenv("MOSYNCDIR");
		string fn = mosyncDir + "/etc/contacts.xml";
		LOG("Reading %s...\n", fn.c_str());
		bool res = cl->open(fn);
		if(!res) {
			// default contacts file
			string ofn = mosyncDir + "/bin/default_contacts.xml";
			LOG("Reading %s...\n", ofn.c_str());
			res = cl->open(ofn);
			if(res) {
				// save a copy
				res = cl->save(fn);
				if(!res) {
					LOG("Could not write %s\n", fn.c_str());
				}
			}
		}
		if(!res) {
			delete cl;
			ContactParser::sCL = NULL;
			SAFE_DELETE(ContactParser::sCI);
			return -2;
		}
		pl = cl;
	} else {
		return -2;
	}
	mPimLists.insert(mPimListNextHandle, pl);
	return mPimListNextHandle++;
}

#endif	//EMULATOR
