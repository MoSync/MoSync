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

#include <MAUtil/Moblet.h>
#include <MAUtil/Downloader.h>

#include <conprint.h>

using namespace MAUtil;


class MyMoblet : public Moblet, public DownloadListener {
public:
	MyMoblet() {
		printf("hello!\n");
		
		audioDownloader = new AudioDownloader();
		audioDownloader->addDownloadListener(this);
		audioDownloader->beginDownloading("http://http.de.scene.org/pub/music/groups/mono/mp3/sushi-brother-lionfish.mp3", 0, "audio/mpeg");

		imageDownloader = new ImageDownloader();
		imageDownloader->addDownloadListener(this);
		imageDownloader->beginDownloading("http://www.thealarmclock.com/mt/archives/porn.png", 0);
		
		Extent e = maGetScrSize();
		maSetColor(0);
		maFillRect(0,0, EXTENT_X(e), EXTENT_Y(e));
		printf("hello 2\n");
	}

	void drawProgressBar(int color, int startX, int endX, int pos, int len)
	{
		Extent e = maGetScrSize();
		maSetColor(color);
		maFillRect(startX,0, endX-startX, (EXTENT_Y(e)*pos)/len);
		maUpdateScreen();
	}

	void keyPressEvent(int keyCode) {
		// todo: handle key presses
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}

	void notifyProgress(Downloader *dl, int downloadedBytes, int totalBytes)
	{
		//printf("notifyProgress %i/%i\n", downloadedBytes, totalBytes);
		Extent e = maGetScrSize();
		if(dl==(Downloader*)imageDownloader)
			drawProgressBar(0xff0000, 0, EXTENT_X(e)>>1, downloadedBytes, totalBytes);
		if(dl==(Downloader*)audioDownloader) 
			drawProgressBar(0x00ff00, EXTENT_X(e)>>1, EXTENT_X(e), downloadedBytes, totalBytes);	
	}

	void finishedDownloading(Downloader *dl, Handle data)
	{
		if(dl == audioDownloader) {
			maSoundPlay(data, 0, maGetDataSize(data));
		}
		if(dl == imageDownloader)
		{
			MAPoint2d dstPoint = {0, 0};
			MARect srcRect = {0, 0, EXTENT_X(maGetImageSize(data)), EXTENT_Y(maGetImageSize(data))};
			maDrawImageRegion(data, &srcRect, &dstPoint, 0);
			maUpdateScreen();
		}
	}
	
	void downloadCancelled(Downloader* downloader) {
		printf("downloadCancelled\n");
	}
	
	void error(Downloader* downloader, int code) {
		printf("error %i\n", code);
	}
		

private:
	AudioDownloader* audioDownloader;
	ImageDownloader *imageDownloader;
	//Handle image;
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
