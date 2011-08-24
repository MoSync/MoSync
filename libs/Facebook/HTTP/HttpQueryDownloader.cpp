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

#include "HttpQueryDownloader.h"
#include <MAUtil/util.h>
#include <mastdlib.h>
#include <mactype.h>
#include <mastring.h>
#include <MAP/MemoryMgr.h>
//#include <mastdio.h>
#include <mavsprintf.h>

#include "LOG.h"

using namespace MAPUtil;
using namespace MAUtil;

// code found here:
// http://www.geekhideout.com/urlcode.shtml

/* Converts a hex character to its integer value */
char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char to_hex(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(const char *str) {
	const char *pstr = str;
	char *buf = (char*)malloc(strlen(str) * 3 + 1), *pbuf = buf;
	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ')
			*pbuf++ = '+';
		else
			*pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(const char *str) {
	const char *pstr = str;
	char *buf = (char*)malloc(strlen(str) + 1), *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') {
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

//------------------------------------------------------------------------------------

HttpQueryField::HttpQueryField() : mField(""), mValue("") {
}

HttpQueryField::HttpQueryField(const String& field, const String& value) :
	mField(field), mValue(value)
{
}

HttpQueryField::HttpQueryField(const String& field, int value) :
	mField(field)
{
	mValue = integerToString(value);
}

HttpQueryField::HttpQueryField(const String& field, double value) :
	mField(field)
{
	mValue = doubleToString(value);
}

HttpQueryField::HttpQueryField(const String& field, bool value) :
	mField(field)
{
	if(value == true)
		mValue = "true";
	else
		mValue = "false";
}

HttpQueryDownloader::HttpQueryDownloader(bool deliverResource) :
	mMethod(HTTP_GET),
	mURL(""),
	mDeliverResource(deliverResource),
	mMultipartFormDataBoundary("----------------------------5e9f6e177b97")
{
	mDownloader = new HeaderDownloader();
	mDownloader->addDownloadListener(this);
	mDataPlaceholder = maCreatePlaceholder();

	mHttpConnection =	new HttpConnection(this);

	multipartFormData = new String();
}

void HttpQueryDownloader::clearFields() {
	mFields.clear();
}

void HttpQueryDownloader::addField(const HttpQueryField& field) {
	mFields.add(field);
}

void HttpQueryDownloader::doQuery() {
	if(mURL == "")
		maPanic(1, "Invalid url!");

	String url = mURL;
	if(mFields.size()>0)
	{
		url += "?";
	}

	if(HTTP_POST == mMethod )
	{
		if(mFields.size() == 0)
		{
			url += "&";
		}
		url += "method=POST&";
	}

	addEncodedFieldsToUrl(url);

	LOG("\n\t\t\t\tencodedURL: %s\n", url.c_str());

	if (mMethod == HTTP_GET) {
		// For GET requests, we use the downloader
		// Then we won't even need my changes to downloader
		mDownloader->beginDownloading(url.c_str()); //mDataPlaceholder

	} else if (HTTP_POST == mMethod || HTTP_DELETE == mMethod) {
		// For POST, we use HttpConnection
		mHttpConnection->create(url.c_str(), mMethod);
		// FIXME: check return value.
	}
}

void HttpQueryDownloader::addEncodedFieldsToUrl(MAUtil::String &url)
{
	for(int i = 0; i < mFields.size(); i++)
	{
		const HttpQueryField& field = mFields[i];

		char* encodedName = url_encode(field.mField.c_str());
		char* encodedValue = url_encode(field.mValue.c_str());

		url += String(encodedName, strlen(encodedName)) + "=" + String(encodedValue, strlen(encodedValue));
		if(i!=(mFields.size()-1)) url+= "&";

		free(encodedName);
		free(encodedValue);
	}
}

void HttpQueryDownloader::sendRequest(HttpRequest *request)
{
	setMethod(request->getMethod());
	setURL(request->getUrl());
	writeMessageBody(request->getMessageBodyParams());

	if(mURL == "")
		maPanic(1, "Invalid url!");


	String url = mURL;
	if(mFields.size()>0)
	{
		url += "?";
	}

	if(HTTP_POST == mMethod )
	{
		if(mFields.size() == 0)
		{
			url += "&";
		}
		url += "method=POST&";
	}

	addEncodedFieldsToUrl(url);

	LOG("\n\t\t\t\tencodedURL: %s\n", url.c_str());

	if (mMethod == HTTP_GET) {
		// For GET requests, we use the downloader
		// Then we won't even need my changes to downloader
		mDownloader->beginDownloading(url.c_str()); //mDataPlaceholder

	} else if (HTTP_POST == mMethod || HTTP_DELETE == mMethod) {
		// For POST, we use HttpConnection
		mHttpConnection->create(url.c_str(), mMethod);
		// FIXME: check return value.
	}

	writeHeaders(request->getRequestHeaders());

	if (request->getMethod() == HTTP_POST || request->getMethod() == HTTP_DELETE)
	{
		if(request->getDataSize()>0)
		{
			write(request->getData(), request->getDataSize());
		}
	}
}

void HttpQueryDownloader::sendRequest(UploadRequest *request)
{
	setMethod(request->getMethod());
	setURL(request->getUrl());
	writeMessageBody(request->getMessageBodyParams());

	if(mURL == "")
		maPanic(1, "Invalid url!");


	String url = mURL;
	if(mFields.size()>0)
	{
		url += "?";
	}

	url += "method=POST&"; //HTTP/1.1

	PICTURE = request->getPicture();

	getMultipartFormData(*multipartFormData, request->getFileRawData(), request->getFileRawDataSize(),
			request->getContentType(), request->getFileName());

	url+= *multipartFormData;

	MAHandle testStore = maOpenStore("TEST.txt", MAS_CREATE_IF_NECESSARY);
	MAHandle myData = maCreatePlaceholder();
	if(maCreateData(myData, multipartFormData->length()) == RES_OK)
	{
		LOG("\t\t writing the request to file.");
		maWriteData(myData, multipartFormData->c_str(), 0, multipartFormData->length());
		maWriteStore(testStore, myData);
		// FIXME: check return value.
	}
	else
	{
		maPanic(1, "error creating store");
	}

	int res = mHttpConnection->create(url.c_str(), HTTP_POST);
	if(res>0)
	{
		setRequestHeader( "Content-Type", "multipart/form-data; boundary=" + mMultipartFormDataBoundary);
		writeHeaders(request->getRequestHeaders());
		write((byte*)multipartFormData->c_str(), multipartFormData->size());
	}
	else
	{
		maPanic(1, "Couln't create connection.");
	}

}

void HttpQueryDownloader::getMultipartFormData(MAUtil::String &url, const int *const fileRawData, int fileSize,
	const MAUtil::String &contentType,
	const MAUtil::String &fileName)
{
	using MAUtil::String;
	String crlf("\r\n");

	for(int i = 0; i < mFields.size(); i++)
	{
		addMultipartFormDataField(url, mFields[i].mField, mFields[i].mValue);
	}

	addFile(url, fileRawData, fileSize, contentType, fileName);

	url += "--" + mMultipartFormDataBoundary + "--" + crlf + crlf;
}

void HttpQueryDownloader::addFile(MAUtil::String &url, const int *const fileRawData, int fileSize,
	const MAUtil::String &contentType,
	const MAUtil::String &fileName)
{
	using MAUtil::String;
	String crlf("\r\n");

	String field = "--" + mMultipartFormDataBoundary + crlf;
	field += MAUtil::String("Content-Disposition: form-data; name=") +
			MAUtil::String("\"file\"; filename=") + "\"" + fileName + "\"" + crlf;
	field += "Content-Type: " + contentType + crlf + crlf;//e.g: image/jpeg

	//MAUtil::String fileBuffer;
	//convertIntArrayToString(fileRawData, fileSize, fileBuffer);
	//field += fileBuffer;

	char* encodedName = url_encode(PICTURE.c_str());
	field += String(encodedName, strlen(encodedName)) + crlf + crlf;
	free(encodedName);

//	field += PICTURE + crlf + crlf;

	url += field;
}

/*
 * Converts an array of ints into a string object.
 */
void HttpQueryDownloader::convertIntArrayToString(const int *const pixels, int arraySize, MAUtil::String &str)
{
	char buff [sizeof(int)+1];

	//int SIZE = sizeof(pixels)/sizeof(int);
	char result[arraySize*sizeof(int)];

	for(int i=0; i<arraySize; i++)
	{
		memcpy(buff, &pixels[i], sizeof(int));
		buff[sizeof(int)]='\0';
		memcpy(result + i*sizeof(int), buff, sizeof(int));

	}

	str += result;
}

void HttpQueryDownloader::addMultipartFormDataField(MAUtil::String &url, const MAUtil::String &key, const MAUtil::String &value)
{
	using MAUtil::String;
	String crlf("\r\n");

	String field = "--" + mMultipartFormDataBoundary + crlf;
	field += MAUtil::String("Content-Disposition: form-data; name=") + MAUtil::String("\"") + key + MAUtil::String("\"")
	+ crlf + crlf;

	field += value + crlf;

	url += field;
}

void HttpQueryDownloader::writeMessageBody(const Map<String, String> &params)
{
	for (Map<String, String>::ConstIterator iter = params.begin(); iter != params.end(); ++iter)
	{
		addField(HttpQueryField(iter->first, iter->second));
	}
}

void HttpQueryDownloader::writeHeaders(const Map<String, String> &params)
{
	for(Map<String, String>::ConstIterator iter = params.begin(); iter!=params.end(); ++iter)
	{
		setRequestHeader(iter->first, iter->second);
	}
}

void HttpQueryDownloader::write(const byte* data, int length) {
	// TODO: must only be called once, unlike HttpConnection::write
	char buffer[256];
	sprintf(buffer, "%d", length);
	mHttpConnection->setRequestHeader("content-length", buffer);
	LOG("!!!!!!!!!!!!!!!!!!!!HttpQueryDownloader::write, length =%d", length);
	mHttpConnection->write(data, length);
}

void HttpQueryDownloader::setMethod(int method) {
	mMethod = method;
}

void HttpQueryDownloader::setURL(const String& url) {
//	LOG("%s", url.c_str());
	mURL = url;
}

void HttpQueryDownloader::setRequestHeader(const MAUtil::String &key, const MAUtil::String &value)
{
	mHttpConnection->setRequestHeader(key.c_str(), value.c_str());
}

void HttpQueryDownloader::addListener(HttpQueryDownloadListener* listener) {
	mListeners.add(listener);
}

void HttpQueryDownloader::finishedDownloading(Downloader* downloader, MAHandle data) {
	if(mDeliverResource) {
		for(int i = 0; i < mListeners.size(); i++) {
			mListeners[i]->httpQueryFinished(this, data);
		}
	} else {
		int dataLength = maGetDataSize(data);
		// not tracked by manager - arrays not yet supported
		byte* bytes = new byte[dataLength];
		maReadData(data, bytes, 0, dataLength);

		maDestroyObject(data);

		for(int i = 0; i < mListeners.size(); i++) {
			mListeners[i]->httpQueryFinished(this, (const byte*)bytes, dataLength);
		}

		// not tracked by manager - arrays not yet supported
		// NOTE (LAV 20110120): should be delete[] bytes?
		// niklas: Only if it is an array of objects (to make delete run destructors on all the objects, as far as I remember).
		delete []bytes;
	}
}

HttpQueryDownloader::~HttpQueryDownloader()
{
	delete mDownloader;
	delete mHttpConnection;
	delete multipartFormData;
}

void HttpQueryDownloader::downloadCancelled(Downloader* downloader) {
	for(int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->httpQueryCancelled(this);
	}
}

void HttpQueryDownloader::error(Downloader* downloader, int code) {
	LOG("\n\n\t\tHttpQueryDownloader::error, code = %d", code);

	if (code == 301 || code == 302) {
		String url;
		HeaderDownloader* headerDownloader = (HeaderDownloader*) downloader;
		headerDownloader->getResponseHeader("Location", url);
		// FIXME: check return value.
		LOG("\t\tRedirect. New URL = %s", url.c_str());
		headerDownloader->closeConnection();

		downloader->beginDownloading(url.c_str());
	} else
	{
		for(int i = 0; i < mListeners.size(); i++)
		{
			mListeners[i]->httpQueryError(this, code);
		}
	}
}

void HttpQueryDownloader::httpFinished(MAUtil::HttpConnection *conn, int result) {
	 LOG("\n\n\t\tHttpQueryDownloader::httpFinished %d", result);
	 String contentLengthStr;

	int responseBytes = mHttpConnection->getResponseHeader("Content-Length", &contentLengthStr);
		LOG("\n\n\t\tHttpQueryDownloader::httpFinished contentLength=%s", contentLengthStr.c_str());
		int contentLength = 0;
		if(responseBytes != CONNERR_NOHEADER)
				contentLength = atoi(contentLengthStr.c_str());

		if(contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0) {
			maPanic(1, "finished: HTTP RECV not supported");
				mHttpConnection->recv(mHttpConnectionBuffer, CONNECTION_BUFFER_SIZE);

		} else {
				mHttpConnectionBuffer[contentLength] = 0;
				mHttpConnection->read(mHttpConnectionBuffer, contentLength);
		}
}

void HttpQueryDownloader::connRecvFinished(MAUtil::Connection *conn, int result) {
	LOG("HttpQueryDownloader::connRecvFinished %d", result);
	mHttpConnection->close();
}

void HttpQueryDownloader::connReadFinished(MAUtil::Connection *conn, int result) {
	LOG("\n\t\tHttpQueryDownloader::connReadFinished %d", result);

	mHttpConnection->close();

	int dataLength = strlen(mHttpConnectionBuffer);

	for(int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->httpQueryFinished(this, (const byte*)mHttpConnectionBuffer, dataLength);
	}
	lprintfln("data: %s", mHttpConnectionBuffer);
}

void HttpQueryDownloader::connWriteFinished(MAUtil::Connection *conn, int result) {
	LOG("\t\tHttpQueryDownloader::connWriteFinished %d", result);
	for(int i = 0; i < mListeners.size(); i++)
	{
		mListeners[i]->httpWriteFinished(this, result);
	}
	if(result<0)
	{
		maMessageBox("Error HttpConnection", "write operation failed");
		return;
	}

	mHttpConnection->finish();
}

void HttpQueryDownloadListener::httpQueryCancelled(HttpQueryDownloader* downloader) {
}

void HttpQueryDownloadListener::httpQueryError(HttpQueryDownloader* downloader, int code) {
	LOG("\n\t\tHttpQueryDownloadListener::httpQueryError %d", code);
	maPanic(1, "httpQueryError");
}
