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

#include <math.h>
#include <limits.h>

#ifndef _WIN32_WCE
#include <errno.h>
#endif

#include "Syscall.h"
#include "FileStream.h"
#include "MemStream.h"
#include <helpers/smartie.h>
#include <filelist/filelist.h>

#ifdef WIN32
#include <windows.h>
#ifndef _WIN32_WCE
#include <io.h>
#endif	//_WIN32_WCE
#endif	//WIN32

#ifdef _android
#include <android/log.h>
#endif

#ifdef _WIN32_WCE
#include <windows.h>
#include "wce_helpers.h"
#include "wce_errors.h"
#endif	//_WIN32_WCE

#if !defined(SYMBIAN) && !defined(_android)
#include <vector>
#include <set>
#include "helpers/mkdir.h"
#endif

#if !defined(SYMBIAN) && !defined(_WIN32_WCE)
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#define _stat stat
#define _open open
#endif	//WIN32
#endif	//SYMBIAN && _WIN32_WCE

#if defined(LINUX) || defined(__IPHONE__) || defined(DARWIN)
#include <sys/statvfs.h>
#define stricmp(x, y) strcasecmp(x, y)
#endif

using namespace Base;

namespace Base {

	uint getMaxCustomEventSize() {
		#define COUNT_CUSTOM_EVENT(eventType, dataType)\
			if(maxCustomEventSize < sizeof(dataType)) maxCustomEventSize = sizeof(dataType);

		uint maxCustomEventSize = 0;
		CUSTOM_EVENTS(COUNT_CUSTOM_EVENT);
		DUMPHEX(maxCustomEventSize);
		maxCustomEventSize = (maxCustomEventSize+0x3) & (~0x3); // align to sizeof(int)

		return maxCustomEventSize;
	}

#ifdef RESOURCE_MEMORY_LIMIT
	uint size_RT_FLUX(void* size) {
		return (uint)(size_t)size;
	}
	uint size_RT_PLACEHOLDER(void*) {
		return 0;
	}
	uint size_RT_LABEL(Label* r) {
		return sizeof(Label) + strlen(r->getName());
	}
	uint size_RT_BINARY(Stream* r) {
		if(r->ptrc() == NULL)
			return 0;
		int length;
		DEBUG_ASSERT(r->length(length));
		return sizeof(MemStream) + length;
	}
#endif	//RESOURCE_MEMORY_LIMIT

#if !defined(SYMBIAN) && !defined(_android)
#if defined(_WIN32_WCE)
	struct FileListItem {
		std::string name;
	};
	struct FileList {
		std::vector<FileListItem> files;
		std::vector<FileListItem>::const_iterator itr;
	};
#else
	struct FileListItem {
		std::string name;
		struct stat s;
		int sorting;
		bool operator<(const FileListItem&) const;
	};
	struct FileList {
		std::set<FileListItem> files;
		std::set<FileListItem>::const_iterator itr;
	};
#endif	//_WIN32_WCE
	typedef std::map<int, FileList> FileListMap;
	typedef FileListMap::iterator FileListItr;
	static FileListMap sFileListings;
	static int sFileListNextHandle = 1;
#endif	//SYMBIAN

	void Syscall::init() {
		mPanicOnProgrammerError = true;
		gStoreNextId = 1;
		gFileNextHandle = 1;
	}

	Syscall::~Syscall() {
		LOGD("~Syscall\n");
		gStores.close();
		gFileHandles.close();
		platformDestruct();
	}

