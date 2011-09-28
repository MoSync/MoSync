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

static CContactCard* CCC_NEW_LC() {
	CContactCard* ccc = CContactCard::NewLC();
	LOGP("CContactCard::NewLC(): 0x%08x\n", ccc);
	return ccc;
}

static CContactItemField* CIF_NEW_LC(int st, TUid ft) {
	CContactItemField* cif = CContactItemField::NewLC(st, ft);
	LOGP("CContactItemField::NewLC(%i, 0x%08x): 0x%08x\n", st, ft, cif);
	return cif;
}

static void SET_TEXTL(CContactTextField* ctf, const TDesC& text) {
	ctf->SetTextL(text);
	TBuf8<128> tmp;
	tmp.Copy(text);
	LOGP("0x%08x->SetTextL(%s)\n", ctf, tmp.PtrZ());
}

static void SET_MAPPING(CContactItemField* cif, TUid uid) {
	cif->SetMapping(uid);
	LOGP("0x%08x->SetMapping(0x%08x)\n", cif, uid);
}

#if 0
static void ADD_FIELD_TYPEL(CContactItemField* cif, TUid uid) {
	cif->AddFieldTypeL(uid);
	LOGP("0x%08x->AddFieldTypeL(0x%08x)\n", cif, uid);
}
#endif

static void ADD_FIELDL(CContactItem* cci, CContactItemField* cif) {
	cci->AddFieldL(*cif);
	LOGP("0x%08x->AddFieldL(0x%08x)\n", cci, cif);
}

static void ADD_NEW_CONTACTL(CContactDatabase* db, CContactItem* cci) {
	db->AddNewContactL(*cci);
	LOGP("0x%08x->AddNewContactL(0x%08x)\n", db, cci);
}

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


// m(KUidContactField##, MA_PIM_FIELD_CONTACT_##, MA_PIM_CONTACT_##_##,
// KUidContactFieldVCardMap##)
#define ADDR_NUM 7
#define ADDR_FIELDS(m)\
	m(Address, ADDR, STREET, ADR)\
	m(PostOffice, ADDR, POBOX, POSTOFFICE)\
	m(ExtendedAddress, ADDR, EXTRA, EXTENDEDADR)\
	m(Locality, ADDR, LOCALITY, LOCALITY)\
	m(Region, ADDR, REGION, REGION)\
	m(PostCode, ADDR, POSTALCODE, POSTCODE)\
	m(Country, ADDR, COUNTRY, COUNTRY)\

#define NAME_NUM 5
#define NAME_FIELDS(m)\
	m(FamilyName, NAME, FAMILY, UnusedN)\
	m(GivenName, NAME, GIVEN, UnusedN)\
	m(AdditionalName, NAME, OTHER, UnusedN)\
	m(PrefixName, NAME, PREFIX, UnusedN)\
	m(SuffixName, NAME, SUFFIX, UnusedN)\

// m(KUidContactField##, MA_PIM_FIELD_CONTACT_##, KUidContactFieldVCardMap##)
#define SINGLE_FIELDS(m)\
	m(Birthday, BIRTHDAY, BDAY)\
	m(EMail, EMAIL, EMAILINTERNET)\
	m(SecondName, NICKNAME, SECONDNAME)\
	m(Note, NOTE, NOTE)\
	m(CompanyName, ORG, ORG)\
	m(Picture, PHOTO, PHOTO)\
	m(PhoneNumber, TEL, TEL)\
	m(JobTitle, TITLE, TITLE)\
	m(Url, URL, URL)\

// no support for CLASS.
// no support for FORMATTED_ADDR.
// no support for FORMATTED_NAME.
// no support for PHOTO_URL.
// no obvious support for PUBLIC_KEY.
// no obvious support for PUBLIC_KEY_STRING.
// no obvious support for REVISION.
// support for UID would come from CContactItem::Id().

// m(KUidContactFieldVCardMap##, MA_PIM_ATTR_##)
#define ATTRIBUTES(m)\
	m(WORK, WORK)\
	m(HOME, HOME)\
	m(FAX, FAX)\
	m(PREF, PREFERRED)\
	m(CELL, MOBILE)\
	m(PAGER, PAGER)\
	m(Assistant, ASST)\


