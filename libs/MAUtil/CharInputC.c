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

#ifdef __cplusplus
#error c++?
#endif

#include "CharInput.h"

#define CI_ACTIVE 1
#define CI_INACTIVE 0

#define CI_SHORT_PRESS_TIME 500
#define CI_LONG_PRESS_TIME 500

#define CI_RELEASED 0
#define CI_PRESSED 1

static StartTimerCallback sStartTimerCallback = 0;
static StopTimerCallback sStopTimerCallback = 0;

static CharCallback sCharacterChangedCallback = 0;
static CharCallback sCharacterDeployedCallback = 0;
static void* sCharacterChangedUserData = 0;
static void* sCharacterDeployedUserData = 0;

static int sCurrentCharMapIndex = 0;
static int sCurrentCharMapListIndex = 0;

static int sCurrentMode = CI_INACTIVE;
static int sCurrentCharMode = CI_MODE_CHAR_MODE_LOWERCASE;

static BOOL sQwerty = FALSE;

void CharInput_Reset(void) {
	sCurrentCharMapIndex = 0;
	sCurrentCharMapListIndex = 0;
	sCurrentMode = CI_INACTIVE;
	sCurrentCharMode = CI_MODE_CHAR_MODE_LOWERCASE;
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
static const char *charMapLowerCase[]= {	".,@:?!'\"/1",	"abcåä2",		"def3",
							"ghi4",			"jkl5",		"mnoö6",
							"pqrs7",		"tuv8",		"wxyz9",
							"*+<>()[]{}",	" ",		" "};

static const char *charMapUpperCase[]= {	".,@:?!'\"/1",	"ABCÅÄ2",		"DEF3",
							"GHI4",			"JKL5",		"MNOÖ6",
							"PQRS7",		"TUV8",		"WXYZ9",
							"*+<>()[]{}",	" ",		"  "};
#endif

static char numMap[] = {'1', '2', '3',
				 '4', '5', '6',
				 '7', '8', '9',
				 '+', '0', ' '};

static char getChar(int charMode) {
	char c;
	switch(charMode) {
		case CI_MODE_CHAR_MODE_LOWERCASE:
			c = charMapLowerCase[sCurrentCharMapIndex][sCurrentCharMapListIndex];
			break;
		case CI_MODE_CHAR_MODE_UPPERCASE:
			c = charMapUpperCase[sCurrentCharMapIndex][sCurrentCharMapListIndex];
			break;
		case CI_MODE_CHAR_MODE_NUMBERS:
			c = numMap[sCurrentCharMapIndex];
			break;
		default:
			c = 0;
	}
	return c;
}

void CharInput_setLowerCaseMapElement(int index, const char *str) {
	if(index<0 || index>=12) maPanic(1, "Map element index out of bounds");
	charMapLowerCase[index] = str;
}

void CharInput_setUpperCaseMapElement(int index, const char *str) {
	if(index<0 || index>=12) maPanic(1, "Map element index out of bounds");
	charMapUpperCase[index] = str;
}

void CharInput_setNumMapElement(int index, char num) {
	if(index<0 || index>=12) maPanic(1, "Map element index out of bounds");
	numMap[index] = num;
}

static void CharInput_ShortPress(void) {
	if(sStopTimerCallback)
		sStopTimerCallback();

	if(sCurrentMode == CI_INACTIVE) return;

	if(sCharacterDeployedCallback)
		sCharacterDeployedCallback(getChar(sCurrentCharMode), sCharacterDeployedUserData);
	sCurrentMode = CI_INACTIVE;
}

static void CharInput_LongPress(void) {
	if(sStopTimerCallback)
		sStopTimerCallback();

	if(sCurrentMode == CI_INACTIVE) return;

	if(sCharacterDeployedCallback)
		sCharacterDeployedCallback(getChar(CI_MODE_CHAR_MODE_NUMBERS), sCharacterDeployedUserData);
	sCurrentMode = CI_INACTIVE;
}

void CharInput_setMode(int charMode) {
	sCurrentCharMode = charMode;
}

int CharInput_getMode(void) {
	return sCurrentCharMode; 
}

void CharInput_Pressed(int keyCode) {
	int newCharMapIndex;
	if(sQwerty) {
		//if in numeric mode
		if(sCurrentCharMode == CI_MODE_CHAR_MODE_NUMBERS) {
			//discard all non-number keys
			if(keyCode < MAK_0 || keyCode > MAK_9)
				return;
		}
		if(keyCode >= MAK_SPACE && keyCode < MAK_DELETE) {
			sCharacterChangedCallback(keyCode, sCharacterChangedUserData);
			sCharacterDeployedCallback(keyCode, sCharacterDeployedUserData);
		}
		return;
	}

	newCharMapIndex = sCurrentCharMapIndex;
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

	if(sCurrentMode == CI_ACTIVE && newCharMapIndex == sCurrentCharMapIndex) {
		sCurrentCharMapListIndex++;
		if(getChar(sCurrentCharMode) == 0) {
			sCurrentCharMapListIndex = 0;
		}
	} else {
		if(sCurrentMode == CI_ACTIVE) {
			if(sCharacterDeployedCallback)
				sCharacterDeployedCallback(getChar(sCurrentCharMode), sCharacterDeployedUserData);
		}

		sCurrentCharMapListIndex = 0;
		sCurrentCharMapIndex = newCharMapIndex;
	}

	if(sCharacterChangedCallback)
		sCharacterChangedCallback(getChar(sCurrentCharMode), sCharacterChangedUserData);

	sCurrentMode = CI_ACTIVE;

	if(sStopTimerCallback)
		sStopTimerCallback();
	if(sStartTimerCallback)
		sStartTimerCallback(CharInput_LongPress, CI_LONG_PRESS_TIME);
}

void CharInput_Released(int keyCode) {
	if(sQwerty) {
		return;
	}
	if(sStopTimerCallback)
		sStopTimerCallback();
	if(sStartTimerCallback)
		sStartTimerCallback(CharInput_ShortPress, CI_SHORT_PRESS_TIME);
}

void CharInput_RegisterTimerCallback(StartTimerCallback startCallback, StopTimerCallback stopCallback) {
	sStartTimerCallback = startCallback;
	sStopTimerCallback = stopCallback;
}

void CharInput_RegisterCharacterChangedCallback(CharCallback callback, void *userData) {
	sCharacterChangedUserData = userData;
	sCharacterChangedCallback = callback;
}

void CharInput_RegisterCharacterDeployedCallback(CharCallback callback, void *userData) {
	sCharacterDeployedUserData = userData;
	sCharacterDeployedCallback = callback;
}

const char* CharInput_getCurrentCharList(void) {
	return (const char*) charMapUpperCase[sCurrentCharMapIndex];
}

int CharInput_getCurrentCharListIndex(void) {
	return sCurrentCharMapListIndex;
}

void CharInput_ForceDeployment(void) {
	CharInput_ShortPress();
}

void CharInput_setQwerty(BOOL on) {
	if(!sQwerty && on && sStopTimerCallback)
		sStopTimerCallback();
	sQwerty = on;
}

BOOL CharInput_getQwerty(void) {
	return sQwerty;
}
