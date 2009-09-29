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

/** \file TestController.cpp
* TEST CONTROL APP (TCA)
*
* The TCA is loaded from TEST BOOT, and it in turn invokes one or more test apps, by user choice.
*
* TCA downloads a list of test application URLs and saves it to storage. Then a list of all available
* tests is printed, and the user can choose to run a single test or all tests consecutively.
* During running the list of tests, the number of the current test running and running mode (single
* or consecutive) are all saved in storage.
* On quit, all storages are closed and deleted.
* Note: No actual results of executed tests are saved. Only Download/Receive errors in TCA are stored.
*
* TODO: Panics freezes emulator, but only at end of next test -> passed to Fredrik
* TODO: Catch errors
* TODO: Get real device name
* TODO: Get Panics
* TODO: Look-and-feel? Get print to top?
* Checked resource use, seems okay.
*
* \author Ann-Sofie Lindblom
*/

#include "MAHeaders.h"
#include <MAUtil/Moblet.h>
#include <MAUtil/Downloader.h>
#include "BuffDownloader.h"
#include <MAUtil/util.h>
#include <conprint.h>
#include <maapi.h>

using namespace MAUtil;

void Tokenize(const Handle data, const String delimiter, Vector<String> *test_names, Vector<String> *test_urls);
void PrintMenu(const Vector<String> *tests);

static const char* listURL = "http://www.mosync.com/wap/index.php?f=MSE_test_list.txt";
//static const char* listURL = "http://www.zzz.com/wap/index.php?f=MSE_test_list.txt";
static const String delimiter = " ";
static const char* tester_id = "2"; // TODO: change to real id, now using Aaron's.
static const char* device_name = "ABCD2"; //TODO: Get real device name.

