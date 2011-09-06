/*
 * HeaderDownloader.h
 *
 *  Created on: Jun 24, 2011
 *      Author: gabi
 */

#ifndef HEADERDOWNLOADER_H_
#define HEADERDOWNLOADER_H_

#include <MAUtil/Downloader.h>
#include <MAUtil/String.h>

template<class MyDownloader = MAUtil::Downloader>
class HeaderDownloader : public MyDownloader {
public:
	int getResponseHeader(const char* header, MAUtil::String& data) const;
	void closeConnection();
};

template<class MyDownloader>
int HeaderDownloader<MyDownloader>::getResponseHeader(const char* header, MAUtil::String& data) const {
	return this->mConn->getResponseHeader(header, &data);
}

template<class MyDownloader>
void HeaderDownloader<MyDownloader>::closeConnection() {
	this->mConn->close();
}


#endif /* HEADERDOWNLOADER_H_ */
