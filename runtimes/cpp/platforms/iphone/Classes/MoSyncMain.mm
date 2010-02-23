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
// MoSync Runtime Environment (Iphone Version)						  //
// (c) Mobile Sorcery 2007                                            //
//--------------------------------------------------------------------//

#include "MoSyncMain.h"

#include "config_platform.h"
#include <core/Core.h>
#include <base/Syscall.h>
#include <helpers/log.h>
#include "Base/ThreadPool.h"

#include <helpers/CriticalSection.h>

using namespace Base;

static MAHandle gReloadHandle = 0;
bool gRunning = false;

static UIView *sMoSyncView;
static int sWidth, sHeight;

int MoSyncThreadMain(void *args) {
	NSAutoreleasePool	 *autoreleasepool = [[NSAutoreleasePool alloc] init];
	
	char program[] = "program";
	char resources[] = "resources";

	Base::Syscall *syscall = 0;
	syscall = new Base::Syscall(sWidth, sHeight);
	
	gCore = Core::CreateCore(*syscall);
	MYASSERT(Core::LoadVMApp(gCore, program, resources), ERR_PROGRAM_LOAD_FAILED);
	gRunning = true;
	
	while(1) {
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
	
	[autoreleasepool release];
	
	return 0;
}

MoSyncThread mosyncThread;
CRITICAL_SECTION gViewMutex;
void MoSyncMain(int width, int height, UIView* mosyncView) {
	sWidth = width;
	sHeight = height;
	sMoSyncView = mosyncView;

	InitializeCriticalSection(&gViewMutex);
	
	mosyncThread.start(MoSyncThreadMain, NULL);
}

void UpdateMoSyncView(CGContextRef ref) {
	EnterCriticalSection(&gViewMutex);
	
	LeaveCriticalSection(&gViewMutex);
}

SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
	Base::gSyscall->VM_Yield();
	//you should get out of the VM loop before you can reload, but this will actually work anyway.
	gReloadHandle = data;
	//gReload = gReload || (reload != 0); ???? fredrik?
}
