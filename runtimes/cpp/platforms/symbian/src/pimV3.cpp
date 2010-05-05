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

#include <cntitem.h>
#include <cntfldst.h>

using namespace MoSyncError;
using namespace Base;

//******************************************************************************
// Config
//******************************************************************************
#define DO_DUMP 0

#if DO_DUMP
#define DUMPLOG LOG
#else
#define DUMPLOG(a...)
#endif

//******************************************************************************
// Contacts
//******************************************************************************

#if DO_DUMP
#define CASE_VALUE(name) case KUidContactField##name##Value: return #name;
#define CASE_VCARD(name) case KIntContactFieldVCardMap##name: return "VCard " #name;

#define VALUES(m) \
	m(Address) \
	m(PostOffice) \
	m(ExtendedAddress) \
	m(Locality) \
	m(Region) \
	m(PostCode) \
	m(Country) \
	m(CompanyName) \
	m(CompanyNamePronunciation) \
	m(PhoneNumber) \
	m(GivenName) \
	m(FamilyName) \
	m(GivenNamePronunciation) \
	m(FamilyNamePronunciation) \
	m(AdditionalName) \
	m(SuffixName) \
	m(PrefixName) \
	m(Hidden) \
	m(EMail) \
	m(Msg) \
	m(Sms) \
	m(Fax) \
	m(DefinedText) \
	m(Note) \
	m(Birthday) \
	m(Url) \
	m(StorageInline) \
	m(TemplateLabel) \
	m(Picture) \
	m(RingTone) \
	m(DTMF) \
	m(None) \
	m(JobTitle) \
	m(ICCSlot) \
	m(ICCPhonebook) \
	m(ICCGroup) \
	m(IMAddress) \
	m(SecondName) \
	m(SIPID) \
	m(Assistant) \
	m(Anniversary) \
	m(Spouse) \
	m(Children) \
	m(Class) \
	m(DepartmentName) \

#define VCARDS(m) \
	m(WORK) \
	m(HOME) \
	m(MSG) \
	m(VOICE) \
	m(FAX) \
	m(PREF) \
	m(CELL) \
	m(PAGER) \
	m(BBS) \
	m(MODEM) \
	m(CAR) \
	m(ISDN) \
	m(VIDEO) \
	m(DOM) \
	m(ADR) \
	m(POSTOFFICE) \
	m(EXTENDEDADR) \
	m(LOCALITY) \
	m(REGION) \
	m(POSTCODE) \
	m(COUNTRY) \
	m(AGENT) \
	m(BDAY) \
	m(EMAILINTERNET) \
	m(GEO) \
	m(LABEL) \
	m(LOGO) \
	m(MAILER) \
	m(NOTE) \
	m(ORG) \
	m(ORGPronunciation) \
	m(PHOTO) \
	m(ROLE) \
	m(SOUND) \
	m(TEL) \
	m(TELFAX) \
	m(TITLE) \
	m(URL) \
	m(UnusedN) \
	m(UnusedFN) \
	m(NotRequired) \
	m(UnknownXDash) \
	m(Unknown) \
	m(UID) \
	m(INTL) \
	m(POSTAL) \
	m(PARCEL) \
	m(GIF) \
	m(CGM) \
	m(WMF) \
	m(BMP) \
	m(MET) \
	m(PMB) \
	m(DIB) \
	m(PICT) \
	m(TIFF) \
	m(PDF) \
	m(PS) \
	m(JPEG) \
	m(MPEG) \
	m(MPEG2) \
	m(AVI) \
	m(QTIME) \
	m(TZ) \
	m(KEY) \
	m(X509) \
	m(PGP) \
	m(SMIME) \
	m(WV) \
	m(SECONDNAME) \
	m(SIPID) \
	m(POC) \
	m(SWIS) \
	m(VOIP) \
	m(Assistant) \
	m(AssistantTel) \
	m(Anniversary) \
	m(Spouse) \
	m(Children) \
	m(Class) \
	m(Department) \

