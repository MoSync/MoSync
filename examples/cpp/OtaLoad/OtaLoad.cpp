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

/** \file main.cpp
* OtaLoad is a small program that downloads another MoSync program
* Over The Air (using http), caches it in permanent storage and executes it.\n
* \n
* If there is already a program in the cache, the user is given the option to
* run it immediately or ask the server for a new version.\n
*
* To use this example you must compile a program, with the same version of
* MoSync as you build OtaLoad, with  and put the comb file on a webserver.
* A comb file ( program.comb ) is a MoSync program in which both the program file and
* the resource file are combined into one file.  This file is located in your projects
* Output/<Build Configuration>/ or
* FinalOutput/<Build Configuration>/<Vendor>/<Device>/ folder.
* The project you will download can't contain any unloaded resources such
* as ubin or umedia.
*
* If you haven't got a webserver you can use free services like www.dropbox.com
* to host your file and give you a URL to it.
*/

#include <ma.h>
#include <conprint.h>
#include <mavsprintf.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>

using namespace MAUtil;

/**
	This is the url to your program.comb file, example "http://www.example.com/folder/program.comb"
*/
static const char* URL = "";

static const char* SAV = "OtaLoad.sav";
static const char* MODSAV = "OtaLoad.mod.sav";

class MyMoblet : public Moblet, HttpConnectionListener {
public:
	MyMoblet() : mState(eIdle), mHttp(this) {
		mProgram = maCreatePlaceholder();

		//read program from cache
		MAHandle store = maOpenStore(SAV, 0);
		if(store > 0) {
			int res = maReadStore(store, mProgram);
			maCloseStore(store, 0);
			if(res < 0) {
				printf("Cache read failed: %i\n", res);
			} else {
				printf("Program cached (%i bytes).\n", maGetDataSize(mProgram));
				printf("Tap screen or press fire to run.\n");
			}
			printf("Press 6/LSK to check for update.\n");
			readLastModified();
			mState = eReady;
		} else {
			printf("No program cached.\n");
			startDownload();
		}
	}
private:
	enum State {
		eIdle, eReady
	}	mState;
	MAHandle mProgram;
	HttpConnection mHttp;
	String mLastModified;

	void readLastModified() {
		//read Last-Modified header from cache
		MAHandle store = maOpenStore(MODSAV, 0);
		if(store > 0) {
			MAHandle data = maCreatePlaceholder();
			maReadStore(store, data);
			int size = maGetDataSize(data);
			if(size > 0) {
				mLastModified.resize(size);
				maReadData(data, mLastModified.pointer(), 0, size);
				maDestroyPlaceholder(data);
			}
		}
	}

	virtual void keyPressEvent(int keyCode, int nativeCode) {
		switch(keyCode) {
		case MAK_BACK:
		case MAK_SOFTRIGHT:
		case MAK_0:
			lprintfln("Naveed");
			maExit(0);
			break;
		case MAK_FIRE:
			if(mState == eReady) {
				maLoadProgram(mProgram, 1);
			}
			break;
		case MAK_SOFTLEFT:
		case MAK_6:
			if(mState == eReady) {
				mState = eIdle;
				startDownload();
			}
			break;
		}
	}

	virtual void pointerPressEvent(MAPoint2d p) {
		if(mState == eReady) {
			maLoadProgram(mProgram, 1);
		}
	}

	void startDownload() {

		if(0 == strcmp(URL, ""))
		{
			printf("No url provided, \nsee source code for \ninformation on how \nto use this example");
			return;
		}

		printf("Downloading from %s\n", URL);
		//in case of re-download
		mHttp.close();

		mHttp.create(URL, HTTP_GET);
		if(mLastModified.length() > 0)
			mHttp.setRequestHeader("If-Modified-Since", mLastModified.c_str());
		mHttp.finish();
	}

	virtual void httpFinished(HttpConnection*, int result) {
		if(result == 304) {	//Not Modified
			printf("Program unchanged.\n");
			printf("Tap screen or press Fire to run.\n");
			mState = eReady;
			return;
		}
		if(result >= 200 && result <= 299) {	//OK
			printf("HTTP %i\n", result);
			String cls;
			int res = mHttp.getResponseHeader("content-length", &cls);
			if(res < 0) {
				printf("CL error: %i\n", res);
				return;
			}
			int contentLength = stringToInteger(cls);
			printf("Content-Length: %i\n", contentLength);
			maDestroyObject(mProgram);
			if(maCreateData(mProgram, contentLength) == RES_OUT_OF_MEMORY) {
				printf("Out of memory!\n");
				return;
			}
			mHttp.readToData(mProgram, 0, contentLength);
			return;
		} else {	//error
			printf("HTTP %s %i\n", (result < 0) ? "error" : "response", result);
		}
	}

	virtual void connReadFinished(Connection*, int result) {
		if(result < 0) {
			printf("Read error %i\n", result);
			return;
		}

		//save Last-Modified header to cache
		mHttp.getResponseHeader("last-modified", &mLastModified);
		MAHandle data = maCreatePlaceholder();
		maCreateData(data, mLastModified.length());
		maWriteData(data, mLastModified.c_str(), 0, mLastModified.length());
		MAHandle store = maOpenStore(MODSAV, MAS_CREATE_IF_NECESSARY);
		if(store < 0) {
			printf("Meta-cache open error: %i\n", store);
		} else {
			int res = maWriteStore(store, data);
			if(res < 0) {
				printf("Meta-cache write error: %i\n", res);
			}
			maCloseStore(store, 0);
		}
		maDestroyPlaceholder(data);

		//save program to cache
		store = maOpenStore(SAV, MAS_CREATE_IF_NECESSARY);
		if(store < 0) {
			printf("Cache open error: %i\n", store);
		} else {
			int res = maWriteStore(store, mProgram);
			if(res < 0) {
				printf("Cache write error: %i\n", res);
			}
			maCloseStore(store, 0);
		}

		printf("Program downloaded.\n");
		printf("Tap screen or press Fire to run.\n");
		mState = eReady;
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
}
