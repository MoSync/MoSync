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

//*********************************************************************************************
//				    PIP-e II Loader Execution Engine
// 				   Copyright  Mobile Sorcery AB 2005
// 					Written by A.R.Hartley
// 				     Modified for Symbian by P.F.Eldh
//			This code is based on the GameTasker v2.0 Module System
//*********************************************************************************************

//#define LOG_STATE_CHANGE	//TEMP HACK
//#define DEBUG_DISASM
//#define CORE_DEBUGGING_MODE
//#define SYSCALL_DEBUGGING_MODE

#include <config_platform.h>
#include <Platform.h>
#include <helpers/helpers.h>
#include <helpers/maapi_defs.h>

#include <base/FileStream.h>

#include "helpers/TranslateSyscall.h"
//#undef LOGC
//#define LOGC(...) do { if(InstCount > 8200000) LOG(__VA_ARGS__); } while(0)

#ifdef GDB_DEBUG
#define UPDATE_IP
/*
#include "debugger.h"
*/
#include "GdbStub.h"
#include "GdbCommon.h"
#endif

#include <base/base_errors.h>
using namespace MoSyncError;

#ifndef MOBILEAUTHOR
#include <base/Syscall.h>
#else
#include <Deimos/NodeManager.h>
#include <Deimos/DeimosSyscall.h>
#define Syscall DeimosSyscall
#endif

#ifdef MEMORY_DEBUG
#include "disassembler.h"
#endif

#ifdef COUNT_INSTRUCTION_USE
#include <algorithm>
#include <vector>
#endif

#include <config_platform.h>
#if defined(USE_ARM_RECOMPILER)
/*
#include "disassembler.h"
#include "recompiler.h"
#include <MAUtil/Vector.h>
*/

#include "Recompiler/ArmRecompiler.h"

#if 0
void chrashTestDummy();
#else
//implementation must be outside of this module to protect the calls from the optimizer
void chrashTestDummyV(const char* fmt, VA_LIST vaList);

__inline void chrashTestDummy(const char* fmt, ...) {
	VA_LIST argptr;
	VA_START(argptr, fmt);
	chrashTestDummyV(fmt, argptr);
}
#endif	// 1
#endif	// USE_ARM_RECOMPILER

#include "Core.h"

#if defined (FAKE_CALL_STACK)
#include "sld.h"
#endif

#ifdef __SYMBIAN32__
#undef _WIN32
#include "my_stdlib.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef FAKE_CALL_STACK
#include <vector>
#endif

namespace Core {

using namespace Base;

void InvokeSysCall(int id);

class VMCoreInt : public VMCore {
public:

#ifdef LOG_STATE_CHANGE
	int csRegs[128];
	int *csMem;

	struct StateChange { int ip, instCount, reg, before, after; };

#define STATE_BUFFER_SIZE 1024
	StateChange stateChanges[STATE_BUFFER_SIZE];
	int curStateChange;
	
	void initStateChange() {
		csMem = 0;
		curStateChange = 0;
	}

	void freeStateChange() {
		if(csMem) delete []csMem;
	}

	int stateChangeInstCount;
	bool stateChangeAppend;