static const char* ft2string(int uidValue) {
	switch(uidValue) {
		VALUES(CASE_VALUE);
		VCARDS(CASE_VCARD);
		case KUidContactsVoiceDialFieldValue: return "VoiceDialField";
		default: return "MoSync Unknown";
	}
}
#endif	//DO_DUMP

#define ADDR_NUM 7
#define NAME_NUM 5

class ContactItem : public PimItem {
private:
	// what about string arrays?
	struct MoSyncValue {
		int attr;
		// into CContactItemFieldSet.
		// invalidated on insert/remove, but not on add.
		// we never do inserts, but removes are possible.
		int index;
	};
	struct MoSyncField {
		RArray<MoSyncValue> values;
	};

	const TContactItemId mId;
	CContactItem* mItem;
	const PimList* mList;
	HashMap<MoSyncField> mFieldMap;	//key: mosync field id.

	int mAddr[ADDR_NUM], mName[NAME_NUM];
	bool mHasAddr, mHasName;
public:
	ContactItem(TContactItemId id, const PimList* list, const MAHandle plh)
		: PimItem(plh), mId(id), mItem(NULL), mList(list),
		mHasAddr(0), mHasName(0)
	{
		memset(mAddr, -1, sizeof(mAddr));
		memset(mName, -1, sizeof(mName));
	}
	void ConstructL(CContactDatabase* db) {
		mItem = db->ReadMinimalContactLC(mId);
		CleanupStack::Pop(mItem);

		// we need to set up a symbian-to-mosync field map.
		// go through all the symbian fields and decide what to do with them.
		// symbian fields may, by definition, have multiple types.
		// only certain combinations are supported.
		// some combinations are illegal. what do to with them?
		// panics are no good for 3rd-party input data.
		// maybe ignore the field altogether, or ignore all broken parts of the combo.

#if 0	//dump the native item, for research.
		const CContactItemFieldSet& fs(mItem->CardFields());
		LOG("%i fields\n", fs.Count());
		for(int i=0; i<fs.Count(); i++) {
			const CContactItemField& f(fs[i]);
			LOG(" StorageType: %i\n", f.StorageType());
			const CContentType& ct(f.ContentType());
			LOG(" %i types\n", ct.FieldTypeCount());
			for(int j=0; j<ct.FieldTypeCount(); j++) {
				TFieldType ft(ct.FieldType(j));
				LOG("  0x%08X(%s)\n", ft.iUid, ft2string(ft.iUid));
			}
		}
#endif

		// it appears that the VCard types appear only together with the other fieldTypes,
		// and map onto our MA_PIM_ATTR values.
		// the other types map onto a MA_PIM_FIELD_CONTACT / MA_PIM_CONTACT combination.
		const CContactItemFieldSet& fs(mItem->CardFields());
		DUMPLOG("%i fields\n", fs.Count());
		for(int i=0; i<fs.Count(); i++) {
			parseField(fs[i], i);
		}

		// for string array types, only one of each array is allowed.
		// we'll assume that this holds true for data we read,
		// and enforce it for data we write.
		// also, they don't have any attributes.
	}

	virtual ~ContactItem() {
		mFieldMap.close();
		if(mItem)
			delete mItem;
	}

	int count() const {
		return 1 + mHasAddr + mHasName + mFieldMap.size();
	}

	int fieldId(int index) const {
		if(index == 0) {
			return MA_PIM_FIELD_CONTACT_UID;
		}
		int i = 1;
		if(mHasAddr) {
			if(index == i)
				return MA_PIM_FIELD_CONTACT_ADDR;
			i++;
		}
		if(mHasName) {
			if(index == i)
				return MA_PIM_FIELD_CONTACT_NAME;
			i++;
		}
		HashMap<MoSyncField>::TIteratorC itr = mFieldMap.begin();
		while(itr.hasMore()) {
			const HashMap<MoSyncField>::Pair& p(itr.next());
			if(i == index) {
				return p.key;
			}
			i++;
		}
		BIG_PHAT_ERROR(ERR_INVALID_PIM_FIELD_INDEX);
	}

