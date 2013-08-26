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
 * HeaderDownloader.h
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