	void writeStateChanges() {
#ifdef SYMBIAN
#define STATE_LOG_DIR "C:\\data"
#else
#define STATE_LOG_DIR ""
#endif
#ifdef USE_ARM_RECOMPILER
		Base::WriteFileStream stateLog(STATE_LOG_DIR "arm_recompiler_state_change.txt", stateChangeAppend);
#else
		Base::WriteFileStream stateLog(STATE_LOG_DIR "arm_vm_state_change.txt", stateChangeAppend);
#endif
		stateChangeAppend = true;

		//stateLog.write(stateChanges, STATE_BUFFER_SIZE*sizeof(StateChange));
		for(int i = 0; i < curStateChange; i++) {
			int len;
			//if(stateChanges[i].reg == 0xffffffff) {
			//	len = sprintf(temp, "%d, %x\n", stateChanges[i].before, stateChanges[i].ip);
			//} else {
#ifdef SYMBIAN
			TBuf8<1024> buf;
			char* temp = (char*)buf.Ptr();
			_LIT8(KFmt, "%x, %d: REG%d: %x != %x\n");
			buf.Format(KFmt,
#else
			char temp[1024];
			len = sprintf(temp, "%x, %d: REG%d: %x != %x\n",
#endif
				stateChanges[i].ip, stateChanges[i].instCount,
				stateChanges[i].reg, stateChanges[i].before, stateChanges[i].after);
			//}
#ifdef SYMBIAN
			len = buf.Length();
#endif
			stateLog.write(temp, len);
		}

		curStateChange = 0;
	}

	void logStateChange(int ip) {
		//LOG("logStateChange(0x%x)\n", ip);
		
		if(!csMem) {
			curStateChange = 0;
			//csMem = new int[gCore->DATA_SEGMENT_SIZE>>2];
			csMem = (int*)1;
			for(unsigned int i = 0; i < 128; i++) {
				csRegs[i] = regs[i];
			}
			//for(unsigned int i = 0; i < gCore->DATA_SEGMENT_SIZE>>2; i++) {
			//	csMem[i] = gCore->mem_ds[i];
			//}

			stateChangeInstCount = 0;
			stateChangeAppend = false;
		}
		stateChangeInstCount++;

		//LOG("\nIP: %x\n", ip);

		unsigned int i = 128;
		while(i--) {
			if(csRegs[i] != regs[i]) {
				//LOG("R%d: 0x%x != %d\n", i, csRegs[i], gCore->regs[i]);
				
				stateChanges[curStateChange].ip = ip;
				stateChanges[curStateChange].instCount = stateChangeInstCount;
				stateChanges[curStateChange].reg = i;
				stateChanges[curStateChange].before = csRegs[i];
				stateChanges[curStateChange].after = regs[i];
				curStateChange++;
				if(curStateChange>=STATE_BUFFER_SIZE) {
					writeStateChanges();
				}

				csRegs[i] = regs[i];
			}
		}
		/*
		for(unsigned int i = 0; i < gCore->DATA_SEGMENT_SIZE>>2; i++) {
			if(csMem[i] != gCore->mem_ds[i]) {
				LOG("%x: %x\n", i, gCore->mem_ds[i]);
				csMem[i] = gCore->mem_ds[i];
			}
		}
		*/
		//LOG("logStateChange ends\n");
	}
#endif

#define _DBG_OP _ENDOP

	uint IP;
	byte* rIP;

	int VM_Yield;
	
	void* customEventPointer;

#ifdef USE_ARM_RECOMPILER
	MoSync::ArmRecompiler recompiler;
#endif

#ifdef MEMORY_DEBUG
	int InstCount;
#endif

#ifdef INSTRUCTION_PROFILING
	int* instruction_count;
#endif

#ifdef FAKE_CALL_STACK
	int* fakeCallStack;
	int fakeCallStackDepth;	//measured in ints
	int fakeCallStackCapacity;	//measured in ints

#ifdef FUNCTION_PROFILING
	class ProfTree {
	private:
		class ProfTime {
		private:
			LARGE_INTEGER li;
		public:
			static ProfTime now() {
				ProfTime n;
#ifdef _WIN32
				DEBUG_ASSERT(QueryPerformanceCounter(&n.li));
#else
				struct timeval tv;
				DEBUG_ASSERT(!gettimeofday(&tv, NULL));
				n.li.QuadPart = (int64_t) tv.tv_sec * 1000000 + (int64_t) tv.tv_usec;
#endif
				return n;
			}
			ProfTime() {
				li.QuadPart = 0;
			}
			ProfTime(const ProfTime& other) : li(other.li) {}
			ProfTime& operator+=(const ProfTime& other) {
				li.QuadPart += other.li.QuadPart;
				return *this;
			}
			ProfTime operator-(const ProfTime& other) {
				ProfTime t;
				t.li.QuadPart = li.QuadPart - other.li.QuadPart;
				return t;
			}
			double toMilliSeconds() {
#ifdef _WIN32
				LARGE_INTEGER freq;
				DEBUG_ASSERT(QueryPerformanceFrequency(&freq));
				return double(li.QuadPart) * 1000 / double(freq.QuadPart);
#else
				return li.QuadPart / 1000;
#endif
			}
		};

		class ProfNode {
		private:
			ProfTime mLastStartTime, mRunTime;
			int mCount;
			bool mRunning;
			std::vector<ProfNode*> mChildren;
			ProfNode* mParent;
			const int mIp;
			friend class ProfTree;
		public:
			const std::vector<ProfNode*>& children() const { return mChildren; }
			bool running() const { return mRunning; }

			ProfNode(int ip, ProfNode* parent) : mCount(0), mRunning(false), mParent(parent), mIp(ip) {}
			~ProfNode() {
				for(size_t i=0; i<mChildren.size(); i++) {
					delete mChildren[i];
				}
			}
			void start() {
				DEBUG_ASSERT(!mRunning);
				mRunning = true;
				mLastStartTime = ProfTime::now();
				mCount++;
			}
			void stop() {
				DEBUG_ASSERT(mRunning);
				ProfTime endTime = ProfTime::now();
				mRunTime += (endTime - mLastStartTime);
				mRunning = false;
			}
			const ProfTime& getRunTime() const {
				return mRunTime;
			}
			int getCount() const {
				return mCount;
			}

			void dump(FILE* file) {
				static int dLev = 0;
				//if(runTime.toMilliSeconds() > 10 || count > 10)
				{
					if(dLev > 0) {
						fprintf(file, "%*c", dLev, ' ');
					}
					fprintf(file, "<f ");
					const char* fName;
					if(mIp < 0) {	//hacked syscall number
						fName = translateSyscall(-mIp);
					} else {
						fName = mapFunction(mIp);
					}
					if(fName) {
						char nameBuf[1024];
						//if(strchr(fName, '&'))
						{
							//replace illegal XML character
							DEBUG_ASSERT(strlen(fName) < sizeof(nameBuf));
							char* ptr = nameBuf;
							while(*fName) {
								switch(*fName) {
								case '&':
									ptr += sprintf(ptr, "&amp;");
									break;
								case '<':
									ptr += sprintf(ptr, "&lt;");
									break;
								case '>':
									ptr += sprintf(ptr, "&gt;");
									break;
								default:
									*ptr = *fName;
									ptr++;
								}
								fName++;
							}
							*ptr = 0;
							fName = nameBuf;
						}
						fprintf(file, "n=\"%s\"", fName);
					} else {
						fprintf(file, "a=\"0x%x\"", mIp);
					}
					ProfTime childrenTime;
					for(size_t i=0; i<mChildren.size(); i++) {
						ProfNode* n = mChildren[i];
						childrenTime += n->mRunTime;
					}
					fprintf(file, " c=\"%i\" t=\"%f\" lt=\"%f\" ch=\"%i\" cht=\"%f\">\n", mCount,
						mRunTime.toMilliSeconds(), (mRunTime - childrenTime).toMilliSeconds(),
						(int)mChildren.size(), childrenTime.toMilliSeconds());
					for(size_t i=0; i<mChildren.size(); i++) {
						ProfNode* n = mChildren[i];
						dLev++;
						n->dump(file);
						dLev--;
					}
					if(dLev > 0) {
						fprintf(file, "%*c", dLev, ' ');
					}
					fprintf(file, "</f>\n");
				}
			}
		};

		ProfNode* mRoot;
		ProfNode* mTop;

		int level, maxLevel, nodes;	//debug
	public:
		ProfTree() : mRoot(NULL) {}
		void init(int entryPoint) {
			SAFE_DELETE(mRoot);
			mRoot = new ProfNode(entryPoint, NULL);
			DEBUG_ASSERT(mRoot != NULL);
			mTop = mRoot;
			level = maxLevel = 0;
			nodes = 1;
			mRoot->start();
		}
		~ProfTree() {
			DUMPINT(level);
			DUMPINT(maxLevel);
			DUMPINT(nodes);
			while(level > 0) {
				ret();
			}
			if(mRoot->running())
				mRoot->stop();
			{
				FILE* file = fopen("fp.xml", "w");
				DEBUG_ASSERT(file);
				mRoot->dump(file);
				fclose(file);
			}
			SAFE_DELETE(mRoot);
		}

		void call(int ip) {
			ProfNode* node = NULL;
			for(size_t i=0; i<mTop->mChildren.size(); i++) {
				ProfNode* n = mTop->mChildren[i];
				if(n->mIp == ip) {
					node = n;
					break;
				}
			}
			if(node == NULL) {
				node = new ProfNode(ip, mTop);
				nodes++;
				DEBUG_ASSERT(node != NULL);
				mTop->mChildren.push_back(node);
			}
			mTop = node;
			node->start();
			level++;
			if(level > maxLevel)
				maxLevel = level;
		}

		void ret() {
			mTop->stop();
			mTop = mTop->mParent;
			DEBUG_ASSERT(mTop != 0);
			level--;
		}

		const ProfNode* getRoot() const { return mRoot; }
	} profTree;
#endif	//FUNCTION_PROFILING

	//init functions
	void allocFakeCallStack() {
		fakeCallStackCapacity = 16;
		fakeCallStack = (int*)malloc(fakeCallStackCapacity * sizeof(int));
		DEBUG_ASSERT(fakeCallStack != NULL);
		resetFakeCallStack();
	}
	void resetFakeCallStack() {
		fakeCallStackDepth = 0;
	}
	void freeFakeCallStack() {
		if(fakeCallStack != NULL)
			free(fakeCallStack);
	}

	//core functions
	void fakePush(int returnAddress, int callAddress) {
		if(fakeCallStackDepth == fakeCallStackCapacity) {
			fakeCallStackCapacity *= 2;
			int* temp = (int*)realloc(fakeCallStack, fakeCallStackCapacity * sizeof(int));
			DEBUG_ASSERT(temp != NULL);	//should cause destructor to be called on failure
			fakeCallStack = temp;
		}
		fakeCallStack[fakeCallStackDepth++] = returnAddress;
#ifdef FUNCTION_PROFILING
		profTree.call(callAddress);
#endif
	}
	void fakePop() {
		if(fakeCallStackDepth > 0)
			fakeCallStackDepth--;
		else {
			LOG("Warning: Call stack broken @ IP 0x%X\n", IP);
		}
#ifdef FUNCTION_PROFILING
		profTree.ret();
#endif
	}
#else
#define fakePush(i,j)
#define fakePop()
#endif

	//***************************************
	//
	//***************************************

	void Run2() {
#ifdef USE_ARM_RECOMPILER
		//aIP = RunArm(aIP);
		rIP = (byte*)recompiler.run((int)rIP);
#else
		rIP = Run(rIP);
#endif
	}

#ifdef MOBILEAUTHOR
	void RunFrom(int ip) {
		rIP = mem_cs + ip;
		Run2();
	}
#endif

	void GenConstTable() {
		int n, p;
		int mask;
		
		for(p=0; p<32; p++) {
			regs[p] = 0;
		}

		for (n=1;n<17;n++)
		{
			regs[p++] = n;
			regs[p++] = -n;
		}

		mask = 0x20;

		for (n=0;n<32-5;n++)
		{
			regs[p++] = mask-1;
			regs[p++] = mask;
			mask <<= 1;
		}

		mask = 0x10;

		for (n=0;n<10;n++)
		{
			regs[p++] = mask ^ 0xffffffff;
			mask <<= 1;
		}
	}

	//****************************************
	//				 Init MA
	//****************************************
#ifndef MOBILEAUTHOR

#ifndef _android
	bool LoadVMApp(const char* modfile, const char* resfile) {
		InitVM();

		FileStream mod(modfile);
		if(!LoadVM(mod))
			return false;
			
		FileStream res(resfile);
		if(!mSyscall.loadResources(res, resfile))
			return false;
#else
	bool LoadVMApp(FILE* modfile, FILE* resfile) {
		InitVM();

		FileStream mod(modfile);

		if(!LoadVM(mod))
			return false;

		if(NULL != resfile)
		{
			FileStream res(resfile);

			if(!mSyscall.loadResources(res, "resources"))
				return false;
		}
#endif


#ifdef FUNCTION_PROFILING
		profTree.init(Head.EntryPoint);
#endif

#ifdef GDB_DEBUG
		if(mGdbOn) {
			if(!mGdbStub) {
				mGdbStub = new GdbStub(this);
				mGdbStub->setupDebugConnection();
			}
			mGdbStub->waitForRemote();
		}
		mGdbSignal = eNone;
#endif
		return true;
	}
#endif

	bool LoadVMApp(Stream& stream, const char* combfile) {
		LOG("LoadVMApp...\n");
		InitVM();
		if(!LoadVM(stream))
			return false;
#if !defined(MOBILEAUTHOR)
		if(!mSyscall.loadResources(stream, combfile))
			return false;
#endif

#ifdef FUNCTION_PROFILING
		profTree.init(Head.EntryPoint);
#endif

		return true;
	}

	void InitVM() {
		LOG("InitVM .. \n");
		VM_Yield = 0;
#ifdef MEMORY_DEBUG
		InstCount = 0;
#endif
		GenConstTable();
#ifdef FAKE_CALL_STACK
		resetFakeCallStack();
#endif

#ifdef COUNT_INSTRUCTION_USE
	initInstructionUseCount();
#endif

#if 0
		LOGC("Regs:\n");
		for(int i=0; i<128; i++) {
			LOGC("%i: 0x%x(%i)\n", i, regs[i], regs[i]);
		}
		LOGC("\n");
#endif

#ifdef USE_ARM_RECOMPILER
		//closeRecompiler();
		LOG("Close recompiler\n");
		recompiler.close();
		LOG("Recompiler Closed!\n");
#endif
	}
	
	//****************************************
	//Loader
	//****************************************
	int LoadVM(Stream& file) {
	
		LOG("LoadVM\n");

		TEST(file.isOpen());
		TEST(file.readObject(Head));	// Load header
		if(Head.Magic != 0x5844414d) {	//MADX, big-endian
			LOG("Magic error: 0x%08x should be 0x5844414d\n", Head.Magic);
			FAIL;
		}

		SAFE_DELETE(mem_cs);
		SAFE_DELETE(mem_ds);
		SAFE_DELETE(mem_cp);

#ifdef MEMORY_PROTECTION
		SAFE_DELETE(protectionSet);
#endif

		// Init regs + IP
		int maxCustomEventSize = getMaxCustomEventSize();

		STACK_TOP = Head.DataSize - maxCustomEventSize;
		STACK_BOTTOM = STACK_TOP-Head.StackSize;

		regs[REG_sp] = STACK_TOP - 16;
		regs[REG_i0] = Head.DataSize;
		regs[REG_i1] = Head.StackSize;
		regs[REG_i2] = Head.HeapSize;
		DUMPHEX(Head.DataSize);
		DUMPHEX(Head.StackSize);
		DUMPHEX(Head.HeapSize);
		DUMPHEX(Head.EntryPoint);
		IP = Head.EntryPoint;

#ifdef TRACK_SYSCALL_ID
		currentSyscallId = -1;
#endif

		DUMPHEX(Head.CodeLen);
		if(Head.CodeLen > 0) {
			CODE_SEGMENT_SIZE = nextPowerOf2(2, Head.CodeLen);
			mem_cs = new byte[CODE_SEGMENT_SIZE];
			if(!mem_cs) BIG_PHAT_ERROR(ERR_OOM);
#ifdef INSTRUCTION_PROFILING
			SAFE_DELETE(instruction_count);
			instruction_count = new int[CODE_SEGMENT_SIZE];
			if(!instruction_count) BIG_PHAT_ERROR(ERR_OOM);
			ZEROMEM(instruction_count, sizeof(int)*CODE_SEGMENT_SIZE);
#endif
			TEST(file.read(mem_cs, Head.CodeLen));
			ZEROMEM(mem_cs + Head.CodeLen, CODE_SEGMENT_SIZE - Head.CodeLen);
		} else {
			BIG_PHAT_ERROR(ERR_PROGRAM_FILE_BROKEN);
		}
		DUMPHEX(CODE_SEGMENT_SIZE);
		rIP = mem_cs + IP;

		DUMPHEX(Head.DataLen);
		DUMPHEX(Head.DataSize);
		if(Head.DataLen > 0) {
			DATA_SEGMENT_SIZE = nextPowerOf2(16, Head.DataSize);
			
#ifdef _android
			/*
			jclass cls = mJniEnv->GetObjectClass(mJThis);
			jmethodID methodID = mJniEnv->GetMethodID(cls, "generateDataSection", "(I)Ljava/nio/ByteBuffer;");
			if (methodID == 0) return -1;
			jobject jo = mJniEnv->CallObjectMethod(mJThis, methodID, (DATA_SEGMENT_SIZE));
			
			// if the java object jo is NULL at this point there was not enought memory to allocate the data section
			if(jo == NULL) 
			{
				__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "Deta section was allocated, not enough memory!");
				return 0;
			}
			
			mem_ds = (int*)mJniEnv->GetDirectBufferAddress(jo);
			mJniEnv->DeleteLocalRef(cls);
			*/

			char* buffer = (char*)malloc(DATA_SEGMENT_SIZE);
			if(buffer == NULL) return 0;
			
			jobject byteBuffer = mJniEnv->NewDirectByteBuffer((void*)buffer, DATA_SEGMENT_SIZE);
			if(byteBuffer == NULL) return 0;
			
			jclass cls = mJniEnv->GetObjectClass(mJThis);
			jmethodID methodID = mJniEnv->GetMethodID(cls, "generateDataSection", "(Ljava/nio/ByteBuffer;)Z");
			if (methodID == 0) return 0;
			
			jboolean ret = mJniEnv->CallBooleanMethod(mJThis, methodID, byteBuffer);
			if(false == ret) 
			{
				__android_log_write(ANDROID_LOG_INFO, "MoSync Syscall", "Deta section was allocated, not enough memory!");
				return 0;
			}

			mem_ds = (int*)buffer;

			mJniEnv->DeleteLocalRef(cls);
			mJniEnv->DeleteLocalRef(byteBuffer);
		
#else
			mem_ds = new int[DATA_SEGMENT_SIZE / sizeof(int)];
#endif		

			if(!mem_ds) BIG_PHAT_ERROR(ERR_OOM);
			TEST(file.read(mem_ds, Head.DataLen));
			ZEROMEM((byte*)mem_ds + Head.DataLen, DATA_SEGMENT_SIZE - Head.DataLen);
#ifdef MEMORY_PROTECTION
			protectionSet = new byte[(DATA_SEGMENT_SIZE+7)>>3];
			ZEROMEM(protectionSet, (DATA_SEGMENT_SIZE+7)>>3);
			//unprotectMemory(0, DATA_SEGMENT_SIZE);
			//protectMemory(Head.DataSize-Head.StackSize-Head.HeapSize-16, Head.HeapSize);
#endif
		} else {
			BIG_PHAT_ERROR(ERR_PROGRAM_FILE_BROKEN);
		}
		DUMPHEX(DATA_SEGMENT_SIZE);

		DUMPHEX(Head.IntLen);
		if(Head.IntLen > 0) {
			mem_cp = new int[Head.IntLen];
			if(!mem_cp) BIG_PHAT_ERROR(ERR_OOM);
			TEST(file.read(mem_cp, Head.IntLen * 4));
		} else {
			BIG_PHAT_ERROR(ERR_PROGRAM_FILE_BROKEN);
		}
#if 0
		LOGC("Constant pool (%i entries):\n", Head.IntLen);
		for(int i=0; i<Head.IntLen; i++) {
			LOGC("%i: 0x%x(%i)\n", i, mem_cp[i], mem_cp[i]);
		}
		LOGC("\n");
#endif

		customEventPointer = ((char*)mem_ds) + (Head.DataSize - maxCustomEventSize);
		
#ifdef USE_ARM_RECOMPILER
		//initRecompilerVariables();
#ifndef _android
		recompiler.init(this, &VM_Yield);
#else
		recompiler.init(this, &VM_Yield, mJniEnv, mJThis);
#endif
#endif

		return 1; //good load
	}

	//****************************************
	//Definitions
	//****************************************
#ifdef COUNT_INSTRUCTION_USE
#define OPC(opcode)	case _##opcode: LOGC("%x: %i %s", (int)(ip - mem_cs - 1), _##opcode, #opcode); countInstructionUse(#opcode, op);
#else
#define OPC(opcode)	case _##opcode: LOGC("%x: %i %s", (int)(ip - mem_cs - 1), _##opcode, #opcode);
#endif
#ifdef CORE_DEBUGGING_MODE
#define EOP	LOGC("\n"); break;
#else
#define EOP	LOGC("\n"); RUN_LOOP;
#endif	//CORE_DEBUGGING_MODE

#define REG(nn)	regs[nn]
#define	RD	(regs[rd])
#define	RS	(regs[rs])
#define	RDU	(((uint32_t*)regs)[rd])
#define	RSU	(((uint32_t*)regs)[rs])

#ifdef STACK_POINTER_VERIFICATION
void WRITE_REG(int reg, int value) {
	if(reg == REG_sp) {
		uint addr(value);
		if(addr<STACK_BOTTOM || addr>STACK_TOP) {
			BIG_PHAT_ERROR(ERR_STACK_OOB);
		}
	}
	regs[reg] = value;
}
#else
#define WRITE_REG(reg, value) regs[reg] = value
#endif

#define IMMU	((uint32_t) imm32)
#define IMM	((int32_t) imm32)

#ifdef MEMORY_DEBUG
#ifdef CORE_DEBUGGING_MODE
	void dumpJump(uint address) {
#ifdef SYMBIAN
		LOGC("\nJump to 0x%x\n", address);
#else
		std::string file;
		int line;
		mapIp(address, line, file);
		LOGC("\nJump to 0x%x %s %s:%i\n", address, mapFunction(address), file.c_str(), line);
#endif
	}
#else
#define dumpJump(a)
#endif
#define JMP_GENERIC(address) dumpJump(address); if(uint(address) >= CODE_SEGMENT_SIZE) {\
	LOG("\nIllegal jump to 0x%04X\n", (uint)address); BIG_PHAT_ERROR(ERR_IMEM_OOB); }\
		ip = (byte*)(mem_cs + (address));
#else
#define JMP_GENERIC(address) \
	ip = (byte*)(mem_cs + ((address) & CODE_SEGMENT_MASK));
#endif  //MEMORY_DEBUG

#define	JMP_IMM	JMP_GENERIC(IMM)
#define	JMP_RD	JMP_GENERIC(RD)

#define	CALL_IMM	REG(REG_rt) = (int32_t) (ip - mem_cs); JMP_IMM;
#define	CALL_RD		REG(REG_rt) = (int32_t) (ip - mem_cs); JMP_RD;

#define IB ((int)(*ip++))

#define FETCH_RD	rd = IB; LOGC(" rd%i(0x%08x)", rd, RD);
#define FETCH_RS	rs = IB; LOGC(" rs%i(0x%08x)", rs, RS);
#ifdef USE_VAR_INT
#define FETCH_CONST	imm32 = IB; if(imm32>127) {imm32=((imm32&127)<<8)+IB;}\
	LOGC(" c[%i]", imm32); imm32=mem_cp[imm32]; LOGC("(%i)", imm32);
#define FETCH_INT	imm32 = IB; if(imm32>127) {imm32=((imm32&127)<<8)+IB;}\
	LOGC(" i%i", imm32);
#else
#define FETCH_CONST	imm32 = IB; imm32=(imm32<<8)+IB;\
	imm32=mem_cp[imm32]; LOGC(" c%i", imm32);
#define FETCH_INT imm32 = IB; imm32=(imm32<<8)+IB; LOGC(" i%i", imm32);
#endif	//USE_VAR_INT

#define FETCH_IMM8	imm32 = IB; LOGC(" n%i", imm32);
#define FETCH_IMM16	imm32 = IB << 8; imm32 += IB; LOGC(" m%i(0x%x)", imm32, imm32);
#define FETCH_IMM24	imm32 = IB << 16; imm32 += IB << 8; imm32 += IB;\
	LOGC(" i%i(0x%x)", imm32, imm32);
/*#define FETCH_IMM32	imm32 = IB << 24; imm32 += IB << 16; imm32 += IB << 8;\
	imm32 += IB; LOGC(" n%i(0x%x)", imm32, imm32);*/

#define FETCH_RD_RS		FETCH_RD FETCH_RS
#define FETCH_RD_CONST		FETCH_RD FETCH_CONST
#define FETCH_RD_RS_CONST	FETCH_RD FETCH_RS FETCH_CONST
#define FETCH_RD_RS_ADDR16	FETCH_RD FETCH_RS FETCH_IMM16
#define FETCH_RD_RS_ADDR24	FETCH_RD FETCH_RS FETCH_IMM24
#define FETCH_RD_IMM8		FETCH_RD FETCH_IMM8

#define ARITH(a_reg, a, oper, b) LOGC("\t%i %s %i = ", (a), #oper, (b)); WRITE_REG(a_reg, (a) oper (b));\
	LOGC("%i", (a));

#define DIVIDE(a_reg, a, b) if((b) == 0) { BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO); } else { ARITH(a_reg, a, /, b); }

#define RAW_MEMREF(type, addr) (*(type*)(((char*)mem_ds) + (addr)))
#define MEMREF(type, addr) RAW_MEMREF(type, \
	(addr) & DATA_SEGMENT_MASK & ~(sizeof(type) - 1))

#ifdef MEMORY_PROTECTION
#define SET_PROTECTION(x) (protectionSet[(x)>>3]|=(1<<((x)&0x7)))
#define RESET_PROTECTION(x) (protectionSet[(x)>>3]&=~(1<<((x)&0x7)))
#define GET_PROTECTION(x) (protectionSet[(x)>>3]&(1<<((x)&0x7)))

