/*
 * HTTPProtocol.cpp
 *
 *  Created on: Feb 23, 2011
 *      Author: Magnus Hult <magnus@magnushult.se>
 */

#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <mavsprintf.h>

#include "HttpQueryDownloader.h"
#include "HttpProtocol.h"
#include "../LOG.h"

using namespace MAUtil;

HttpProtocol::HttpProtocol() {
}

void HttpProtocol::request(HttpRequest* req)
{

	HttpQueryDownloader* downloader = new HttpQueryDownloader();
	// We store the mapping downloader -> request to be able to give the
	// request instead of the downloader to the listener
	mRequests[downloader] = req;

	// Then we add ourselves as the listener
	downloader->addListener(this);

	downloader->sendRequest(req);
}

void HttpProtocol::request(UploadRequest* req)
{
	HttpQueryDownloader* downloader = new HttpQueryDownloader();
	// We store the mapping downloader -> request to be able to give the
	// request instead of the downloader to the listener
	mRequests[downloader] = req;

	// Then we add ourselves as the listener
	downloader->addListener(this);

	downloader->sendRequest(req);
}

void HttpProtocol::addListener(HttpProtocolListener* listener) {
	mListeners.add(listener);
}

void HttpProtocol::httpQueryFinished(HttpQueryDownloader *downloader,
		const byte* data, int length)
{
	HttpRequest *req = getRequest(downloader);
	HttpResponse *res = new HttpResponse(200, length, data);

	for (int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->response(req, res);
	}

	delete downloader;
}

void HttpProtocol::httpQueryCancelled(HttpQueryDownloader *downloader)
{
	HttpRequest *req = getRequest(downloader);

	for (int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->cancel(req);
	}

	delete downloader;
}

void HttpProtocol::httpQueryError(HttpQueryDownloader *downloader, int code)
{
	LOG("\n\tHttpProtocol::httpQueryError code = %d", code);

	HttpRequest *req = getRequest(downloader);
	HttpResponse *res = new HttpResponse(code);

	for (int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->response(req, res);
	}

	mRequests.erase(downloader);
	//delete downloader;
}

HttpRequest* HttpProtocol::getRequest(HttpQueryDownloader* downloader)
{

	HttpRequest *req = mRequests[downloader];
	if (req == NULL)
		maPanic(1, "No request found");
	return req;
}

void HttpProtocol::writeHeaders(HttpQueryDownloader *downloader, HttpRequest *req)
{
	const Map<String, String>& params = req->getRequestHeaders();
	for(Map<String, String>::ConstIterator iter = params.begin(); iter!=params.end(); ++iter)
	{
		downloader->setRequestHeader(iter->first, iter->second);
	}
}

void HttpProtocol::writeMessageBody(HttpQueryDownloader *downloader, HttpRequest *req)
{
	const Map<String, String>& params = req->getMessageBodyParams();
	for (Map<String, String>::ConstIterator iter = params.begin(); iter != params.end(); ++iter)
	{
		downloader->addField(HttpQueryField(iter->first, iter->second));
	}
}
