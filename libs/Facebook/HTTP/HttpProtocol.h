/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * HTTPProtocol.h
 *
 *  Created on: Feb 23, 2011
 *      Author: magnus
 */

#ifndef HTTPPROTOCOL_H_
#define HTTPPROTOCOL_H_

#include <MAUtil/String.h>
#include <MAUtil/Map.h>

#include "HttpQueryDownloader.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace MAUtil;

class HttpProtocolListener {
public:
	virtual void response(HttpRequest* request, HttpResponse* response) = 0;
	virtual void cancel(HttpRequest* request) {}
};

class HttpProtocol : public HttpQueryDownloadListener {
public:
	HttpProtocol();

	void request(HttpRequest* request);
	void request(UploadRequest* request);

	void addListener(HttpProtocolListener* listener);

	// HttpQueryDownloadListener
	void httpQueryFinished(HttpQueryDownloader* downloader, const byte* data, int length);
	void httpQueryFinished(HttpQueryDownloader* downloader, MAHandle resource) GCCATTRIB(noreturn);
	void httpWriteFinished(HttpQueryDownloader* downloader, int code) GCCATTRIB(noreturn);
	void httpQueryCancelled(HttpQueryDownloader* downloader);
	void httpQueryError(HttpQueryDownloader *downloader, int code);

	inline virtual ~HttpProtocol() {}

private:
	void writeHeaders(HttpQueryDownloader *downloader, HttpRequest *request);
	void writeMessageBody(HttpQueryDownloader *downloader, HttpRequest *request);

protected:
	HttpRequest* getRequest(HttpQueryDownloader*);
	Vector<HttpProtocolListener*> mListeners;
	Map<HttpQueryDownloader*, HttpRequest*> mRequests;
};

#endif /* HTTPPROTOCOL_H_ */
