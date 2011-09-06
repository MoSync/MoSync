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
									int length) {}
	virtual void httpQueryFinished(HttpQueryDownloader* downloader, MAHandle resource) {}
	virtual void httpQueryCancelled(HttpQueryDownloader* downloader);
	virtual void httpQueryError(HttpQueryDownloader* downloader, int code) GCCATTRIB(noreturn);
	virtual void httpWriteFinished(HttpQueryDownloader* downloader, int result) {}
};

class HttpQueryDownloader : MAUtil::DownloadListener, MAUtil::HttpConnectionListener {
public:
	HttpQueryDownloader();

	void clearFields();
	void addField(const HttpQueryField& field);

	void setMethod(int method);

	// the final url will be composed as url?fields..
	void setURL(const MAUtil::String& url);

	void setRequestHeader(const MAUtil::String &key, const MAUtil::String &value);

	void addListener(HttpQueryDownloadListener* listener);

	void sendRequest(HttpRequest *req);
	void sendRequest(UploadRequest *req);


	// for posts, we can then write to it
	void write(const byte* data, int length);

	virtual ~HttpQueryDownloader();


private:
	void writeMessageBody(const MAUtil::Map<String, String> &params);
	void writeHeaders(const Map<String, String> &params);
	void addEncodedFieldsToUrl(MAUtil::String &url);
	void addMultipartFormDataField(MAUtil::String &url, const MAUtil::String &key, const MAUtil::String &value);


	void getMultipartFormData(MAUtil::String &url, const byte *fileRawData, int fileSize,
			const MAUtil::String &contentType, const MAUtil::String &fileName);

	void addFile(MAUtil::String &url, const byte *const fileRawData, int fileSize, const MAUtil::String &contentType,
			const MAUtil::String &fileName);
private:
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data);
	void downloadCancelled(MAUtil::Downloader* downloader);
	void error(MAUtil::Downloader* downloader, int code);

    void httpFinished(MAUtil::HttpConnection *conn, int result);
    void connRecvFinished(MAUtil::Connection *conn, int result);
    void connReadFinished(MAUtil::Connection *conn, int result);
    void connWriteFinished(MAUtil::Connection* conn, int result);

private:
	MAHandle 									mDataPlaceholder;
	MAUtil::Vector<HttpQueryField> 				mFields;
	MAUtil::Map<MAUtil::String, MAUtil::String> mHeaders;

	typedef HeaderDownloader<MAUtil::Downloader> HeaderDownloader;
	HeaderDownloader* 							mDownloader;

	MAUtil::HttpConnection* 					mHttpConnection;
	char 										mHttpConnectionBuffer[CONNECTION_BUFFER_SIZE];

	int 										mMethod;
	MAUtil::String 								mURL;
	MAUtil::String								mMultipartFormDataBoundary;

	MAUtil::String  							*mMultipartFormData;

	MAUtil::Vector<HttpQueryDownloadListener*> mListeners;
};

#endif
