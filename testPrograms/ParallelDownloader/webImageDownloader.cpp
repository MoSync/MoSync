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