	int fieldCount(int field) const {
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			return 1;
		}
		if(field == MA_PIM_FIELD_CONTACT_ADDR) {
			return mHasAddr;
		}
		if(field == MA_PIM_FIELD_CONTACT_NAME) {
			return mHasName;
		}
		const MoSyncField* mf = mFieldMap.find(field);
		if(mf == NULL) {
			return 0;
		}
		return mf->values.Count();
	}

	int attr(int field, int index) const {
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			return 0;
		}
		if(field == MA_PIM_FIELD_CONTACT_ADDR) {
			MYASSERT(mHasAddr, ERR_MISSING_PIM_FIELD);
			return 0;
		}
		if(field == MA_PIM_FIELD_CONTACT_NAME) {
			MYASSERT(mHasName, ERR_MISSING_PIM_FIELD);
			return 0;
		}
		const MoSyncField* mf = mFieldMap.find(field);
		MYASSERT(mf != NULL, ERR_MISSING_PIM_FIELD);
		MYASSERT(index >= 0 && index < mf->values.Count(), ERR_INVALID_PIM_VALUE_INDEX);
		return mf->values[index].attr;
	}

	int getValue(int field, int index, void* buf, int bufSize) const {
		//LOG("getValue(%i, %i, 0x%x, %i)\n", field, index, buf, bufSize);
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			MYASSERT(index == 0, ERR_INVALID_PIM_VALUE_INDEX);
			if(bufSize >= 4)
				*(int*)buf = mId;
			return 4;
		}

		if(field == MA_PIM_FIELD_CONTACT_ADDR) {
			return getStringArray(buf, bufSize, index, mHasAddr, mAddr, ADDR_NUM);
		}
		if(field == MA_PIM_FIELD_CONTACT_NAME) {
			return getStringArray(buf, bufSize, index, mHasName, mName, NAME_NUM);
		}

		const MoSyncField* mf = mFieldMap.find(field);
		MYASSERT(mf != NULL, ERR_MISSING_PIM_FIELD);
		MYASSERT(index >= 0 && index < mf->values.Count(), ERR_INVALID_PIM_VALUE_INDEX);
		const CContactItemFieldSet& fs(mItem->CardFields());
		const CContactItemField& f(fs[mf->values[index].index]);
		switch(f.StorageType()) {
		case KStorageTypeText:
			if(mList->type(field) != MA_PIM_TYPE_STRING) {
				LOG("Bad StorageType for f%i: %i\n", field, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			return getText(buf, bufSize, f.TextStorage());
		case KStorageTypeStore:
			if(mList->type(field) != MA_PIM_TYPE_BINARY) {
				LOG("Bad StorageType for f%i: %i\n", field, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			return getStore(buf, bufSize, f.StoreStorage());
		case KStorageTypeDateTime:
			if(mList->type(field) != MA_PIM_TYPE_DATE) {
				LOG("Bad StorageType for f%i: %i\n", field, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			return getDateTime(buf, bufSize, f.DateTimeStorage());
		case KStorageTypeContactItemId:
		default:
			LOG("Unknown StorageType: %i\n", f.StorageType());
			return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
		}
	}

private:
	static int getStore(void* buf, int bufSize, CContactStoreField* sf) {
		HBufC8* thing = sf->Thing();
		if(bufSize >= thing->Size()) {
			memcpy(buf, thing->Ptr(), thing->Size());
		}
		return thing->Size();
	}
	
	static int getText(void* buf, int bufSize, CContactTextField* tf) {
		TPtrC ptrc = tf->Text();
		int size = ptrc.Size() + sizeof(wchar);
		if(bufSize >= size) {
			memcpy(buf, ptrc.Ptr(), ptrc.Size());
			*(wchar*)((byte*)buf + ptrc.Size()) = 0;
		}
		return size;
	}
	
	static int getDateTime(void* buf, int bufSize, CContactDateField* df) {
		if(bufSize < 4)
			return 4;
		TTime time = df->Time();
		*(int*)buf = unixTime(time);
		return 4;
	}

	int getStringArray(void* buf, int bufSize, int index, bool has,
		const int* arr, int num) const
	{
		MYASSERT(index == 0, ERR_INVALID_PIM_VALUE_INDEX);
		MYASSERT(has, ERR_MISSING_PIM_FIELD);
		int size = sizeof(int);
		const CContactItemFieldSet& fs(mItem->CardFields());
		for(int i=0; i<num; i++) {
			size += sizeof(wchar);
			if(arr[i] < 0)
				continue;
			const CContactItemField& f(fs[arr[i]]);
			if(f.StorageType() != KStorageTypeText) {
				LOG("Bad StorageType for SA%i: %i\n", num, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			size += f.TextStorage()->Text().Size();
		}
		if(bufSize < size)
			return size;
		*(int*)buf = num;
		wchar* wptr = (wchar*)((byte*)buf + sizeof(int));
		for(int i=0; i<num; i++) {
			if(arr[i] >= 0) {
				const CContactItemField& f(fs[arr[i]]);
				TPtrC ptrc = f.TextStorage()->Text();
				memcpy(wptr, ptrc.Ptr(), ptrc.Size());
				wptr += ptrc.Length();
			}
			*wptr = 0;
			wptr++;
		}
		return size;
	}

	void parseField(const CContactItemField& f, int fieldIndex) {
		int mosyncFieldType = -1;
		int arrayIndex = -1;	//if fieldType's data type is String Array.
		int nativeFieldType = -1;
		int attr = 0;

#define SET(mft) if(mosyncFieldType != -1) {\
	LOG("Multiple field types: old 0x%08X, new 0x%08X\n", nativeFieldType, ft);\
	return; }\
	nativeFieldType = ft; mosyncFieldType = mft; break;

#define SET_ARR(mft, index) arrayIndex = index; SET(mft)

		DUMPLOG(" StorageType: %i\n", f.StorageType());
		const CContentType& ct(f.ContentType());
		DUMPLOG(" %i types\n", ct.FieldTypeCount());
		for(int j=0; j<ct.FieldTypeCount(); j++) {
			int ft(ct.FieldType(j).iUid);
			DUMPLOG("  0x%08X(%s)\n", ft, ft2string(ft));
			switch(ft) {
			case KUidContactFieldAddressValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_STREET);
			case KUidContactFieldPostOfficeValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_POBOX);
			case KUidContactFieldExtendedAddressValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_EXTRA);
			case KUidContactFieldLocalityValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_LOCALITY);
			case KUidContactFieldRegionValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_REGION);
			case KUidContactFieldPostCodeValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_POSTALCODE);
			case KUidContactFieldCountryValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_ADDR, MA_PIM_CONTACT_ADDR_COUNTRY);
			case KUidContactFieldBirthdayValue:
				SET(MA_PIM_FIELD_CONTACT_BIRTHDAY);
			// no support for CLASS.
			case KUidContactFieldEMailValue:
				SET(MA_PIM_FIELD_CONTACT_EMAIL);
			// no support for FORMATTED_ADDR.
			// no support for FORMATTED_NAME.
			case KUidContactFieldFamilyNameValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_NAME, MA_PIM_CONTACT_NAME_FAMILY);
			case KUidContactFieldGivenNameValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_NAME, MA_PIM_CONTACT_NAME_GIVEN);
			case KUidContactFieldAdditionalNameValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_NAME, MA_PIM_CONTACT_NAME_OTHER);
			case KUidContactFieldPrefixNameValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_NAME, MA_PIM_CONTACT_NAME_PREFIX);
			case KUidContactFieldSuffixNameValue:
				SET_ARR(MA_PIM_FIELD_CONTACT_NAME, MA_PIM_CONTACT_NAME_SUFFIX);
			case KUidContactFieldSecondNameValue:
				SET(MA_PIM_FIELD_CONTACT_NICKNAME);
			case KUidContactFieldNoteValue:
				SET(MA_PIM_FIELD_CONTACT_NOTE);
			case KUidContactFieldCompanyNameValue:
				SET(MA_PIM_FIELD_CONTACT_ORG);
			case KUidContactFieldPictureValue:
				SET(MA_PIM_FIELD_CONTACT_PHOTO);
			// no support for PHOTO_URL.
			// no obvious support for PUBLIC_KEY.
			// no obvious support for PUBLIC_KEY_STRING.
			// no obvious support for REVISION.
			case KUidContactFieldPhoneNumberValue:
				SET(MA_PIM_FIELD_CONTACT_TEL);
			case KUidContactFieldJobTitleValue:
				SET(MA_PIM_FIELD_CONTACT_TITLE);
			// support for UID would come from CContactItem::Id().
			case KUidContactFieldUrlValue:
				SET(MA_PIM_FIELD_CONTACT_URL);

			case KIntContactFieldVCardMapWORK:
				attr |= MA_PIM_ATTR_WORK; break;
			case KIntContactFieldVCardMapHOME:
				attr |= MA_PIM_ATTR_HOME; break;
			case KIntContactFieldVCardMapFAX:
				attr |= MA_PIM_ATTR_FAX; break;
			case KIntContactFieldVCardMapPREF:
				attr |= MA_PIM_ATTR_PREFERRED; break;
			case KIntContactFieldVCardMapCELL:
				attr |= MA_PIM_ATTR_MOBILE; break;
			case KIntContactFieldVCardMapPAGER:
				attr |= MA_PIM_ATTR_PAGER; break;
			case KIntContactFieldVCardMapAssistant:
				attr |= MA_PIM_ATTR_ASST; break;

			case KUidContactFieldSmsValue:
				attr |= MA_PIM_ATTR_SMS;
				SET(MA_PIM_FIELD_CONTACT_TEL);
			case KUidContactFieldFaxValue:
				attr |= MA_PIM_ATTR_FAX;
				SET(MA_PIM_FIELD_CONTACT_TEL);
			}	//switch
		}	//for
		
		if(mosyncFieldType == MA_PIM_FIELD_CONTACT_ADDR) {
			if(mAddr[arrayIndex] != -1) {
				LOG("Duplicate address field: %i\n", arrayIndex);
			}
			mAddr[arrayIndex] = fieldIndex;
			mHasAddr = 1;
		} else if(mosyncFieldType == MA_PIM_FIELD_CONTACT_NAME) {
			if(mName[arrayIndex] != -1) {
				LOG("Duplicate name field: %i\n", arrayIndex);
			}
			mName[arrayIndex] = fieldIndex;
			mHasName = 1;
		} else {
			MoSyncField* f = mFieldMap.find(mosyncFieldType);
			if(f == NULL) {
				f = new (ELeave) MoSyncField();
				mFieldMap.insert(mosyncFieldType, f);
			}
			MoSyncValue v = { attr, fieldIndex };
			f->values.AppendL(v);
		}
	}
};