	bool Syscall::loadResources(Stream& file, const char* aFilename)  {
		bool hasResources = true;
		if(!file.isOpen())
			hasResources = false;
		else {
			int len, pos;
			TEST(file.length(len));
			TEST(file.tell(pos));
			if(len == pos)
				hasResources = false;
		}
		if(!hasResources/* && aFilename != NULL*/) {
			resources.init(0);
			return true;
		}

#define MATCH_BYTE(c) { DAR_UBYTE(b); if(b != c) { FAIL; } }
		MATCH_BYTE('M');
		MATCH_BYTE('A');
		MATCH_BYTE('R');
		MATCH_BYTE('S');

		DAR_UVINT(nResources);
		DAR_UVINT(rSize);
		resources.init(nResources);

		// rI is the resource index.
		int rI = 1;

		while(true) {
			DAR_UBYTE(type);
			if(type == 0)
				break;

			//dispose flag

			DAR_UVINT(size);
			LOG_RES("Type %i, size %i\n", type, size);

			switch(type) {
			case RT_BINARY:
				{
#ifndef _android
					MemStream* ms = new MemStream(size);
#else
					char* b = loadBinary(rI, size);
					MemStream* ms = new MemStream(b, size);
#endif
					TEST(file.readFully(*ms));
					ROOM(resources.dadd_RT_BINARY(rI, ms));
#ifdef _android
					checkAndStoreAudioResource(rI);
#endif
				}
				break;
			case RT_UBIN:
				{
					int pos;
					MYASSERT(aFilename, ERR_RES_LOAD_UBIN);
					TEST(file.tell(pos));
#ifndef _android
					ROOM(resources.dadd_RT_BINARY(rI,
						new LimitedFileStream(aFilename, pos, size)));
#else
					// Android loads ubins by using JNI.
					loadUBinary(rI, pos, size);
					ROOM(resources.dadd_RT_BINARY(rI,
						new LimitedFileStream(
							aFilename,
							pos,
							size,
							getJNIEnvironment(),
							getJNIThis())));
#endif
					TEST(file.seek(Seek::Current, size));
				}
				break;
			case RT_PLACEHOLDER:
				ROOM(resources.dadd_RT_PLACEHOLDER(rI, NULL));
				break;
			case RT_IMAGE:
				{
					MemStream b(size);
					TEST(file.readFully(b));
#ifndef _android
					// On all platforms except Android, we load and add
					// the image data. "dadd" means "delete and add",
					// and is defined in runtimes\cpp\base\ResourceArray.h
                    RT_IMAGE_Type* image = loadImage(b);
                    if(!image)
                        BIG_PHAT_ERROR(ERR_IMAGE_LOAD_FAILED);
					ROOM(resources.dadd_RT_IMAGE(rI, image));
#else
					// On Android images are stored on the Java side.
					// Here we allocate a dummy array (real image is
					// in a table in Java) so that the resource handling,
					// like deleting resources, will work also on Android.
					// The actual image will be garbage collected on
					// Android when a resource is replaced in the Java table.
					ROOM(resources.dadd_RT_IMAGE(rI, new int[1]));
					int pos;
					file.tell(pos);
					loadImage(
						rI,
						pos - size,
						size,
						Base::gSyscall->getReloadHandle());
#endif
				}
				break;
			case RT_SPRITE:
				{
					DAR_USHORT(indexSource);
					DAR_USHORT(left);
					DAR_USHORT(top);
					DAR_USHORT(width);
					DAR_USHORT(height);
					DAR_SHORT(cx);
					DAR_SHORT(cy);
#ifndef _android
					ROOM(resources.dadd_RT_IMAGE(rI, loadSprite(resources.get_RT_IMAGE(indexSource),
						left, top, width, height, cx, cy)));
#endif
				}
				break;
			case RT_LABEL:
				{
					MemStream b(size);
					TEST(file.readFully(b));
					ROOM(resources.dadd_RT_LABEL(rI, new Label((const char*)b.ptr(), rI)));
				}
				break;

#ifdef LOGGING_ENABLED
			case 99:  //testtype
#define DUMP_UVI { DAR_UVINT(u); LOG_RES("u %i\n", u); }
#define DUMP_SVI { DAR_SVINT(s); LOG_RES("s %i\n", s); }
				DUMP_UVI;
				DUMP_UVI;
				DUMP_UVI;
				DUMP_SVI;
				DUMP_SVI;
				DUMP_SVI;
				DUMP_SVI;
				DUMP_SVI;
				DUMP_SVI;
				break;
#endif
			default:
				TEST(file.seek(Seek::Current, size));
			}
			rI++;
		}
		if(rI != nResources + 1) {
			LOG("rI %i, nR %i\n", rI, nResources);
			BIG_PHAT_ERROR(ERR_RES_FILE_INCONSISTENT);
		}
		LOG_RES("ResLoad complete\n");
		return true;
	}
}	//namespace Base

	//***************************************************************************
	// Special Syscalls
	//***************************************************************************

	SPECIAL_SYSCALL(void*, memcpy(void* dst, const void* src, unsigned long size)) {
		return ::memcpy(dst, src, size);
	}
	SYSCALL(void*, __memcpy(void* dst, const void* src, unsigned long size)) {
		SYSCALL_THIS->ValidateMemRange(dst, size);
		SYSCALL_THIS->ValidateMemRange(src, size);
		return SPECIAL(memcpy)(dst, src, size);
	}

	SPECIAL_SYSCALL(void*, memset(void* dst, int val, unsigned long size)) {
		return ::memset(dst, val, size);
	}
	SYSCALL(void*, __memset(void* dst, int val, unsigned long size)) {
		SYSCALL_THIS->ValidateMemRange(dst, size);
		return SPECIAL(memset)(dst, val, size);
	}

	SPECIAL_SYSCALL(char*, strcpy(char* dst, const char* src)) {
#if defined(SYMBIAN)
		//TODO: use TPtr8 and TPtrC8 to be able to call an optimized function
		//check to see if that's faster than this homebrew.
		char* oldDst = dst;
		do {
			*(dst++) = *(src);
		} while(*src++);
		return oldDst;
#else
		return (char*)::strcpy(dst, src);
#endif
	}
	SYSCALL(char*, __strcpy(char* dst, const char* src)) {
		SYSCALL_THIS->ValidateMemRange(dst, SYSCALL_THIS->ValidatedStrLen(src));
		return SPECIAL(strcpy)(dst, src);
	}

	SPECIAL_SYSCALL(int, strcmp(const char* a, const char* b)) {
#if defined(SYMBIAN)
		TPtrC8 ta(CBP a);
		TPtrC8 tb(CBP b);
		return ta.Compare(tb);
#else
		return ::strcmp(a, b);
#endif
	}
	SYSCALL(int, __strcmp(const char* a, const char* b)) {
		SYSCALL_THIS->ValidatedStrLen(a);
		SYSCALL_THIS->ValidatedStrLen(b);
		return SPECIAL(strcmp)(a, b);
	}

	//***************************************************************************
	// Proper Syscalls
	//***************************************************************************

#ifndef _android

	// doubles -------------------------------------------
	SYSCALL(double, __adddf3(double a, double b)) {
		return a+b;
	}
	SYSCALL(double, __subdf3(double a, double b)) {
		return a-b;
	}
	SYSCALL(double, __muldf3(double a, double b)) {
		return a*b;
	}
	SYSCALL(double, __divdf3(double a, double b)) {
#ifndef ALLOW_FLOAT_DIVISION_BY_ZERO
		if(b == 0
#ifdef EMULATOR
			&& !gSyscall->mAllowDivZero
#endif	//EMULATOR
			)
		{
			BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
		}
#endif	//ALLOW_FLOAT_DIVISION_BY_ZERO
		return a/b;
	}
	SYSCALL(double, __negdf2(double a)) {
		return -a;
	}
	SYSCALL(int, __fixdfsi(double a)) {
		return (int)a;
	}
	SYSCALL(double, __floatsidf(int a)) {
		return (double)a;
	}
	SYSCALL(double, __extendsfdf2(float f)) {
		return (double)f;
	}
	SYSCALL(uint, __fixunsdfsi(double f)) {
		return (uint)f;
	}
