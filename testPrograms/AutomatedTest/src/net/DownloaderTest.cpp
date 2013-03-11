/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/
/*
 * DownloaderTest.cpp
 *
 * Created on: 11 nov 2010
 * Author: Mikael Kindborg
 */

#include <conprint.h>
#include <mastring.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Downloader.h>
//#include "Downloader.h"
#include <Testify/testify.hpp>
#include "../base/EventBase.h"
#include "connection.h"

using namespace Testify;
using namespace MAUtil;

// Urls used in test cases.
#define URL_DOCUMENT_THAT_HAS_CONTENT_LENGTH "http://www.mosync.com/"
#define URL_DOCUMENT_THAT_HAS_NO_CONTENT_LENGTH "http://divineprogrammer.blogspot.com/2010/10/mobile-lua.html"
#define URL_IMAGE "http://www.mosync.com/sites/all/themes/custom/mosync2/images/header.png"

// Global status of test cases. Since we test callbacks
// this is the most convenient way.
static bool gPassedDownloadUsingContentLengthTest;
static bool gPassedDownloadNotUsingContentLengthTest;
static bool gPassedDownloadImageTest;

/**
 * Class that holds the environment used by the tests.
 * We need this for downloaders to work since messages
 * are routed through the environment.
 */
class TestEnvironment : public Environment
{
public:
	TestEnvironment() : mIsRunning(true) { }
	bool isRunning() { return mIsRunning; }
	void startRunning() { mIsRunning = true; }
	void stopRunning() { mIsRunning = false; }
	void fireConnEvent(const MAConnEventData& data) { this->Environment::fireConnEvent(data); }
private:
	bool mIsRunning;
};

// Single global instance of the Environment class.
TestEnvironment gTestEnvironment;

/**
 * Helper function that checks if a string is found within a text resource.
 */
static bool dataContainsString(MAHandle data, const char* stringToLookFor)
{
	int size = maGetDataSize(data);
	char* buf = new char[size + 1];
	maReadData(data, buf, 0, size);
	buf[size] = '\0'; // Zero terminate string
	maWriteLog(buf, size);
	char* result = strstr(buf, stringToLookFor);
	delete buf;
	return (bool)result;
}

/**
 * Base class for download listeners.
 */
class DownloadListenerBase : public DownloadListener
{
public:
	void notifyProgress(Downloader* downloader, int downloadedBytes, int totalBytes)
	{
	}

	bool outOfMemory(Downloader* downloader)
	{
#ifdef VERBOSE
		printf("outOfMemory");
#endif
		// Cannot call assert in a function that has a non-void return type.
		//TESTIFY_ASSERT(0);
		return false;
	}

	void finishedDownloading(Downloader* downloader, MAHandle data)
	{
		TESTIFY_ASSERT(0); // Must implement in subclasses.
	}

	void downloadCancelled(Downloader* downloader)
	{
#ifdef VERBOSE
		printf("downloadCancelled");
#endif
		TESTIFY_ASSERT(0);
	}

	void error(Downloader* downloader, int code)
	{
#ifdef VERBOSE
		printf("error, code: %i", code);
#endif
		TESTIFY_ASSERT(0);
	}
};

/**
 * Listener for test DownloadUsingContentLengthTest.
 */
class DownloadUsingContentLengthTestListener : public DownloadListenerBase
{
public:
	void finishedDownloading(Downloader* downloader, MAHandle data)
	{
#ifdef VERBOSE
		printf("finishedDownloading");
#endif
		gPassedDownloadUsingContentLengthTest =
			dataContainsString(data, "MoSync");
		maDestroyObject(data);
		gTestEnvironment.stopRunning();
	}
};

/**
 * Listener for test DownloadNotUsingContentLengthTest.
 */
class DownloadNotUsingContentLengthTestListener : public DownloadListenerBase
{
public:
	void finishedDownloading(Downloader* downloader, MAHandle data)
	{
#ifdef VERBOSE
		printf("finishedDownloading");
#endif
		gPassedDownloadNotUsingContentLengthTest =
			dataContainsString(data, "MOBILELUA_BEGIN");
		maDestroyObject(data);
		gTestEnvironment.stopRunning();
	}
};

