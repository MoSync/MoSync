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

#include <conprint.h>
#include <MAUtil/Downloader.h>
#include <MATest/Test.h>
#include <common.h>

class DownloaderTest : private MAUtil::DownloadListener, public KeyBaseCase {
public:
	DownloaderTest() : KeyBaseCase("Downloader") {}

	bool mIsActive;

	void open() {
		KeyBaseCase::open();
	}
	void close() {
		KeyBaseCase::close();
	}

	//KeyListener
	void keyPressEvent(int keyCode) {
	}
	void keyReleaseEvent(int keyCode) {
		if(!mIsActive)
			checkYesNo(keyCode);
		else
			printf("please wait.. downloading!\n");
	}

	void showEndText()
	{
		printf("\nAutomated tests finished!\nPress right softkey.\n");
	}

	void start() {
		mIsActive = false;
		printf("Automated Downloader test\n");
		mDown.addDownloadListener(this);
		int res = mDown.beginDownloading("http://www.example.com/");
		printf("begin: %i\n", res);

		if(res <= 0) {
			assert("begin", false);
			showEndText();
		}
		else
			mIsActive = true;
	}

	void notifyProgress(MAUtil::Downloader* downloader, int downloadedBytes, int totalBytes) {
		printf("progress %d\%\n", downloadedBytes/totalBytes * 100);
	}
	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data) {
		mIsActive = false;
		printf("Finished\nMAHandle: 0x%x\n", data);
		assert("finished", data > 0);
		showEndText();
	}
	void downloadCancelled(MAUtil::Downloader* downloader) {
		printf("cancelled\n");
	}
	void error(MAUtil::Downloader* downloader, int code) {
		mIsActive = false;
		printf("error %i\n", code);
		assert("error", false);
		showEndText();
	}
private:
	MAUtil::Downloader mDown;
};

void addDownloaderTests(MATest::TestSuite* suite);
void addDownloaderTests(MATest::TestSuite* suite) {
	suite->addTestCase(new DownloaderTest);
}