#endif	// NOT _android
	SYSCALL(int, dcmp(double a, double b)) {
		if(a > b)
			return 1;
		else if(a == b)
			return 0;
		else	//a < b		//or NaN!
			return -1;
	}

#ifndef _android
	// floats -------------------------------------------
	SYSCALL(float, __addsf3(float a, float b)) {
		return a+b;
	}
	SYSCALL(float, __subsf3(float a, float b)) {
		return a-b;
	}
	SYSCALL(float, __mulsf3(float a, float b)) {
		return a*b;
	}
	SYSCALL(float, __divsf3(float a, float b)) {
#ifndef ALLOW_FLOAT_DIVISION_BY_ZERO
		if(b == 0
#ifdef EMULATOR
			&& !gSyscall->mAllowDivZero
#endif	//EMULATOR
			)
		{
			BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
		}
#endif	//ALLOW_FLOAT_DIVISION_BY_ZERO
		return a/b;
	}
	SYSCALL(float, __negsf2(float a)) {
		return -a;
	}
	SYSCALL(int, __fixsfsi(float a)) {
		return (int)a;
	}
	SYSCALL(float, __floatsisf(int a)) {
		return (float)a;
	}
	SYSCALL(float, __truncdfsf2(double a)) {
		return (float)a;
	}
	SYSCALL(uint, __fixunssfsi(float f)) {
		return (uint)f;
	}
#endif	// NOT _android
	SYSCALL(int, fcmp(float a, float b)) {
		if(a > b)
			return 1;
		else if(a == b)
			return 0;
		else	//a < b		//or NaN!
			return -1;
	}

	SYSCALL(MAHandle, maCreatePlaceholder()) {
		return (MAHandle) SYSCALL_THIS->resources.create_RT_PLACEHOLDER();
	}

	SYSCALL(void, maDestroyObject(MAHandle handle)) {
#ifdef _android
		// On Android, we call into the Java side to
		// delete objects, unless it is a binary resource,
		// which is handled in the standard way.
		if(!SYSCALL_THIS->destroyBinaryResource(handle))
		{
			SYSCALL_THIS->destroyResource(handle);
		}
#endif
		SYSCALL_THIS->resources.destroy(handle);

	}

	SYSCALL(int, maCreateData(MAHandle placeholder, int size)) {
#ifndef _android
		MemStream* ms = new MemStream(size);
#else
		char* b = SYSCALL_THIS->loadBinary(placeholder, size);
		if(b == NULL) return RES_OUT_OF_MEMORY;

		MemStream* ms = new MemStream(b, size);
#endif
		if(ms == 0) return RES_OUT_OF_MEMORY;
		if(ms->ptr()==0) { delete ms; return RES_OUT_OF_MEMORY; }

		return SYSCALL_THIS->resources.add_RT_BINARY(placeholder, ms);
	}

	SYSCALL(int, maGetDataSize(MAHandle data)) {
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		int len;
		DEBUG_ASSERT(b->length(len));
		return len;
	}
	SYSCALL(void, maReadData(MAHandle data, void* dst, int offset, int size)) {
		SYSCALL_THIS->ValidateMemRange(dst, size);
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		MYASSERT(b->seek(Seek::Start, offset), ERR_DATA_OOB);
		MYASSERT(b->read(dst, size), ERR_DATA_OOB);
	}
	SYSCALL(void, maWriteData(MAHandle data, const void* src, int offset, int size)) {
		SYSCALL_THIS->ValidateMemRange(src, size);
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		MYASSERT(b->seek(Seek::Start, offset), ERR_DATA_OOB);
		MYASSERT(b->write(src, size), ERR_DATA_OOB);
	}
	SYSCALL(void, maCopyData(const MACopyData* a)) {
		Stream* dst = SYSCALL_THIS->resources.get_RT_BINARY(a->dst);
		Stream* src = SYSCALL_THIS->resources.get_RT_BINARY(a->src);
		MYASSERT(dst->seek(Seek::Start, a->dstOffset), ERR_DATA_OOB);
		MYASSERT(src->seek(Seek::Start, a->srcOffset), ERR_DATA_OOB);
		MYASSERT(dst->writeStream(*src, a->size), ERR_DATA_OOB);
	}

