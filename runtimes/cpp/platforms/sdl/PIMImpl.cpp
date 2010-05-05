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

#include "syscall.h"
#include "pim.h"
#include "strptime.h"
#include "helpers/smartie.h"
#include "ConfigParser.h"
#include <expat.h>
#include <limits.h>
#include <vector>
#include <map>
#include <stdexcept>

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
// Contacts
//******************************************************************************

namespace ContactParser {
	static void start(void *data, const char *el, const char **attr);
	static void end(void *data, const char *el);
	static void error(const char* msg) GCCATTRIB(noreturn);
}

struct ContactValue {
	int attr;

	ContactValue() : attr(0) {}
	virtual int getValue(void* buf, int bufSize) const = 0;
};

template<class T> struct TContactValue : ContactValue {
	T mValue;

	int getValue(void* buf, int bufSize) const;
};

template<>
int TContactValue<int>::getValue(void* buf, int bufSize) const {
	if(bufSize < (int)sizeof(int))
		return sizeof(int);
	*(int*)buf = mValue;
	return sizeof(int);
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
int TContactValue<wstring>::getValue(void* buf, int bufSize) const {
	int size = (mValue.length() + 1) * sizeof(wchar);
	if(bufSize < size)
		return size;
	wstringCpy((wchar*)buf, mValue);
	return size;
}

template<>
int TContactValue<vector<wstring> >::getValue(void* buf, int bufSize) const {
	int size = sizeof(int);
	for(size_t i=0; i<mValue.size(); i++) {
		size += (mValue[i].length() + 1) * sizeof(wchar);
	}
	if(bufSize < size)
		return size;
	*(int*)buf = mValue.size();
	wchar* dst = (wchar*)((byte*)buf + sizeof(int));
	memset(dst, -1, bufSize-4);	//temp
	for(size_t i=0; i<mValue.size(); i++) {
		wstringCpy(dst, mValue[i]);
		dst += mValue[i].length() + 1;
	}
	return size;
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
			LOG("%s\n", e.what());
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
	virtual PimItem* next() {
		if(mPos >= mItems.size())
			return NULL;
		return mItems[mPos++];
	}
	int type(int field) const {
		return pimContactFieldType(field);
	}

	void add(ContactItem* ci) {
		mItems.push_back(ci);
	}
};

namespace ContactParser {
	struct FIELD_NAME {
		int id;
		const char* name;
	};

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
		{ MA_PIM_FIELD_CONTACT_BIRTHDAY, "birthday" },
		//{ MA_PIM_FIELD_CONTACT_CLASS, "class" },
		{ MA_PIM_FIELD_CONTACT_EMAIL, "email" },
		{ MA_PIM_FIELD_CONTACT_FORMATTED_ADDR, "formatted_addr" },
		{ MA_PIM_FIELD_CONTACT_FORMATTED_NAME, "formatted_name" },
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
		if(ttime > INT_MAX)
			error("date out of bounds");
		d = (int)ttime;
	}

	template<class T> static ContactValue* attrT(void (*parse)(T&, const char*),
		const char** attr)
	{
		Smartie<TContactValue<T> > v(new TContactValue<T>);
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
		Smartie<TContactValue<vector<wstring> > > v(new TContactValue<vector<wstring> >);
		vector<bool> valueSet(nNames, false);
		v->mValue.resize(nNames);
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
				int i, field;
				for(i=0; i<nNames; i++) {
					const FIELD_NAME& fn(fnp[i]);
					if(streq(name, fn.name)) {
						field = fn.id;
						break;
					}
				}
				// sanity checks
				if(i == nNames)
					error("unknown element");
				DEBUG_ASSERT(field < nNames);
				if(valueSet[field]) {
					LOG("duplicate value: %s\n", name);
					error("duplicate value");
				}
				valueSet[field] = true;
				// convert the string
				parseString(v->mValue[field], value);
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
				v = attrT(&parseDate, attr);
				break;
			case MA_PIM_TYPE_INT:
				v = attrT(&parseInt, attr);
				break;
			case MA_PIM_TYPE_STRING:
				v = attrT(&parseString, attr);
				break;
			case MA_PIM_TYPE_STRING_ARRAY:
			case MA_PIM_TYPE_BINARY:
			case MA_PIM_TYPE_BOOLEAN:
			default:
				DEBIG_PHAT_ERROR;
			}
		}
		ContactItem::FieldMap::iterator itr = sCI->mFields.find(field);
		if(itr == sCI->mFields.end()) {
			// add field if item didn't have it already.
			pair<int, vector<ContactValue*> > p;
			p.first = field;
			itr = sCI->mFields.insert(p).first;
		}
		vector<ContactValue*>& vcv(itr->second);
		vcv.push_back(v);
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

//******************************************************************************
// Syscalls
//******************************************************************************

MAHandle Syscall::maPimListOpen(int listType) {
	PimList* pl;
	if(listType == MA_PIM_CONTACTS) {
		ContactList* cl = new ContactList();
		string mosyncDir = getenv("MOSYNCDIR");
		string fn = mosyncDir + "/etc/contacts.xml";
		bool res = cl->open(fn);
		if(!res) {
#if 0
			// todo: copy default contacts file.

			res = cl->open(fn);
			LOG("Cannot read %s\n", fn.c_str());
#endif
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
