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

#include <openssl/err.h>
#include <openssl/tls1.h>

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

//***************************************************************************
//SslConnection
//***************************************************************************

static void dumpSslErrors() {
	while(int err = ERR_get_error()) {
		char buf[1024];
		ERR_error_string_n(err, buf, sizeof(buf));
		LOG("%s\n", buf);
	}
}

#define TSSL_CUSTOM(func, test, action) { int _res = (int)(func); if(_res test) { IN_FILE_ON_LINE;\
	LOG("OpenSSL error %i\n", _res); dumpSslErrors(); action; } }

#define TSSL(func, test) TSSL_CUSTOM(func, test, return CONNERR_SSL)
#define TSSLZ(func) TSSL(func, == 0)
#define TSSLLTZ(func) TSSL(func, <= 0)

static SSL_CTX* sSslContext = NULL;

void MANetworkSslInit() {
	DEBUG_ASSERT(sSslContext == NULL);
	SSL_library_init();
	SSL_load_error_strings();
	sSslContext = SSL_CTX_new(SSLv23_client_method());
	TSSL_CUSTOM(sSslContext, == 0, DEBIG_PHAT_ERROR);
}

void MANetworkSslClose() {
	if(sSslContext != NULL) {
		SSL_CTX_free(sSslContext);
		sSslContext = NULL;
	}
}

SslConnection::~SslConnection() {
	close();
}

int SslConnection::connect() {
	TLTZ_PASS(TcpConnection::connect());
	TSSLZ(mSession = SSL_new(sSslContext));
	mState = eInit;
	TSSLLTZ(SSL_set_tlsext_host_name(mSession, mHostname.c_str()));
	TSSLZ(SSL_set_fd(mSession, mSock));
	TSSLLTZ(SSL_connect(mSession));
	mState = eHandshook;
	return 1;
}

int SslConnection::read(void* dst, int max) {
	int res = SSL_read(mSession, dst, max);
	if(res == 0) if(SSL_get_shutdown(mSession))
		return CONNERR_CLOSED;
	TSSLLTZ(res);
	return res;
}

int SslConnection::write(const void* src, int len) {
	TSSLLTZ(SSL_write(mSession, src, len));
	return 1;
}

void SslConnection::close() {
	if(mState == eHandshook) {
		TSSL_CUSTOM(SSL_shutdown(mSession), <0, );
		mState = eInit;
	}
	if(mState == eInit) {
		SSL_free(mSession);
	}
	mState = eIdle;
	TcpConnection::close();
}
