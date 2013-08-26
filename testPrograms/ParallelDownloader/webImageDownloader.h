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

#ifndef WEBIMAGEDOWNLOADER_H_
#define WEBIMAGEDOWNLOADER_H_

#include <maapi.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

#include <ma.h> 				// Syscalls
#include <MAUtil/String.h>		// C++ String class
#include <MAUtil/Downloader.h>

using namespace MAUtil;
class webImageDownloader;
class webImageDownloaderListener {
public:
	virtual void downloadFinished(webImageDownloader *downloader) = 0;
};

class webImageDownloader :  public DownloadListener {
public:
	webImageDownloader(MAUtil::String data, MAUtil::String dataId, webImageDownloaderListener *listener);
	virtual ~webImageDownloader();

    void notifyProgress(Downloader *dl, int downloadedBytes, int totalBytes);
    void error(Downloader* dl, int code);
    void downloadCancelled(Downloader* dl);

	void finishedDownloading(Downloader *dl, MAHandle data);

	MAHandle newPlaceholder;

	MAHandle mImageResource;
	Downloader * imageDownloader;

	DownloadListener * listener;
	webImageDownloaderListener *mWebListener;
};


#endif /* WEBIMAGEDOWNLOADER_H_ */
