/*
Copyright (C) 2011 MoSync AB

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
	virtual void response(HttpRequest* req, HttpResponse* res) {}
	virtual void cancel(HttpRequest* req) {}
};

class HttpProtocol : public HttpQueryDownloadListener {
public:
	HttpProtocol();

	void request(HttpRequest* req);
	void request(UploadRequest* req);

	void addListener(HttpProtocolListener* listener);

	void httpQueryFinished(HttpQueryDownloader* downloader, const byte* data, int length);
	void httpQueryCancelled(HttpQueryDownloader* downloader);
	void httpWriteFinished(HttpQueryDownloader* downloader);
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
