/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