	void checkProtection(uint address, uint size) const {
		if(protectionEnabled) {
		for(uint i = address; i < address+size; i++) 
			if(GET_PROTECTION(i)) {
				BIG_PHAT_ERROR(ERR_MEMORY_PROTECTED);
			}
		}
	}
#endif	//MEMORY_PROTECTION

#ifdef MEMORY_DEBUG
#define MEM(type, addr, write) getValidatedMemRef##write<type>(addr)
	template<class T> const T& getValidatedMemRefREAD(uint address) {
		return getValidatedMemRefBase<T, 0>(address);
	}
	template<class T> T& getValidatedMemRefWRITE(uint address) {
		return getValidatedMemRefBase<T, 1>(address);
	}

	template<class T, bool write> T& getValidatedMemRefBase(uint address) {
		DEBUG_ASSERT(isPowerOf2(sizeof(T)));
		if(address >= DATA_SEGMENT_SIZE || (address+sizeof(T)) > DATA_SEGMENT_SIZE ||
			((address & (sizeof(T) - 1)) != 0) || //alignment check
			(address < 4))	//NULL pointer check
		{
			LOG("Memory reference validation failed. Size %"PFZT", address 0x%x\n",
				sizeof(T), address);
			if((address & (sizeof(T) - 1)) != 0) {
				BIG_PHAT_ERROR(ERR_MEMORY_ALIGNMENT);
			} else if(address >= DATA_SEGMENT_SIZE || (address+sizeof(T)) > DATA_SEGMENT_SIZE) {
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
			} else {
				BIG_PHAT_ERROR(ERR_MEMORY_NULL);
			}
		}

#ifdef MEMORY_PROTECTION	
		checkProtection(address, sizeof(T));
#endif

		return MEMREF(T, address);
	}
#else
#define MEM(type, addr, write) MEMREF(type, addr)
#endif	//_DEBUG

