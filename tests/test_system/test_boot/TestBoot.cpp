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
