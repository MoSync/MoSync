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
*
* Downloads file from gURL with HttpGetToObject, and saves it to a permanent storage on the device.
* On user invocation (press 5 or 'fire') file is loaded and executed.
* On quitting the downloaded (child) program, the execution of the main (parent) program is also terminated. 
*
* //Check for cached program ?
* //Where to put list and menu ?
*/

#include <ma.h>
#include "MAHeaders.h"
#include "../common/helpers.h"
#include <MAUtil/HttpGet.h>
#include <conprint.h>
#include <maassert.h>

void myDrawText(int left, int top, const char* text) {
	maSetColor(0);
	maDrawText(left+5, top, text);
}

//int handleStore(Handle store);	//returns <= 0 on failure.
//int drawTextCharWrap(int top, const char* text);	//Prints download address AND returns number of lines used

int screenHeight, screenWidth, textHeight;
static const char* gURL = "http://www.mosync.com/wap/index.php?f=MSE_sketchy.comb";

extern "C" int MAMain() {
	Extent screenSize = maGetScrSize();
	screenHeight = EXTENT_Y(screenSize);
	screenWidth = EXTENT_X(screenSize);
	textHeight = EXTENT_Y(maGetTextSize(" "));
	int res = 0;
	int line = 0;
	
	//create permanent storage
	Handle store = maOpenStore("testLoad.sav", MAS_CREATE_IF_NECESSARY);  // Open storage
	maSetColor(0xffffff);
	maFillRect(0, 0, screenWidth, screenHeight);
	myDrawText(0, textHeight*line++, "Open storage");
	maUpdateScreen();
	/*
	if(store > 0) { // if content in storage
		int res = handleStore(store);
		if(res <= 0)
			return res;
	} else {
		myDrawText(0,0, "No program cached");
		maUpdateScreen();
	}
	*/
	
	// Download
	myDrawText(0, textHeight*line++, "Connecting...");
	maUpdateScreen();
	maDestroyObject(RES_STORE);
	HttpGetToObject http(gURL, RES_STORE);
	while(res == 0) { // While ongoing transfer
		myDrawText(0, textHeight*line++, ".");
		maUpdateScreen();
		EVENT event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				http.notify(event.conn);
				res = http.state();
			}
		}
	}

	if(res < 0) { // HttpGet failed
		char buffer[128];
		sprintf(buffer, "Error %i", res);
		myDrawText(0, textHeight*line++, buffer);
		Freeze(0);
	}
	if(res != maGetDataSize(RES_STORE)) { // Wrong size
		char buffer[128];
		sprintf(buffer, "Error: %i != %i", res, maGetDataSize(RES_STORE));
		myDrawText(0, textHeight*line++, buffer);
		Freeze(0);
	}
	// Download succeeded
	char buffer[128];
	sprintf(buffer, "Got %i bytes.", res);
	myDrawText(0, textHeight*line++, buffer);
	maUpdateScreen();

	// Save to file
	/*
	if(store <= 0) {
		store = maOpenStore("testLoad.sav", MAS_CREATE_IF_NECESSARY);
	} */
	if(store <= 0) {  // Storage not available
		char buffer[128];
		sprintf(buffer, "Error saving: %i.1", store);
		myDrawText(0, textHeight*line, buffer);
		Freeze(0);
	}
	else {  // Write to store
		res = maWriteStore(store, RES_STORE);
		if(res <= 0) {
			char buffer[128];
			sprintf(buffer, "Error saving: %i.2", store);
			myDrawText(0, textHeight*line, buffer);
			Freeze(0);
		} else {
			myDrawText(0, textHeight*line++, "File saved.");
		}
	}
	myDrawText(0, textHeight*line++, "Press 5 or 'Fire' to run.");
	maUpdateScreen();

	// Get user input
	while(true) {
		EVENT event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
			else if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
				case MAK_0:
					maExit(0);
				case MAK_1:
					break;
				case MAK_2:
					break;
				case MAK_3:
					break;
				case MAK_4:
					break;
				case MAK_FIRE:
				case MAK_5:
					maCloseStore(store, 0);
					maLoadProgram(RES_STORE, 0);
					break;
			}
		}
	}
	
	return 0;
}

/*	
int handleStore(Handle store) {
	maReadStore(store, RES_STORE);
	char buffer[128];
	sprintf(buffer, "Program cached (%i bytes)", maGetDataSize(RES_STORE));
	myDrawText(0,0, buffer);
	myDrawText(0, gTextHeight, "Press 5 or 'Fire' to run");
	myDrawText(0, gTextHeight*2, "Press 6 to re-download");
	maUpdateScreen();
	while(true) {
		EVENT event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
			else if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
				case MAK_0:
					maExit(0);
				case MAK_FIRE:
				case MAK_5:
					maCloseStore(store, 0);
					maLoadProgram(RES_STORE, 0);
					break;
				case MAK_6:
					return 1;
			}
		}
	}
}

int drawTextCharWrap(int top, const char* text) {
	if(*text == 0)
		return 0;
	if(EXTENT_X(maGetTextSize(text)) <= gScrWidth) {
		myDrawText(0, top, text);
		return 1;
	}
	int nLines = 0, position = 0;
	int totalLen = strlen(text);
	while(position < totalLen) {
		char buffer[256];
		strcpy(buffer, text + position);
		int len = strlen(buffer);
		int yPos = top + gTextHeight * nLines;
		while(EXTENT_X(maGetTextSize(buffer)) > gScrWidth) {
			len--;
			buffer[len] = 0;
		} 
		myDrawText(0, yPos, buffer);
		position += len;
		nLines++;
	} //while(position < totalLen);
	return nLines;
}
*/