	//****************************************
	//Memory validation
	//****************************************
#define PTR2ADDRESS(ptr) ((unsigned)((char*)ptr - (char*)mem_ds))
	void ValidateMemStringAddress(unsigned address) const {
#ifdef MEMORY_PROTECTION	
		int initialAddr = address;
#endif
		do {
			if(address >= DATA_SEGMENT_SIZE)
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
		} while(RAW_MEMREF(char, address++) != 0);
#ifdef MEMORY_PROTECTION	
		checkProtection(initialAddr, address-initialAddr);
#endif
	}
	void ValidateMemWStringAddress(unsigned address) const {
#ifdef MEMORY_PROTECTION	
		int initialAddr = address;
#endif
		address -= 2;
		do {
			address += 2;
			if(address >= DATA_SEGMENT_SIZE)
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
		} while(RAW_MEMREF(short, address) != 0);
#ifdef MEMORY_PROTECTION	
		checkProtection(initialAddr, address-initialAddr);
#endif
	}
	int ValidatedStrLen(const char* ptr) const {
		unsigned address = PTR2ADDRESS(ptr);
		do {
			if(address >= DATA_SEGMENT_SIZE)
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
		} while(RAW_MEMREF(char, address++) != 0);

#ifdef MEMORY_PROTECTION	
		checkProtection(PTR2ADDRESS(ptr), address-PTR2ADDRESS(ptr));
#endif
		return address - PTR2ADDRESS(ptr) - 1;
	}
	void ValidateMemRange(const void* ptr, unsigned int size) const {
		unsigned address = PTR2ADDRESS(ptr);
		if(address >= DATA_SEGMENT_SIZE || (address+size) >= DATA_SEGMENT_SIZE ||
			size > DATA_SEGMENT_SIZE)
			BIG_PHAT_ERROR(ERR_MEMORY_OOB);
#ifdef MEMORY_PROTECTION	
		checkProtection(address, size);
#endif
	}
	void* GetValidatedMemRange(int address, int size) {
		if(uint(address) >= DATA_SEGMENT_SIZE || uint(address+size) >= DATA_SEGMENT_SIZE ||
			uint(size) > DATA_SEGMENT_SIZE)
			BIG_PHAT_ERROR(ERR_MEMORY_OOB);
#ifdef MEMORY_PROTECTION	
		checkProtection(address, size);
#endif
		return ((char*)mem_ds) + address;
	}

