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

/**
 * @file FileTest.cpp
 * @author Mikael Kindborg
 *
 * Application for testing parts of the File API.
 *
 * How to use:
 *
 * Build and run the app.
 *
 * Inspect the output shown on the screen. If all test
 * passes "All tests passed." is displayed, if a test fails,
 * "Test failed." is displayed.
 *
 * On Android the SD card is used for file tests, on other
 * platforms the local app directory is used.
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <conprint.h>
#include "FileUtil.h"

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

	/**
	 * Get the path to be used for file tests.
	 * On Android SD card is used, on other platforms
	 * the local app directory.
	 * @return A path.
	 */
	MAUtil::String getPath()
	{
		int SIZE = 256;
		char os[SIZE];
		maGetSystemProperty("mosync.device.OS", os, SIZE);
		if (0 == strncmp(os , "Android", 7))
		{
			printf("Using SD card\n");
			return mPlatform->getAndroidSDCardPath();
		}
		else
		{
			printf("Using local app folder\n");
			return mPlatform->getLocalPath();
		}
	}

	void testFileAPI()
	{
		MAUtil::String fileName1 = "testfile1";
		MAUtil::String fileName2 = "testfile2";
		MAUtil::String fileName3 = "testfile3";
		MAUtil::String fileName4 = "testfile4";
		MAUtil::String fullFileName4 = getPath() + fileName4;
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
			getPath() + fileName.c_str(),
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
			getPath() + fileName.c_str(),
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
			getPath() + fileName.c_str(),
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
			getPath() + fileName.c_str(),
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
		MAUtil::String fullFileName = getPath() + fileName;
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
