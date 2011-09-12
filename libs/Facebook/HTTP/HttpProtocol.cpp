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
 * HTTPProtocol.cpp
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
