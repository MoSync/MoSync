/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "config_platform.h"

#if defined(EMULATOR) || defined(SYMBIAN)

#include "Syscall.h"
#include "pim.h"
#include "helpers/CPP_IX_PIM.h"

using namespace Base;

//******************************************************************************
// Field types
//******************************************************************************

int pimContactFieldType(int field) {
	switch(field) {
	case MA_PIM_FIELD_CONTACT_ADDR:
		return MA_PIM_TYPE_STRING_ARRAY;
	case MA_PIM_FIELD_CONTACT_BIRTHDAY:
		return MA_PIM_TYPE_DATE;
	case MA_PIM_FIELD_CONTACT_CLASS:
		return MA_PIM_TYPE_INT;
	case MA_PIM_FIELD_CONTACT_EMAIL:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_FORMATTED_NAME:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_NAME:
		return MA_PIM_TYPE_STRING_ARRAY;
	case MA_PIM_FIELD_CONTACT_NICKNAME:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_NOTE:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_ORG:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_PHOTO:
		return MA_PIM_TYPE_BINARY;
	case MA_PIM_FIELD_CONTACT_PHOTO_URL:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_PUBLIC_KEY:
		return MA_PIM_TYPE_BINARY;
	case MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_REVISION:
		return MA_PIM_TYPE_DATE;
	case MA_PIM_FIELD_CONTACT_TEL:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_TITLE:
		return MA_PIM_TYPE_STRING;
	case MA_PIM_FIELD_CONTACT_UID:
		return MA_PIM_TYPE_INT;
	case MA_PIM_FIELD_CONTACT_URL:
		return MA_PIM_TYPE_STRING;
	default:
		return -2;
	}
}

//******************************************************************************
// Helper functions
//******************************************************************************

void Syscall::pimInit() {
	mPimListNextHandle = 1;
	mPimItemNextHandle = 1;
}

void Syscall::pimClose() {
	mPimItems.close();
	mPimLists.close();
}

PimItem* Syscall::pimGetItem(MAHandle h) {
	PimItem* pi = mPimItems.find(h);
	MYASSERT(pi != NULL, ERR_INVALID_PIM_HANDLE);
	MYASSERT(mPimLists.find(pi->pimList) != NULL, ERR_PIM_LIST_CLOSED);
	return pi;
}

//******************************************************************************
// Syscalls
//******************************************************************************

MAHandle Syscall::maPimListNext(MAHandle list) {
	PimList* pl = mPimLists.find(list);
	MYASSERT(pl != NULL, ERR_INVALID_PIM_HANDLE);
	PimItem* pi = pl->next();
	if(pi == NULL)
		return 0;
	mPimItems.insert(mPimItemNextHandle, pi);
	return mPimItemNextHandle++;
}
int Syscall::maPimListClose(MAHandle list) {
	mPimLists.erase(list);
	return 0;
}

MAHandle Syscall::maPimItemCreate(MAHandle list) {
	PimList* pl = mPimLists.find(list);
	MYASSERT(pl != NULL, ERR_INVALID_PIM_HANDLE);
	PimItem* pi = pl->createItem();
	DEBUG_ASSERT(pi != NULL);
	mPimItems.insert(mPimItemNextHandle, pi);
	return mPimItemNextHandle++;
}

int Syscall::maPimItemRemove(MAHandle list, MAHandle item) {
	PimList* pl = mPimLists.find(list);
	MYASSERT(pl != NULL, ERR_INVALID_PIM_HANDLE);
	PimItem* pi = pimGetItem(item);
	int res = pl->removeItem(pi);
	if(res < 0)
		return res;
	mPimItems.erase(item);
	return 0;
}

int Syscall::maPimItemClose(MAHandle item) {
	PimItem* pi = mPimItems.find(item);
	MYASSERT(pi != NULL, ERR_INVALID_PIM_HANDLE);
	pi->close();
	mPimItems.erase(item);
	return 0;
}

int Syscall::maPimItemCount(MAHandle item) {
	PimItem* pi = pimGetItem(item);
	return pi->count();
}
int Syscall::maPimItemGetField(MAHandle item, int index) {
	PimItem* pi = pimGetItem(item);
	MYASSERT(index < pi->count(), ERR_INVALID_PIM_FIELD_INDEX);
	int i = pi->fieldId(index);
	//LOGP("fieldId(%i): %i\n", index, i);
	return i;
}
int Syscall::maPimItemFieldCount(MAHandle item, int field) {
	PimItem* pi = pimGetItem(item);
	return pi->fieldCount(field);
}
int Syscall::maPimItemGetAttributes(MAHandle item, int field, int index) {
	PimItem* pi = pimGetItem(item);
	MYASSERT(index < pi->fieldCount(field), ERR_INVALID_PIM_VALUE_INDEX);
	return pi->attr(field, index);
}

int Syscall::maPimFieldType(MAHandle list, int field) {
	PimList* pl = mPimLists.find(list);
	MYASSERT(pl != NULL, ERR_INVALID_PIM_HANDLE);
	return pl->type(field);
}

int Syscall::maPimItemGetValue(const MA_PIM_ARGS* args, int index) {
	PimItem* pi = pimGetItem(args->item);
	return pi->getValue(args->field, index,
		GetValidatedMemRange((int)(size_t)args->buf, args->bufSize), args->bufSize);
}

int Syscall::maPimItemSetValue(const MA_PIM_ARGS* args, int index, int attributes) {
	PimItem* pi = pimGetItem(args->item);
	return pi->setValue(args->field, index,
		GetValidatedMemRange((int)(size_t)args->buf, args->bufSize), args->bufSize,
		attributes);
}

int Syscall::maPimItemAddValue(const MA_PIM_ARGS* args, int attributes) {
	PimItem* pi = pimGetItem(args->item);
	return pi->addValue(args->field,
		GetValidatedMemRange((int)(size_t)args->buf, args->bufSize), args->bufSize,
		attributes);
}

int Syscall::maPimItemRemoveValue(MAHandle item, int field, int index) {
	PimItem* pi = pimGetItem(item);
	return pi->removeValue(field, index);
}

#endif	//EMULATOR