	int GetValidatedStackValue(int offset) {
		int address = REG(REG_sp) + offset;
		if(((address&0x03)!=0) || uint(address)<STACK_BOTTOM || uint(address)>STACK_TOP)
			BIG_PHAT_ERROR(ERR_STACK_OOB);
		address>>=2;
		return mem_ds[address];
	}

	const char* GetValidatedStr(int a) const {
		unsigned address = a;
		do {
			if(address >= DATA_SEGMENT_SIZE)
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
		} while(RAW_MEMREF(char, address++) != 0);
#ifdef MEMORY_PROTECTION	
		checkProtection(address, address-a);
#endif
		return ((char*)mem_ds) + a;
	}

	const wchar* GetValidatedWStr(int a) const {
		unsigned address = a - sizeof(wchar);
		MYASSERT((address & (sizeof(wchar)-1)) == 0, ERR_MEMORY_ALIGNMENT);
		do {
			address += sizeof(wchar);
			if(address >= DATA_SEGMENT_SIZE)
				BIG_PHAT_ERROR(ERR_MEMORY_OOB);
		} while(RAW_MEMREF(wchar, address) != 0);
#ifdef MEMORY_PROTECTION	
		checkProtection(address, address-a);
#endif
		return (wchar*)(((char*)mem_ds) + a);
	}

#ifdef MEMORY_PROTECTION
	void protectMemory(uint start, uint length) {
		char *ptr = ((char*)mem_ds)+start;
		ValidateMemRange(ptr, length);
		//memset(&protectionSet[start], 1, length);
		for(uint i = start; i < start+length; i++)
			SET_PROTECTION(i);
	}

