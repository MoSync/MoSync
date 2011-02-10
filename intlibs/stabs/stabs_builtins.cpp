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
	Bool() : Builtin("bool", 1, eBool) {}
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
	printPrimitiveByFormat<int>(pf, data, "%i", fmt, TypeBase::eDecimal);
}

void Char::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	
	if(fmt==eNatural) {
		printPrimitiveByFormat<char>(pf, data, "%i", eDecimal, eDecimal);
		char c = *(char*)data;
		if(c>=0 && c<=32) pf(" \'\\\\%o\'", c);
		else pf(" \'%c\'", c);
	} else {
		printPrimitiveByFormat<char>(pf, data, "%i", fmt, fmt);	
	}
}

void LongInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<int>(pf, data, "%i", fmt, TypeBase::eDecimal);
}

void UnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<int>(pf, data, "%u", fmt, TypeBase::eDecimal);
}

void LongUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<unsigned int>(pf, data, "%u", fmt, TypeBase::eDecimal);
}

void LongLongInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<s64>(pf, data, "%"INT64PREFIX"i", fmt, TypeBase::eDecimal);
}

void LongLongUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<u64>(pf, data, "%"INT64PREFIX"u", fmt, TypeBase::eDecimal);
}

void ShortInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<short>(pf, data, "%i", fmt, TypeBase::eDecimal);
}

void ShortUnsignedInt::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<unsigned short>(pf, data, "%u", fmt, TypeBase::eDecimal);
}

void SignedChar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<char>(pf, data, "%c", fmt, TypeBase::eDecimal);
}

void UnsignedChar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<unsigned char>(pf, data, "%u", fmt, TypeBase::eDecimal);
}

void Float::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	if(fmt == TypeBase::eDecimal || fmt == TypeBase::eNatural) pf("%f", *(float*)data);
	else pf("0");
}

void Double::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	if(fmt == TypeBase::eDecimal || fmt == TypeBase::eNatural) pf("%f", *(double*)data);
	else pf("0");
}

#define BROKEN_BUILTIN_FUNCTIONS(name, id) \
	void id::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const { \
	DEBIG_PHAT_ERROR; }

BROKEN_BUILTINS(BROKEN_BUILTIN_FUNCTIONS);

void Bool::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<bool>(pf, data, "%i", fmt, TypeBase::eDecimal);
}

void VTablePtr::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	printPrimitiveByFormat<const void*>(pf, data, "%p", fmt, TypeBase::eDecimal);
}

void Wchar::printMI(printfPtr pf, const void* data, TypeBase::PrintFormat fmt) const {
	//printPrimitiveByFormat<wchar_t>(pf, data, "%C", fmt, TypeBase::eDecimal);
	printPrimitiveByFormat<unsigned short>(pf, data, "%u", fmt, TypeBase::eHexadecimal);
	pf(" '%C'", *(unsigned short*)data);
}
