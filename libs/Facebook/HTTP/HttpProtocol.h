/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