	void unprotectMemory(uint start, uint length) {
		char *ptr = ((char*)mem_ds)+start;
		ValidateMemRange(ptr, length);
		//memset(&protectionSet[start], 0, length);
		for(uint i = start; i < start+length; i++)
			RESET_PROTECTION(i);
	}

	void setMemoryProtection(int enable) {
		this->protectionEnabled = enable;
	}

	int getMemoryProtection() {
		return this->protectionEnabled;
	}
#endif

	//****************************************
	//Syscall
	//****************************************
	void _debug_hex(int SCDEBUG_ARG(hex)) { LOGSC("0x%08x", hex); }

#define _SYSCALL_HANDLERES_DEFAULT(type) REG(REG_r14) = _SYSCALL_CONVERTRES_##type(res); LOGSC(");\n");

	void debug_MACopyData(const MACopyData* SCDEBUG_ARG(p)) {
		LOGSC("(%i,%i, %i,%i, %i)",
			p->dst, p->dstOffset,	p->src, p->srcOffset,	p->size);
	}
	MACopyData* _SYSCALL_CONVERT_MACopyData(int ptr) {
		_debug_hex(ptr);
		MACopyData* p = (MACopyData*)((char*)mem_ds + ptr);
		ValidateMemRange(p, sizeof(MACopyData));
		debug_MACopyData(p);
		return p;
	}

	void debug_MAPoint2d(const MAPoint2d* SCDEBUG_ARG(p)) {
		LOGSC("(%i,%i)", p->x, p->y);
	}
	MAPoint2d* _SYSCALL_CONVERT_MAPoint2d(int ptr) {
		_debug_hex(ptr);
		MAPoint2d* p = (MAPoint2d*)((char*)mem_ds + ptr);
		ValidateMemRange(p, sizeof(MAPoint2d));
		debug_MAPoint2d(p);
		return p;
	}

	void debug_MARect(const MARect* SCDEBUG_ARG(r)) {
		LOGSC("(%i,%i,%i,%i)", r->left, r->top, r->width, r->height);
	}
	MARect* _SYSCALL_CONVERT_MARect(int ptr) {
		_debug_hex(ptr);
		MARect* r = (MARect*)((char*)mem_ds + ptr);
		ValidateMemRange(r, sizeof(MARect));
		debug_MARect(r);
		return r;
	}

	void debug_MAConnAddr(const MAConnAddr* SCDEBUG_ARG(r)) {
		LOGSC("(%i)", r->family);
	}
	MAConnAddr* _SYSCALL_CONVERT_MAConnAddr(int ptr) {
		_debug_hex(ptr);
		MAConnAddr* r = (MAConnAddr*)((char*)mem_ds + ptr);
		ValidateMemRange(r, sizeof(MAConnAddr));
		debug_MAConnAddr(r);
		return r;
	}

	void debug_MAString(char* SCDEBUG_ARG(str)) { LOGSC("(\"%s\")", str); }
	char* _SYSCALL_CONVERT_MAString(int str) {
		_debug_hex(str);
		ValidateMemStringAddress(str);
		char* res = (char*)mem_ds + str;
		debug_MAString(res);
		return res;
	}
#define _SYSCALL_CONVERTRES_MAString _SYSCALL_CONVERTRES_MAAddress
#define _SYSCALL_HANDLERES_MAString _SYSCALL_HANDLERES_DEFAULT(MAString)
#define _SYSCALL_CONVERT_NCString (char*)_SYSCALL_CONVERT_MAAddress

	void debug_MAWString(wchar* SCDEBUG_ARG(str)) {
#ifdef SYSCALL_DEBUGGING_MODE
		LOGSC("(\"");
		while(*str) {
			LOGSC("%C", *str);
			str++;
		}
		LOGSC("\")");
#endif
	}
	wchar* _SYSCALL_CONVERT_MAWString(int str) {
		_debug_hex(str);
		ValidateMemWStringAddress(str);
		wchar* res = (wchar*)((char*)mem_ds + str);
		debug_MAWString(res);
		return res;
	}
#define _SYSCALL_CONVERTRES_MAWString _SYSCALL_CONVERTRES_MAAddress
#define _SYSCALL_HANDLERES_MAWString _SYSCALL_HANDLERES_DEFAULT(MAWString)

	void debug_int(int SCDEBUG_ARG(i)) { LOGSC("(%i)", i); }
	int _SYSCALL_CONVERT_int(int i) {
		_debug_hex(i);
		debug_int(i);
		return i;
	}
	int _SYSCALL_CONVERTRES_int(int i) { return i; }
#define _SYSCALL_HANDLERES_int _SYSCALL_HANDLERES_DEFAULT(int)

	int _SYSCALL_CONVERTRES_uint(uint i) { return i; }
#define _SYSCALL_HANDLERES_uint _SYSCALL_HANDLERES_DEFAULT(uint)

	void debug_ulong(int SCDEBUG_ARG(i)) { LOGSC("(%u)", i); }
	int _SYSCALL_CONVERT_ulong(int i) {
		_debug_hex(i);
		debug_int(i);
		return i;
	}
#define _SYSCALL_HANDLERES_ulong _SYSCALL_HANDLERES_DEFAULT(ulong)

	void debug_float(float SCDEBUG_ARG(f)) { LOGSC("(%f)", f); }
	#define _SYSCALL_CONVERT_float(a) _convert_float(&a);
	float _convert_float(int* ptr) {
		_debug_hex(*((int*)ptr));
		debug_float(*((float*)ptr));
		return *((float*)ptr);
	}
	int _SYSCALL_CONVERTRES_float(float f) { return MAKE(int, f); }
#define _SYSCALL_HANDLERES_float _SYSCALL_HANDLERES_DEFAULT(float)

	void debug_short(short SCDEBUG_ARG(i)) { LOGSC("(%i)", i); }
	short _SYSCALL_CONVERT_short(short i) {
		_debug_hex(i);
		debug_short(i);
		return i;
	}
	short _SYSCALL_CONVERTRES_short(short i) { return i; }
#define _SYSCALL_HANDLERES_short _SYSCALL_HANDLERES_DEFAULT(short)

	void debug_MAExtent(MAExtent SCDEBUG_ARG(e)) { LOGSC("(%i, %i)", EXTENT_X(e), EXTENT_Y(e)); }
	MAExtent _SYSCALL_CONVERT_MAExtent(int e) {
		_debug_hex(e);
		debug_MAExtent(e);
		return e;
	}
	int _SYSCALL_CONVERTRES_MAExtent(MAExtent e) { return e; }
#define _SYSCALL_HANDLERES_MAExtent _SYSCALL_HANDLERES_DEFAULT(MAExtent)

