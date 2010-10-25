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
#include <MAUtil/Environment.h>
#include <common.h>

#ifdef MAPIP
#include <maprofile.h>
#else
#define MA_PROF_DEVICE_EMULATOR
#endif

class DownloaderTest : private MAUtil::DownloadListener, public MATest::TestCase, public MAUtil::TimerListener {
public:
	DownloaderTest(String url) : TestCase("Downloader"), mURL(url) {}

	void start() {
		printf("Automated Downloader test, url: %s\n", mURL.c_str());
		mDown.addDownloadListener(this);
		int res = mDown.beginDownloading(mURL.c_str());
		printf("begin: %i\n", res);

		if(res <= 0) {
			assert("begin", false);
		}

		// Add a timeout timer for connections, use lower value when running in the emulator or MoRE.

#ifdef MA_PROF_DEVICE_EMULATOR
		Environment::getEnvironment().addTimer(this, 4000, 1);
#else
		Environment::getEnvironment().addTimer(this, 20000, 1);
#endif
	}

	void notifyProgress(MAUtil::Downloader* downloader, int downloadedBytes, int totalBytes) {
		printf("progress %d%%\n", downloadedBytes/totalBytes * 100);
		Environment::getEnvironment().removeTimer(this);
	}

	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data) {
		printf("Finished\nMAHandle: 0x%x\n", data);
		assert("finished", data > 0);
		Environment::getEnvironment().removeTimer(this);
		suite->runNextCase();
	}

	void downloadCancelled(MAUtil::Downloader* downloader) {
		printf("cancelled\n");
		Environment::getEnvironment().removeTimer(this);
		suite->runNextCase();
	}

	void error(MAUtil::Downloader* downloader, int code) {
		printf("error %i\n", code);
		Environment::getEnvironment().removeTimer(this);
		assert("error", false);
		suite->runNextCase();
	}

	void runTimerEvent() {
		printf("Downloader timed out!!\n");
		assert("timeout", false);
		mDown.cancelDownloading();
	}

private:
	MAUtil::Downloader mDown;
	MAUtil::String mURL;
};

void addDownloaderTests(MATest::TestSuite* suite);
void addDownloaderTests(MATest::TestSuite* suite) {
	suite->addTestCase(new DownloaderTest("http://www.example.com/"));
	suite->addTestCase(new DownloaderTest("https://secure.wikimedia.org/"));
}
