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
	DownloaderTest(const String& name, const String& url) : TestCase(name), mURL(url) {}

	void start() {
		printf("Automated Downloader test, url: %s\n", mURL.c_str());
		mDown.addDownloadListener(this);
		int res = mDown.beginDownloading(mURL.c_str());
		printf("begin: %i\n", res);

		if(res <= 0) {
			assert("begin", false);
			suite->runNextCase();
		} else {
			// Add a timeout timer for connections, use lower value when running in the emulator or MoRE.
#ifdef MA_PROF_DEVICE_EMULATOR
			Environment::getEnvironment().addTimer(this, 4000, 1);
#else
			Environment::getEnvironment().addTimer(this, 20000, 1);
#endif
		}
	}

	void notifyProgress(MAUtil::Downloader* downloader, int downloadedBytes, int totalBytes) {
		if(totalBytes) {
			printf("progress %d%%\n", (downloadedBytes * 100)/totalBytes);
		} else {
			printf("progress %i bytes\n", downloadedBytes);
		}
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
	suite->addTestCase(new DownloaderTest("http downloader", "http://www.example.com/"));
	suite->addTestCase(new DownloaderTest("https downloader", "https://secure.wikimedia.org/"));
}