	void debug_MAHandle(MAHandle h) { debug_int(h); }
	MAHandle _SYSCALL_CONVERT_MAHandle(int h) {
		_debug_hex(h);
		debug_MAHandle(h);
		return h;
	}
	int _SYSCALL_CONVERTRES_MAHandle(MAHandle h) { return h; }
#define _SYSCALL_HANDLERES_MAHandle _SYSCALL_HANDLERES_DEFAULT(MAHandle)

	void debug_MAAddress(MAAddress) {}
	void* _SYSCALL_CONVERT_MAAddress(MAAddress a) {
		_debug_hex(a);
		debug_MAAddress(a);
		return (a + (byte*)mem_ds);
	}
	int _SYSCALL_CONVERTRES_MAAddress(const void* a) {
		return (int)((byte*)a - (byte*)mem_ds);
	}
#define _SYSCALL_HANDLERES_MAAddress _SYSCALL_HANDLERES_DEFAULT(MAAddress)

#define _SYSCALL_CONVERT_MAEvent(i) (MAEvent*)_SYSCALL_CONVERT_MAAddress(i)

#define _SYSCALL_CONVERT_double(a) _convert_double(&a);
	double _convert_double(int* ptr) {
#ifdef DEBUGGING_MODE
		DEBUG_ASSERT(sizeof(MA_DV) == 8);
		DEBUG_ASSERT(sizeof(double) == 8);
#endif
		MA_DV dv;
		dv.MA_DV_HI = ptr[0];
		dv.MA_DV_LO = ptr[1];
		DV_debug(dv);
		return dv.d;
	}
	void DV_debug(const MA_DV& SCDEBUG_ARG(dv)) {
		LOGSC("0x%08x%08x", dv.MA_DV_HI, dv.MA_DV_LO); LOGSC("(%g)", dv.d);
	}
#define _SYSCALL_HANDLERES_double { MA_DV dv; dv.d = res;\
	REG(REG_r14) = dv.MA_DV_HI; REG(REG_r15) = dv.MA_DV_LO; LOGSC(" = "); DV_debug(dv); LOGSC(");\n"); }

#define _SYSCALL_HANDLERES_longlong { MA_DV dv; dv.ll = res;\
	REG(REG_r14) = dv.MA_LL_HI; REG(REG_r15) = dv.MA_LL_LO; LOGSC(");\n"); }

#define _SYSCALL_HANDLERES_void LOGSC(");\n");

	//****************************************
	//			Invoke SysCalls
	//****************************************

	void ISC2(int syscall_id) {

		switch(syscall_id) {
#ifdef MOBILEAUTHOR
#include <Deimos/DeimosInvokeSyscall.h>
#else

#define memset __memset
#define memcpy __memcpy
#define strcpy __strcpy
#define strcmp __strcmp
#include "invoke_syscall_cpp.h"
#undef memset
#undef memcpy
#undef strcpy
#undef strcmp

#endif
		default:  //bad syscall
			BIG_PHAT_ERROR(ERR_BAD_SYSCALL);
		}
	}

	void InvokeSysCall(int syscall_id) {
#ifdef TRACK_SYSCALL_ID
		currentSyscallId = syscall_id;
#endif
#if defined(SYMBIAN) && defined(SUPPORT_RELOAD)
		TRAP(symbianError, ISC2(syscall_id));
		if(symbianError) {
			LOG("Trapped core leave, code %i\n", symbianError);
			VM_Yield = 1;
		}
#else
		ISC2(syscall_id);
#endif
#ifdef TRACK_SYSCALL_ID
		currentSyscallId = -1;
#endif
	}

	/*
#ifdef USE_ARM_RECOMPILER
#include "recompiler_core.h"
#endif
	*/


#ifdef GDB_DEBUG
# define CHECK_SIGNAL if(mGdbSignal) { IP = uint(ip - mem_cs); waitForRemote(mGdbSignal); }
#else
# define CHECK_SIGNAL
#endif

#define RUN_NAME Run
#ifdef CORE_DEBUGGING_MODE
# define RUN_LOOP CHECK_SIGNAL return ip
#else
# define RUN_LOOP CHECK_SIGNAL goto vmloop
#endif
#define VMLOOP_LABEL vmloop:
#define STEP 0
#include "core_run.h"
#undef STEP
#undef VMLOOP_LABEL
#undef RUN_NAME
#undef RUN_LOOP

#if 0//def GDB_DEBUG
#define RUN_NAME Step
#define RUN_LOOP return ip
#define VMLOOP_LABEL
#define STEP 1	//todo: replace the rest of the defines with this one; move code into core_run.h.
#include "core_run.h"
#undef STEP
#undef VMLOOP_LABEL
#undef RUN_NAME
#undef RUN_LOOP
#endif	//GDB_DEBUG

#if defined(UPDATE_IP) && defined(GDB_DEBUG)
	void waitForRemote(int code) {
		mGdbStub->exceptionHandler(code);
		if(mGdbStub->waitForRemote()) {
			MoSyncExit(code);
		}
	}
#endif

	int GetIp() const {
		return IP;
	}

	void SetIp(int ip) {
		IP = ip;
	}

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

	VMCoreInt(Syscall& aSyscall)
	: rIP(NULL)
#ifdef MEMORY_DEBUG
	, InstCount(0)
#endif
#ifdef INSTRUCTION_PROFILING
	,instruction_count(NULL)
#endif
	, mSyscall(aSyscall) {

#ifdef FAKE_CALL_STACK
		allocFakeCallStack();
#endif
#ifdef LOG_STATE_CHANGE
		initStateChange();
#endif
	}
	
	virtual ~VMCoreInt() {
#ifdef GDB_DEBUG
		if(mGdbOn)
			mGdbStub->closeDebugConnection();
#endif

#ifdef MEMORY_DEBUG
		DUMPINT(InstCount);	//includes any eventual illegal instruction

		//on error, this will probably have read some or all of the crashing instruction
		LOG("IP: 0x%04X\n", (int)(size_t)(rIP - mem_cs));
		LOGC("Regs:\n");
		for(int i=0; i<32; i++) {
			LOGC("%2i: 0x%08X\n", i, regs[i]);
		}
#endif

#ifdef COUNT_INSTRUCTION_USE
	logInstructionUse();
#endif
		delete mem_cs;
		delete mem_ds;
		delete mem_cp;

#ifdef MEMORY_PROTECTION
		delete protectionSet;
#endif

#ifdef LOG_STATE_CHANGE
		freeStateChange();
#endif

#ifdef USE_ARM_RECOMPILER
		//closeRecompiler();
		recompiler.close();
#endif

#ifdef FAKE_CALL_STACK
		freeFakeCallStack();
#endif
#ifdef INSTRUCTION_PROFILING
		if(instruction_count != NULL) {
			FILE* file = fopen("profile.txt", "w");
			if(file) {
				fprintf(file, "INSTRUCTION COUNTS: %i\n", CODE_SEGMENT_SIZE);
				for(uint i=0; i<CODE_SEGMENT_SIZE; i++) {
					if(instruction_count[i] != 0)
						fprintf(file, "%x: %i\n", i, instruction_count[i]);
				}
				fclose(file);
				LOG("Profile dumped.\n");
			} else {
				LOG("Profile dump failed; couldn't open file for writing.\n");
			}
		}
		delete instruction_count;
#endif

	}
	
