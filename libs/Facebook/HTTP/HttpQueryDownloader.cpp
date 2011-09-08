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


#include "HttpQueryDownloader.h"
#include <MAUtil/util.h>
#include <mastdlib.h>
#include <mactype.h>
#include <mastring.h>
#include <MAP/MemoryMgr.h>
#include <mavsprintf.h>

#include "../LOG.h"

using namespace MAUtil;
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
	while (*pstr)
	{
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
	while (*pstr)
	{
		if(*pstr == '%')
		{
			if (pstr[1] && pstr[2])
			{
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
				pstr += 2;
			}
		}
		else if (*pstr == '+')
		{
			*pbuf++ = ' ';
		}
		else
		{
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
		mField(field), mValue(value) {
}

HttpQueryField::HttpQueryField(const String& field, int value) :
		mField(field) {
	mValue = integerToString(value);
}

HttpQueryField::HttpQueryField(const String& field, double value) :
		mField(field) {
	mValue = doubleToString(value);
}

HttpQueryField::HttpQueryField(const String& field, bool value) :
		mField(field) {
	if(value == true)
		mValue = "true";
	else
		mValue = "false";
}

HttpQueryDownloader::HttpQueryDownloader() :
		mMethod(HTTP_GET),
		mMultipartFormDataBoundary("----------------------------5e9f6e177b97")
{

	mDownloader = new HeaderDownloader();
	mDownloader->addDownloadListener(this);
	mDataPlaceholder = maCreatePlaceholder();
	mHttpConnection =  new HttpConnection(this);

	mMultipartFormData = new MAUtil::String();
}

void HttpQueryDownloader::clearFields() {
	mFields.clear();
}

void HttpQueryDownloader::addField(const HttpQueryField& field) {
	mFields.add(field);
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

void HttpQueryDownloader::addMultipartFormDataField(MAUtil::String &url, const MAUtil::String &key,
		const MAUtil::String &value)
{
		using MAUtil::String;
		String crlf("\r\n");

		String field = "--" + mMultipartFormDataBoundary + crlf;
		field += MAUtil::String("Content-Disposition: form-data; name=") + MAUtil::String("\"") + key +
				MAUtil::String("\"") + crlf + crlf;

		field += value + crlf;

		url += field;
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
		int res = mHttpConnection->create(url.c_str(), mMethod);
		if( res < 0 )
		{
			for(int i = 0; i < mListeners.size(); i++)
			{
				mListeners[i]->httpQueryError(this, 0);
			}
		}
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

	url += "method=POST&\n";

	getMultipartFormData(*mMultipartFormData, request->getFileRawData(), request->getFileRawDataSize(),
			request->getContentType(), request->getFileName());

	url+= *mMultipartFormData;
	LOG("\t\turl=%s", url.c_str());

	int res = mHttpConnection->create(url.c_str(), HTTP_POST);
	if(res>0)
	{
		setRequestHeader( "Content-Type", "multipart/form-data; boundary=" + mMultipartFormDataBoundary);
		writeHeaders(request->getRequestHeaders());
		write((byte*)mMultipartFormData->c_str(), mMultipartFormData->size());
	}
	else
	{
		maPanic(1, "Couln't create connection.");
	}
}

void HttpQueryDownloader::getMultipartFormData(
		MAUtil::String &url,
		const byte *fileRawData,
		int fileSize,
		const MAUtil::String &contentType,
		const MAUtil::String &fileName)
{
	using MAUtil::String;
	String crlf("\r\n");

	for(int i = 0; i < mFields.size(); i++)
	{
		addMultipartFormDataField(url, mFields[i].mField, mFields[i].mValue);
	}

	//addFile(url, fileRawData, fileSize, contentType, fileName);

	url += "--" + mMultipartFormDataBoundary + "--" + crlf + crlf;
}

//void HttpQueryDownloader::addFile(MAUtil::String &url, const byte *const fileRawData, int fileSize,
//		const MAUtil::String &contentType,
//		const MAUtil::String &fileName)
//{
//	using MAUtil::String;
//	String crlf("\r\n");
//
//	String field = "--" + mMultipartFormDataBoundary + crlf;
//	field += MAUtil::String("Content-Disposition: form-data; name=") +
//			MAUtil::String("\"file\"; filename=") + "\"" + fileName + "\"" + crlf;
//	field += "Content-Type: " + contentType + crlf + crlf;
//
//
//	LOG("\t\tadding file to the request: fileSize=%d, fileRawData=%d", fileSize, fileRawData);
//	field.append((char*)fileRawData, fileSize);
//
//	url += field;
//}

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
	LOG("HttpQueryDownloader::write, length =%d", length);
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

	int dataLength = maGetDataSize(data);
	// not tracked by manager - arrays not yet supported
	byte* bytes = new byte[dataLength];
	maReadData(data, bytes, 0, dataLength);

	maDestroyObject(data);

	for(int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->httpQueryFinished(this, (const byte*)bytes, dataLength);
	}

	delete []bytes;

}

HttpQueryDownloader::~HttpQueryDownloader()
{
	LOG("\n\tHttpQueryDownloader::~HttpQueryDownloader()");
	delete mDownloader;
	delete mHttpConnection;
}

void HttpQueryDownloader::downloadCancelled(Downloader* downloader) {
	for(int i = 0; i < mListeners.size(); i++) {
		mListeners[i]->httpQueryCancelled(this);
	}
}

void HttpQueryDownloader::error(Downloader* downloader, int code)
{
	LOG("\n\t\tHttpQueryDownloader::error, code = %d", code);

	if (code == 301 || code == 302)
	{
		String url;
		HeaderDownloader* headerDownloader = (HeaderDownloader*) downloader;
		headerDownloader->getResponseHeader("Location", url);
		LOG("\t\tRedirect. New URL = %s", url.c_str());
		headerDownloader->closeConnection();

		downloader->beginDownloading(url.c_str());
	}
	else
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

	}
	else
	{
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
#ifndef WIN32
	maPanic(1, "httpQueryError");
#endif
}
