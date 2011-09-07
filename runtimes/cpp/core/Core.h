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

#ifndef CORE_H
#define CORE_H

#include "Platform.h"

#if !defined(_WIN32) && !defined(SYMBIAN)
#include <stdint.h>
#include <sys/time.h>
typedef union _LARGE_INTEGER {
	struct {
		uint32_t LowPart;
		int32_t HighPart;
	}   ;
	struct {
		uint32_t LowPart;
		int32_t HighPart;
	}   u;
	int64_t QuadPart;
} LARGE_INTEGER;
#endif

#include <helpers/types.h>
#include <helpers/helpers.h>

#ifdef _android
#include <jni.h>

typedef unsigned short wchar;
#endif

#define USE_VAR_INT

#ifdef GDB_DEBUG
class GdbStub;
#include "GdbCommon.h"
#endif

namespace Base {
#ifdef MOBILEAUTHOR
#define Syscall DeimosSyscall
#endif
	class Syscall;
	class Stream;
}

namespace Core {
	using namespace Base;

#include "CoreCommon.h"

	class VMCore {
	public:

		typedef struct _MA_HEAD
		{
			int Magic;
			int CodeLen;
			int DataLen;
			int DataSize;
			int StackSize;
			int HeapSize;
			int AppCode;
			int AppID;
			int EntryPoint;
			int IntLen;
		} MA_HEAD;


		//****************************************
		//			Static variables
		//****************************************

		MA_HEAD	Head;

		/*#define CODE_SEGMENT_SIZE (64*1024)
		#define CODE_SEGMENT_MASK (64*1024 - 1)
		#define DATA_SEGMENT_SIZE (64*1024)
		#define DATA_SEGMENT_MASK (64*1024 - 1)*/

		uint CODE_SEGMENT_SIZE;
#define CODE_SEGMENT_MASK (CODE_SEGMENT_SIZE - 1)

		uint DATA_SEGMENT_SIZE;
#define DATA_SEGMENT_MASK (DATA_SEGMENT_SIZE - 1)

		uint STACK_TOP;
		uint STACK_BOTTOM;

		byte* mem_cs;
		int* mem_ds;
		int* mem_cp;
		
#if defined(_android)
		JNIEnv* mJniEnv;
		jobject mJThis;
		jobject mem_ds_jobject;
#endif

#ifdef MEMORY_PROTECTION
		byte* protectionSet;
		int protectionEnabled;
#endif

#if defined(SYMBIAN) && defined(SUPPORT_RELOAD)
		int symbianError;
#endif

#ifdef TRACK_SYSCALL_ID
		int currentSyscallId;
#endif
		int regs[128];

#ifdef GDB_DEBUG
		GdbStub *mGdbStub;
		bool mGdbOn;
		GdbSignal mGdbSignal;	//used for exec-interrupt and exec-step
#endif

		VMCore();
		virtual ~VMCore();

#ifdef LOG_STATE_CHANGE
		void logStateChange(int ip);
#endif
		void invokeSysCall(int id);
		
	};

	class Application;

	const char* CurrentSyscallString(VMCore* core, char* buffer, int bufSize);

	//for main
	VMCore* CreateCore(Syscall&);
	void DeleteCore(VMCore* core);
	int GetIp(const VMCore* core);	//only valid if VM is not Running, or UPDATE_IP is defined
	void SetIp(VMCore* core, int ip);
	int GetFakeCallStackDepth(const VMCore* core);
	const int* GetFakeCallStack(const VMCore* core);

	//returns false on failure
#ifdef MOBILEAUTHOR
	void RunFrom(VMCore* core, int ip);
#else
#ifndef _android
	bool LoadVMApp(VMCore* core, const char* modfile, const char* resfile);
#else
	bool LoadVMApp(VMCore* core, int modFd, int resFd);
#endif
#endif
	bool LoadVMApp(VMCore* core, Stream& stream, const char* combfile=0);
	void Run2(VMCore* core);


	//for debugger
#ifdef ENABLE_DEBUGGER
	bool initDebugger(VMCore* core, int port);
	void closeDebugger();
#endif

#ifdef GDB_DEBUG
	void Step(VMCore*);
#endif

	//for syscall
	int& GetVMYield(VMCore* core);
	void ValidateMemRange(VMCore* core, const void* ptr, int size);
	int ValidatedStrLen(const VMCore* core, const char* ptr);

	//for ioctl
	void* GetValidatedMemRange(VMCore* core, int address, int size);
	int GetValidatedStackValue(VMCore* core, int offset);
	int TranslateNativePointerToMoSyncPointer(VMCore* core, void* ptr);
	const char* GetValidatedStr(const VMCore* core, int address);
	const wchar* GetValidatedWStr(const VMCore* core, int address);
	void* GetCustomEventPointer(VMCore* core);
	
}

#ifndef SYMBIAN
extern Core::VMCore* gCore;
#endif

#endif	//CORE_H