private:
	Syscall& mSyscall;
};

VMCore::VMCore() : mem_cs(NULL), mem_ds(NULL), mem_cp(NULL)
#ifdef MEMORY_PROTECTION
	,protectionSet(NULL)
	,protectionEnabled(1)
#endif
#ifdef TRACK_SYSCALL_ID
	,currentSyscallId(-1)
#endif
#ifdef GDB_DEBUG
	, mGdbStub(NULL), mGdbOn(false)
#endif
{}

//Functions for outside access
VMCore* CreateCore(Syscall& aSyscall) {
	VMCore *core = new VMCoreInt(aSyscall);
	if(!core) {
		FAIL
	} else {
		return core;
	}
}
#define CORE ((VMCoreInt*)core)
void DeleteCore(VMCore* core) {
	delete CORE;
}

VMCore::~VMCore() {}

void VMCore::invokeSysCall(int id) {
	((VMCoreInt*)this)->InvokeSysCall(id);
}

#ifdef LOG_STATE_CHANGE
void VMCore::logStateChange(int ip) {
	((VMCoreInt*)this)->logStateChange(ip);
}
#endif

#ifndef MOBILEAUTHOR
#ifndef _android
bool LoadVMApp(VMCore* core, const char* modfile,const char* resfile) {
	return CORE->LoadVMApp(modfile, resfile);
}
#else
bool LoadVMApp(VMCore* core, FILE* modfile, FILE* resfile) {
	return CORE->LoadVMApp(modfile, resfile);
}
#endif
#endif

bool LoadVMApp(VMCore* core, Stream& stream, const char* combfile) {
	return CORE->LoadVMApp(stream, combfile);
}

int GetIp(const VMCore* core) {
	return CORE->GetIp();
}

void SetIp(VMCore* core, int ip) {
	CORE->SetIp(ip);
}

#ifdef FAKE_CALL_STACK
int GetFakeCallStackDepth(const VMCore* core) {
	return CORE->fakeCallStackDepth;
}
const int* GetFakeCallStack(const VMCore* core) {
	return CORE->fakeCallStack;
}
#endif

#ifdef MOBILEAUTHOR
void RunFrom(VMCore* core, int ip) {
	CORE->RunFrom(ip);
}
#endif

// ***************************************
// Debugger
// ***************************************	
/*
#ifdef ENABLE_DEBUGGER
bool initDebugger(VMCore* core, int port) {
	return Debugger::init(port, CORE->regs, (byte*)CORE->mem_ds, CORE->DATA_SEGMENT_SIZE, (byte*)CORE->mem_cs, CORE->CODE_SEGMENT_SIZE, &CORE->IP);
}

void closeDebugger() {
	Debugger::close();
}
#endif
*/
// ***************************************	

void Run2(VMCore* core) {
	CORE->Run2();
	//LOGD("::Run2 returning...\n");
}
#if 0//def GDB_DEBUG
void Step(VMCore* core) {
	CORE->rIP = CORE->Step(CORE->rIP);
}
#endif
int& GetVMYield(VMCore* core) {
	return CORE->VM_Yield;
}
void ValidateMemRange(VMCore* core, const void* ptr, int size) {
	CORE->ValidateMemRange(ptr, size);
}
int ValidatedStrLen(const VMCore* core, const char* ptr) {
	return CORE->ValidatedStrLen(ptr);
}
void* GetValidatedMemRange(VMCore* core, int address, int size) {
  return CORE->GetValidatedMemRange(address, size);
}
int GetValidatedStackValue(VMCore* core, int offset) {
  return CORE->GetValidatedStackValue(offset);
}
const char* GetValidatedStr(const VMCore* core, int address) {
	return CORE->GetValidatedStr(address);
}
const wchar* GetValidatedWStr(const VMCore* core, int address) {
	return CORE->GetValidatedWStr(address);
}
void* GetCustomEventPointer(VMCore* core) {
	return CORE->customEventPointer;
}

#ifdef MEMORY_PROTECTION
static void protectMemory(VMCore* core, int start, int length) {
	CORE->protectMemory(start, length);
}
static void unprotectMemory(VMCore* core, int start, int length) {
	CORE->unprotectMemory(start, length);
}
static void setMemoryProtection(VMCore* core, int enable) {
	CORE->setMemoryProtection(enable);
}
static int getMemoryProtection(VMCore* core) {
	return CORE->getMemoryProtection();
}
#endif

#if 0//ndef SYMBIAN
int getRuntimeIp() {
	if(gCore) {
		VMCore* core = gCore;
		return CORE->IP;
	}
	return -1;
}
#endif

#ifdef TRANSLATE_PANICS
const char* CurrentSyscallString(VMCore* core, char* buffer, int bufSize) {
	if(CORE == NULL)
		return NULL;
	if(CORE->currentSyscallId == -1)
		return NULL;
	const char* t = translateSyscall(CORE->currentSyscallId);
	if(t == NULL) {
#ifdef SYMBIAN
		TPtr8 ptr((byte*)buffer, bufSize);
		ptr.Format(_L8("Unknown %i"), core->currentSyscallId);
		buffer[ptr.Length()] = 0;
#else
		sprintf(buffer, "Unknown %i", CORE->currentSyscallId);
#endif
		return buffer;
	}
	return t;
}
#endif
}	//namespace Core

#ifdef TRANSLATE_PANICS
#include <helpers/TranslateSyscall.cpp>
#endif

#ifndef SYMBIAN
Core::VMCore* gCore = NULL;
#endif

void* Base::Syscall::GetValidatedMemRange(int address, int size) {
	return Core::GetValidatedMemRange(gCore, address, size);
}
int Base::Syscall::GetValidatedStackValue(int offset VSV_ARGPTR_DECL) {
	return Core::GetValidatedStackValue(gCore, offset);
}
const char* Base::Syscall::GetValidatedStr(int address) {
	return Core::GetValidatedStr(gCore, address);
}
const wchar* Base::Syscall::GetValidatedWStr(int address) {
	return Core::GetValidatedWStr(gCore, address);
}
void Base::Syscall::ValidateMemRange(const void* ptr, int size) {
	Core::ValidateMemRange(gCore, ptr, size);
}
int Base::Syscall::ValidatedStrLen(const char* ptr) {
	return Core::ValidatedStrLen(gCore, ptr);
}

#ifdef MEMORY_PROTECTION
void Base::Syscall::protectMemory(int start, int length) {
	Core::protectMemory(gCore, start ,length);
}

void Base::Syscall::unprotectMemory(int start, int length) {
	Core::unprotectMemory(gCore, start ,length);
}

void Base::Syscall::setMemoryProtection(int enable) {
	Core::setMemoryProtection(gCore, enable);
}

int Base::Syscall::getMemoryProtection() {
	return Core::getMemoryProtection(gCore);
}
#endif

void Base::Syscall::VM_Yield() {
	Core::GetVMYield(gCore) = 1;
}