/**
 * Listener for the image download tests.
 */
class DownloadImageTestListener : public DownloadListenerBase
{
public:
	void finishedDownloading(Downloader* downloader, MAHandle data)
	{
#ifdef VERBOSE
		printf("finishedDownloading");
#endif
		TESTIFY_ASSERT(maGetDataSize(data) > 5000);
		maDestroyObject(data);
		gPassedDownloadImageTest = true;
		gTestEnvironment.stopRunning();
	}
};

/**
 * Listener that starts a second download.
 */
class DownloadFromListenerTestListener : public DownloadListenerBase
{
public:
	void finishedDownloading(Downloader* downloader, MAHandle data)
	{
#ifdef VERBOSE
		printf("finishedDownloading");
#endif

		// First document downloaded should contain this string.
		if (dataContainsString(data, "MOBILELUA_BEGIN"))
		{
			downloader->beginDownloading(URL_DOCUMENT_THAT_HAS_CONTENT_LENGTH);
		}
		else
		{
			// Second document should contain this string.
			gPassedDownloadUsingContentLengthTest =
				dataContainsString(data, "MoSync");
			gTestEnvironment.stopRunning();
		}

		maDestroyObject(data);
	}
};

/**
 * DownloadController class. Used here to test redirect via handleUrlRedirection method.
 */
class DownloadControllerTest : public DownloadController
{
	virtual bool handleRedirect(const char* newLocation)
	{
		printf("\nRedirected to: %s\n", newLocation);
		return true;
	}
};

/**
 * Automated Downloader test case.
 */
class DownloaderTestCase : public TestCase, public EventBase
{
public:
	DownloaderTestCase()
	: TestCase("Downloader test case")
	{
		addTest(bind(&DownloaderTestCase::downloadUsingContentLengthTest, this),
			"Download using content-length header");
		addTest(bind(&DownloaderTestCase::downloadNotUsingContentLengthTest, this),
			"Download NOT using content-length header");
		addTest(bind(&DownloaderTestCase::downloadMultipleTimesTest, this),
			"Download multiple documents using two downloaders");
		addTest(bind(&DownloaderTestCase::downloadFromListenerTest, this),
			"Download a second document from the listener");
		addTest(bind(&DownloaderTestCase::downloadImageUsingSystemHandleTest, this),
			"Download an image to a system supplied handle");
		addTest(bind(&DownloaderTestCase::downloadImageUsingUserHandleTest, this),
			"Download an image to a user supplied handle");
	}

	/**
	 * Function that gets connection events as long as the
	 * environment's running flag is true.
	 */
	void runEventLoop()
	{
		MAEvent event;
		gTestEnvironment.startRunning();
		while (gTestEnvironment.isRunning())
		{
			TESTIFY_ASSERT(waitForEvent(MAX_DELAY, EVENT_TYPE_CONN, event));
			gTestEnvironment.fireConnEvent(event.conn);
		}
	}

	virtual void testSetUp()
	{
	}

	virtual void testTearDown()
	{
		// Wait for half a second. Why? Pause between tests?
		long startTime = maGetMilliSecondCount( );
		while ( maGetMilliSecondCount()-startTime < 500 );
	}

	/**
	 * Test downloading an html page that has a content-length header in the response.
	 */
	void downloadUsingContentLengthTest()
	{
		gPassedDownloadUsingContentLengthTest = false;
		Downloader downloader;
		DownloadUsingContentLengthTestListener downloadListener;
		downloader.addDownloadListener(&downloadListener);
		int result = downloader.beginDownloading(URL_DOCUMENT_THAT_HAS_CONTENT_LENGTH);
		TESTIFY_ASSERT(0 < result);
		runEventLoop();
		TESTIFY_ASSERT(gPassedDownloadUsingContentLengthTest);
	}

