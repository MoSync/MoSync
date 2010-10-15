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
/*
 Authors: Niklas Nummelin, Romain Chalant
 */

#include <CoreFoundation/CoreFoundation.h>
#import <Foundation/NSStream.h>

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

//***************************************************************************
// Implementation of SslConnection
//***************************************************************************

void MANetworkSslInit() {
}

void MANetworkSslClose() {
}

/**
 * Establishes a connection to a defined host
 * and port. See Constructor.
 * Note: The highest level security protocol that can be negotiated
 *		 is set as the security protocol.
 *
 * @return	1 If the connection could be established.
 *			CONNERR_SSL if there was an error.
 */
int SslConnection::connect() {
	// Open a socket and bind two streams to it
	CFStringRef hostname = 
		CFStringCreateWithCString(NULL, mHostname.c_str(), kCFStringEncodingASCII);
	CFReadStreamRef readStream;
	CFWriteStreamRef writeStream;
	CFStreamCreatePairWithSocketToHost(NULL, hostname, mPort, &readStream, &writeStream);
	if (readStream == NULL || writeStream == NULL) {
		return CONNERR_SSL;
	}
	
	// Set up the streams for SSL.
	// The highest level security protocol that can be negotiated
	// is set as the security protocol.
	mInputStream = (NSInputStream *)readStream;
	mOutputStream = (NSOutputStream *)writeStream;
	[mInputStream setProperty:NSStreamSocketSecurityLevelNegotiatedSSL
					   forKey:NSStreamSocketSecurityLevelKey];
	[mOutputStream setProperty:NSStreamSocketSecurityLevelNegotiatedSSL
						forKey:NSStreamSocketSecurityLevelKey];
	[mInputStream scheduleInRunLoop:[NSRunLoop currentRunLoop]
							forMode:NSDefaultRunLoopMode];
	[mOutputStream scheduleInRunLoop:[NSRunLoop currentRunLoop]
							 forMode:NSDefaultRunLoopMode];
	
	// Open the streams
	[mInputStream open];
	[mOutputStream open];
	
	// Check if the streams could be opened and return
	int inputStreamStatus = [mInputStream streamStatus];
	int outputStreamStatus = [mOutputStream streamStatus];
	
	if ( inputStreamStatus == NSStreamStatusError 
			|| outputStreamStatus == NSStreamStatusError ) {
		return CONNERR_SSL;
	}
	else {
		return 1;
	}

}

/**
 * Reads up to a given number of bytes into a given buffer.
 *
 * @param dst	Pointer to the destination buffer
 * @param max	Maximum number of bytes to read
 * @return		A positive number indicates the number of bytes read;
 *				0 indicates that the end of the buffer was reached;
 *				CONNERR_SSL means that the operation failed.
 */
int SslConnection::read(void* dst, int max) {
	int result = [mInputStream read:(uint8_t *)dst maxLength:max];
	if (result < 0) {
		return CONNERR_SSL;
	}
	else {
		return result;
	}
}

/**
 * Writes the contents of a provided data buffer to the receiver.
 *
 * @param src	Pointer to the buffer to write
 * @param len	Number of bytes to write
 * @return		The number of bytes actually written, or CONNERR_SSL
 *				if an error occurs.
 *				If the receiver has reached its capacity, 0 is returned.
 */
int SslConnection::write(const void* src, int len) {
	int result = [mOutputStream write:(const uint8_t *)src maxLength:len];
	if (result < 0) {
		return CONNERR_SSL;
	}
	else {
		return result;
	}
}

/**
 * Closes the connection.
 */
void SslConnection::close() {
	[mInputStream close];
	[mOutputStream close];
}

/**
 * Destructor.
 */
SslConnection::~SslConnection() {
	
}