#if !defined(_android)
#ifdef SYMBIAN
#else
#define STORE_PATH "stores/"
#endif

	SYSCALL(MAHandle, maOpenStore(const char* name, int flags))
	{
		const char* path;
		int len;
#ifdef _WIN32_WCE
		char temp[256];
		TCHAR wtemp[256];
		getWorkingDirectory(temp, 256);
		strcat(temp, "\\");
		strcat(temp, STORE_PATH);
		convertAsciiToUnicode(wtemp, 256, temp);
		CreateDirectory(wtemp, NULL);
#elif defined(SYMBIAN)
		MyRFs myrfs;
		myrfs.Connect();
#ifdef DEBUGGING_MODE
		int res =
#endif
			FSS.MkDir(KMAStorePath16);
		LOGD("MkDir %i\n", res);
#elif defined(__IPHONE__)
#else
		_mkdir(STORE_PATH);
#endif	//_WIN32_WCE

#ifdef SYMBIAN
		TPtrC8 nameDesC(CBP name);
		TCleaner<HBufC8> hbuf(HBufC8::NewLC(KMAStorePath8().Length() + nameDesC.Length() + 1));
		TPtr8 des = hbuf->Des();
		des.Append(KMAStorePath8);
		des.Append(nameDesC);
		path = CCP des.PtrZ();
		len = des.Length();
#elif defined(__IPHONE__)
		std::string newPath = getWriteablePath(STORE_PATH);
		std::string newFile =  newPath + "/" + std::string(name);
		path = newFile.c_str();
		len = newFile.length();
		_mkdir(newPath.c_str());
#else
		std::string newPath = STORE_PATH + std::string(name);
		path = newPath.c_str();
		len = newPath.length();
#endif

		FileStream readFile(path);
		if(!readFile.isOpen())
		{
			if(flags & MAS_CREATE_IF_NECESSARY)
			{
				WriteFileStream writeFile(path);
				if(!writeFile.isOpen())
					return STERR_GENERIC;
			}
			else
			{
				return STERR_NONEXISTENT;
			}
		}

		SYSCALL_THIS->gStores.insert(SYSCALL_THIS->gStoreNextId, path, len);
		return SYSCALL_THIS->gStoreNextId++;
	}

	SYSCALL(int, maWriteStore(MAHandle store, MAHandle data))
	{
		const char* name = SYSCALL_THIS->gStores.find(store);
		MYASSERT(name, ERR_STORE_HANDLE_INVALID);

		WriteFileStream writeFile(name);
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		if(!b->seek(Seek::Start, 0)) {
			return STERR_GENERIC;
		}
		if(!writeFile.writeFully(*b)) {
			return STERR_GENERIC;
		}

		return 1;
	}

	SYSCALL(int, maReadStore(MAHandle store, MAHandle placeholder))
	{
		const char* name = SYSCALL_THIS->gStores.find(store);
		MYASSERT(name, ERR_STORE_HANDLE_INVALID);

		FileStream readFile(name);
		int len;
		MYASSERT(readFile.length(len), ERR_STORE_READ_FAILED);
		Smartie<MemStream> b(new MemStream(len));
		if(!readFile.readFully(*b))
		{
			BIG_PHAT_ERROR(ERR_STORE_READ_FAILED);
		}
		return SYSCALL_THIS->resources.add_RT_BINARY(placeholder, b.extract());
	}

	SYSCALL(void, maCloseStore(MAHandle store, int del))
	{
		const char* name = SYSCALL_THIS->gStores.find(store);
		MYASSERT(name, ERR_STORE_HANDLE_INVALID);
		if(del)
		{
#ifdef SYMBIAN
			MyRFs myrfs;
			myrfs.Connect();
			TCleaner<HBufC> desc(CreateHBufC16FromCStringLC(name));
			LHEL(FSS.Delete(*desc));
#elif defined(_WIN32_WCE)
			char temp[MAX_PATH];
			getWorkingDirectory(temp, MAX_PATH);
			strcat(temp, "\\");
			strcat(temp, name);
			int a = strlen(temp);

			BSTR unicodestr = SysAllocStringLen(NULL, a);
			::MultiByteToWideChar(CP_ACP, 0, temp, a, unicodestr, a);

			BOOL ret = DeleteFile(unicodestr);
			::SysFreeString(unicodestr);
			if(!ret)
			{
				DWORD error = GetLastError();

				if(ret==ERROR_FILE_NOT_FOUND)
				{
					BIG_PHAT_ERROR(WCEERR_STORE_FILE_NOT_FOUND);
				}
				else if(ret==ERROR_ACCESS_DENIED)
				{
					BIG_PHAT_ERROR(WCEERR_STORE_ACCESS_DENIED);
				}
				else
				{
					BIG_PHAT_ERROR(WCEERR_STORE_CLOSE_FAILED);
				}
			}
#else
			int res = remove(name);
			if(res != 0) {
				LOG("maCloseStore: remove error %i. errno %i.\n", res, errno);
				DEBIG_PHAT_ERROR;
			}
#endif	//SYMBIAN
		}
		SYSCALL_THIS->gStores.erase(store);
	}
#endif // NOT _android

	SYSCALL(int, maLoadResources(MAHandle data)) {
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		return SYSCALL_THIS->loadResources(*b, NULL);
	}

	SYSCALL(void, maExit(int result)) {
		SYSCALL_THIS->VM_Yield();
		LOG("Exit %i\n", result);
		MoSyncExit(result);
	}

	SYSCALL(int, maFindLabel(const char* name)) {
		for(unsigned i = 1; i < SYSCALL_THIS->resources.size(); i++) {
			if(SYSCALL_THIS->resources.get_type(i)==RT_LABEL)
			{
				Label *l = SYSCALL_THIS->resources.get_RT_LABEL(i);
				if(::strcmp(l->getName(), name)==0) {
					return i;
				}
			}
		}
		return -1;
	}

	SYSCALL(int, maCheckInterfaceVersion(int hash)) {
		if(hash == (int)MAIDL_HASH) {
			LOG("IDL version match!\n");
		} else {
			LOG("IDL version mismatch: runtime 0x%08x != user 0x%08x\n", MAIDL_HASH, hash);

			BIG_PHAT_ERROR(ERR_IDL_VERSION);
		}
		return MAIDL_HASH;
	}

#if !defined(_android)
	int Syscall::maBtGetNewDevice(MABtDevice* dst) {
		MABtDeviceNative dn;
		dn.name = (char*)GetValidatedMemRange(dst->name, dst->nameBufSize);
		int res = BLUETOOTH(maBtGetNewDevice)(&dn);
		dst->actualNameLength = dn.actualNameLength;
		dst->address = dn.address;
		return res;
	}

	int Syscall::maBtGetNewService(MABtService* dst) {
		MABtServiceSize ss;
		int res = BLUETOOTH(maBtGetNextServiceSize)(&ss);
		if(res <= 0)
			return res;
		MABtServiceNative sn;
		sn.name = (char*)GetValidatedMemRange(dst->name, dst->nameBufSize);
		sn.uuids = (MAUUID*)GetValidatedMemRange(dst->uuids, ss.nUuids * sizeof(MAUUID));
		res = BLUETOOTH(maBtGetNewService)(&sn);
		dst->port = sn.port;
		return res;
	}

