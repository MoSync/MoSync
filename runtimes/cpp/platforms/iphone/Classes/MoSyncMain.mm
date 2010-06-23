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

//--------------------------------------------------------------------//
// MoRE                                                               //
// MoSync Runtime Environment (iPhone Version)                        //
// (c) Mobile Sorcery 2010                                            //
//--------------------------------------------------------------------//



#include "MoSyncMain.h"
#include "MosyncView.h"

#include "config_platform.h"
#include <core/Core.h>
#include <base/Syscall.h>
#include <helpers/log.h>
#include "Base/ThreadPool.h"
#include <base/FileStream.h>

#include <helpers/CriticalSection.h>

#include "iphone_helpers.h"

//#define _USE_REBUILDER_

#ifdef _USE_REBUILDER_
//#include "rebuild.build.cpp"

int sp;
unsigned char *mem_ds;
void *sCustomEventData;
static int sDataSize;
static int sCustomEventDataPointer;

void MoSyncDiv0() {
	BIG_PHAT_ERROR(ERR_DIVISION_BY_ZERO);
}

void* Base::Syscall::GetValidatedMemRange(int address, int size) {
	return (byte*)mem_ds + address;
}
void Base::Syscall::ValidateMemRange(const void* ptr, int size) {
}
int Base::Syscall::ValidatedStrLen(const char* ptr) {
	return strlen(ptr);
}
const char* Base::Syscall::GetValidatedStr(int address) {
	return (const char*)mem_ds+address;
}

void Base::Syscall::VM_Yield() {
}

unsigned char* CppInitReadData(const char* file, int fileSize, int mallocSize) {
	unsigned char* data = new unsigned char[mallocSize];
	memset(data, 0, mallocSize);
	Base::FileStream fileStream(file);
	fileStream.read(data, fileSize);
	
	// setup some stuff here.
	int mces =  Base::getMaxCustomEventSize();
	sp -= mces;
	sDataSize = mallocSize;
	sCustomEventDataPointer = mallocSize-mces;
	sCustomEventData = &data[sCustomEventDataPointer];
	
	return data;
}


void cpp_main();

#endif

using namespace Base;

static MAHandle gReloadHandle = 0;
bool gRunning = false;

static UIView *sMoSyncView;
static int sWidth, sHeight;



int MoSync_ThreadMain(void *args) {
	NSAutoreleasePool	 *autoreleasepool = [[NSAutoreleasePool alloc] init];
	
	char program[] = "program";
	char resources[] = "resources";

	InitLog(getWriteablePath("log.txt"));
	
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
	gCore = Core::CreateCore(*syscall);
	MYASSERT(Core::LoadVMApp(gCore, program, resources), ERR_PROGRAM_LOAD_FAILED);
	gRunning = true;
	
	while(gRunning) {
		Core::Run2(gCore);
		
		if(gReloadHandle > 0) {
			Base::Stream* stream = Base::gSyscall->resources.extract_RT_BINARY(gReloadHandle);
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

void MoSync_Main(int width, int height, UIView* mosyncView) {
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

void MoSync_ShowMessageBox(const char *msg, bool kill) {
	[sMoSyncView showMessageBox:[NSString stringWithCString:msg length:strlen(msg)] shouldKill:kill];  	
}

void MoSync_Exit() {
	[[UIApplication sharedApplication] terminateWithSuccess];
}

void MoSync_StartUpdatingLocation() {
	[sMoSyncView startUpdatingLocation];
}

void MoSync_StopUpdatingLocation() {
	[sMoSyncView stopUpdatingLocation];
}

void MoSync_AddTouchPressedEvent(int x, int y) {
	Base::gEventQueue.addPointerEvent(x, y, EVENT_TYPE_POINTER_PRESSED);
}

void MoSync_AddTouchMovedEvent(int x, int y) {
	Base::gEventQueue.addPointerEvent(x, y, EVENT_TYPE_POINTER_DRAGGED);
}

void MoSync_AddTouchReleasedEvent(int x, int y) {
	Base::gEventQueue.addPointerEvent(x, y, EVENT_TYPE_POINTER_RELEASED);
}

void MoSync_AddScreenChangedEvent() {
	Base::gEventQueue.addScreenChangedEvent();
}

void MoSync_AddCloseEvent() {
	Base::gEventQueue.addCloseEvent();
}

void MoSync_AddEvent(const MAEvent &e) {
	Base::gEventQueue.put(e);
}

void* MoSync_GetCustomEventData() {
	return sCustomEventData;
}

void* MoSync_GetCustomEventDataMoSyncPointer() {
	return (void*) sCustomEventDataPointer;
}

/*
void MoSync_AddLocationProviderEvent(int state) {
}

void MoSync_AddLocationEvent(int state, const MALocation& location) {
}
 */