class ContactList : public PimList {
private:
	CContactDatabase* mDb;
	const CContactIdArray* mList;
	int mPos;
	const MAHandle mPlh;
public:
	ContactList(MAHandle plh) : mDb(NULL), mPlh(plh) {}
	void ConstructL() {
		LTRAP(mDb = CContactDatabase::OpenL());
		mList = mDb->SortedItemsL();
		mPos = 0;
	}
	virtual ~ContactList() {
		if(mDb)
			delete mDb;
	}
	virtual PimItem* next() {
		if(mPos >= mList->Count())
			return NULL;
		ContactItem* ci = new (ELeave) ContactItem((*mList)[mPos++], this, mPlh);
		ci->ConstructL(mDb);
		return ci;
	}
	int type(int field) const {
		return pimContactFieldType(field);
	}
};

//******************************************************************************
// Events
//******************************************************************************

//TODO
class EventList : public PimList {
	virtual ~EventList() {}
	
	virtual PimItem* next() {
		return NULL;
	}
};

MAHandle Syscall::maPimListOpen(int listType) {
	PimList* pl;
	if(listType == MA_PIM_CONTACTS) {
		ContactList* cl = new (ELeave) ContactList(mPimListNextHandle);
		cl->ConstructL();
		pl = cl;
	//} else if(listType == MA_PIM_EVENTS) {
	//	pl = new (ELeave) EventList();
	} else {
		return -2;
	}
	mPimLists.insert(mPimListNextHandle, pl);
	return mPimListNextHandle++;
}
