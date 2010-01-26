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

#include "Syscall.h"
#include "FileStream.h"
#include "MemStream.h"
#include <helpers/smartie.h>
#include <filelist/filelist.h>

#ifdef IX_FILE
#include <helpers/CPP_IX_FILE.h>
#ifdef WIN32
#include <windows.h>
#endif	//WIN32
#endif	//IX_FILE

#ifdef _WIN32_WCE
//#include <windows.h>
#include "wce_helpers.h"
#elif !defined(SYMBIAN)
#include "helpers/mkdir.h"
#endif	//_WIN32_WCE

using namespace Base;

namespace Base {

#ifdef RESOURCE_MEMORY_LIMIT
	uint size_RT_FLUX(void* size) {
		return (int)size;
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

#ifndef SYMBIAN
	int gStoreId = 1;
	typedef std::map<int, std::string> StoreMap;
	typedef StoreMap::iterator StoreItr;
	StoreMap gStores;

#ifdef IX_FILE
	struct FileList {
		std::vector<std::string> files;
		int pos;
	};
	typedef std::map<int, FileList> FileListMap;
	typedef FileListMap::iterator FileListItr;
	static FileListMap sFileListings;
	static int sFileListNextHandle = 1;

	typedef std::map<int, FileStream*> FileMap;
	typedef FileMap::iterator FileItr;
	static FileMap sFileHandles;
	static int sFileNextHandle = 1;

#ifdef EMULATOR
#define FILESYSTEM_CHROOT 1
#define FILESYSTEM_DIR "filesystem"
#else
#define FILESYSTEM_CHROOT 0
#endif	//EMULATOR
#endif	//IX_FILE
#endif	//SYMBIAN

	void Syscall::init() {
	}

	Syscall::~Syscall() {
		LOGD("~Syscall\n");
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
					MemStream* ms = new MemStream(size);
					TEST(file.readFully(*ms));
					ROOM(resources.dadd_RT_BINARY(rI, ms));
				}
				break;
			case RT_UBIN:
				{
					int pos;			
					MYASSERT(aFilename, ERR_RES_LOAD_UBIN);
					TEST(file.tell(pos));
					ROOM(resources.dadd_RT_BINARY(rI,
						new LimitedFileStream(aFilename, pos, size)));
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
					ROOM(resources.dadd_RT_IMAGE(rI, loadImage(b)));
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
					ROOM(resources.dadd_RT_IMAGE(rI, loadSprite(resources.get_RT_IMAGE(indexSource),
						left, top, width, height, cx, cy)));
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
		do {
			*(dst++) = *(src);
		} while(*src++);
		return dst;
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

#if 1//!(defined(SYMBIAN) && !defined(__SERIES60_3X__))	//S60v2

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
		if(b == 0)
			BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
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

	SYSCALL(double, f2d(float f)) {
		return (double)f;
	}
	SYSCALL(int, dcmp(double a, double b)) {
		if(a > b)
			return 1;
		else if(a == b)
			return 0;
		else	//a < b		//or NaN!
			return -1;
	}

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
		if(b == 0)
			BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
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
	SYSCALL(float, d2f(double a)) {
		return (float)a;
	}
	SYSCALL(int, fcmp(float a, float b)) {
		if(a > b)
			return 1;
		else if(a == b)
			return 0;
		else	//a < b		//or NaN!
			return -1;
	}
#endif	//S60v2

	SYSCALL(MAHandle, maCreatePlaceholder()) {
		return (MAHandle) SYSCALL_THIS->resources.create_RT_PLACEHOLDER();
	}

	SYSCALL(void, maDestroyObject(MAHandle handle)) {
		SYSCALL_THIS->resources.destroy(handle);
	}

	SYSCALL(int, maCreateData(MAHandle placeholder, int size)) {
		MemStream* ms = new MemStream(size);
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
		MYASSERT(dst->write(src, a->size), ERR_DATA_OOB);
	}


#ifndef SYMBIAN
#define STORE_PATH "stores/"
	bool storeDirectoryCreated = false;
	SYSCALL(MAHandle, maOpenStore(const char* name, int flags)) 
	{

		if(storeDirectoryCreated == false) {
#ifdef _WIN32_WCE 
			char temp[256];
			TCHAR wtemp[256];
			getWorkingDirectory(temp, 256);
			strcat(temp, "\\");
			strcat(temp, STORE_PATH);
			convertAsciiToUnicode(wtemp, 256, temp);
			CreateDirectory(wtemp, NULL);
#else
			_mkdir(STORE_PATH);
#endif
			storeDirectoryCreated = true;
		}

		std::string newPath = STORE_PATH + std::string(name);

		FileStream readFile(newPath.c_str());
		if(!readFile.isOpen())
		{
			if(flags & MAS_CREATE_IF_NECESSARY)
			{
				WriteFileStream writeFile(newPath.c_str());
				if(!writeFile.isOpen())
					return STERR_GENERIC;
			}
			else
			{
				return STERR_NONEXISTENT;
			}
		}

		gStores[gStoreId] = newPath;
		return gStoreId++;
	}

	SYSCALL(int, maWriteStore(MAHandle store, MAHandle data)) 
	{
		StoreItr iter = gStores.find(store);

		if(iter == gStores.end()) {
			BIG_PHAT_ERROR(ERR_STORE_HANDLE_INVALID);
		}

		WriteFileStream writeFile((*iter).second.c_str());
		Stream* b = gSyscall->resources.get_RT_BINARY(data);
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
		StoreItr iter = gStores.find(store);

		if(iter == gStores.end()) {
			BIG_PHAT_ERROR(ERR_STORE_HANDLE_INVALID);
		}

		FileStream readFile((*iter).second.c_str());
		int len;
		MYASSERT(readFile.length(len), ERR_STORE_READ_FAILED);
		Smartie<MemStream> b(new MemStream(len));
		if(!readFile.readFully(*b))
		{
			BIG_PHAT_ERROR(ERR_STORE_READ_FAILED);
		}
		return gSyscall->resources.add_RT_BINARY(placeholder, b.extract());
	}
#endif	//SYMBIAN


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
		for(unsigned i = 0; i < SYSCALL_THIS->resources.size(); i++) {
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

	int Base::maCheckInterfaceVersion(int hash) {
		if(hash == (int)MAIDL_HASH) {
			LOG("IDL version match!\n");
		} else {
			LOG("IDL version mismatch: runtime 0x%08x != user 0x%08x\n", MAIDL_HASH, hash);
			BIG_PHAT_ERROR(ERR_IDL_VERSION);
		}
		return MAIDL_HASH;
	}

	int Syscall::maBtGetNewDevice(MABtDevice* dst) {
		char* vmName = dst->name;
		dst->name = (char*)GetValidatedMemRange((int)dst->name, dst->nameBufSize);
		int res = BLUETOOTH(maBtGetNewDevice)(dst);
		dst->name = vmName;
		return res;
	}

	int Syscall::maBtGetNewService(MABtService* dst) {
		MABtServiceSize ss;
		int res = BLUETOOTH(maBtGetNextServiceSize)(&ss);
		if(res <= 0)
			return res;
		char* vmName = dst->name;
		MAUUID* vmUuids = dst->uuids;
		dst->name = (char*)GetValidatedMemRange((int)dst->name, dst->nameBufSize);
		dst->uuids = (MAUUID*)GetValidatedMemRange((int)dst->uuids, ss.nUuids * sizeof(MAUUID));
		res = BLUETOOTH(maBtGetNewService)(dst);
		dst->name = vmName;
		dst->uuids = vmUuids;
		return res;
	}

#ifdef IX_FILE

	/*constset int MA_ACCESS_ {
		READ = 1;
		WRITE = 2;
		READ_WRITE = 3;
	}*/

#if 0
	MAHandle maFileOpen(const char* path, int mode) {
	}

	int maFileExists(MAHandle file) {
	}

	int maFileClose(MAHandle file) {
	}

	int maFileCreate(MAHandle file) {
	}

	int maFileDelete(MAHandle file) {
	}
	int maFileSize(MAHandle file);
	int maFileAvailableSpace(MAHandle file);
	int maFileTotalSpace(MAHandle file);
	int maFileDate(MAHandle file);
	int maFileRename(MAHandle file, const char* newName);
	int maFileTruncate(MAHandle file, int offset);

	int maFileWrite(MAHandle file, const void* src, int len) {
	}

	/*struct MA_FILE_DATA {
		MAHandle file;
		MAHandle data;
		int offset;
		int len;
	}*/

	int maFileWriteFromData(const MA_FILE_DATA* args) {
	}

	int maFileRead(MAHandle file, void* dst, int len) {
	}

	int maFileReadToData(const MA_FILE_DATA* args) {
	}

	int maFileTell(MAHandle file);

	int maFileSeek(MAHandle file, int offset, int whence);

	/*constset int MA_SEEK_ {
		/// Beginning of file.
		SET = 0;
		/// Current position.
		CUR = 1;
		/// End of file.
		END = 2;
	}*/
#endif

#ifndef SYMBIAN
	static FileList sFileList;
	static std::string sFileListRealDir;

	static void fileListCallback(const char* filename) {
		if(!strcmp(filename, ".") || !strcmp(filename, ".."))
			return;
		std::string fn = filename;
		if(isDirectory((sFileListRealDir + fn).c_str())) {
			fn += "/";
		}
		sFileList.files.push_back(fn);
	}

	// if this is MoRE, the emulator,
	// we'll put all filesystem access into a separate directory, like chroot.
	MAHandle maFileListStart(const char* path, const char* filter) {
		sFileList.files.clear();
		sFileList.pos = 0;
		if(path[0] == 0) {	//empty string
			//list filesystem roots
#if FILESYSTEM_CHROOT || defined(LINUX)
			sFileList.files.push_back("/");
#else
#ifdef WIN32
			DWORD res = GetLogicalDrives();
			GLE(res);
			char buf[] = "X:/";
			for(int i=0; i<32; i++) {
				if((res & (1 << i)) != 0) {
					buf[0] = 'A' + i;
					sFileList.files.push_back(buf);
				}
			}
#else
#error list them roots!
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
			if(res)
				return MA_FLERR_GENERIC;
		}
		std::pair<FileListItr, bool> ires = sFileListings.insert(
			std::pair<int, FileList>(sFileListNextHandle, sFileList));
		DEBUG_ASSERT(ires.second);
		return sFileListNextHandle++;
	}

	int maFileListNext(MAHandle list, char* nameBuf, int bufSize) {
		FileListItr itr = sFileListings.find(list);
		MYASSERT(itr != sFileListings.end(), ERR_FILE_HANDLE_INVALID);
		FileList& fl(itr->second);
		if(fl.pos == fl.files.size())
			return 0;
		const std::string& name(fl.files[fl.pos]);
		if((int)name.size() >= bufSize)
			return name.size();
		memcpy(nameBuf, name.c_str(), name.size() + 1);
		fl.pos++;
		return name.size();
	}

	int maFileListClose(MAHandle list) {
		FileListItr itr = sFileListings.find(list);
		MYASSERT(itr != sFileListings.end(), ERR_FILE_HANDLE_INVALID);
		sFileListings.erase(itr);
		return 0;
	}
#endif	//SYMBIAN
#endif	//IX_FILE

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