class MyMoblet : public Moblet, public DownloadListener, public BuffDownloadListener {
public:
	MyMoblet() {
		// Create download listener
		downloader = new Downloader();
		downloader->addDownloadListener(this);

		// Get list of tests
		Handle test_list = maOpenStore("TestList.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(test_list, TEST_LIST);
		maCloseStore(test_list, false);
		int list_size = maGetDataSize(TEST_LIST);
		if(list_size <= 0) {
			// List resource empty, download list
			maDestroyObject(TEST_LIST);
			printf("Downloading...\n");
			downloader->beginDownloading(listURL, TEST_LIST);
		}
		else {
			// Tokenize
			Tokenize(TEST_LIST, delimiter, &test_names, &test_urls);
		}

		// Get (or set) run mode
		Handle run_mode = maOpenStore("RunMode.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(run_mode, RUN_MODE);
		if(maGetDataSize(RUN_MODE) <= 0) {
			// First time, no mode set
			maDestroyObject(RUN_MODE);
			maCreateData(RUN_MODE, sizeof(int));
			current_mode = 0;
			maWriteData(RUN_MODE, &current_mode, 0, sizeof(int));
			maWriteStore(run_mode, RUN_MODE);
		}
		else {
			maReadData(RUN_MODE, &current_mode, 0, sizeof(int));
		}
		maCloseStore(run_mode, false);

		// Get and update current test number
		Handle test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(test_number, TEST_NO);
		if(maGetDataSize(TEST_NO) <= 0) {
			// First test
			maDestroyObject(TEST_NO);
			maCreateData(TEST_NO, sizeof(int));
			current_no = 0;
		}
		else {
			maReadData(TEST_NO, &current_no, 0, sizeof(int));
			if(current_mode == 1) // In consecutive run mode
				++current_no;
		}
		maWriteData(TEST_NO, &current_no, 0, sizeof(int));
		maWriteStore(test_number, TEST_NO);
		maCloseStore(test_number, false);

		// Get test results and post to matrix
		if(test_names.size() > 0) {
			String remarks = getTestResults();
			if(remarks > 0) {
				printf("Bat country ahead...\n");
				// Post to test matrix
				postTestResults("NOT+WORKING");
			}
			else {
				printf("Yay, success!\n");
				// Test successful!
				postTestResults("WORKING");
			}
		}

		// Download next test or show menu
		if( current_no < test_urls.size() && current_mode == 1 ) {
			printf("Downloading...\n");
			downloader->beginDownloading(test_urls[current_no].pointer(), RES_STORE);
		}
		else if(test_names.size() > 0) {
			PrintMenu(&test_names);
		}
	}

	void keyPressEvent(int keyCode) {
		// Open run mode store
		Handle test_number;
		Handle run_mode = maOpenStore("RunMode.sav", MAS_CREATE_IF_NECESSARY);

		// Set run mode,   0 == run only chosen test
		current_mode = 0;
		maWriteData(RUN_MODE, &current_mode, 0, sizeof(int));
		maWriteStore(run_mode, RUN_MODE);
		maCloseStore(run_mode, false);

		switch(keyCode) {
			case MAK_1:
				// Set test number
				current_no = 0;
				maDestroyObject(TEST_NO);
				maCreateData(TEST_NO, sizeof(int));
				maWriteData(TEST_NO, &current_no, 0, sizeof(int));
				test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(test_number, TEST_NO);
				maCloseStore(test_number, false);

				printf("Downloading...\n");
				maDestroyObject(RES_STORE);
				downloader->beginDownloading(test_urls[current_no].pointer(), RES_STORE);
				break;
			case MAK_2:
				// Set test number
				current_no = 1;
				maWriteData(TEST_NO, &current_no, 0, sizeof(int));
				test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(test_number, TEST_NO);
				maCloseStore(test_number, false);

				printf("Downloading...\n");
				maDestroyObject(RES_STORE);
				downloader->beginDownloading(test_urls[current_no].pointer(), RES_STORE);
				break;
			case MAK_3:
				// Set test number
				current_no = 2;
				maWriteData(TEST_NO, &current_no, 0, sizeof(int));
				test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(test_number, TEST_NO);
				maCloseStore(test_number, false);

				printf("Downloading...\n");
				//printf("test_urls[current_no] = %s\n", test_urls[current_no].c_str());
				maDestroyObject(RES_STORE);
				downloader->beginDownloading(test_urls[current_no].pointer(), RES_STORE);
				break;
			case MAK_8:
				// Set run mode,  1 == run all tests consecutively
				current_mode = 1;
				maWriteData(RUN_MODE, &current_mode, 0, sizeof(int));
				run_mode = maOpenStore("RunMode.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(run_mode, RUN_MODE);
				maCloseStore(run_mode, false);

				// Set test number
				current_no = 0;
				maWriteData(TEST_NO, &current_no, 0, sizeof(int));
				test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(test_number, TEST_NO);
				maCloseStore(test_number, false);

				printf("Downloading...\n");
				maDestroyObject(RES_STORE);
				downloader->beginDownloading(test_urls[current_no].pointer(), RES_STORE);
				break;
			case MAK_0:
			case MAK_9:
				// Quit: Delete all stores.
				Handle test_controller = maOpenStore("TestController.sav", MAS_CREATE_IF_NECESSARY);
				Handle test_list = maOpenStore("TestList.sav", MAS_CREATE_IF_NECESSARY);
				test_number = maOpenStore("TestNumber.sav", MAS_CREATE_IF_NECESSARY);
				run_mode = maOpenStore("RunMode.sav", MAS_CREATE_IF_NECESSARY);
				Handle test_res = maOpenStore("TestResults.sav", MAS_CREATE_IF_NECESSARY);
				//delete downloader;

				maCloseStore(test_controller, true);
				maCloseStore(test_list, true);
				maCloseStore(test_number, true);
				maCloseStore(run_mode, true);
				maCloseStore(test_res, true);
				maExit(0);
			default:
				break;
		}
	}

	void keyReleaseEvent(int keyCode) {}

	void finishedDownloading(Downloader *dl, Handle data) // store download
	{
		if(dl == downloader) {
			if(test_names.size() == 0) {
				// Save test list in store
				Handle test_list = maOpenStore("TestList.sav", MAS_CREATE_IF_NECESSARY);
				maWriteStore(test_list, data);
				maCloseStore(test_list, false);

				// Read content in file and save as tokens
				Tokenize(data, delimiter, &test_names, &test_urls);
				PrintMenu(&test_names);
			}
			else {
				// Run test
				maLoadProgram(data, true);
			}
		}
	}

	void notifyProgress(Downloader *dl, int downloadedBytes, int totalBytes) { // TODO: Better progress bar? // store download
		Extent screenSize = maGetScrSize();
		int screenWidth = EXTENT_X(screenSize);
		maSetColor(0x0000ff);
		int bar = screenWidth*(downloadedBytes/totalBytes);
		maFillRect(0, 0, bar, 25);
		maUpdateScreen();
	}

	void downloadCancelled(Downloader *dl) {} // store download

	void error(Downloader *dl, int errno) { // store download
		if(test_names.size() == 0) {
			printf("Failed to download test list.\nNo error report sent to matrix.\n");
			printf("Press 9 to exit.\n");
		}
		else {
			addTestResult("Downloader", errno);
			postTestResults("NOT+WORKING");
			printf("Downloader error: %i\n", errno);
			PrintMenu(&test_names);
		}
	}

	String getTestResults() {
		Handle test_results = maOpenStore("TestResults.sav", MAS_CREATE_IF_NECESSARY);
		maDestroyObject(TEST_RES);
		maReadStore(test_results, TEST_RES);
		maCloseStore(test_results, false);
		int res_size = maGetDataSize(TEST_RES);

		if(res_size > 0) {
			char *remarks = new char[res_size];
			maReadData(TEST_RES, remarks, 0, res_size);
			remarks[res_size] = '\0'; // adding null termination to binary data
			return String(remarks);
		}
		return "";
	}

	void addTestResult(String errtype, int errno) {
		Handle test_results = maOpenStore("TestResults.sav", MAS_CREATE_IF_NECESSARY);
		maDestroyObject(TEST_RES);
		maReadStore(test_results, TEST_RES);
		String str_remarks;
		int res_size = maGetDataSize(TEST_RES);
		if(res_size > 0) {
			// Error report available
			char* remarks = new char[res_size];
			maReadData(TEST_RES, remarks, 0, res_size);
			remarks[res_size] = '\0'; // adding null termination to binary data
			str_remarks = String(remarks);
		}

		// Add error msg
		if(res_size > 0)
			str_remarks = str_remarks + "+" + errtype + "+error:+" + integerToString(errno);
		else
			str_remarks = errtype + "+error:+" + integerToString(errno);
		int str_size = str_remarks.size();
		const char *str = str_remarks.c_str();

		// Write to TestResults.sav
		maDestroyObject(TEST_RES);
		maCreateData(TEST_RES, str_size);
		maWriteData(TEST_RES, str, 0, str_size);
		maWriteStore(test_results, TEST_RES);
		maCloseStore(test_results, false);
	}

	void checkPanicReport() { // back
		Handle panic_report = maOpenStore("panic.report", MAS_CREATE_IF_NECESSARY);
		maReadStore(panic_report, PANIC_REP);
		maCloseStore(panic_report, true);

		int p_size = maGetDataSize(PANIC_REP);

		if(p_size > 0) {
			for(int i = 0; i < 10; i++) {
				printf("Panic report available! \n");
			}
/*			String delim = " ";
			//char *str = new char[p_size+1]; //must be char* for maReadData
			//maReadData(PANIC_REP, str, 0, p_size);
			//str[p_size] = '\0'; // adding null termination to binary data
			Tokenize(PANIC_REP, delim, &panic_info); // use stringsplit method instead
			if(panic_info.size() > 0) {
				printf("Panic info:\n");
				for(int i = 0; i < panic_info.size(); i++) {
					printf("%i: %s\n", i, panic_info[i].c_str());
				}
				//postPanicReport();
			} */
		} else {
//			for(int i = 0; i < 10; i++) {
//				printf("Panic report NOT available! \n");
//			}
		}
		maDestroyObject(PANIC_REP);
	}

	void postTestResults(String status) { // constx2, & ?
		// Create buffered downloaders
		bd_testid = new BuffDownloader();
		bd_testid->addBuffDownloadListener(this);
		bd_device = new BuffDownloader();
		bd_device->addBuffDownloadListener(this);
		bd_round = new BuffDownloader();
		bd_round->addBuffDownloadListener(this);
		bd_post = new BuffDownloader();
		bd_post->addBuffDownloadListener(this);

		param_count = 0;

		// TODO: Check if any panics were caused during previous test run
		checkPanicReport();
		String remarks = getTestResults();

		// Build post data
		post_data = "http://msdev.mine.nu:8282/deviceTest/api.php?f=submit_detail";
		post_data = post_data + "&status=" + status + "&remarks=" + remarks + "&tester_id=" + tester_id;

		// Get test id from matrix
		String url1 = "http://msdev.mine.nu:8282/deviceTest/api.php?f=search_test&test_name=";
		url1 = url1 + test_names[current_no];
		bd_testid->beginDownloading(url1.c_str());

		// Get device id from matrix
		String url2 = "http://msdev.mine.nu:8282/deviceTest/api.php?f=search_device&device_name=";
		url2 = url2 + device_name;
		bd_device->beginDownloading(url2.c_str());

		// Get latest round from matrix
		char *url3 = "http://msdev.mine.nu:8282/deviceTest/api.php?f=get_latest_round";
		bd_round->beginDownloading(url3);
	}

	void finishedDownloading(BuffDownloader *bd, char *data) { // buffer download
		++param_count;
		// Add post data
		if(bd == bd_testid) {
			post_data += "&test_id=";
			post_data += data;
		}else if(bd == bd_device) {
			post_data += "&device_id=";
			post_data += data;
		}else if(bd == bd_round) {
			post_data += "&round_id=";
			post_data += data;
		}else if(bd == bd_post) {
			printf("Report delivered to test matrix.\n");
			param_count = 0;
		}
		if(param_count == 3) {
			printf("post_data == %s\n", post_data.c_str());
			bd_post->beginDownloading(post_data.c_str());
		}
	}

	void downloadCancelled(BuffDownloader* bd) {} // buffer download

	void error(BuffDownloader *bd, int errno) { // buffer download
		addTestResult("BuffDownloader", errno);
		addTestResult("Error+getting+data+to/from+testmatrix", errno);
		postTestResults("NOT+WORKING");
		printf("BuffDownloader error: %i\n", errno);
		PrintMenu(&test_names);
	}

private:
	Downloader *downloader;
	Vector<String> test_names;
	Vector<String> test_urls;
	//Vector<String> panic_info;
	int current_no, current_mode;
	BuffDownloader *bd_testid;
	BuffDownloader *bd_device;
	BuffDownloader *bd_round;
	BuffDownloader *bd_post;
	String post_data;
	int param_count;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new MyMoblet());
	return 0;
};

void Tokenize(const Handle data, const String delimiter, Vector<String> *test_names, Vector<String> *test_urls) {
	// Get data to String
	int list_size = maGetDataSize(data);
	char *str = new char[list_size]; // must be char* for maReadData
	maReadData(data, str, 0, list_size);
	str[list_size] = '\0'; // adding null termination to binary data
	String mStr(str);
	delete str;

	// Tokenize
	Vector<String> tokens;
	stringSplit(mStr, delimiter, tokens);
	for(int i = 0; i < tokens.size(); i++) {
		if(i%2 == 0)	// even tokens are names of tests
			test_names->add(tokens[i].c_str());
		else			// uneven tokens are urls to tests
			test_urls->add(tokens[i].c_str());
	}
};

void PrintMenu(const Vector<String> *tests) {
	printf("Choose what test(s) to run:\n");
	for(int i = 0; i < tests->size(); i++) {
		printf("%i: %s\n", i+1, (*tests)[i].c_str() );
	}
	printf("%i: Run all\n", 8);
	printf("%i: Exit\n", 9);
};