class ContactItem : public PimItem {
private:
	// what about string arrays?
	struct MoSyncValue {
		int attr;
		// into CContactItemFieldSet.
		// invalidated on insert/remove, but not on add.
		// we never do inserts, but removes are possible.
		// thus, on remove, all indexes above the removed one must be
		// decremented to remain valid.
		int index;
	};
	struct MoSyncField {
		RArray<MoSyncValue> values;
	};
	struct NativeFieldType {
		TUid field, vcard;
	};

	CContactItem* mItem;
	PimList* mList;
	HashMap<MoSyncField> mFieldMap;	//key: mosync field id.

	int mAddr[ADDR_NUM], mName[NAME_NUM];
	// Native Field Types
	NativeFieldType mADDR_NFT[ADDR_NUM], mNAME_NFT[NAME_NUM];
	bool mHasAddr, mHasName;
	bool mIsModified, mIsNew;
public:
	int id() const {
		if(mIsNew)
			return -1;
		return mItem->Id();
	}

	ContactItem(CContactItem* item, PimList* list, const MAHandle plh, bool isNew)
		: PimItem(plh), mItem(item), mList(list),
		mHasAddr(0), mHasName(0), mIsNew(isNew)
	{
		mIsModified = false;

		memset(mAddr, -1, sizeof(mAddr));
		memset(mName, -1, sizeof(mName));

#define SET_ARRAY_NFT(uidPart, fieldPart, indexPart, vcardPart)\
	{ NativeFieldType nft = { KUidContactField##uidPart##Value,\
	KUidContactFieldVCardMap##vcardPart };\
	m##fieldPart##_NFT[MA_PIM_CONTACT_##fieldPart##_##indexPart] = nft; }

		ADDR_FIELDS(SET_ARRAY_NFT);
		NAME_FIELDS(SET_ARRAY_NFT);
	}
	void ConstructL(CContactDatabase* db) {

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
		//LOGP("fieldId(%i)\n", index);
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
				//LOGP("fieldId(%i) p.key: %i\n", index, p.key);
				if((int)p.key < 0) {
					LOG("FieldMap[%i] = [", p.key);
					for(int j=0; j<p.value->values.Count(); j++) {
						LOG("%s%i", j == 0 ? "" : ", ", p.value->values[j].index);
					}
					LOG("]\n");
					DEBIG_PHAT_ERROR;
				}
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

	typedef int (ContactItem::*TouchStringArray)(void* buf, int bufSize,
		int index, bool has, int* arr, int num, const NativeFieldType*);
	typedef int (*TouchText)(void* buf, int bufSize, CContactTextField*);
	typedef int (*TouchStore)(void* buf, int bufSize, CContactStoreField*);
	typedef int (*TouchDateTime)(void* buf, int bufSize, CContactDateField*);

	int touchValue(int field, int index, void* buf, int bufSize,
		TouchStringArray touchStringArray, TouchText touchText,
		TouchStore touchStore, TouchDateTime touchDateTime)
	{
		if(field == MA_PIM_FIELD_CONTACT_ADDR) {
			return (this->*touchStringArray)(buf, bufSize, index,
				mHasAddr, mAddr, ADDR_NUM, mADDR_NFT);
		}
		if(field == MA_PIM_FIELD_CONTACT_NAME) {
			return (this->*touchStringArray)(buf, bufSize, index,
				mHasName, mName, NAME_NUM, mNAME_NFT);
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
			return touchText(buf, bufSize, f.TextStorage());
		case KStorageTypeStore:
			if(mList->type(field) != MA_PIM_TYPE_BINARY) {
				LOG("Bad StorageType for f%i: %i\n", field, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			return touchStore(buf, bufSize, f.StoreStorage());
		case KStorageTypeDateTime:
			if(mList->type(field) != MA_PIM_TYPE_DATE) {
				LOG("Bad StorageType for f%i: %i\n", field, f.StorageType());
				return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
			}
			return touchDateTime(buf, bufSize, f.DateTimeStorage());
		case KStorageTypeContactItemId:
		default:
			LOG("Unknown StorageType: %i\n", f.StorageType());
			return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
		}
	}

	int getValue(int field, int index, void* buf, int bufSize) const {
		//LOG("getValue(%i, %i, 0x%x, %i)\n", field, index, buf, bufSize);
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			MYASSERT(index == 0, ERR_INVALID_PIM_VALUE_INDEX);
			if(bufSize >= 4)
				*(int*)buf = id();
			return 4;
		}

		return ((ContactItem*)this)->touchValue(field, index, buf, bufSize,
			&ContactItem::getStringArray, &getText,
			&getStore, &getDateTime);
	}
	int setValue(int field, int index, void* buf, int bufSize, int attributes) {
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}
		mIsModified = true;

		return touchValue(field, index, buf, bufSize,
			&ContactItem::setStringArray, &ContactItem::setText,
			&ContactItem::setStore, &ContactItem::setDateTime);
	}
	int addValue(int field, void* buf, int bufSize, int attributes) {
		LOGP("addValue(%i, %i, 0x%x)\n", field, bufSize, attributes);
		MYASSERT(field > 0, ERR_MISSING_PIM_FIELD);
		mIsModified = true;

		TUid fieldType, mapping;
		// special attributes
		if(attributes & MA_PIM_ATTR_SMS) {
			if(field != MA_PIM_FIELD_CONTACT_TEL) {
				return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
			}
			fieldType = KUidContactFieldSms;
			mapping = KUidContactFieldVCardMapMSG;
		}
		if(attributes & MA_PIM_ATTR_FAX) {
			if(field != MA_PIM_FIELD_CONTACT_TEL) {
				return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
			}
			fieldType = KUidContactFieldFax;
			mapping = KUidContactFieldVCardMapTELFAX;
		}

#define ADD_CONTENTTYPE_SINGLE(uidPart, fieldPart, vcardPart)\
	case MA_PIM_FIELD_CONTACT_##fieldPart:\
	fieldType = TUid::Uid(KUidContactField##uidPart##Value);\
	mapping = KUidContactFieldVCardMap##vcardPart;\
	break;

// m(KUidContactFieldVCardMap##, MA_PIM_ATTR_##)
#define ADD_CONTENTTYPE_ATTRIBUTE(vcardPart, attrPart)\
	if(attributes & MA_PIM_ATTR_##attrPart) {\
	fieldType = KUidContactFieldVCardMap##vcardPart;\
	mapping = KUidContactFieldVCardMap##vcardPart; }

		switch(field) {
		case MA_PIM_FIELD_CONTACT_ADDR:
			if(mHasAddr)
				return MA_PIM_ERR_FIELD_COUNT_MAX;
			setStringArray(buf, bufSize, 0, true, mAddr, ADDR_NUM, mADDR_NFT);
			mHasAddr = 1;
			return 0;
		case MA_PIM_FIELD_CONTACT_NAME:
			if(mHasName)
				return MA_PIM_ERR_FIELD_COUNT_MAX;
			setStringArray(buf, bufSize, 0, true, mName, NAME_NUM, mNAME_NFT);
			mHasName = 1;
			return 0;

		SINGLE_FIELDS(ADD_CONTENTTYPE_SINGLE);

		default:
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}
		ATTRIBUTES(ADD_CONTENTTYPE_ATTRIBUTE);

		TStorageType st;
		switch(mList->type(field)) {
		case MA_PIM_TYPE_STRING: st = KStorageTypeText; break;
		case MA_PIM_TYPE_BINARY: st = KStorageTypeStore; break;
		case MA_PIM_TYPE_DATE: st = KStorageTypeDateTime; break;
		default:
			LOG("Unhandled StorageType: %i\n", mList->type(field));
			return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
		}

		CContactItemField* cif = CIF_NEW_LC(st, fieldType);

		// don't forget to actually set the value
		int res;
		switch(mList->type(field)) {
		case MA_PIM_TYPE_STRING:
			res = setText(buf, bufSize, cif->TextStorage());
			break;
		case MA_PIM_TYPE_BINARY:
			res = setStore(buf, bufSize, cif->StoreStorage());
			break;
		case MA_PIM_TYPE_DATE:
			res = setDateTime(buf, bufSize, cif->DateTimeStorage());
			break;
		}

		SET_MAPPING(cif, mapping);
		ADD_FIELDL(mItem, cif);
		addValue(field, attributes, cif->Id());
		CleanupStack::Pop(cif);
		return 0;
	}

	int removeValue(int field, int index) {
		LOGP("removeValue(%i, %i)\n", field, index);
		if(field == MA_PIM_FIELD_CONTACT_UID) {
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}
		mIsModified = true;

		if(field == MA_PIM_FIELD_CONTACT_ADDR) {
			return removeStringArray(index,
				mHasAddr, mAddr, ADDR_NUM, mADDR_NFT);
		}
		if(field == MA_PIM_FIELD_CONTACT_NAME) {
			return removeStringArray(index,
				mHasName, mName, NAME_NUM, mNAME_NFT);
		}

		const MoSyncField* mf = mFieldMap.find(field);
		MYASSERT(mf != NULL, ERR_MISSING_PIM_FIELD);
		MYASSERT(index >= 0 && index < mf->values.Count(), ERR_INVALID_PIM_VALUE_INDEX);

		removeNativeField(mf->values[index].index);
		return 0;
	}

	void close();

private:
	void removeNativeField(int index) {
		mItem->RemoveField(index);
		//todo: repair indices of all the other items?
	}

	int removeStringArray(int index, bool& has,
		int* arr, int num, const NativeFieldType*)
	{
		MYASSERT(index == 0, ERR_INVALID_PIM_VALUE_INDEX);
		MYASSERT(has, ERR_MISSING_PIM_FIELD);

		for(int i=0; i<num; i++) {
			if(arr[i] >= 0) {
				removeNativeField(arr[i]);
				arr[i] = -1;
			}
		}
		has = 0;
		return 0;
	}

	static int getStore(void* buf, int bufSize, CContactStoreField* sf) {
		HBufC8* thing = sf->Thing();
		if(bufSize >= thing->Size()) {
			memcpy(buf, thing->Ptr(), thing->Size());
		}
		return thing->Size();
	}
	static int setStore(void* buf, int bufSize, CContactStoreField* sf) {
		sf->SetThingL(TPtrC8(CBP buf, bufSize));
		return 0;
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
	static int setText(void* buf, int bufSize, CContactTextField* tf) {
		MYASSERT(bufSize > (int)sizeof(wchar), ERR_INVALID_PIM_BUFFER_SIZE);
		MYASSERT((bufSize % sizeof(wchar)) == 0, ERR_INVALID_PIM_BUFFER_SIZE);
		DEBUG_ASSERT(sizeof(wchar) == sizeof(TUint16));
		TPtrC ptr((const wchar*)buf, bufSize / sizeof(wchar));
		SET_TEXTL(tf, ptr);
		return 0;
	}

	static int getDateTime(void* buf, int bufSize, CContactDateField* df) {
		if(bufSize < 4)
			return 4;
		TTime time = df->Time();
		*(int*)buf = unixTime(time);
		return 4;
	}
	static int setDateTime(void* buf, int bufSize, CContactDateField* df) {
		MYASSERT(bufSize == 4, ERR_INVALID_PIM_BUFFER_SIZE);
		df->SetTime(symbianTime(*(int*)buf));
		return 0;
	}

	int getStringArray(void* buf, int bufSize, int index, bool has,
		int* arr, int num, const NativeFieldType* nativeFieldTypes)
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

	int setStringArray(void* buf, int bufSize, int index, bool has,
		int* arr, int num, const NativeFieldType* nativeFieldTypes)
	{
		LOGP("setStringArray(%i)\n", num);
		MYASSERT(index == 0, ERR_INVALID_PIM_VALUE_INDEX);
		MYASSERT(has, ERR_MISSING_PIM_FIELD);

		MYASSERT(bufSize > (int)sizeof(int), ERR_INVALID_PIM_BUFFER_SIZE);
		int nStrings = *(int*)buf;
		MYASSERT((bufSize % sizeof(wchar)) == 0, ERR_INVALID_PIM_BUFFER_SIZE);
		int nChars = (bufSize - sizeof(int)) / sizeof(wchar);
		MYASSERT(nChars != nStrings, ERR_PIM_EMPTY_STRING_ARRAY);
		MYASSERT(nChars > nStrings, ERR_INVALID_PIM_BUFFER_SIZE);
		const wchar* src = (wchar*)((int*)buf + 1);
		const wchar* end = src + nChars;
		for(int i=0; i<num; i++) {
			const wchar* stringStart = src;
			while(true) {
				MYASSERT(src != end, ERR_PIM_UNTERMINATED_STRING);
				wchar c = *src;
				src++;
				if(c == 0)
					break;
			}
			TPtrC ptr(stringStart, (src - stringStart) - 1);
#if 1
			TBuf8<128> tmp;
			tmp.Copy(ptr);
			LOGP("setStringArray %i/%i: %s\n", i, num, tmp.PtrZ());
#endif
			if(arr[i] < 0 && ptr.Length() > 0) {
				// add a new subfield, but don't populate it yet.
				CContactItemField* cif = CIF_NEW_LC(KStorageTypeText,
					nativeFieldTypes[i].field);
				LOGP("setStringArray: 0x%08x 0x%08x\n", nativeFieldTypes[i].field,
					nativeFieldTypes[i].vcard);
				SET_TEXTL(cif->TextStorage(), _L("SAF"));//ptr);
				SET_MAPPING(cif, nativeFieldTypes[i].vcard);
				ADD_FIELDL(mItem, cif);
				arr[i] = cif->Id();
				CleanupStack::Pop(cif);
			} else if(arr[i] >= 0) {
				if(ptr.Length() == 0) {
					// todo: remove subfield.
					DEBIG_PHAT_ERROR;
				}
				// populate/change subfield.
				const CContactItemField& f(mItem->CardFields()[arr[i]]);
				if(f.StorageType() != KStorageTypeText) {
					LOG("Bad StorageType for SA %i/%i: %i\n", i, num, f.StorageType());
					return MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
				}
				SET_TEXTL(f.TextStorage(), ptr);
			}
		}
		MYASSERT(src == end, ERR_INVALID_PIM_BUFFER_SIZE);
		return 0;
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

#define PARSE_ARRAY_FIELD(uidPart, fieldPart, indexPart, vcardPart)\
	case KUidContactField##uidPart##Value:\
	SET_ARR(MA_PIM_FIELD_CONTACT_##fieldPart, MA_PIM_CONTACT_##fieldPart##_##indexPart)

#define PARSE_SINGLE_FIELD(uidPart, fieldPart, vcardPart)\
	case KUidContactField##uidPart##Value:\
	SET(MA_PIM_FIELD_CONTACT_##fieldPart)

#define PARSE_ATTRIBUTE(vcardPart, attrPart)\
	case KIntContactFieldVCardMap##vcardPart:\
	attr |= MA_PIM_ATTR_##attrPart; break;

		DUMPLOG(" StorageType: %i\n", f.StorageType());
		const CContentType& ct(f.ContentType());
		DUMPLOG(" %i types\n", ct.FieldTypeCount());
		for(int j=0; j<ct.FieldTypeCount(); j++) {
			int ft(ct.FieldType(j).iUid);
			DUMPLOG("  0x%08X(%s)\n", ft, ft2string(ft));
			switch(ft) {
				ADDR_FIELDS(PARSE_ARRAY_FIELD);
				NAME_FIELDS(PARSE_ARRAY_FIELD);
				SINGLE_FIELDS(PARSE_SINGLE_FIELD);
				ATTRIBUTES(PARSE_ATTRIBUTE);

			// no support for CLASS.
			// no support for FORMATTED_ADDR.
			// no support for FORMATTED_NAME.
			// no support for PHOTO_URL.
			// no obvious support for PUBLIC_KEY.
			// no obvious support for PUBLIC_KEY_STRING.
			// no obvious support for REVISION.
			// support for UID would come from CContactItem::Id().

			// compund cases, handled without macros since they are so few.
			case KUidContactFieldSmsValue:
				attr |= MA_PIM_ATTR_SMS;
				SET(MA_PIM_FIELD_CONTACT_TEL);
			case KUidContactFieldFaxValue:
				attr |= MA_PIM_ATTR_FAX;
				SET(MA_PIM_FIELD_CONTACT_TEL);
			}	//switch
		}	//for
		DUMPLOG(" Mapping: 0x%08X(%s)\n", ct.Mapping().iUid, ft2string(ct.Mapping().iUid));

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
		} else if(mosyncFieldType > 0) {
			addValue(mosyncFieldType, attr, fieldIndex);
		}
	}

	void addValue(int mosyncFieldType, int attr, int fieldIndex) {
		LOGP("addValue(%i, 0x%x, %i)\n", mosyncFieldType, attr, fieldIndex);
		MoSyncField* f = mFieldMap.find(mosyncFieldType);
		if(f == NULL) {
			f = new (ELeave) MoSyncField();
			mFieldMap.insert(mosyncFieldType, f);
		}
		MoSyncValue v = { attr, fieldIndex };
		f->values.AppendL(v);
	}
};

#if 0
static void addDummyItem(CContactDatabase* db) {
	static bool done = false;
	if(done)
		return;
	done = true;
	TCleaner<CContactItem> cci(CCC_NEW_LC());
	CContactItemField* cif;

	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldNote);
	SET_TEXTL(cif->TextStorage(), _L("this is a note"));
	SET_MAPPING(cif, KUidContactFieldVCardMapNOTE);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);

	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldGivenName);
	SET_TEXTL(cif->TextStorage(), _L("Qnish"));
	SET_MAPPING(cif, KUidContactFieldVCardMapUnusedN);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);

	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldFamilyName);
	SET_TEXTL(cif->TextStorage(), _L("Qgarwal"));
	SET_MAPPING(cif, KUidContactFieldVCardMapUnusedN);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);

	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldSuffixName);
	SET_TEXTL(cif->TextStorage(), _L("Cronnk"));
	SET_MAPPING(cif, KUidContactFieldVCardMapUnusedN);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);

