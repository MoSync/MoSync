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

class DownloaderTest : private MAUtil::DownloadListener, public MATest::TestCase {
public:
	DownloaderTest() : TestCase("Downloader") {}

	void start() {
		printf("Automated Downloader test\n");
		mDown.addDownloadListener(this);
		int res = mDown.beginDownloading("http://www.example.com/");
		printf("begin: %i\n", res);

		if(res <= 0) {
			assert("begin", false);
		}
	}

	void notifyProgress(MAUtil::Downloader* downloader, int downloadedBytes, int totalBytes) {
		printf("progress %d%%\n", downloadedBytes/totalBytes * 100);
	}

	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data) {
		printf("Finished\nMAHandle: 0x%x\n", data);
		assert("finished", data > 0);
		suite->runNextCase();
	}

	void downloadCancelled(MAUtil::Downloader* downloader) {
		printf("cancelled\n");
		suite->runNextCase();
	}

	void error(MAUtil::Downloader* downloader, int code) {
		printf("error %i\n", code);
		assert("error", false);
		suite->runNextCase();
	}
private:
	MAUtil::Downloader mDown;
};

void addDownloaderTests(MATest::TestSuite* suite);
void addDownloaderTests(MATest::TestSuite* suite) {
	suite->addTestCase(new DownloaderTest);
}
