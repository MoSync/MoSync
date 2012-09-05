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
