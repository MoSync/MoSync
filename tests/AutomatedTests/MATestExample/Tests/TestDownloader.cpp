#include <ma.h>
#include <MAUtil/Downloader.h>
#include <MATest/TestRunner.h>

/**
 * Example of async test.
 */
class TestDownloader :
	public MATest::TestCase,
	public MAUtil::DownloadListener
{
public:
	TestDownloader(const MAUtil::String& name) :
		MATest::TestCase(name)
	{
	}

	void start()
	{
		setTimeOut(5000);

		expect("DataDownloaded");

		mDownloader.addDownloadListener(this);
		mDownloader.beginDownloading("http://www.mosync.com");
	}

	void finishedDownloading(MAUtil::Downloader* downloader, MAHandle data)
	{
		assert("DataDownloaded", true);
		runNextTestCase();
	}

	void downloadCancelled(MAUtil::Downloader* downloader)
	{
		assert("DataDownloaded", false);
		runNextTestCase();
	}

	void error(MAUtil::Downloader* downloader, int code)
	{
		assert("DataDownloaded", false);
		runNextTestCase();
	}

	MAUtil::Downloader mDownloader;
};

TESTCASE(TestDownloader)

