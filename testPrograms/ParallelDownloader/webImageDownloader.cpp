#include "webImageDownloader.h"

webImageDownloader::webImageDownloader(MAUtil::String data, MAUtil::String dataId, webImageDownloaderListener *listener){
	imageDownloader = new Downloader();
	imageDownloader->addDownloadListener(this);
	mWebListener = listener;
	mImageResource = maCreatePlaceholder();
	printf("Placeholder:%d",mImageResource);
	imageDownloader->beginDownloading(data.c_str(), mImageResource);
}

webImageDownloader::~webImageDownloader(){}

void webImageDownloader::finishedDownloading(Downloader *dl, MAHandle data){
	printf("finished downloading");

	if(dl == imageDownloader) {
	printf("beginning");

	printf("datasize = %d", maGetDataSize(data));
	printf("MImage:%d   data:%d", mImageResource, data);
	MAHandle mystore = maOpenStore("teststore.jpg", MAS_CREATE_IF_NECESSARY);
	if(mystore != STERR_NONEXISTENT) {
		MAHandle myData = maCreatePlaceholder();
		//if(maCreateData(myData, maGetDataSize(data)) == RES_OK) {
		//	maWriteData(myData, text.c_str(), 0, maGetDataSize(data));
		//}
		int result = maWriteStore(mystore, data);
		 maCloseStore(mystore, 0);
		 maDestroyPlaceholder(myData);
	}
	maDestroyPlaceholder(mImageResource);
	mWebListener->downloadFinished(this);
	printf("end of downloader");
	}
}

void webImageDownloader::error(Downloader* dl, int code) { }
//maAlert("error", "", "ok", 0, 0);}
void webImageDownloader::downloadCancelled(Downloader* dl) {}
void webImageDownloader::notifyProgress(Downloader *dl, int downloadedBytes, int totalBytes){}
