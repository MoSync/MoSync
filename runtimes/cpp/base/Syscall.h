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

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "Platform.h"

#include "ResourceArray.h"
#include "Stream.h"
#include "MemStream.h"

#ifndef SYMBIAN
#include <map>
#include <string>
#endif

#include <hashmap/hashmap.h>

#include <helpers/CPP_IX_STREAMING.h>

struct MAConnAddr;

namespace Base {

#include "SyscallImpl.h"

	public:
		bool loadResources(Stream& file, const char* aFilename);

		void init();
		virtual ~Syscall();
		void platformDestruct();

#ifdef SYMBIAN
#define SPECIAL(name) name
#define SYSCALL(type, nargs) type nargs
#define memset __memset
#define memcpy __memcpy
#define strcpy __strcpy
#define strcmp __strcmp
#include <helpers/cpp_maapi.h>
#undef memset
#undef memcpy
#undef strcpy
#undef strcmp
#undef SYSCALL

		void* memset(void* dst, int val, ulong size);
		void* memcpy(void* dst, const void* src, ulong size);
		int strcmp(const char* str1, const char* str2);
		char* strcpy(char* dst, const char* src);

#define SPECIAL_SYSCALL SYSCALL
#define SYSCALL(type, nargs) type Syscall::nargs
#define CALL_SYSCALL(syscall) mSyscall.syscall
#define SYSCALL_THIS this
#endif

		int gStoreNextId;
		StringMap gStores;

		ResourceArray resources;

		void ValidateMemRange(const void* ptr, int size);
		int ValidatedStrLen(const char* ptr);

		//for ioctl
		void* GetValidatedMemRange(int address, int size);
		const char* GetValidatedStr(int address);

#ifdef MEMORY_PROTECTION
		void protectMemory(int start, int length);
		void unprotectMemory(int start, int length);
		void setMemoryProtection(int enabled);
		int getMemoryProtection();
#endif

		void VM_Yield();

		int maBtGetNewDevice(MABtDevice* dst);
		int maBtGetNewService(MABtService* dst);
	};

	int maCheckInterfaceVersion(int hash);

	void maAccept(MAHandle conn);

	//platform-dependent, works like atoi.
	int atoiLen(const char* str, int len);
}

#include <helpers/CPP_IX_FILE.h>
MAHandle maFileOpen(const char* path, int mode);
int maFileExists(MAHandle file);
int maFileClose(MAHandle file);
int maFileCreate(MAHandle file);
int maFileDelete(MAHandle file);
int maFileSize(MAHandle file);
int maFileAvailableSpace(MAHandle file);
int maFileTotalSpace(MAHandle file);
int maFileDate(MAHandle file);
int maFileRename(MAHandle file, const char* newName);
int maFileTruncate(MAHandle file, int offset);

int maFileWrite(MAHandle file, const void* src, int len);
int maFileWriteFromData(const MA_FILE_DATA* args);
int maFileRead(MAHandle file, void* dst, int len);
int maFileReadToData(const MA_FILE_DATA* args);

int maFileTell(MAHandle file);
int maFileSeek(MAHandle file, int offset, int whence);

MAHandle maFileListStart(const char* path, const char* filter);
int maFileListNext(MAHandle list, char* nameBuf, int bufSize);
int maFileListClose(MAHandle list);

#ifndef SYMBIAN
#define SPECIAL(name) _##name
#define SPECIAL_SYSCALL(type, nargs) static type SPECIAL(nargs)

#ifdef _MSC_VER
#define SYSCALL(type, nargs) /*__declspec(dllexport)*/ type nargs
#else
#define SYSCALL(type, nargs) type nargs __attribute__((visibility("default")))
#endif

#define memset __memset
#define memcpy __memcpy
#define strcpy __strcpy
#define strcmp __strcmp
#include <helpers/cpp_maapi.h>
#undef memset
#undef memcpy
#undef strcpy
#undef strcmp
#undef SYSCALL

#ifdef _MSC_VER
#define SYSCALL(type, nargs) extern "C" /*__declspec(dllexport)*/ type nargs
#else
#define SYSCALL(type, nargs) extern "C" type nargs
#endif

#define CALL_SYSCALL(syscall) syscall
namespace Base {
	extern Syscall* gSyscall;
}
#define SYSCALL_THIS gSyscall
#endif	//SYMBIAN


#define GVMR(p, type) (type*)SYSCALL_THIS->GetValidatedMemRange(p, sizeof(type))
#define GVMRA(type) GVMR(a, type)

//Custom event handling
#ifdef MA_PROF_SUPPORT_VIDEO_STREAMING
#define CUSTOM_EVENT_STREAM(m) m(EVENT_TYPE_STREAM, MAStreamEventData)
#else
#define CUSTOM_EVENT_STREAM(m)
#endif

#ifdef MA_PROF_SUPPORT_LOCATIONAPI
#define CUSTOM_EVENT_LOCATION(m) m(EVENT_TYPE_LOCATION, MALocation)
#else
#define CUSTOM_EVENT_LOCATION(m)
#endif

#define CUSTOM_EVENTS(m)\
	CUSTOM_EVENT_LOCATION(m)\
	CUSTOM_EVENT_STREAM(m)\

#endif // _SYSCALL_H_
