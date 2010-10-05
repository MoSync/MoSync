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

#include <sslsock.h>

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

int CALLBACK SSLValidateCertHook(DWORD dwType, LPVOID pvArg, DWORD dwChainLen,
	LPBLOB pCertChain, DWORD dwFlags)
{
	// Unsafe, but since we don't have any proper documentation on this function,
	// we'll keep it simple for now.
	return SSL_ERR_OKAY;
}

int SslConnection::connect() {
	int result;
	int iRet;

	// Create socket
	mSock = MASocketCreate(mHostname.c_str(), result, mInetAddr);
	if(mSock == INVALID_SOCKET) {
		LOG_GLE;
		return result;
	}

	// Set up SSL
	DWORD value = SO_SEC_SSL;
	iRet = setsockopt(mSock, SOL_SOCKET, SO_SECURE, (char*)&value, sizeof(value));
	if(iRet == SOCKET_ERROR) {
		LOG_GLE;
		return CONNERR_SSL;
	}
	SSLVALIDATECERTHOOK hook;
	hook.HookFunc = SSLValidateCertHook;
	hook.pvArg = (PVOID)mSock;
	iRet = WSAIoctl(mSock, SO_SSL_SET_VALIDATE_CERT_HOOK, &hook,
		sizeof(hook), 0, 0, NULL, 0, 0);
	if(iRet == SOCKET_ERROR) {
		LOG_GLE;
		return CONNERR_SSL;
	}
	iRet = WSAIoctl(mSock, SO_SSL_SET_PEERNAME, (LPVOID)mHostname.c_str(), mHostname.length() + 1,
		NULL, 0, NULL, NULL, NULL);
	if(iRet == SOCKET_ERROR) {
		LOG_GLE;
		return CONNERR_SSL;
	}

	// Connect to the server
	return MASocketConnect(mSock, mInetAddr, mPort);
}
