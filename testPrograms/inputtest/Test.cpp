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

#include <MAAPI.h>
#include <MAUtil/String.h>
#include <MAUtil/CharInput.h>
#include <MAUI/Font.h>
#include "MAHeaders.h"
#include <mavsprintf.h>

using namespace MAUI;

struct InputString {
	BOOL isSelecting;
	int timeDeployment;
	int position;
	int length;
	char chars[256];
};

#define MAX_CHARS 512
#define MAX_LINES 32
struct Console {
	char buffer[MAX_CHARS];
	char *line[MAX_LINES];
	int currentLine;
	int currentChar;

	int screenTopLine;
	int screenBottomLine;
	int lineSpacing;
	int fontHeight;
	
	Font *font;
};

Console console;

void Console_Init() {
	
	memset(console.buffer, 0, MAX_CHARS);
	memset(console.line, 0, MAX_LINES*sizeof(char*));
	console.line[0] = console.buffer;
	console.currentLine = 0;
	console.screenTopLine = 0;
	console.currentChar = 0;

	console.font = new MAUI::Font(FONT, FONT_METRICS, 0xc0c0c0);
	console.lineSpacing = 2;
	Extent fontDims = console.font->getStringDimensions("A");
	console.fontHeight = EXTENT_Y(fontDims);
	console.screenBottomLine = EXTENT_Y(maGetScrSize())/(console.fontHeight+console.lineSpacing);
	
};

void Console_Draw() {
	int y = console.lineSpacing;
	for(int i = console.screenTopLine; i != console.screenBottomLine; i=(i+1)%MAX_LINES) {
		if(console.line[i]) 
			console.font->drawString(console.line[i], 0, y);
		y+=console.fontHeight+console.lineSpacing;
	}
}

void updateConsolePointers() {
			console.currentLine++;

			if(console.currentLine%(console.screenBottomLine+2)==console.screenBottomLine+1) {
				console.screenTopLine++;
				console.screenTopLine%=MAX_LINES;
				console.screenBottomLine++;
				console.screenBottomLine%=MAX_LINES;
			}

			if(console.currentLine>=MAX_LINES){
				console.currentLine = 0;
			}
}

char tempBuffer[1024];
void prntf(const char* str, ...) {
	va_list args;
	va_start ( args, str );
	int numChars = vsprintf(tempBuffer, str, args);
	va_end ( args );

	char *src = tempBuffer;
	char *c = &console.buffer[console.currentChar];
	while(numChars--) {
		*c = *src;
		if(*c == '\n') {
			*c = 0;
			updateConsolePointers();
			console.line[console.currentLine] = c+1;
		}
		src++;
		c++;
		console.currentChar++;

		if(console.currentChar == MAX_CHARS-1) {
			*c = 0;
			c = &console.buffer[0];
		
			char *s = console.line[console.currentLine];
			while(*s) {
				*c++=*s++;
			}
			console.line[console.currentLine] = &console.buffer[0];
			console.currentChar = 0;
		}
	}
}


void myCurrentCharCallback(char c, void *userData) {
	InputString *str = (InputString*)userData;
	str->isSelecting = TRUE;
	str->chars[str->position] = c;
}

void myFinalCharCallback(char c, void *userData) {
	InputString *str = (InputString*)userData;
	
	if(c=='\b') {
		str->position--;
		str->length--;
		str->isSelecting = FALSE;
		if(str->position<0) str->position = 0;
		if(str->length<0) str->length = 0;
		return;
	}
	
	str->timeDeployment = maGetMilliSecondCount();
	str->chars[str->position++] = c;
	str->length++;
	str->isSelecting = FALSE;
}

#define CARET_FLICKER_SPEED 500
void updateString(InputString &inputString) {
		int pos;
		if(inputString.isSelecting == FALSE) pos = inputString.position-1;
		else pos = inputString.position;

		int time = maGetMilliSecondCount()-inputString.timeDeployment;
		if(inputString.isSelecting == FALSE  && time%CARET_FLICKER_SPEED<=CARET_FLICKER_SPEED>>1)	
			inputString.chars[pos+1] = '_';
		else 
			inputString.chars[pos+1] = ' ';	
		
		inputString.chars[pos+2] = 0;	
}

extern "C" int MAMain() {
	Font *font = new MAUI::Font(FONT, FONT_METRICS, 0xc0c0c0);

	// clear input string
	InputString inputString;
	inputString.position = 0;
	inputString.length = 0;
	memset(inputString.chars, 0, 256);

	// register char callbacks
	CharInput_RegisterCurrentCharCallback(myCurrentCharCallback, &inputString);
	CharInput_RegisterFinalCharCallback(myFinalCharCallback, &inputString);
	
	Console_Init();
	
	while(1) {
		EVENT event;
		while(maGetEvent(&event)) {
			switch(event.type) {
				case EVENT_TYPE_KEY_PRESSED:
					// update char handler
					CharInput_Pressed(event.key);
					prntf("pressed\n");
					break;
				case EVENT_TYPE_KEY_RELEASED:
					// update char handler
					CharInput_Released(event.key);
					prntf("released\n");
					break;				
				case EVENT_TYPE_CLOSE:
					maExit(0);
					break;
			}
		}

		// update char handler
		/*
		CharInput_Update();
		*/

		updateString(inputString);

		// draw string
		//Extent screenSize = maGetScrSize();
		maSetColor(0x000000);
		maFillRect(0, 0, EXTENT_X(screenSize), EXTENT_Y(screenSize));
		font->drawString(inputString.chars, 4, 4);
		//maSetColor(0xff0000);
		//maDrawText(0, 0, inputString.chars);

		Extent screenSize = maGetScrSize();
		maSetColor(0x000000);
		maFillRect(0, 0, EXTENT_X(screenSize), EXTENT_Y(screenSize));
		//Console_Draw();
		
		maUpdateScreen();
	}
	
	return 0;
}
