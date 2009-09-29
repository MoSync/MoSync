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

#include "CharInput.h"

#define CI_ACTIVE 1
#define CI_INACTIVE 0

#define CI_SHORT_PRESS_TIME 500
#define CI_LONG_PRESS_TIME 500

#define CI_RELEASED 0
#define CI_PRESSED 1

static StartTimerCallback startTimerCallback = 0;
static StopTimerCallback stopTimerCallback = 0;

static CharCallback characterChangedCallback = 0;
static CharCallback characterDeployedCallback = 0;
static void* characterChangedUserData = 0;
static void* characterDeployedUserData = 0;

static int currentCharMapIndex = 0;
static int currentCharMapListIndex = 0;

static int currentMode = CI_INACTIVE;
static int currentCharMode = CI_MODE_CHAR_MODE_LOWERCASE;

void CharInput_Reset() {
	currentCharMapIndex = 0;
	currentCharMapListIndex = 0;
	currentMode = CI_INACTIVE;
	currentCharMode = CI_MODE_CHAR_MODE_LOWERCASE;
}

#if defined(NOKIA)
static const char *charMapLowerCase[]= {	".,:?!'\"/1", "abcåä2", "def3",
							"ghi4",				"jkl5",		"mnoö6",
							"pqrs7",			"tuv8",		"wxyz9",
							"*+<>()[]{}",		" 0\n",		"#"};

static const char *charMapUpperCase[]= {	".,:?!'\"/1",	"ABCÅÄ2",		"DEF3",
							"GHI4",				"JKL5",		"MNOÖ6",
							"PQRS7",			"TUV8",		"WXYZ9",
							"*+<>()[]{}",		" 0\n",		"#"};
#elif defined(ERICSSON)
static const char *charMapLowerCase[]= {	".,:?!'\"/1",	"abcåä2",		"def3",
							"ghi4",				"jkl5",		"mnoö6",
							"pqrs7",			"tuv8",		"wxyz9",
							"*+<>()[]{}",		"0+\n",		" #"};

static const char *charMapUpperCase[]= { 	".,:?!'\"/1",		"ABCÅÄ2",		"DEF3",
							"GHI4",				"JKL5",		"MNOÖ6",
							"PQRS7",			"TUV8",		"WXYZ9",
							"*+<>()[]{}",		" 0+",		"\n#"};
#else
static const char *charMapLowerCase[]= {	".,:?!'\"/1",	"abcåä2",		"def3",
							"ghi4",			"jkl5",		"mnoö6",
							"pqrs7",		"tuv8",		"wxyz9",
							"*+<>()[]{}",	" ",		" "};

static const char *charMapUpperCase[]= {	".,:?!'\"/1",	"ABCÅÄ2",		"DEF3",
							"GHI4",			"JKL5",		"MNOÖ6",
							"PQRS7",		"TUV8",		"WXYZ9",
							"*+<>()[]{}",	" ",		"  "};
#endif

static const char numMap[] = {'1', '2', '3',
				 '4', '5', '6',
				 '7', '8', '9',
				 '+', '0', ' '};

static char getChar(int charMode) {
	char c;
	switch(charMode) {
		case CI_MODE_CHAR_MODE_LOWERCASE:
			c = charMapLowerCase[currentCharMapIndex][currentCharMapListIndex];
			break;
		case CI_MODE_CHAR_MODE_UPPERCASE:
			c = charMapUpperCase[currentCharMapIndex][currentCharMapListIndex];
			break;
		case CI_MODE_CHAR_MODE_NUMBERS:
			c = numMap[currentCharMapIndex];
			break;
		default:
			c = 0;
	}
	return c;
}

static void CharInput_ShortPress(void) {
	if(stopTimerCallback)
		stopTimerCallback();

	if(currentMode == CI_INACTIVE) return;

	if(characterDeployedCallback)
		characterDeployedCallback(getChar(currentCharMode), characterDeployedUserData);
	currentMode = CI_INACTIVE;
}

static void CharInput_LongPress(void) {
	if(stopTimerCallback)
		stopTimerCallback();

	if(currentMode == CI_INACTIVE) return;

	if(characterDeployedCallback)
		characterDeployedCallback(getChar(CI_MODE_CHAR_MODE_NUMBERS), characterDeployedUserData);
	currentMode = CI_INACTIVE;
}

void CharInput_setMode(int charMode) {
	currentCharMode = charMode;
}

int CharInput_getMode() {
	return currentCharMode; 
}

void CharInput_Pressed(int keyCode) {
	int newCharMapIndex = currentCharMapIndex;
	switch(keyCode) {
		case MAK_0:
			newCharMapIndex = 10;
			break;
		case MAK_1:
		case MAK_2:
		case MAK_3:
		case MAK_4:
		case MAK_5:
		case MAK_6:
		case MAK_7:
		case MAK_8:
		case MAK_9:
			newCharMapIndex = keyCode-MAK_1;
			break;
		case MAK_STAR:
			newCharMapIndex = 9;
			break;

		default:
			return;
	}

	if(currentMode == CI_ACTIVE && newCharMapIndex == currentCharMapIndex) {
		currentCharMapListIndex++;
		if(getChar(currentCharMode) == 0) {
			currentCharMapListIndex = 0;
		}
	} else {
		if(currentMode == CI_ACTIVE) {
			if(characterDeployedCallback)
				characterDeployedCallback(getChar(currentCharMode), characterDeployedUserData);
		}

		currentCharMapListIndex = 0;
		currentCharMapIndex = newCharMapIndex;
	}

	if(characterChangedCallback)
		characterChangedCallback(getChar(currentCharMode), characterChangedUserData);

	currentMode = CI_ACTIVE;

	if(stopTimerCallback)
		stopTimerCallback();
	if(startTimerCallback)
		startTimerCallback(CharInput_LongPress, CI_LONG_PRESS_TIME);
}

void CharInput_Released(int keyCode) {
	if(stopTimerCallback)
		stopTimerCallback();
	if(startTimerCallback)
		startTimerCallback(CharInput_ShortPress, CI_SHORT_PRESS_TIME);
}

void CharInput_RegisterTimerCallback(StartTimerCallback startCallback, StopTimerCallback stopCallback) {
	startTimerCallback = startCallback;
	stopTimerCallback = stopCallback;
}

void CharInput_RegisterCharacterChangedCallback(CharCallback callback, void *userData) {
	characterChangedUserData = userData;
	characterChangedCallback = callback;
}

void CharInput_RegisterCharacterDeployedCallback(CharCallback callback, void *userData) {
	characterDeployedUserData = userData;
	characterDeployedCallback = callback;
}

const char* CharInput_getCurrentCharList() {
	return (const char*) charMapUpperCase[currentCharMapIndex];
}

int CharInput_getCurrentCharListIndex() {
	return currentCharMapListIndex;
}

void CharInput_ForceDeployment() {
	CharInput_ShortPress();
}