#define FILE_FAIL(val) do { LOG_VAL(val); return val; } while(0)

	static int openFile(Syscall::FileHandle& fh) {
		int res = isDirectory(fh.name);
		if(res < 0) {
			LOGF("File: %s\n", fh.name.p());
		} else if((res > 0) != fh.isDirectory()) {
			FILE_FAIL(MA_FERR_WRONG_TYPE);
		}
		if(fh.mode == MA_ACCESS_READ_WRITE) {
			if(res == 0) {	//file exists and is not a directory
				fh.fs = new WriteFileStream(fh.name, false, true);
			}
		} else if((fh.mode & MA_ACCESS_READ) != 0) {
			if(res == 0) {	//file exists and is not a directory
				fh.fs = new FileStream(fh.name);
			}
		} else {
			BIG_PHAT_ERROR(ERR_INVALID_FILE_ACCESS_MODE);
		}
		if(fh.fs) if(!fh.fs->isOpen()) {
			delete fh.fs;
			fh.fs = NULL;
			FILE_FAIL(MA_FERR_GENERIC);
		}
		return 0;
	}

	MAHandle Syscall::maFileOpen(const char* path, int mode) {
		LOGF("maFileOpen(%s, %x): %i\n", path, mode, gFileNextHandle);
		Smartie<FileHandle> fhs(new FileHandle);
		FileHandle& fh(*fhs);
		fh.mode = mode;
		int size;
		const char* fn;
#if FILESYSTEM_CHROOT
		std::string name = std::string(FILESYSTEM_DIR) + path;
		fn = name.c_str();
		MYASSERT(path[0] == '/', ERR_FILE_PATH_INVALID);
		size = name.length() + 1;
#else
		fn = path;
		size = strlen(path) + 1;
#endif
		fh.name.resize(size);
#ifdef SYMBIAN
		// Switch directory separators.
		for(int i=0; i<size; i++) {
			if(fn[i] == '/') {
				fh.name[i] = '\\';
			} else {
				fh.name[i] = fn[i];
			}
		}
#elif defined(__IPHONE__)
		std::string newPath = fn;
		//fn = newPath.c_str();
		size = newPath.size();
		fh.name.resize(size+1);
		memcpy(fh.name, fn, size);
        fh.name[size] = 0;
        LOG("Opening file: %s\n", newPath.c_str());
#else
		memcpy(fh.name, fn, size);
#endif

		fh.fs = NULL;
		TLTZ_PASS(openFile(fh));
		FileHandle* fhp = fhs.extract();
		CLEANUPSTACK_PUSH(fhp);
		gFileHandles.insert(gFileNextHandle, fhp);
		CLEANUPSTACK_POP(fhp);
		return gFileNextHandle++;
	}

	Syscall::FileHandle& Syscall::getFileHandle(MAHandle file) {
		FileHandle* fhp = gFileHandles.find(file);
		if(!fhp) {
			LOG("Handle: %i\n", file);
		}
		MYASSERT(fhp, ERR_FILE_HANDLE_INVALID);
		return *fhp;
	}

	int Syscall::maFileExists(MAHandle file) {
		LOGF("maFileExists(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		if(fh.fs) {
			LOGF("file is opened.\n");
			return 1;
		}
		int res = isDirectory(fh.name);
		LOGF("isDir: %i\n", res);
		return res >= 0;
	}

	int Syscall::maFileClose(MAHandle file) {
		LOGF("maFileClose(%i)\n", file);
		FileHandle* fhp = gFileHandles.find(file);
		MYASSERT(fhp, ERR_FILE_HANDLE_INVALID);
		FileHandle& fh(*fhp);
		SAFE_DELETE(fh.fs);
		gFileHandles.erase(file);
		return 0;
	}

	int Syscall::maFileCreate(MAHandle file) {
		LOGF("maFileCreate(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		MYASSERT(fh.mode == MA_ACCESS_READ_WRITE, ERR_INVALID_FILE_ACCESS_MODE);
		if(fh.fs) {
			FILE_FAIL(MA_FERR_GENERIC);
		}
		if(fh.isDirectory()) {
#ifdef _WIN32_WCE
			size_t len = strlen(fh.name);
			WCHAR *unicode = new WCHAR[len];
			convertAsciiToUnicode(unicode, len, fh.name);
			int res = _wmkdir(unicode);
			delete unicode;
#else
			int res = _mkdir(fh.name);
#endif
			if(res < 0)
				FILE_FAIL(MA_FERR_GENERIC);
			return 0;
		} else {
			fh.fs = new WriteFileStream(fh.name);
			if(!fh.fs->isOpen()) {
				delete fh.fs;
				fh.fs = NULL;
				FILE_FAIL(MA_FERR_GENERIC);
			}
			return 0;
		}
	}

	int Syscall::maFileDelete(MAHandle file) {
		LOGF("maFileDelete(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		MYASSERT(fh.mode == MA_ACCESS_READ_WRITE, ERR_INVALID_FILE_ACCESS_MODE);
		SAFE_DELETE(fh.fs);
		int res;
#ifdef _WIN32_WCE
		size_t len = strlen(fh.name);
		WCHAR *unicode = new WCHAR[len];
		convertAsciiToUnicode(unicode, len, fh.name);
		if(fh.isDirectory()) {
			res = _wrmdir(unicode);
		} else {
			res = wremove(unicode);
		}
		delete unicode;
#else
		if(fh.isDirectory()) {
			res = _rmdir(fh.name);
		} else {
			res = remove(fh.name);
		}
#endif
		if(res < 0)
			FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}

	int Syscall::maFileSize(MAHandle file) {
		LOGF("maFileSize(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		if(!fh.fs)
			FILE_FAIL(MA_FERR_GENERIC);
		int len;
		bool res = fh.fs->length(len);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		LOGF("file size: %i\n", len);
		return len;
	}

#if !defined(_WIN32_WCE)
	int Syscall::maFileRename(MAHandle file, const char* newName) {
		Syscall::FileHandle& fh(SYSCALL_THIS->getFileHandle(file));
		LOGF("maFileRename(%i, %s)\n", file, newName);

		// close the file while we're renaming.
		bool wasOpen;
		int oldPos;
		if(fh.fs)
			wasOpen = fh.fs->isOpen();
		else
			wasOpen = false;
		if(wasOpen) {
			if(!fh.fs->tell(oldPos)) FILE_FAIL(MA_FERR_GENERIC);
			delete fh.fs;
			fh.fs = NULL;
		}

		bool hasPath = false;
#if defined(WIN32) && !defined(_WIN32_WCE) && !FILESYSTEM_CHROOT
		// If fh.name and newName are on different file systems,
		// forbid the operation.
		if(newName[1] == ':' && toupper(fh.name[0]) != toupper(newName[0])) {
			return MA_FERR_RENAME_FILESYSTEM;
		}
		if(newName[1] == ':' || newName[0] == '/')
			hasPath = true;
#else
		if(strrchr(newName, '/'))
			hasPath = true;
#endif

#ifdef SYMBIAN
#define DIRSEP '\\'
#else
#define DIRSEP '/'
#endif

		Array<char> nn(0);
		if(!hasPath) {
			int oldPathLen = 0;
			const char* lastSlash = strrchr(fh.name, DIRSEP);
			if(lastSlash != NULL) {
				oldPathLen = (lastSlash - fh.name) + 1;
			}
			if(oldPathLen > 0) {
				int newNameLen = strlen(newName);
				nn.resize(oldPathLen + newNameLen + 1);
				memcpy(nn, fh.name, oldPathLen);
#ifdef SYMBIAN
				char* dst = nn + oldPathLen;
				const char* src = newName;
				// Switch directory separators.
				while(*src) {
					if(*src == '/') {
						*dst = '\\';
					} else {
						*dst = *src;
					}
					src++;
					dst++;
				}
				*dst = 0;
#else
				strcpy(nn + oldPathLen, newName);
#endif
				newName = nn;
			}
		}
#if FILESYSTEM_CHROOT
		else {
			const char* fsd = FILESYSTEM_DIR;
			int fsdLen = strlen(fsd);
			int newNameLen = strlen(newName);
			nn.resize(fsdLen + newNameLen + 1);
			memcpy(nn, fsd, fsdLen);
			strcpy(nn + fsdLen, newName);
			newName = nn;
		}
#endif
		int res = rename(fh.name, newName);
		if(res != 0) {
#ifdef SYMBIAN
			int err = res;
#else
			int err = errno;
#endif
			if(err == EXDEV)
				FILE_FAIL(MA_FERR_RENAME_FILESYSTEM);
			else if(err == EACCES)
				FILE_FAIL(MA_FERR_FORBIDDEN);
			else
				FILE_FAIL(MA_FERR_GENERIC);
		}
		fh.name.resize(strlen(newName) + 1);
		strcpy(fh.name, newName);

		if(!wasOpen)
			return 0;
		TEST_LTZ(openFile(fh));
		if(!fh.fs->seek(Seek::Start, oldPos)) FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}
#endif	//_WIN32_WCE

#if !defined(SYMBIAN) && !defined(_WIN32_WCE)

#ifdef WIN32
	static int fileSpace(MAHandle file, unsigned _diskfree_t::* clusters) {
		_diskfree_t df;
		unsigned drive;
#if FILESYSTEM_CHROOT
		SYSCALL_THIS->getFileHandle(file);	// just to make sure the file handle is ok.
		drive = 0;
#else
		Syscall::FileHandle& fh(SYSCALL_THIS->getFileHandle(file));
		drive = (fh.name[0] - 'A') + 1;
#endif	//FILESYSTEM_CHROOT
		if(_getdiskfree(drive, &df) != 0) {
			LOG("_getdiskfree errno %i\n", errno);
			FILE_FAIL(MA_FERR_GENERIC);
		}
		return (int)MIN(df.bytes_per_sector * df.sectors_per_cluster * (u64)(df.*clusters), INT_MAX);
	}
#define SPACE_AVAIL &_diskfree_t::avail_clusters
#define SPACE_TOTAL &_diskfree_t::total_clusters

#else	//!WIN32

	static int fileSpace(MAHandle file, fsblkcnt_t statvfs::* clusters) {
		Syscall::FileHandle& fh(SYSCALL_THIS->getFileHandle(file));
		struct statvfs s;
		if(statvfs(fh.name, &s) != 0) {
			LOG("statvfs errno %i\n", errno);
			FILE_FAIL(MA_FERR_GENERIC);
		}
		return (int)MIN(s.f_frsize * s.*clusters, INT_MAX);
	}
#define SPACE_AVAIL &statvfs::f_bavail
#define SPACE_TOTAL &statvfs::f_blocks
#endif	//WIN32

	int Syscall::maFileAvailableSpace(MAHandle file) {
		return fileSpace(file, SPACE_AVAIL);
	}

	int Syscall::maFileTotalSpace(MAHandle file) {
		return fileSpace(file, SPACE_TOTAL);
	}

	int Syscall::maFileDate(MAHandle file) {
		LOGF("maFileDate(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		time_t t;
		if(fh.fs) {
			if(!fh.fs->isOpen()) FILE_FAIL(MA_FERR_GENERIC);
			if(!fh.fs->mTime(t)) FILE_FAIL(MA_FERR_GENERIC);
		} else {
			const char* statName = fh.name;
			Array<char> temp(0);
			if(fh.name[fh.name.size()-2] == '/') {
				// would cause stat() to fail. get rid of the slash.
				//temp.assign(fh.name, fh.name.size()-2);
				temp.resize(fh.name.size()-1);
				memcpy(temp, fh.name, fh.name.size()-2);
				temp[fh.name.size()-2] = 0;
				statName = temp;
			}
			struct _stat st;
			if(_stat(statName, &st) != 0) {
				LOG("maFileDate:stat(%s) failed. errno: %i(%s)\n", statName, errno, strerror(errno));
				FILE_FAIL(MA_FERR_GENERIC);
			}
			t = st.st_mtime;
		}
		LOGF("mtime: %i\n", (int)t);
		return (int)t;
	}

	int Syscall::maFileTruncate(MAHandle file, int offset) {
		LOGF("maFileTruncate(%i, %i)\n", file, offset);
		FileHandle& fh(getFileHandle(file));
		if(!fh.fs) FILE_FAIL(MA_FERR_GENERIC);
		if(!fh.fs->isOpen()) FILE_FAIL(MA_FERR_GENERIC);
		if(!fh.fs->truncate(offset)) FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}
#endif	//SYMBIAN && _WIN32_WCE

	int Syscall::maFileWrite(MAHandle file, const void* src, int len) {
		LOGF("maFileWrite(%i, 0x%"PFP", %i)\n", file, src, len);
		FileHandle& fh(getFileHandle(file));
		if(!fh.fs)
			FILE_FAIL(MA_FERR_GENERIC);
		bool res = fh.fs->write(src, len);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}

	int Syscall::maFileWriteFromData(MAHandle file, MAHandle data, int offset, int len) {
		LOGF("maFileWriteFromData(%i, %i)\n", file, len);
		FileHandle& fh(getFileHandle(file));
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		MYASSERT(b->seek(Seek::Start, offset), ERR_DATA_OOB);
		//todo: add ERR_DATA_OOB check for length.
		if(!fh.fs)
			FILE_FAIL(MA_FERR_GENERIC);
		bool res = fh.fs->writeStream(*b, len);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}

	int Syscall::maFileRead(MAHandle file, void* dst, int len) {
		LOGF("maFileRead(%i, 0x%"PFP", %i)\n", file, dst, len);
		FileHandle& fh(getFileHandle(file));
		if(!fh.fs)
			FILE_FAIL(MA_FERR_GENERIC);
		bool res = fh.fs->read(dst, len);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}

	int Syscall::maFileReadToData(MAHandle file, MAHandle data, int offset, int len) {
		LOGF("maFileReadToData(%i, %i)\n", file, len);
		FileHandle& fh(getFileHandle(file));
		Stream* b = SYSCALL_THIS->resources.get_RT_BINARY(data);
		MYASSERT(b->seek(Seek::Start, offset), ERR_DATA_OOB);
		//todo: add ERR_DATA_OOB check for length.
		if(!fh.fs)
			FILE_FAIL(MA_FERR_GENERIC);
		bool res = b->writeStream(*fh.fs, len);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return 0;
	}

	int Syscall::maFileTell(MAHandle file) {
		LOGF("maFileTell(%i)\n", file);
		FileHandle& fh(getFileHandle(file));
		MYASSERT(fh.fs, ERR_FILE_CLOSED);
		int pos;
		bool res = fh.fs->tell(pos);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return pos;
	}

	int Syscall::maFileSeek(MAHandle file, int offset, int whence) {
		LOGF("maFileSeek(%i, %i, %i)\n", file, offset, whence);
		FileHandle& fh(getFileHandle(file));
		MYASSERT(fh.fs, ERR_FILE_CLOSED);
		Seek::Enum mode;
		switch(whence) {
		case MA_SEEK_SET: mode = Seek::Start; break;
		case MA_SEEK_CUR: mode = Seek::Current; break;
		case MA_SEEK_END: mode = Seek::End; break;
		default: BIG_PHAT_ERROR(ERR_INVALID_FILE_SEEK_MODE);
		}
		bool res = fh.fs->seek(mode, offset);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		int pos;
		res = fh.fs->tell(pos);
		if(!res)
			FILE_FAIL(MA_FERR_GENERIC);
		return pos;
	}

#ifndef SYMBIAN
	static FileList sFileList;
	static std::string sFileListRealDir;
#ifndef _WIN32_WCE
	static int sFileListSorting;
#ifdef _WIN32
	typedef ino_t _ino_t;
#endif
#endif	//_WIN32_WCE

	static void fileListCallback(const char* filename) {
		if(!strcmp(filename, ".") || !strcmp(filename, ".."))
			return;
		std::string fn = filename;
		if(isDirectory((sFileListRealDir + fn).c_str())) {
			fn += "/";
		}
		struct FileListItem fli;
		fli.name = fn;
#ifdef _WIN32_WCE
		sFileList.files.push_back(fli);
#else
		fli.sorting = sFileListSorting;
		if(sFileListSorting == MA_FL_SORT_NONE)
			// hack: we store an ordinal in st_ino.
			// looks like it would break on Windows if a directory has
			// more than 65k files. I don't care to fix it.
			fli.s.st_ino = (ino_t)sFileList.files.size();
		else
			stat(fn.c_str(), &fli.s);

		sFileList.files.insert(fli);
#endif	//_WIN32_WCE
	}

#ifndef _WIN32_WCE
	bool FileListItem::operator<(const FileListItem& o) const {
		DEBUG_ASSERT(sorting == o.sorting);
		if(sorting == MA_FL_SORT_NONE)
			return s.st_ino < o.s.st_ino;
		int sortType = sorting & 0xFFFF;
		int sortOrder = sorting & 0xFFFF0000;
		bool sortDesc;
		switch(sortOrder) {
		case MA_FL_ORDER_ASCENDING:
			sortDesc = false;
			break;
		case MA_FL_ORDER_DESCENDING:
			sortDesc = true;
			break;
		default:
			BIG_PHAT_ERROR(ERR_FILE_LIST_SORT);
		}
		switch(sortType) {
		case MA_FL_SORT_DATE:
			if(sortDesc)
				return s.st_mtime > o.s.st_mtime;
			else
				return s.st_mtime < o.s.st_mtime;
		case MA_FL_SORT_SIZE:
			if(sortDesc)
				return s.st_size > o.s.st_size;
			else
				return s.st_size < o.s.st_size;
		case MA_FL_SORT_NAME:
			if(sortDesc)
				return stricmp(name.c_str(), o.name.c_str()) < 0;
			else
				return stricmp(name.c_str(), o.name.c_str()) > 0;
		default:
			BIG_PHAT_ERROR(ERR_FILE_LIST_SORT);
		}
	}
#endif	//_WIN32_WCE

	// if this is MoRE, the emulator,
	// we'll put all filesystem access into a separate directory, like chroot.
	MAHandle Syscall::maFileListStart(const char* path, const char* filter, int sorting) {
		LOGF("maFileListStart(%s, %s, 0x%x)\n", path, filter, sorting);
#ifndef _WIN32_WCE
		sFileListSorting = sorting;
#endif
		sFileList.files.clear();
		if(path[0] == 0) {	//empty string
			//list filesystem roots
#if FILESYSTEM_CHROOT || defined(LINUX) || defined(__IPHONE__) || defined(_WIN32_WCE)
			FileListItem fli;
			fli.name = "/";
#ifdef _WIN32_WCE
			sFileList.files.push_back(fli);
#else
			fli.sorting = MA_FL_SORT_NONE;
			sFileList.files.insert(fli);
#endif	//_WIN32_WCE
#else	//FILESYSTEM_CHROOT
#ifdef WIN32
			FileListItem fli;
			DWORD res = GetLogicalDrives();
			GLE(res);
			char buf[] = "X:/";
			for(int i=0; i<32; i++) {
				if((res & (1 << i)) != 0) {
					buf[0] = 'A' + i;
					fli.name = buf;
					fli.s.st_ino = i;
					sFileList.files.insert(fli);
				}
			}
#else
#error Unsupported platforms
#endif	//WIN32
#endif	//FILESYSTEM_CHROOT || defined(LINUX)
		} else {
			//list files in a directory
			std::string scanPath;
#if FILESYSTEM_CHROOT
			scanPath = FILESYSTEM_DIR;
			_mkdir(FILESYSTEM_DIR);
#endif
			scanPath += path;
			if(scanPath[scanPath.size()-1] != '/')
				scanPath += "/";
			sFileListRealDir = scanPath;
			scanPath += filter;
			int res = scanDirectory(scanPath.c_str(), fileListCallback);
			if(res) {
				LOG("scanDirectory failed: %i\n", res);
				FILE_FAIL(MA_FERR_GENERIC);
			}
		}
		std::pair<FileListItr, bool> ires = sFileListings.insert(
			std::pair<int, FileList>(sFileListNextHandle, sFileList));
		DEBUG_ASSERT(ires.second);
		FileList& fl(ires.first->second);
		fl.itr = fl.files.begin();
		return sFileListNextHandle++;
	}

	int Syscall::maFileListNext(MAHandle list, char* nameBuf, int bufSize) {
		FileListItr itr = sFileListings.find(list);
		MYASSERT(itr != sFileListings.end(), ERR_FILE_HANDLE_INVALID);
		FileList& fl(itr->second);
		if(fl.itr == fl.files.end())
			return 0;
		const std::string& name(fl.itr->name);
		if((int)name.size() >= bufSize)
			return name.size();
		memcpy(nameBuf, name.c_str(), name.size() + 1);
		fl.itr++;
		return name.size();
	}

	int Syscall::maFileListClose(MAHandle list) {
		FileListItr itr = sFileListings.find(list);
		MYASSERT(itr != sFileListings.end(), ERR_FILE_HANDLE_INVALID);
		sFileListings.erase(itr);
		return 0;
	}
#endif	// NOT SYMBIAN
#endif // NOT _android

#ifdef SYMBIAN
#if 0
void chrashTestDummy() {
}
#else
//see recompiler_core.h, RunArm()

#ifdef __SERIES60_3X__
_LIT(KLogFilePath, "C:\\data\\msrlog.txt");
_LIT(KOldLogFilePath, "C:\\data\\msrlogold.txt");
#else	//Series 60, 2nd Ed.
_LIT(KLogFilePath, "C:\\msrlog.txt");
_LIT(KOldLogFilePath, "C:\\msrlogold.txt");
#endif	//__SERIES60_3X__

void chrashTestDummyV(const char* fmt, VA_LIST args) {
	//TBuf8<512> buffer;
	//TPtrC8 fmtP(CBP fmt);
	//buffer.FormatList(fmtP, args);
	//AppendToFile(KLogFilePath, buffer);

#if 0	//enable to avoid crash
	MyRFs myrfs;
	myrfs.Connect();
#endif

	/*RFile file;
	PanicIfError(file.Open(FSS, aFileName,
		EFileWrite | EFileShareAny | EFileStreamText));
	TInt pos = 0;
	PanicIfError(file.Seek(ESeekEnd, pos));
	PanicIfError(file.Write(aText));
	file.Close();*/
}
#endif	// 0
#endif	//SYMBIAN
