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
#include <wincrypt.h>
#include <schnlsp.h>
#include <Schannel.h>

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

static HMODULE sSchannelDLL = NULL;
static SSL_CRACK_CERTIFICATE_FN sSslCrackCertificate = NULL;
static SSL_FREE_CERTIFICATE_FN sSslFreeCertificate = NULL;

void MANetworkSslInit() {
	sSchannelDLL = LoadLibrary(TEXT("schannel.dll"));
	if(sSchannelDLL == NULL) {
		LOG("Cannot load schannel.dll. SSL will be unavailable.\n");
		return;
	}
	sSslCrackCertificate = (SSL_CRACK_CERTIFICATE_FN)GetProcAddress(sSchannelDLL,
		TEXT("SslCrackCertificate"));
	DEBUG_ASSERT(sSslCrackCertificate);
	sSslFreeCertificate = (SSL_FREE_CERTIFICATE_FN)GetProcAddress(sSchannelDLL,
		TEXT("SslFreeCertificate"));
	DEBUG_ASSERT(sSslFreeCertificate);
}

void MANetworkSslClose() {
	if(sSchannelDLL) {
		FreeLibrary(sSchannelDLL);
		sSchannelDLL = NULL;
	}
}

SslConnection::~SslConnection() {
	close();
}

static BOOL IsEarlierThan(FILETIME *ft1, FILETIME *ft2) {
	return CompareFileTime(ft1, ft2) < 0;
}

static char *ParseCN(char *pchSubjLine) {
	char *pchCommonName = NULL;
	char *pchEnd = NULL;

	if (!pchSubjLine) return NULL;

	pchSubjLine = _strlwr(pchSubjLine);
	pchCommonName = strstr(pchSubjLine, "cn=");
	if (!pchCommonName) {
		return NULL;
	}

	pchCommonName += 3;
	pchEnd = pchCommonName;
	// find the next separator: , or \t or space, or apos
	while (*pchEnd != '\0'
		&& *pchEnd != ','
		&& *pchEnd != ' '
		&& *pchEnd != '\t'
		&& *pchEnd != '\'')
	{
		pchEnd++;
	}
	if (*pchEnd != '\0') *pchEnd = '\0';

	return pchCommonName;
}

static int SSLValidateCertHook2(DWORD dwType, LPVOID pvArg, DWORD dwChainLen,
	LPBLOB pCertChain, DWORD dwFlags)
{
	SslConnection* conn = (SslConnection*)pvArg;
	X509Certificate* pCert = NULL;
	int nRet = SSL_ERR_CERT_UNKNOWN;
	LOG("SSLValidateCertHook 0x%x %s, %u, %u bytes, 0x%x\n", dwType, conn->hostname(),
		dwChainLen, pCertChain->cbSize, dwFlags);

	// dwType must be SSL_CERT_X.509
	if (dwType != SSL_CERT_X509) {
		FAIL_PASS(nRet);
	}

	// root chain check is done by the OS.
	if (dwFlags & SSL_CERT_FLAG_ISSUER_UNKNOWN) {
		FAIL_PASS(nRet);
	}

	if (pCertChain == NULL) return nRet;
	ASSERT(dwChainLen == 1);

	if (!sSslCrackCertificate || !sSslFreeCertificate) {
		FAIL_PASS(nRet);
	}

	// crack X.509 Certificate
	if (!sSslCrackCertificate(pCertChain->pBlobData, pCertChain->cbSize, TRUE, &pCert)) {
		FAIL_PASS(SSL_ERR_BAD_DATA);
	}

#if 0	// todo: fix wildcard handling
	// Site check
	{
		char* pchSubject = NULL;
		char* pchCN = NULL;
		BOOL bMatched = FALSE;

		pchSubject = pCert->pszSubject;

		// here you need to parse the subjec to retrieve the CN name
		pchCN = ParseCN(pchSubject);
		if (!pchCN) {
			LOG("ParseCN fail\n");
			goto FuncExit;
		}

		// CN comparison
		bMatched = !(_stricmp(conn->hostname(), pchCN));
		if (!bMatched) {
			LOG("CN mismatch\n");
			goto FuncExit;
		}
	}
#endif

	// validFrom, validUntil check
	{
		SYSTEMTIME stNow;
		FILETIME ftNow;
		FILETIME ftValidFrom = pCert->ValidFrom;
		FILETIME ftValidUntil = pCert->ValidUntil;

		GetSystemTime(&stNow);
		SystemTimeToFileTime(&stNow, &ftNow);

		if(!(IsEarlierThan(&ftValidFrom, &ftNow) && IsEarlierThan(&ftNow, &ftValidUntil))) {
			FAIL_PASS(SSL_ERR_CERT_EXPIRED);
		}
	}

	nRet = SSL_ERR_OKAY;

//FuncExit:
	sSslFreeCertificate(pCert);

	LOG("SSL Cert result: %i\n", nRet);
	return nRet;
}

int CALLBACK SSLValidateCertHook(DWORD dwType, LPVOID pvArg, DWORD dwChainLen,
	LPBLOB pCertChain, DWORD dwFlags)
{
	SslConnection* conn = (SslConnection*)pvArg;
	conn->mSslError = SSLValidateCertHook2(dwType, pvArg, dwChainLen, pCertChain, dwFlags);
	return conn->mSslError;
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
	hook.pvArg = (PVOID)this;
	iRet = WSAIoctl(mSock, SO_SSL_SET_VALIDATE_CERT_HOOK, &hook,
		sizeof(hook), 0, 0, NULL, 0, 0);
	if(iRet == SOCKET_ERROR) {
		LOG_GLE;
		return CONNERR_SSL;
	}

#if _WIN32_WCE >= 0x500
	iRet = WSAIoctl(mSock, SO_SSL_SET_PEERNAME, (LPVOID)mHostname.c_str(), mHostname.length() + 1,
		NULL, 0, NULL, NULL, NULL);
	if(iRet == SOCKET_ERROR) {
		LOG_GLE;
		return CONNERR_SSL;
	}
#endif

	// Connect to the server
	mSslError = 0;
	iRet = MASocketConnect(mSock, mInetAddr, mPort);
	if(mSslError != 0)
		return CONNERR_SSL;
	return iRet;
}