#if 0
	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldCountry);
	SET_TEXTL(cif->TextStorage(), _L("Country"));
	SET_MAPPING(cif, KUidContactFieldVCardMapCOUNTRY);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);

	cif = CIF_NEW_LC(KStorageTypeText, KUidContactFieldPhoneNumber);
	SET_TEXTL(cif->TextStorage(), _L("05622420431"));
	SET_MAPPING(cif, KUidContactFieldVCardMapTEL);
	ADD_FIELD_TYPEL(cif, KUidContactFieldVCardMapCELL);
	ADD_FIELDL(cci(), cif);
	CleanupStack::Pop(cif);
#endif
	ADD_NEW_CONTACTL(db, cci());
}
#endif

class ContactList : public PimList {
private:
	CContactDatabase* mDb;
	const CContactIdArray* mList;
	int mPos;
	const MAHandle mPlh;	// this List's handle.
public:
	ContactList(MAHandle plh) : mDb(NULL), mPlh(plh) {}
	void ConstructL() {
		LTRAP(mDb = CContactDatabase::OpenL());
		//addDummyItem(mDb);
		mList = mDb->SortedItemsL();
		mPos = 0;
	}
	virtual ~ContactList() {
		if(mDb)
			delete mDb;
	}
	PimItem* next() {
		if(mPos >= mList->Count())
			return NULL;
		CContactItem* cci = mDb->OpenContactL((*mList)[mPos++]);
		ContactItem* ci = new (ELeave) ContactItem(cci, this, mPlh, false);
		ci->ConstructL(mDb);
		return ci;
	}
	int type(int field) const {
		return pimContactFieldType(field);
	}
	PimItem* createItem() {
		LOGP("createItem()\n");
		CContactItem* cci = CCC_NEW_LC();
		ContactItem* ci = new (ELeave) ContactItem(cci, this, mPlh, true);
		CleanupStack::Pop(cci);
		ci->ConstructL(mDb);
		return ci;
	}
	int removeItem(PimItem* pi) {
		// FIXME
		//TContactItemId id = ((ContactItem*)pi)->id();
		//if(id >= 0)
			//mDb->DeleteContactL(id);
		return 0;
	}
	CContactDatabase* db() {
		return mDb;
	}
};

void ContactItem::close() {
	LOGP("ContactItem::close, new %i, mod %i\n", mIsNew, mIsModified);
	CContactDatabase* db = ((ContactList*)mList)->db();
	if(mIsNew) {
		LTRAP(ADD_NEW_CONTACTL(db, mItem));
	} else if(mIsModified) {
		LTRAP(db->CommitContactL(*mItem));
	} else {
		db->CloseContactL(mItem->Id());
	}
}


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
		return MA_PIM_ERR_LIST_UNAVAILABLE;
	}
	mPimLists.insert(mPimListNextHandle, pl);
	return mPimListNextHandle++;
}
