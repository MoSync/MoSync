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

#include "fastevents.h"
#include "sdl_syscall.h"

//***************************************************************************
//MoSyncMutex
//***************************************************************************

MoSyncMutex::MoSyncMutex() : mMutex(NULL) {}

void MoSyncMutex::init() {
	mMutex = SDL_CreateMutex();
	MYASSERT(mMutex, ERR_OOM);
}

MoSyncMutex::~MoSyncMutex() {
	DEBUG_ASSERT(mMutex == NULL);	//make sure it's closed
}

void MoSyncMutex::close() {
	if(mMutex) {
		SDL_DestroyMutex(mMutex);
		mMutex = NULL;
	}
}

void MoSyncMutex::lock() {
	DEBUG_ASRTZERO(SDL_mutexP(mMutex));
}

void MoSyncMutex::unlock() {
	DEBUG_ASRTZERO(SDL_mutexV(mMutex));
}

//***************************************************************************
//Helpers
//***************************************************************************

void ConnWaitEvent() {
	if(FE_WaitEvent(NULL) != 1) {
		LOGT("FE_WaitEvent failed");
		DEBIG_PHAT_ERROR;
	}
}
void ConnPushEvent(MAEvent* ep) {
	SDL_UserEvent event = { FE_ADD_EVENT, 0, ep, NULL };
	FE_PushEvent((SDL_Event*)&event);
}
void DefluxBinPushEvent(MAHandle handle, Stream& s) {
	SDL_UserEvent event = { FE_DEFLUX_BINARY, handle, &s, NULL };
	FE_PushEvent((SDL_Event*)&event);
}
