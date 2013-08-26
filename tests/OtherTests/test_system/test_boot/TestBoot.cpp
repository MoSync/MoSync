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

/** \file TestBoot.cpp
* TEST BOOT APP
*
* TEST BOOT is a moblet that downloads and runs a test control application (TCA), 
* the TCA in turn runs one or many test apps.
* If a TCA is already available in storage, download is skipped. This is due to the 
* need to restart TEST BOOT between test app runs. On a proper exit from TCA 
* the stored TCA is deleted.
*
* \author Ann-Sofie Lindblom
*/
#include "MAHeaders.h"
#include <MAUtil/Moblet.h>
#include <MAUtil/Downloader.h>
#include <conprint.h>

using namespace MAUtil;

static const char* controllerURL = "http://www.mosync.com/wap/index.php?f=MSE_test_controller.comb";
	
class BootMoblet : public Moblet, public DownloadListener {
public:
	BootMoblet() {
		// Open and read storage
		Handle test_controller = maOpenStore("TestController.sav", MAS_CREATE_IF_NECESSARY);
		maReadStore(test_controller, RES_STORE);
		maCloseStore(test_controller, false);
		printf("dataSize == %i\n", maGetDataSize(RES_STORE));
		if(maGetDataSize(RES_STORE) <= 0) {
			maDestroyObject(RES_STORE);
			
			// Download controller app
			downloader = new Downloader();
			downloader->addDownloadListener(this);
			printf("Downloading...\n");
			downloader->beginDownloading(controllerURL, RES_STORE);
		}
		else {
			// Load controller app
			maLoadProgram(RES_STORE, 0);
		}
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

	void keyReleaseEvent(int keyCode) {}

	void finishedDownloading(Downloader *dl, Handle data)
	{
		// Open storage and save controller
		Handle test_controller = maOpenStore("TestController.sav", MAS_CREATE_IF_NECESSARY);
		int res;
		if(dl == downloader) {
			printf("Saving file.\n");
			//Save to file
			if(test_controller <= 0) {  // Storage not available
				printf("Error saving: %i.1\n", test_controller);
				Freeze(0);
			}
			else {  					// Write to store
				res = maWriteStore(test_controller, data);
				if(res <= 0) {
					printf("Error saving: %i.2\n", test_controller);
					Freeze(0);
				} else {
					printf("File saved.\n");
				}
			}
			maCloseStore(test_controller, false);

			// Run controller from resource
			maLoadProgram(data, false);
		}
	}

	void notifyProgress(Downloader *dl, int downloadedBytes, int totalBytes) { // TODO: Better progress bar?
		Extent screenSize = maGetScrSize();
		int screenWidth = EXTENT_X(screenSize);
		maSetColor(0x0000ff);
		int bar = screenWidth*(downloadedBytes/totalBytes);
		maFillRect(0, 0, bar, 25);
		maUpdateScreen();
	}
	
	void downloadCancelled(Downloader *dl) {}
	
	void error(Downloader *dl, int errno) {
		printf("Downloader error: %i\n", errno);
	}

private:
	Downloader *downloader;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new BootMoblet());
	return 0;
};
