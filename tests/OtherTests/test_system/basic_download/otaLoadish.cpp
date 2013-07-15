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

/** \file main.cpp
* TESTING
* OtaLoad is a small program that downloads another MoSync program
* Over The Air (using http), caches it in permanent storage and executes it.\n
* \n
* If there is already a program in the cache, the user is given the option to
* run it immediately or re-downloading (a possibly updated version).\n
* \n
* This could be improved (changing "re-download" to "check for update")
* by using the HTTP If-Modified-Since header field.
*/

#include <ma.h>
#include <conprint.h>
#include <maassert.h>
#include <MAUtil/HttpGet.h>
#include "MAHeaders.h"
#include "../common/helpers.h"

void myDrawText(int left, int top, const char* text) {
	maSetColor(0x00FF00);
	maDrawText(left, top, text);
}

int handleStore(Handle store);	//returns <= 0 on failure.
int drawTextCharWrap(int top, const char* text);	//Prints download address AND returns number of lines used

int gTextHeight, gScrHeight, gScrWidth;
static const char* gURL = "http://www.mobilesorcery.com/fred/otaload.v5.comb";

extern "C" int MAMain() {
	Extent scrSize = maGetScrSize();
	gScrHeight = EXTENT_Y(scrSize);
	gScrWidth = EXTENT_X(scrSize);
	gTextHeight = EXTENT_Y(maGetTextSize(" "));

	//Handle store = maOpenStore("OtaLoad.sav", 0);	// Open storage
	/*if(store > 0) {
		int res = handleStore(store);
		if(res <= 0)
			return res;
	} else {
		myDrawText(0,0, "No program cached");
		maUpdateScreen();
	}*/
	//no program cached or user decided to download
	maSetColor(0);
	maFillRect(0, gTextHeight, gScrWidth, gScrHeight);
	myDrawText(0, gTextHeight, "Downloading from");
	int line = 2 + 2; //drawTextCharWrap(gTextHeight*2, gURL);
	maUpdateScreen();

	int res = 0;
	
	maDestroyObject(RES_STORE);
	HttpGetToObject http(gURL, RES_STORE);
	while(res == 0) {
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

	if(res < 0) {
		char buffer[128];
		sprintf(buffer, "Error %i", res);
		myDrawText(0, gTextHeight*line, buffer);
		Freeze(0);
	}
	if(res != maGetDataSize(RES_STORE)) {
		char buffer[128];
		sprintf(buffer, "Error: %i != %i", res, maGetDataSize(RES_STORE));
		myDrawText(0, gTextHeight*line, buffer);
		Freeze(0);
	}
	/*
	char buffer[128];
	sprintf(buffer, "Got %i bytes.", res);
	myDrawText(0, gTextHeight*line++, buffer);
	myDrawText(0, gTextHeight*line++, "Press 5 or 'Fire' to run.");
	maUpdateScreen();

	if(store <= 0) {
		store = maOpenStore("OtaLoad.sav", MAS_CREATE_IF_NECESSARY);
	}
	if(store <= 0) {
		char buffer[128];
		sprintf(buffer, "Error saving: %i.1", store);
		myDrawText(0, gTextHeight*line, buffer);
	} else {
		res = maWriteStore(store, RES_STORE);
		if(res <= 0) {
			char buffer[128];
			sprintf(buffer, "Error saving: %i.2", store);
			myDrawText(0, gTextHeight*line, buffer);
		} else {
			myDrawText(0, gTextHeight*line++, "File saved.");
		}
	}
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
					maLoadProgram(RES_STORE);
					break;
			}
		}
	}
*/
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
					maLoadProgram(RES_STORE);
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
