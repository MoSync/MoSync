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


//todo: cleanup
#define LOGGING_ENABLED
#define CONFIG_H

#include "helpers/helpers.h"

#include "stabs_builtins.h"

#define DECLARE_BUILTIN_BASE(name, id, size, attrib) static class id : public Builtin {\
public:\
	id() : Builtin(#name, size, e##id) {}\
	void printMI(printfPtr, const void*, TypeBase::PrintFormat) const attrib;\
} s##id;

#if defined(_MSC_VER) || defined(__SYMBIAN32__)
#define BROKEN_BASE_ATTRIB
#elif defined(__GNUC__)
#define BROKEN_BASE_ATTRIB __attribute((noreturn))
#else
#error Unsupported platform!
#endif

#define DECLARE_BUILTIN(name, id) DECLARE_BUILTIN_BASE(name, id, sizeof(name),)
#define DECLARE_BROKEN_BUILTIN(name, id) DECLARE_BUILTIN_BASE(name, id, 0, BROKEN_BASE_ATTRIB)

BUILTINS(DECLARE_BUILTIN, DECLARE_BUILTIN);
BROKEN_BUILTINS(DECLARE_BROKEN_BUILTIN);

static class Bool : public Builtin {
public:
	Bool() : Builtin("bool", 0, eBool) {}
	void printMI(printfPtr, const void*, TypeBase::PrintFormat) const;
} sBool;

static class VTablePtr : public Builtin {
public:
	VTablePtr() : Builtin("__vtbl_ptr_type", 4, eVTablePtr) {}
	void printMI(printfPtr, const void*, TypeBase::PrintFormat) const;
} sVTablePtr;

#define ARR_BUILTIN(name, id) {#name, &s##id}
#define COMMA_ARR_BUILTIN(name, id) ,ARR_BUILTIN(name, id)
BUILTINdecl sBuiltins[] = {
	BUILTINS(ARR_BUILTIN, COMMA_ARR_BUILTIN)
	BROKEN_BUILTINS(COMMA_ARR_BUILTIN),
	{"_Bool", &sBool},
	{"bool", &sBool},
	{"__vtbl_ptr_type", &sVTablePtr}
};
const int snBuiltins = sizeof(sBuiltins) / sizeof(BUILTINdecl);

void Int::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%i", *(int*)data);
}

void Char::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%c", *(char*)data);
}

void LongInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%i", *(int*)data);
}

void UnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%u", *(int*)data);
}

void LongUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%u", *(int*)data);
}

void LongLongInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%I64i", *(s64*)data);
}

void LongLongUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%I64u", *(u64*)data);
}

void ShortInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%i", *(short*)data);
}

void ShortUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%u", *(short*)data);
}

void SignedChar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%c", *(char*)data);
}

void UnsignedChar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%c", *(char*)data);
}

void Float::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%f", *(float*)data);
}

void Double::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%f", *(double*)data);
}

#define BROKEN_BUILTIN_FUNCTIONS(name, id) \
	void id::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const { \
	DEBIG_PHAT_ERROR; }

BROKEN_BUILTINS(BROKEN_BUILTIN_FUNCTIONS);

void Bool::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%i", *(bool*)data);
}

void VTablePtr::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%p", data);
}

void Wchar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	pf("%C", *(wchar_t*)data);
}