	/**
	 * Test downloading an html page that has NO content-length header in the response.
	 */
	void downloadNotUsingContentLengthTest()
	{
		gPassedDownloadNotUsingContentLengthTest = false;
		Downloader downloader;
		DownloadNotUsingContentLengthTestListener downloadListener;
		DownloadControllerTest downloadController;

		downloader.setDownloadController(&downloadController);
		downloader.addDownloadListener(&downloadListener);

		int result = downloader.beginDownloading(URL_DOCUMENT_THAT_HAS_NO_CONTENT_LENGTH);
		TESTIFY_ASSERT(0 < result);
		runEventLoop();
		TESTIFY_ASSERT(gPassedDownloadNotUsingContentLengthTest);
	}

	/**
	 * Test reusing the same downloader instance for multiple downloads.
	 */
	void downloadMultipleTimesTest()
	{
		int result;
		Downloader downloader;
		DownloadUsingContentLengthTestListener downloadListener1;
		DownloadNotUsingContentLengthTestListener downloadListener2;

		for (int i = 0; i < 3; ++i)
		{
			gPassedDownloadUsingContentLengthTest = false;
			gPassedDownloadNotUsingContentLengthTest = false;

			// Download document with first downloader.
			downloader.addDownloadListener(&downloadListener1);
			result = downloader.beginDownloading(URL_DOCUMENT_THAT_HAS_CONTENT_LENGTH);
			TESTIFY_ASSERT(0 < result);
			runEventLoop();
			TESTIFY_ASSERT(gPassedDownloadUsingContentLengthTest);
			downloader.removeDownloadListener(&downloadListener1);

			// Download document with second downloader.
			downloader.addDownloadListener(&downloadListener2);
			result = downloader.beginDownloading(URL_DOCUMENT_THAT_HAS_NO_CONTENT_LENGTH);
			TESTIFY_ASSERT(0 < result);
			runEventLoop();
			TESTIFY_ASSERT(gPassedDownloadNotUsingContentLengthTest);
			downloader.removeDownloadListener(&downloadListener2);
		}
	}

	/**
	 * Test doing a second download initiated from the listener.
	 */
	void downloadFromListenerTest()
	{
		// This is the flag we use to determine if the download has passed.
		gPassedDownloadUsingContentLengthTest = false;
		int result;
		Downloader downloader;
		DownloadFromListenerTestListener downloadListener;
		downloader.addDownloadListener(&downloadListener);
		result = downloader.beginDownloading(URL_DOCUMENT_THAT_HAS_NO_CONTENT_LENGTH);
		TESTIFY_ASSERT(0 < result);
		runEventLoop();
		TESTIFY_ASSERT(gPassedDownloadUsingContentLengthTest);
	}

	/**
	 * Test downloading an image using system supplied handle.
	 */
	void downloadImageUsingSystemHandleTest()
	{
		// This is the flag we use to determine if the download has passed.
		gPassedDownloadImageTest = false;
		int result;
		Downloader downloader;
		DownloadImageTestListener downloadListener;
		downloader.addDownloadListener(&downloadListener);
		result = downloader.beginDownloading(URL_IMAGE);
		TESTIFY_ASSERT(0 < result);
		runEventLoop();
		TESTIFY_ASSERT(gPassedDownloadImageTest);
	}

	/**
	 * Test downloading an image using a user supplied handle.
	 */
	void downloadImageUsingUserHandleTest()
	{
		// This is the flag we use to determine if the download has passed.
		gPassedDownloadImageTest = false;
		int result;
		Downloader downloader;
		DownloadImageTestListener downloadListener;
		downloader.addDownloadListener(&downloadListener);
		result = downloader.beginDownloading(URL_IMAGE, maCreatePlaceholder());
		TESTIFY_ASSERT(0 < result);
		runEventLoop();
		TESTIFY_ASSERT(gPassedDownloadImageTest);
	}
};

static TestHook hook( new DownloaderTestCase( ), "net" );

