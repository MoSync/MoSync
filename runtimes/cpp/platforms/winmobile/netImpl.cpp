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

MoSyncMutex::MoSyncMutex() : mMutex(NULL) {}

void MoSyncMutex::init() {
	mMutex = CreateMutex(NULL, FALSE, NULL);
	GLE(mMutex);
}

MoSyncMutex::~MoSyncMutex() {
	DEBUG_ASSERT(mMutex == NULL);
}

void MoSyncMutex::close() {
	if(mMutex != NULL) {
		GLE(CloseHandle(mMutex));
		mMutex = NULL;
	}
}

void MoSyncMutex::lock() {
	if(WaitForSingleObject(mMutex, INFINITE) == WAIT_FAILED) {
		GLE(0);
	}
}

void MoSyncMutex::unlock() {
	GLE(ReleaseMutex(mMutex));
}

//***************************************************************************
//Helpers
//***************************************************************************
namespace Base {
extern HWND g_hwndMain;
extern WSADATA wsaData;
}

void ConnWaitEvent() {
	if(MsgWaitForMultipleObjects(NULL, NULL, FALSE, INFINITE, QS_ALLEVENTS) == WAIT_FAILED) {
		//LOGT("MsgWaitForMultipleObjects failed");
		//MoSyncErrorExit();
		BIG_PHAT_ERROR(ERR_INTERNAL);
	}
}
void ConnPushEvent(MAEvent* ep) {
	PostMessage(g_hwndMain, WM_ADD_EVENT, (WPARAM) ep, 0);
}
void DefluxBinPushEvent(MAHandle handle, Stream& s) {
	PostMessage(g_hwndMain, WM_DEFLUX_BINARY, (WPARAM) &s, handle);
}

//***************************************************************************
//SslConnection
//***************************************************************************

void MANetworkSslInit() {
}

void MANetworkSslClose() {
}

SslConnection::~SslConnection() {
	close();
}

int SslConnection::connect() {
	DEBIG_PHAT_ERROR;
}

int SslConnection::read(void* dst, int max) {
	DEBIG_PHAT_ERROR;
}

int SslConnection::write(const void* src, int len) {
	DEBIG_PHAT_ERROR;
}

void SslConnection::close() {
	DEBIG_PHAT_ERROR;
}
