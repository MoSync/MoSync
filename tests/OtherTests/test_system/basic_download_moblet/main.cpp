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

/*
* TESTING DOWNLOAD
* TODO: Get print to top.
* TODO: Slow start?
*/

#include "MAHeaders.h"
#include <MAUtil/Moblet.h>
#include <MAUtil/Downloader.h>
#include <conprint.h>

using namespace MAUtil;

void Tokenize(const char* mStr, Vector<String> *tokens, const char delimiter);

static const char* testURL = "http://www.mosync.com/wap/index.php?f=MSE_test_list.txt";

class MyMoblet : public Moblet, public DownloadListener {
//TODO: add CloseListener
public:
	MyMoblet() {
		downloader = new Downloader();
		downloader->addDownloadListener(this);
		downloader->beginDownloading(testURL, RES_STORE);
		Extent e = maGetScrSize();
		maSetColor(0);
		maFillRect(0, 0, EXTENT_X(e), EXTENT_Y(e));
		printf("Helllooo!");
	}

	void keyPressEvent(int keyCode) {
		maDestroyObject(RES_STORE);
		switch(keyCode) {
			case MAK_0:
				maExit(0);
			case MAK_1:
				downloader->beginDownloading(tokens[0].pointer(), RES_STORE);
				break;
			case MAK_2:
				downloader->beginDownloading(tokens[1].pointer(), RES_STORE);
				break;
			case MAK_3:
				downloader->beginDownloading(tokens[2].pointer(), RES_STORE);
				break;
			case MAK_4:
				break;
			case MAK_5:
				break;
			case MAK_6:
				break;
			case MAK_7:
				break;
			case MAK_8:
				break;
			case MAK_9:
				break;
		}
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}

	void finishedDownloading(Downloader *dl, Handle data)
	{
		//Handle store = maOpenStore("test.sav", MAS_CREATE_IF_NECESSARY);  // Open storage TODO: move declaration
		//int res;
		if(dl == downloader) {
			printf("Finished downloading.\n");
			printf("Number of tokens: %d\n", tokens.size());

			if(tokens.size() == 0) {
				printf("Making list.\n");
				// Read content in file and save as tokens
				int list_size = maGetDataSize(data);
				char* str = new char[list_size]; //must be char* for maReadData
				maReadData(data, str, 0, list_size);
				Tokenize(str, &tokens, '\n');
			}
			else {
				//printf("Saving file.\n");
				//Save to file
/*				if(store <= 0) {  // Storage not available
					printf("Error saving: %i.1\n", store);
					Freeze(0);
				}
				else {  // Write to store
					res = maWriteStore(store, data);
					if(res <= 0) {
						printf("Error saving: %i.2\n", store);
						Freeze(0);
					} else {
						printf("File saved.\n");
					}
				}
				maCloseStore(store, 0); // TODO: check store functionality, do I even need to store?
				*/
				maLoadProgram(data, 0);
			}

			printf("Choose what test(s) to run:\n");
			for(int i = 0; i < tokens.size(); i++)
				printf("%d: %s\n", i+1, tokens[i].pointer());
			printf("%d: Run all\n", tokens.size()+1);
		}
	}
	
	void downloadCancelled(Downloader *dl) {}

private:
	Downloader *downloader;
	Vector<String> tokens;
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};

void Tokenize(const char* str, Vector<String> *tokens, const char delimiter) {
	//printf("Banana split!\n");
	String mStr(str);	
	// Skip delimiters at beginning.
	int startPos = mStr.findFirstNotOf(delimiter, 0);
	// Find first "non-delimiter".
	int endPos = mStr.findFirstOf(delimiter, startPos);

	while (String::npos != startPos && String::npos != endPos)
	{
		// Found a token, add it to the vector.
		(*tokens).add(mStr.substr(startPos, endPos - startPos));
		// Skip delimiters.  Note the "NotOf"
		startPos = mStr.findFirstNotOf(delimiter, endPos);
		// Find next "non-delimiter"
		endPos = mStr.findFirstOf(delimiter, startPos);
	}
};
