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
