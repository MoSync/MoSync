/* Copyright (C) 2010 MoSync AB

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


#include "MoSyncMain.h"

#include "config_platform.h"
#include <core/Core.h>
#include <base/Syscall.h>
#include <helpers/log.h>
#include "Base/ThreadPool.h"
#include <base/FileStream.h>
#include <helpers/CriticalSection.h>

#include "iphone_helpers.h"

//#define _USE_REBUILDER_
//#undef _USE_REBUILDER_

#ifdef _USE_REBUILDER_
//#include "rebuild.build.cpp"

bool gNativeMode;

int sp;
unsigned char *mem_ds;
void *sCustomEventData;
static int sDataSize;
static int sCustomEventDataPointer;

void MoSyncDiv0() {
#ifndef ALLOW_DIVISION_BY_ZERO
	BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
#endif
}

void* Base::Syscall::GetValidatedMemRange(int address, int size) {
    if(address == 0) return NULL;
	return (byte*)mem_ds + address;
}
void Base::Syscall::ValidateMemRange(const void* ptr, int size) {
}

int Base::Syscall::ValidatedStrLen(const char* ptr) {
	return strlen(ptr);
}
const char* Base::Syscall::GetValidatedStr(int address) {
    if(address == 0) return NULL;
	return (const char*)mem_ds+address;
}

const wchar* Base::Syscall::GetValidatedWStr(int address) {
    if(address == 0) return NULL;
	return (const wchar*)(mem_ds+address);
}

int Base::Syscall::GetValidatedStackValue(int offset VSV_ARGPTR_DECL) {
    if(gNativeMode) {
        sp = (int)VSV_ARGPTR_NAME;
    }
	int address = sp + offset;
	//if(((address&0x03)!=0) || uint(address)<STACK_BOTTOM || uint(address)>STACK_TOP)
	//	BIG_PHAT_ERROR(ERR_STACK_OOB);
	return *(int*)&mem_ds[address];

}

int Base::Syscall::TranslateNativePointerToMoSyncPointer(void *nativePointer) {
    if(nativePointer == NULL)
        return 0;
    else
        return (byte*)nativePointer - (byte*)mem_ds;
}

void Base::Syscall::VM_Yield() {
}

unsigned char* CppInitReadData(const char* file, int fileSize, int mallocSize) {
	unsigned char* data = new unsigned char[mallocSize];
	memset(data, 0, mallocSize);
	Base::FileStream fileStream(getReadablePath(file));
	fileStream.read(data, fileSize);

	// setup some stuff here.
	int mces =  Base::getMaxCustomEventSize();
	sp -= mces;
	sDataSize = mallocSize;
	sCustomEventDataPointer = mallocSize-mces;
	sCustomEventData = &data[sCustomEventDataPointer];

    gNativeMode = false;

	return data;
}

//Used only by native lib approach
extern "C" unsigned char* InitCustomEventPointer(int mallocSize) {
	int mces =  Base::getMaxCustomEventSize();
	sCustomEventDataPointer = (int) malloc(mces);
	sCustomEventData =(void*) sCustomEventDataPointer; //What is the point of this??
    mem_ds = 0; //Basically set msm_ds to behave as the native address space.

    gNativeMode = true;

    return 0;
}


void cpp_main();

#else

#include <core/Core.cpp>

static MAHandle gReloadHandle = 0;

#endif

using namespace Base;

bool gRunning = false;

static MoSyncView *sMoSyncView;
static int sWidth, sHeight;



int MoSync_ThreadMain(void *args) {
	NSAutoreleasePool	 *autoreleasepool = [[NSAutoreleasePool alloc] init];

	const char *resources = getReadablePath("resources");

#ifdef LOGGING_ENABLED
	InitLog(getWriteablePath("log.txt"));
#endif

	Base::Syscall *syscall = 0;
	syscall = new Base::Syscall(sWidth, sHeight);

#ifdef _USE_REBUILDER_
	{
		FileStream res(resources);
		if(!syscall->loadResources(res, resources))
			BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
	}
	cpp_main();
#else
	const char *program = getReadablePath("program");

	gCore = Core::CreateCore(*syscall);
	MYASSERT(Core::LoadVMApp(gCore, program, resources), ERR_PROGRAM_LOAD_FAILED);
	gRunning = true;

	while(gRunning) {
		Core::Run2(gCore);

		if(gReloadHandle > 0) {
			Base::Stream* stream = Base::gSyscall->resources.extract_RT_BINARY(gReloadHandle);
			if(!stream->seek(Seek::Start, 0))
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			bool res = Core::LoadVMApp(gCore, *stream);
			delete stream;
			gReloadHandle = 0;
			if(!res) {
				BIG_PHAT_ERROR(ERR_PROGRAM_LOAD_FAILED);
			}
		}
	}
#endif

	[autoreleasepool release];

	return 0;
}

MoSyncThread mosyncThread;

void MoSync_Main(int width, int height, MoSyncView* mosyncView) {
	sWidth = width;
	sHeight = height;
	sMoSyncView = mosyncView;

	mosyncThread.start(MoSync_ThreadMain, NULL);
}

MoSyncSemaphore mViewSemaphore;
void MoSync_UpdateView(CGImageRef ref) {

	[sMoSyncView updateMoSyncView: ref];
	mViewSemaphore.wait();
}

void MoSync_DoneUpdatingView() {

	mViewSemaphore.post();
}

void MoSync_ReloadProgram(MAHandle data, int reload) {
#ifdef SUPPORT_PROGRAM_RELOAD
	Base::gSyscall->VM_Yield();
	gReloadHandle = data;
	//gReload |= (reload != 0);
#else
	BIG_PHAT_ERROR(ERR_FUNCTION_UNSUPPORTED);
#endif
}

void MoSync_Exit() {
    //[[UIApplication sharedApplication] terminateWithSuccess];
    exit(0);
}

void MoSync_AddTouchPressedEvent(int x, int y, int touchId) {
	Base::gEventQueue.addPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_PRESSED);
}

void MoSync_AddTouchMovedEvent(int x, int y, int touchId) {
	Base::gEventQueue.addPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_DRAGGED);
}

void MoSync_AddTouchReleasedEvent(int x, int y, int touchId) {
	Base::gEventQueue.addPointerEvent(x, y, touchId, EVENT_TYPE_POINTER_RELEASED);
}

void MoSync_AddScreenChangedEvent() {
	Base::gEventQueue.addScreenChangedEvent();
}

void MoSync_AddCloseEvent() {
	Base::gEventQueue.addCloseEvent();
}

void MoSync_AddFocusLostEvent() {
	MAEvent event;
    event.type = EVENT_TYPE_FOCUS_LOST;
    Base::gEventQueue.put(event);
    return;
}

void MoSync_AddFocusGainedEvent() {
	MAEvent event;
    event.type = EVENT_TYPE_FOCUS_GAINED;
    Base::gEventQueue.put(event);
    return;
}


void MoSync_AddEvent(const MAEvent &e) {
	Base::gEventQueue.put(e);
}

void* MoSync_GetCustomEventData() {
#ifdef _USE_REBUILDER_
	return sCustomEventData;
#else
	return GetCustomEventPointer(gCore);
#endif
}

void* MoSync_GetCustomEventDataMoSyncPointer() {
#ifdef _USE_REBUILDER_
	return (void*) sCustomEventDataPointer;
#else
	return (void*) (gCore->Head.DataSize-Base::getMaxCustomEventSize());

#endif
}

void MoSync_AddLayerToView(CALayer* layer){
	if ([[sMoSyncView.layer sublayers] containsObject:layer]==NO) {
		[sMoSyncView.layer addSublayer:layer];
	}
	layer.frame = sMoSyncView.bounds;
}

/*
void MoSync_AddLocationProviderEvent(int state) {
}

void MoSync_AddLocationEvent(int state, const MALocation& location) {
}
 */
