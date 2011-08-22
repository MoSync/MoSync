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

#ifndef _HTTP_QUERY_DOWNLOADER_H_
#define _HTTP_QUERY_DOWNLOADER_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include <MAUtil/Downloader.h>
#include <MAUtil/Map.h>

#include "HeaderDownloader.h"
#include "HttpRequest.h"
#include "UploadRequest.h"

#define CONNECTION_BUFFER_SIZE 2048

char *url_encode(const char *str);

class HttpQueryField {
public:
	HttpQueryField();
	HttpQueryField(const MAUtil::String& field, const MAUtil::String& value);
	HttpQueryField(const MAUtil::String& field, int value);
	HttpQueryField(const MAUtil::String& field, double value);
	HttpQueryField(const MAUtil::String& field, bool value);

	friend class HttpQueryDownloader;

private:
	MAUtil::String mField;
	MAUtil::String mValue;
};

// performs a get query with the provided fields.

class HttpQueryDownloadListener {
public:
	virtual void httpQueryFinished(HttpQueryDownloader* downloader,
		const byte* data,
		int length) = 0;
	virtual void httpQueryFinished(HttpQueryDownloader* downloader, MAHandle resource) = 0;
	virtual void httpQueryCancelled(HttpQueryDownloader* downloader);
	virtual void httpQueryError(HttpQueryDownloader* downloader, int code) GCCATTRIB(noreturn);
	virtual void httpWriteFinished(HttpQueryDownloader* downloader, int result) = 0;
};

class HttpQueryDownloader : MAUtil::DownloadListener, MAUtil::HttpConnectionListener {
public:
	HttpQueryDownloader(bool deliverResource = false);

	void clearFields();
	void addField(const HttpQueryField& field);

	void setMethod(int method);

	// the final url will be composed as url?fields..
	void setURL(const MAUtil::String& url);

	void setRequestHeader(const MAUtil::String &key, const MAUtil::String &value);

	void addListener(HttpQueryDownloadListener* listener);

	// will start and perform the actual query
	void doQuery();

	void sendRequest(HttpRequest *request);

	void sendRequest(UploadRequest *request);

	// for posts, we can then write to it
	void write(const byte* data, int length);

	virtual ~HttpQueryDownloader();


private:
	void addEncodedFieldsToUrl(MAUtil::String &url);

	void getMultipartFormData(MAUtil::String &url, const int *fileRawData, int fileSize,
		const MAUtil::String &contentType, const MAUtil::String &fileName);

	void addMultipartFormDataField(MAUtil::String &url, const MAUtil::String &key, const MAUtil::String &value);
	void addFile(MAUtil::String &url, const int *const fileRawData, int fileSize, const MAUtil::String &contentType,
		const MAUtil::String &fileName="Tami.jpg");

	void writeMessageBody(const MAUtil::Map<String, String> &params);
	void writeHeaders(const Map<String, String> &params);
	void convertIntArrayToString(const int *const pixels, int arraySize, MAUtil::String &str);
private:
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data);
	void downloadCancelled(MAUtil::Downloader* downloader);
	void error(MAUtil::Downloader* downloader, int code);

    void httpFinished(MAUtil::HttpConnection *conn, int result);
    void connRecvFinished(MAUtil::Connection *conn, int result);
    void connReadFinished(MAUtil::Connection *conn, int result);
    void connWriteFinished(MAUtil::Connection* conn, int result);

	MAHandle mDataPlaceholder;
	MAUtil::Vector<HttpQueryField> mFields;
	MAUtil::Map<MAUtil::String, MAUtil::String> mHeaders;

	typedef HeaderDownloader<MAUtil::Downloader> HeaderDownloader;
	HeaderDownloader* mDownloader;

	MAUtil::HttpConnection* mHttpConnection;
	char mHttpConnectionBuffer[CONNECTION_BUFFER_SIZE];

	int mMethod;
	MAUtil::String mURL;

	MAUtil::Vector<HttpQueryDownloadListener*> mListeners;

	bool mDeliverResource;

	MAUtil::String mMultipartFormDataBoundary;

	MAUtil::String PICTURE;
	MAUtil::String *multipartFormData;
};

#endif	//_HTTP_QUERY_DOWNLOADER_H_
