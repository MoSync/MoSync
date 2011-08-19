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

/**
 * @file FileTest.cpp
 * @author Mikael Kindborg
 *
 * Application for testing parts of the File API.
 *
 * Uses a utility library developed for WebView (WebViewUtil.h/cpp).
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <conprint.h>
#include "WebViewUtil.h"

using namespace MoSync;

#define CHECK(success, testName) \
	if (!check(success, testName)) { mTestHasFailed = true; return; }

#define FAILCHECK() \
	if (mTestHasFailed) { return; }

class FileTestApp
{
private:
	Platform* mPlatform;
	bool mTestHasFailed;

public:
	FileTestApp() : mTestHasFailed(false)
	{
		printf("Testing file API.");
		printf("Exit with BACK or 0 key.");

		mPlatform = Platform::create();

		testFileAPI();

		if (mTestHasFailed)
		{
			printf("Test failed.");
		}
		else
		{
			printf("All tests passed.");
		}
	}

	virtual ~FileTestApp()
	{
	}

	bool check(bool success, MAUtil::String testName)
	{
		if (success)
		{
			printf("Test passed: \"%s\"", testName.c_str());
		}
		else
		{
			printf("Test failed: \"%s\"", testName.c_str());
			// Old way to end the test app on a failing test.
			//maPanic(0, "Test failed, inspect log.");
		}

		return success;
	}

	void testFileAPI()
	{
		MAUtil::String fileName1 = "testfile1";
		MAUtil::String fileName2 = "testfile2";
		MAUtil::String fileName3 = "testfile3";
		MAUtil::String fileName4 = "testfile4";
		MAUtil::String fullFileName4 = mPlatform->getLocalPath() + fileName4;
		MAUtil::String testData = "12345";

		// Write data.
		testFileWriteText(fileName1, testData);
		FAILCHECK()

		// Read data.
		testFileReadText(fileName1, testData);
		FAILCHECK()

		// Read to handle.
		MAHandle h = maCreatePlaceholder();
		testFileReadData(fileName1, h, testData.length());
		FAILCHECK()

		// Write to new file from handle.
		testFileWriteData(fileName2, h);
		maDestroyObject(h);
		FAILCHECK()

		// Verify by reading new file.
		testFileReadText(fileName2, testData);
		FAILCHECK()

		// Rename using short file name.
		testFileRename(fileName2, fileName3);
		FAILCHECK()

		// Verify by reading renamed file.
		testFileReadText(fileName3, testData);

		// Rename file using full path.
		testFileRename(fileName3, fullFileName4);
		FAILCHECK()

		// Verify by reading renamed file.
		testFileReadText(fileName4, testData);
		FAILCHECK()
	}

	void testFileWriteText(
		const MAUtil::String& fileName,
		const MAUtil::String& text)
	{
		// Write a string.
		bool success = mPlatform->writeTextToFile(
			mPlatform->getLocalPath() + fileName.c_str(),
			text.c_str());
		CHECK(success, "testFileWriteText")
	}

	void testFileReadText(
		const MAUtil::String& fileName,
		const MAUtil::String& text)
	{
		// Read the string.
		MAUtil::String data;
		bool success = mPlatform->readTextFromFile(
			mPlatform->getLocalPath() + fileName.c_str(),
			data);
		CHECK(success, "testFileReadText")
		CHECK(data.length() == text.length(), "data.length() == text.length()")
		CHECK(data.find(text, 0) == 0, "data.find(text, 0) == 0")
	}

	/**
	 * Read data into a data object referenced by a handle.
	 * @param fileName Short filename of file to read.
	 * @param placeholder Placeholder handle to read data to.
	 * @param dataSize Expected length of data in bytes.
	 */
	void testFileReadData(
		const MAUtil::String& fileName,
		MAHandle placeholder,
		int dataSize)
	{
		// Read to a handle.
		bool success = mPlatform->readDataFromFile(
			mPlatform->getLocalPath() + fileName.c_str(),
			placeholder);
		CHECK(success, "testFileReadData")
		CHECK(
			maGetDataSize(placeholder) == dataSize,
			"maGetDataSize(h) == dataSize")
	}

	void testFileWriteData(
		const MAUtil::String& fileName,
		MAHandle h)
	{
		// Write from handle to file.
		bool success = mPlatform->writeDataToFile(
			mPlatform->getLocalPath() + fileName.c_str(),
			h);
		CHECK(success, "testFileWriteData")
	}

	/**
	 * Test to rename a file.
	 * @param fileName Short file name of old file.
	 * @param newFileName File name of full path name of new file.
	 */
	void testFileRename(
		const MAUtil::String& fileName,
		const MAUtil::String& newFileName)
	{
		if (mTestHasFailed) { return; }

		// Rename file using full path.
		MAUtil::String fullFileName = mPlatform->getLocalPath() + fileName;
		MAHandle file = maFileOpen(fullFileName.c_str(), MA_ACCESS_READ_WRITE);
		CHECK(file > 0, "testFileRename: maFileOpen");
		CHECK(maFileExists(file), "testFileRename: maFileExists");
		int result = maFileRename(file, newFileName.c_str());
		CHECK(0 == result, "testFileRename: maFileRename");
		maFileClose(file);
	}

	void runEventLoop()
	{
		MAEvent event;

		bool isRunning = true;
		while (isRunning)
		{
			maWait(0);
			maGetEvent(&event);
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					isRunning = false;
					break;

				case EVENT_TYPE_KEY_PRESSED:
					if (MAK_BACK == event.key || MAK_0 == event.key)
					{
						isRunning = false;
					}
					break;
			}
		}
	}
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	FileTestApp app;
	app.runEventLoop();
	return 0;
}
