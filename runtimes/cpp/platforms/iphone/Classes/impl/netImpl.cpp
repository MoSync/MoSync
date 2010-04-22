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

#include "config_platform.h"

#include <helpers/helpers.h>

#define NETWORKING_H
#include "networking.h"

#include "Syscall.h"

#include <base_errors.h>
using namespace MoSyncError;

//***************************************************************************
//MoSyncMutex
//***************************************************************************

MoSyncMutex::MoSyncMutex() : mInitialized(false) {}

void MoSyncMutex::init() {
	if(pthread_mutex_init(&mMutex, NULL)!=0) {
		DEBIG_PHAT_ERROR;
	}
	mInitialized = true;
}

MoSyncMutex::~MoSyncMutex() {
	close();
}

void MoSyncMutex::close() {
	if(!mInitialized) return;
	pthread_mutex_destroy(&mMutex);
}

void MoSyncMutex::lock() {
	if(pthread_mutex_lock(&mMutex)!=0) {
		DEBIG_PHAT_ERROR;
	}
}

void MoSyncMutex::unlock() {
	if(pthread_mutex_unlock(&mMutex)!=0) {
		DEBIG_PHAT_ERROR;
	}
}

//***************************************************************************
//Helpers
//***************************************************************************

void ConnWaitEvent() {
	Base::gEventQueue.wait(0);
}
void ConnPushEvent(MAEvent* ep) {
	//PostMessage(g_hwndMain, WM_ADD_EVENT, (WPARAM) ep, 0);
	//DEBIG_PHAT_ERROR;
	Base::gEventQueue.put(*ep);
	delete ep;
}

void DefluxBinPushEvent(MAHandle handle, Stream& s) {
	//PostMessage(g_hwndMain, WM_DEFLUX_BINARY, (WPARAM) &s, handle);
//	DEBIG_PHAT_ERROR;
	InternalEventDefluxBin* ie = new InternalEventDefluxBin;
	ie->handle = handle;
	ie->stream = (Stream*)&s;
	Base::gEventQueue.addInternalEvent(IEVENT_TYPE_DEFLUX_BINARY, ie);
}
