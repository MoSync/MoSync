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
*
* This file contains the main example program source.
*
* Shows how to create a medium advanced game called MoTris (tetris clone).
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <mastdlib.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <maprofile.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include <MAUtil/Geometry.h>

#include "gui.h"
#include "MAHeaders.h"

using namespace MAUtil;

#define PLAYER_UPDATE_TIME 120
#define PLAYER_UPDATE_TIME_DECREASE 15
#define PLAYER_UPDATE_TIME_MIN 30

#define USE_STORE
#define USE_SYNCHRONIZATION

#define NONE 0
#define DEPLOY_BRICK 1
#define OUT_OF_LEFT_BOUND 2
#define OUT_OF_RIGHT_BOUND 3
#define GAME_OVER 4

#define BKG_PATTERN_X 16
#define BKG_PATTERN_Y 16

#define BORDER_COLOR 0x00a2ff
#define OUTER_BACKGROUND 0x5f5f5f
#define BACKGROUND_COLOR 0x0
#define TEXT_COLOR 0x00a2ff
#define SELECTED_TEXT_COLOR 0xff7f00

#define MENU_INIT 0
#define MENU_RUN 1
#define PAUSE_MENU_INIT 2
#define PAUSE_MENU_RUN 3
#define GAME_INIT 4
#define GAME_RUN 5
#define GAME_OVER_INIT 6
#define HIGH_SCORE_INIT 7
#define HIGH_SCORE_RUN 8
#define GAME_SETTINGS_INIT 9
#define GAME_SETTINGS_RUN 10
#define GAME_QUIT 11
#define GAME_RESUME 12

#define MENU_OFFSET 40
#define HIGH_SCORE_OFFSET 60
#define GAME_OVER_OFFSET 60

#define PLAY_FIELD_X 12
#define PLAY_FIELD_Y 20

#define DOWN_SCORE 1
#define FAST_DEPLOY_SCORE 2

int playerUpdateTime = PLAYER_UPDATE_TIME;
int curGameMode = MENU_INIT;

int lastX, lastY, lastRot, brickX, brickY, curRot, curConf, curCol;
int beneathLogoY;
int aboveCopyrightY;
int _time = 0;
int timeToNextUpdate;
int centerX, centerY, brickSize;
int timeToNextPlayerUpdate = 0;

int curScore = 0;
int curRows = 0;
int curLevel = 0;

char tempString[255];
Menu *highScoreListPtr = 0;

// not allowed to have 0 as brick color, means nothing
int playField[PLAY_FIELD_X*PLAY_FIELD_Y];

int lastGameModePtr = 0;
int lastGameMode[16];

void pushGameMode(int mode) {
	lastGameMode[lastGameModePtr++] = mode;
}

void pushGameMode() {
	lastGameMode[lastGameModePtr++] = curGameMode;
}

void popGameMode() {
	lastGameModePtr--;
	curGameMode = lastGameMode[lastGameModePtr];
}

#define MAX_HIGH_SCORE_NAME 16
#define MAX_HIGH_SCORE_SCORE 16
#define MAX_HIGH_SCORE_ENTRIES 10
#define MAX_USER_NAME 16

struct HighScoreEntry {
	char name[MAX_HIGH_SCORE_NAME];
	int score;
};

struct Settings {
	int curHighScore;
	int numHighScoreEntries;
	char userName[MAX_USER_NAME];
	HighScoreEntry highScoreEntries[MAX_HIGH_SCORE_ENTRIES];
};

Settings curSettings;

struct BrickOffset {
	int ofsX, ofsY;
};

// positive y equals downwards
// all configurations -> all rotations -> all offsets
BrickOffset configurations[7][4][4] = {
	// L-shape
	{
		// rotation 0
		{
			{0,0}, {1,0}, {2,0}, {0, 1}
		},
		// rotation 90
		{
			{0,0}, {1,0}, {0,-1}, {0, -2}
		},
		// rotation 180
		{
			{-2,0}, {-1,0}, {0,0}, {0, -1}
		},
		// rotation 270
		{
			{-1,0}, {0,0}, {0,1}, {0, 2}
		},
	},
	// J-shape
	{
		// rotation 0
		{
			{0,0}, {-1,0}, {-2,0}, {0, 1}
		},
		// rotation 90
		{
			{1,0}, {0,0}, {0,1}, {0, 2}
		},
		// rotation 180
		{
			{2,0}, {1,0}, {0,0}, {0, -1}
		},
		// rotation 270
		{
			{0,0}, {-1,0}, {0,-1}, {0, -2}
		},
	},
	// S-shape
	{
		// rotation 0
		{
			{0,0}, {1,0}, {0,1}, {-1, 1}
		},
		// rotation 90
		{
			{0,0}, {0,-1}, {1,0}, {1, 1}
		},
		// rotation 180
		{
			{0,0}, {1,0}, {0,1}, {-1, 1}
		},
		// rotation 270
		{
			{0,0}, {0,-1}, {1,0}, {1, 1}
		},
	},
	// Z-shape
	{
		// rotation 0
		{
			{0,0}, {-1,0}, {0,1}, {1, 1}
		},
		// rotation 90
		{
			{0,0}, {0,1}, {1,0}, {1, -1}
		},
		// rotation 180
		{
			{0,0}, {-1,0}, {0,1}, {1, 1}
		},
		// rotation 270
		{
			{0,0}, {0,1}, {1,0}, {1, -1}
		},
	},
	// T-shape
	{
		// rotation 0
		{
			{0,0}, {1,0}, {0,1}, {-1, 0}
		},
		// rotation 90
		{
			{0,0}, {1,0}, {0,1}, {0, -1}
		},
		// rotation 180
		{
			{-1,0}, {0,0}, {1,0}, {0, -1}
		},
		// rotation 270
		{
			{-1,0}, {0,0}, {0,1}, {0, -1}
		},
	},
	// I-shape
	{
		// rotation 0
		{
			{0,0}, {0,-1}, {0,1}, {0, 2}
		},
		// rotation 90
		{
			{0,0}, {-1,0}, {1,0}, {2, 0}
		},
		// rotation 180
		{
			{0,0}, {0,-1}, {0,1}, {0, 2}
		},
		// rotation 270
		{
			{0,0}, {-1,0}, {1,0}, {2, 0}
		},
	},
	// O-shape
	{
		// rotation 0
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// rotation 90
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// rotation 180
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// rotation 270
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
	},
};

void synchronizeHighScore();
void addPlayerToHighScore();

void initPlayerUpdateTime() {
	playerUpdateTime = PLAYER_UPDATE_TIME;
}

void setTimeToNextPlayerUpdate() {
//	timeToNextPlayerUpdate=_time+PLAYER_UPDATE_TIME;
	_time = maGetMilliSecondCount();
	timeToNextPlayerUpdate=_time+playerUpdateTime;
	if(playerUpdateTime>PLAYER_UPDATE_TIME_MIN) playerUpdateTime-=PLAYER_UPDATE_TIME_DECREASE;
}

void setTimeToNextUpdate() {
	timeToNextUpdate=_time+500-curLevel*30;
}

void deployBrick(int x, int y, int size, int col, int conf, int rotation) {
	rotation&=0x3;

	for(int i = 0; i < 4; i++) {
		int ox = configurations[conf][rotation][i].ofsX;
		int oy = configurations[conf][rotation][i].ofsY;
		playField[(x+ox)+(y+oy)*PLAY_FIELD_X] = col;
	}
}

int testBrick(int x, int y, int size, int col, int conf, int rotation) {
	rotation&=0x3;

	int anyOnBrick = false;
	bool anyOutTop = false;

	for(int i = 0; i < 4; i++) {
		int ox = configurations[conf][rotation][i].ofsX;
		int oy = configurations[conf][rotation][i].ofsY;
		if((x+ox)<0) return OUT_OF_LEFT_BOUND;
		if((x+ox)>=PLAY_FIELD_X) return OUT_OF_RIGHT_BOUND;
		bool outTop = (y+oy)<0;
		int onBrick = false;
		if(!outTop)
			onBrick = playField[(x+ox)+(y+oy)*PLAY_FIELD_X];

		if(!anyOutTop) anyOutTop = outTop;
		if(!anyOnBrick) anyOnBrick = onBrick;

		if((y+oy)>= PLAY_FIELD_Y)
			return DEPLOY_BRICK;
	}

	if(anyOnBrick && anyOutTop)
		return GAME_OVER;
	else if(anyOnBrick)
		return DEPLOY_BRICK;

	return NONE;
}

void drawBrick(int bx, int by, int col, int rotation, int conf) {
	rotation&=0x3;
	int x = bx*brickSize + (centerX-(brickSize*(PLAY_FIELD_X>>1)));
	int y = by*brickSize + (centerY-(brickSize*(PLAY_FIELD_Y>>1)));
	maSetColor(BACKGROUND_COLOR);
	for(int i = 0; i < 4; i++) {
		int ox = configurations[conf][rotation][i].ofsX;
		int oy = configurations[conf][rotation][i].ofsY;
		if(((by+oy)<0)||((bx+ox)<0)||((bx+ox)>=PLAY_FIELD_X)) continue;

		if(col == 0) {
			maFillRect(x+(ox)*brickSize, y+(oy)*brickSize, brickSize, brickSize);
		} else {
			maDrawImage(BRICK1+col-1, x+(ox)*brickSize, y+(oy)*brickSize);
		}
	}
}

void renderField() {
	int size = brickSize;
	int x = centerX - ((PLAY_FIELD_X>>1)*size);
	int y = centerY - ((PLAY_FIELD_Y>>1)*size);
	int sx;
	int sy = y;
	int index = 0;

	maSetColor(BACKGROUND_COLOR);

	drawOutLinedRect(x, y, x+(PLAY_FIELD_X*size), y+(PLAY_FIELD_Y*size), BORDER_COLOR, BACKGROUND_COLOR);

	for(int j = 0; j < PLAY_FIELD_Y; j++) {
		sx = x;
		for(int i = 0; i < PLAY_FIELD_X; i++) {
			if(playField[index]) {
				maDrawImage(BRICK1+playField[index]-1, sx, sy);
			}

			sx+=size;
			index++;
		}
		sy+=size;
	}
}

void clearPlayer() {
	drawBrick(lastX, lastY, 0, lastRot, curConf);
}

void renderPlayer() {
	drawBrick(brickX, brickY, curCol+1, curRot, curConf);
}

int rowScore[4] = {40, 100, 300, 1200};

void checkBoard() {
	int num;
	int index = 0;
	int numRowsFound = 0;

	int size = brickSize;
	int x = centerX - ((PLAY_FIELD_X>>1)*size);
	int y = centerY - ((PLAY_FIELD_Y>>1)*size);

	for(int j = 0; j < PLAY_FIELD_Y; j++) {
		num=0;
		for(int i = 0; i < PLAY_FIELD_X; i++) {
			if(playField[index]!=0) num++;
			index++;
		}

		if(num==PLAY_FIELD_X) {
			maSetColor(BACKGROUND_COLOR);
			for(int i = 0; i < PLAY_FIELD_X; i++) {
				maFillRect(x+i*size, y+j*size, size, size);
				maUpdateScreen();
				maWait(20);
			}
			int index2 = index-1;
			for(int k = j; k > 0; k--) {
				for(int i = 0; i < PLAY_FIELD_X; i++) {
					playField[index2] = playField[index2-PLAY_FIELD_X];
					index2--;
				}
			}
			numRowsFound++;
		}
	}
	if(numRowsFound) {
		renderField();
		curScore+=rowScore[numRowsFound-1];
		curRows+=numRowsFound;
		if(curRows>=(curLevel+1)*10) {
			curLevel++;
			setTimeToNextUpdate();
		}
	}
}


static int lineNumber = PLAY_FIELD_Y-1;
int applyGravity(int line) {
	int *field_ptr = playField + PLAY_FIELD_X*line;
	int updated = 0;

	for(int i = 0; i < PLAY_FIELD_X; i++) {
		if(!*field_ptr) {
			if(*(field_ptr-PLAY_FIELD_X)) {
				*field_ptr = *(field_ptr-PLAY_FIELD_X);
				*(field_ptr-PLAY_FIELD_X) = 0;
				updated = 1;
			}
		}
		field_ptr++;
	}

	return updated;
}
void updateGravity() {

	int _time = maGetMilliSecondCount();
	if((_time&0x3) != 0) return;

	int render = 0;

	if(applyGravity(lineNumber)) {
		render = 1;
	}

	if(render) {
		renderField();
	}

	lineNumber--;
	if(lineNumber<1) lineNumber = PLAY_FIELD_Y-1;
}

void initBrick() {
	lastX = brickX = (PLAY_FIELD_X>>1);
	lastY = brickY = -1;
	curRot = 0;
	curConf = rand()%7;
	curCol = rand()%7;
	initPlayerUpdateTime();
}

void initGame(int x, int y, int b) {
	memset(playField, 0, PLAY_FIELD_X*PLAY_FIELD_Y*sizeof(int));

	centerX = x;
	centerY = y;
	brickSize = b;
	_time=maGetMilliSecondCount();
	setTimeToNextUpdate();
	setTimeToNextPlayerUpdate();
	curScore = 0;
	curRows = 0;
	curLevel = 0;
	srand(maLocalTime());

	clearPlayer();
	renderPlayer();
	initBrick();

	renderField();
}

void resumeGame() {
	clearPlayer();
	renderPlayer();
	renderField();
}

void updatePlayer() {
	int _time=maGetMilliSecondCount();
	if(_time<timeToNextPlayerUpdate) return;

	setTimeToNextPlayerUpdate();
	int code = NONE;
	lastX = brickX;
	lastY = brickY;
	lastRot = curRot;

	if(EventHandler::up_pressed) {
		int r =(curRot+1)&0x3;
		code = testBrick(brickX, brickY, brickSize, 0, curConf, r);
		if(code == NONE)
			curRot = r;

		// adjust the brick so it can rotate within the borders
		if(code==OUT_OF_LEFT_BOUND) {
			while(code == OUT_OF_LEFT_BOUND) {
				brickX++;
				code = testBrick(brickX, brickY, brickSize, 0, curConf, r);
			}
			curRot = r;
		}
		if(code==OUT_OF_RIGHT_BOUND) {
			while(code == OUT_OF_RIGHT_BOUND) {
				brickX--;
				code = testBrick(brickX, brickY, brickSize, 0, curConf, r);
			}
			curRot = r;
		}
		if(code == DEPLOY_BRICK) {
			return;
		}
	}
	else if(EventHandler::fire_pressed) {
		clearPlayer();
		int numSteps = 0;
		while(code!=DEPLOY_BRICK && code != GAME_OVER) {
			code = testBrick(brickX, brickY+1, brickSize, 0, curConf, curRot);
			if(code==NONE) {
				brickY++;
				numSteps++;
			}
		}
		lastX = brickX;
		lastY = brickY;
		renderPlayer();
		curScore+=FAST_DEPLOY_SCORE*numSteps*(curLevel+1);

	}
	else if(EventHandler::down) {
		code = testBrick(brickX, brickY+1, brickSize, 0, curConf, curRot);
		if(code == NONE) {
			brickY++;
			setTimeToNextUpdate();
			curScore+=DOWN_SCORE*(curLevel+1);
		}
	}
	else {
		code = testBrick(brickX, brickY, brickSize, 0, curConf, curRot);
	}

	if(EventHandler::left) {
		code = testBrick(brickX-1, brickY, brickSize, 0, curConf, curRot);
		if(code == NONE)
			brickX--;
		if(code == DEPLOY_BRICK) {
			return;
		}
	}
	else if(EventHandler::right) {
		code = testBrick(brickX+1, brickY, brickSize, 0, curConf, curRot);
		if(code == NONE)
			brickX++;
		if(code == DEPLOY_BRICK) {
			return;
		}
	}

	if(code == GAME_OVER) {
		curGameMode = GAME_OVER_INIT;
	}
	else if(code==DEPLOY_BRICK) {
		deployBrick(lastX, lastY, brickSize, curCol+1, curConf, lastRot);
		checkBoard();
		initBrick();
	}
}

void updateField() {
	_time=maGetMilliSecondCount();

	if(_time>timeToNextUpdate) {
		setTimeToNextUpdate();
		lastX = brickX;
		lastY = brickY;
		lastRot = curRot;
		brickY++;

		int code = testBrick(brickX, brickY, brickSize, 0, curConf, curRot);
		if(code==DEPLOY_BRICK) {
				deployBrick(lastX, lastY, brickSize, curCol+1, curConf, curRot);
				checkBoard();
				initBrick();
		}
	}
}

#ifdef MA_PROF_SUPPORT_STYLUS
void resetEventHandler() {
	EventHandler::left_pressed = EventHandler::up_pressed = EventHandler::right_pressed = EventHandler::down_pressed = false;
	EventHandler::left = EventHandler::up = EventHandler::right = EventHandler::down = false;

	EventHandler::pointer_pressed = false;

	EventHandler::lsk_pressed = false;
	EventHandler::rsk_pressed = false;

	EventHandler::point.x = -1;
	EventHandler::point.y = -1;
}
#endif	// MA_PROF_SUPPORT_STYLUS

char score[255];
char rows[255];
char level[255];

#ifdef MA_PROF_SUPPORT_STYLUS
void showNavKeys(MAPoint2d point) {
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);
	int h = EXTENT_Y(e);

	sprintf(score, "%d", curScore);
	sprintf(rows, "%d", curRows);
	sprintf(level, "%d", curLevel);

	MAExtent ts = maGetTextSize(score);
	MAExtent tr = maGetTextSize(rows);
	MAExtent tl = maGetTextSize(level);

	int x1 = (w>>1)+14;
	int y1 = centerY - ((PLAY_FIELD_Y>>1)*brickSize) + (EXTENT_Y(ts)+EXTENT_Y(tr)+EXTENT_Y(tl))*2+4;
	int x2 = (w>>1)+85;
	int y2 = centerY - ((PLAY_FIELD_Y>>1)*brickSize)+(PLAY_FIELD_Y*brickSize);

	drawOutLinedRect(x1, y1, x2, y2, BORDER_COLOR, BACKGROUND_COLOR);

	MAExtent okSize = maGetImageSize(OK);
	Point centerOk(x1+(x2-x1)/2, y1+(y2-y1)/2);
	Rect rect(centerOk.x - EXTENT_X(okSize)/2, centerOk.y - EXTENT_Y(okSize)/2, EXTENT_X(okSize), EXTENT_Y(okSize));
	maDrawImage(OK, rect.x, rect.y);
	if(rect.contains(point.x, point.y) && EventHandler::pointer_pressed) {
		resetEventHandler();
		EventHandler::fire_pressed = EventHandler::fire = true;
	}

	MAExtent imgSize = maGetImageSize(UP);
	rect = Rect(x1+(x2-x1-EXTENT_X(imgSize))/2, centerOk.y - EXTENT_Y(okSize)/2 - EXTENT_Y(imgSize), EXTENT_X(imgSize), EXTENT_Y(imgSize));
	maDrawImage(UP, rect.x, rect.y);
	if(rect.contains(point.x, point.y) && EventHandler::pointer_pressed) {
		resetEventHandler();
		EventHandler::up_pressed = EventHandler::up = true;
	}

	rect = Rect(x1+(x2-x1-EXTENT_X(imgSize))/2, centerOk.y + EXTENT_Y(okSize)/2, EXTENT_X(imgSize), EXTENT_Y(imgSize));
	maDrawImage(DOWN, rect.x, rect.y);
	if(rect.contains(point.x, point.y) && EventHandler::pointer_pressed) {
		resetEventHandler();
		EventHandler::down_pressed = EventHandler::down = true;
	}

	imgSize = maGetImageSize(LEFT);
	rect = Rect(centerOk.x - EXTENT_X(okSize)/2 - EXTENT_X(imgSize), y1+(y2-y1-EXTENT_Y(imgSize))/2, EXTENT_X(imgSize), EXTENT_Y(imgSize));
	maDrawImage(LEFT, rect.x, rect.y);
	if(rect.contains(point.x, point.y) && EventHandler::pointer_pressed) {
		resetEventHandler();
		EventHandler::left_pressed = EventHandler::left = true;
	}

	rect = Rect(centerOk.x + EXTENT_X(okSize)/2, y1+(y2-y1-EXTENT_Y(imgSize))/2, EXTENT_X(imgSize), EXTENT_Y(imgSize));
	maDrawImage(RIGHT, rect.x, rect.y);
	if(rect.contains(point.x, point.y) && EventHandler::pointer_pressed) {
		resetEventHandler();
		EventHandler::right_pressed = EventHandler::right = true;
	}
}
#endif	// MA_PROF_SUPPORT_STYLUS

void showScore() {
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);
	int h = EXTENT_Y(e);

	sprintf(score, "%d", curScore);
	sprintf(rows, "%d", curRows);
	sprintf(level, "%d", curLevel);

	MAExtent ts = maGetTextSize(score);
	MAExtent tr = maGetTextSize(rows);
	MAExtent tl = maGetTextSize(level);

	int x = (w>>1)+14;
#ifdef MA_PROF_SUPPORT_STYLUS
	int y = centerY - ((PLAY_FIELD_Y>>1)*brickSize);
#else	// MA_PROF_SUPPORT_STYLUS
	int y = (h>>1)-EXTENT_Y(ts)*2 - 2*(EXTENT_Y(tr)>>1);
#endif	// MA_PROF_SUPPORT_STYLUS

	drawOutLinedRect(x, y, (w>>1)+85, y+(EXTENT_Y(ts)+EXTENT_Y(tr)+EXTENT_Y(tl))*2, BORDER_COLOR, BACKGROUND_COLOR);

	x+=2;

	maSetColor(TEXT_COLOR);
	maDrawText(x, y, "score:");
	y+=EXTENT_Y(ts);
	maDrawText(x, y, score);
	y+=EXTENT_Y(ts);
	maDrawText(x, y, "rows:");
	y+=EXTENT_Y(tr);
	maDrawText(x, y, rows);
	y+=EXTENT_Y(tr);
	maDrawText(x, y, "level:");
	y+=EXTENT_Y(tl);
	maDrawText(x, y, level);
	y+=EXTENT_Y(tl);
}

class MainMenuListener : public MenuListener {
	void onEvent(int index, Menu *menu) {
		if(EventHandler::fire_pressed) {
			pushGameMode(MENU_INIT);
			switch(menu->getCurrentlySelectedMenuIndex()) {
				case 0: // new game
					curGameMode = GAME_INIT;
					break;
				case 1: // show high score
					curGameMode = HIGH_SCORE_INIT;
					break;
				case 2:
					curGameMode = GAME_SETTINGS_INIT;
					break;
				case 3: // quit game
					curGameMode = GAME_QUIT;
					break;
			}
		}
#ifdef MA_PROF_SUPPORT_STYLUS
		if(EventHandler::pointer_pressed) {
			pushGameMode(MENU_INIT);
			switch(menu->handlePointerPress(EventHandler::point)) {
				case 0: // new game
					curGameMode = GAME_INIT;
					break;
				case 1: // show high score
					curGameMode = HIGH_SCORE_INIT;
					break;
				case 2:
					curGameMode = GAME_SETTINGS_INIT;
					break;
				case 3: // quit game
					curGameMode = GAME_QUIT;
					break;
			}
		}
#endif	// MA_PROF_SUPPORT_STYLUS
	}
};

class PauseMenuListener : public MenuListener {
	void onEvent(int index, Menu *menu) {
		if(EventHandler::fire_pressed) {
			pushGameMode(PAUSE_MENU_INIT);
			switch(menu->getCurrentlySelectedMenuIndex()) {
				case 0: // resume game
					curGameMode = GAME_RESUME;
					break;
				case 1: // new game
					curGameMode = GAME_INIT;
					break;
				case 2: // show high score
					curGameMode = HIGH_SCORE_INIT;
					break;
				case 3:
					curGameMode = GAME_SETTINGS_INIT;
					break;
				case 4: // quit game
					curGameMode = GAME_QUIT;
					break;
			}
		}
#ifdef MA_PROF_SUPPORT_STYLUS
		if(EventHandler::pointer_pressed) {
			pushGameMode(MENU_INIT);
			switch(menu->handlePointerPress(EventHandler::point)) {
				case 0: // resume game
					curGameMode = GAME_RESUME;
					break;
				case 1: // new game
					curGameMode = GAME_INIT;
					break;
				case 2: // show high score
					curGameMode = HIGH_SCORE_INIT;
					break;
				case 3:
					curGameMode = GAME_SETTINGS_INIT;
					break;
				case 4: // quit game
					curGameMode = GAME_QUIT;
					break;
			}
		}
#endif	// MA_PROF_SUPPORT_STYLUS
	}
};

#define SETTINGS_CHOOSER 0
#define SETTINGS_SET_NAME 1
int curSettingsMode = SETTINGS_CHOOSER;

class SetNameEditBoxListener : public EditBoxListener {

	void onEvent(EditBox *editBox) {
		if(EventHandler::fire_pressed
#ifdef MA_PROF_SUPPORT_STYLUS
			|| EventHandler::pointer_pressed
#endif	// MA_PROF_SUPPORT_STYLUS
		)
		{
			curSettings.userName[0] = 0;
			strncpy(curSettings.userName, (char*)editBox->getCurrentText(), MAX_USER_NAME);
			curSettings.userName[MAX_USER_NAME-1] = 0;
			for(int i = MAX_USER_NAME-1; i >= 1; i--) {
				if(curSettings.userName[i]==' ' && curSettings.userName[i-1]!=' ') {
					curSettings.userName[i] = 0;
					break;
				}
			}

			curSettingsMode = SETTINGS_CHOOSER;
			curGameMode = GAME_SETTINGS_INIT;
		}
	}
};

class SettingsMenuListener : public MenuListener, public EditBoxListener {
	void onEvent(EditBox *editBox) {
			if(EventHandler::fire_pressed) {
				curGameMode = GAME_SETTINGS_INIT;
				curSettingsMode = SETTINGS_CHOOSER;
			}
	}

	void onEvent(int index, Menu *menu) {
		if(EventHandler::fire_pressed) {
			switch(menu->getCurrentlySelectedMenuIndex()) {
				case 0: // resume game
					curSettingsMode = SETTINGS_SET_NAME;
					curGameMode = GAME_SETTINGS_INIT;
					break;
				case 1: // new game
					popGameMode();
					break;
			}
		}
#ifdef MA_PROF_SUPPORT_STYLUS
		if(EventHandler::pointer_pressed) {
			pushGameMode(MENU_INIT);
			switch(menu->handlePointerPress(EventHandler::point)) {
			case 0: // resume game
				curSettingsMode = SETTINGS_SET_NAME;
				curGameMode = GAME_SETTINGS_INIT;
				break;
			case 1: // new game
				popGameMode();
				break;
			}
		}
#endif	// MA_PROF_SUPPORT_STYLUS
	}
};

class HighScoreListListener : public MenuListener {
	void onEvent(int index, Menu *menu) {
		if(EventHandler::fire_pressed
#ifdef MA_PROF_SUPPORT_STYLUS
			|| EventHandler::pointer_pressed
#endif	// MA_PROF_SUPPORT_STYLUS
		) {
			popGameMode();
		}
	}
};

void showHighScore() {
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);

	sprintf(score, "High score: %d", curSettings.curHighScore);

	int x = (w>>1);
	int y = beneathLogoY+HIGH_SCORE_OFFSET;

	drawShadowedText(x-(EXTENT_X(maGetTextSize(score))>>1), y, TEXT_COLOR, score);
}

void updateHighScore() {
	if(EventHandler::fire_pressed) {
		popGameMode();
	}
}

void showGameOver() {

	const char *gameover_str = "Game over";
	MAExtent ge = maGetTextSize(gameover_str);

	int x = centerX-(EXTENT_X(ge)>>1)-2;
	int y = beneathLogoY+GAME_OVER_OFFSET;

	drawOutLinedRect(x, y, x+EXTENT_X(ge)+2, y+EXTENT_Y(ge)+2, BORDER_COLOR, BACKGROUND_COLOR);

	maSetColor(TEXT_COLOR);
	maDrawText(x+2, y, gameover_str);
}

void updateGameOver() {
	if(EventHandler::fire_pressed) {
		addPlayerToHighScore();
		curGameMode = MENU_INIT;
	}
}

void loadSettingsFromStore() {	
	MAHandle settingsStore = maOpenStore("settings", 0);
	if(settingsStore<=0)
	{
		memset(&curSettings, 0, sizeof(Settings));
		sprintf(curSettings.userName, "Tony H.");
		maCreateData(MOTRIS_SETTINGS, sizeof(Settings));
		return;
	}
	
	maReadStore(settingsStore, MOTRIS_SETTINGS);
	if(maGetDataSize(MOTRIS_SETTINGS)==(sizeof(Settings))) {
		maReadData(MOTRIS_SETTINGS, &curSettings, 0, sizeof(Settings));
	}
	else {
		maDestroyObject(MOTRIS_SETTINGS);
		maCreateData(MOTRIS_SETTINGS, sizeof(Settings));
		maWriteData(MOTRIS_SETTINGS, &curSettings, 0, sizeof(Settings));
		maWriteStore(settingsStore, MOTRIS_SETTINGS);
	}
}

void saveSettingsToStore() {
	MAHandle settingsStore = maOpenStore("settings", MAS_CREATE_IF_NECESSARY);
	if(settingsStore<=0)
	{
		return;
	}
	
	maWriteData(MOTRIS_SETTINGS, &curSettings, 0, sizeof(Settings));
	maWriteStore(settingsStore, MOTRIS_SETTINGS);
}

void populateHighScoreList() {
	if(!highScoreListPtr) return;
	highScoreListPtr->clear();
	for(int i = 0; i < MIN(curSettings.numHighScoreEntries, MAX_HIGH_SCORE_ENTRIES); i++) {
		sprintf(tempString, "%d: %s %d", i+1, curSettings.highScoreEntries[i].name, curSettings.highScoreEntries[i].score);
		highScoreListPtr->addMenuItem(String(tempString));
	}
}

void addPlayerToHighScore() {
	int index = MIN(curSettings.numHighScoreEntries, MAX_HIGH_SCORE_ENTRIES);
	int i;
	for(i = 0; i < MIN(curSettings.numHighScoreEntries, MAX_HIGH_SCORE_ENTRIES); i++) {
		if(curScore>curSettings.highScoreEntries[i].score) {
			index = i;
			break;
		}
	}

	if(index == MAX_HIGH_SCORE_ENTRIES) {
		return;
	}

	for(int i = MAX_HIGH_SCORE_ENTRIES-1; i>index; i--) {
		curSettings.highScoreEntries[i] = curSettings.highScoreEntries[i-1];
	}

	strncpy(curSettings.highScoreEntries[index].name, curSettings.userName, MAX_HIGH_SCORE_NAME);
	curSettings.highScoreEntries[index].score = curScore;
	curSettings.numHighScoreEntries++;

	populateHighScoreList();
}

void drawBackground(int w, int h, int x, int y) {
	int col1 = OUTER_BACKGROUND;
	int col2 = OUTER_BACKGROUND-0x0f0f0f;

	if(w<h) {
		for(int j = 0; j < h; j+=y) {
			for(int i = 0; i < w; i+=x) {
				col1^=col2;
				col2^=col1;
				col1^=col2;
				drawOutLinedRect(i+1, j+1, i+x-1, j+y-1, col1, col2);
			}
		}
	} else {
		for(int i = 0; i < w; i+=x) {
			for(int j = 0; j < h; j+=y) {
				col1^=col2;
				col2^=col1;
				col1^=col2;
				drawOutLinedRect(i+1, j+1, i+x-1, j+y-1, col1, col2);
			}
		}
	}

	MAExtent logoExtents = maGetImageSize(MOTRIS_LOGO);
	maDrawImage(MOTRIS_LOGO, (w>>1) - (EXTENT_X(logoExtents)>>1),  beneathLogoY-EXTENT_Y(logoExtents)-4);

	const char *copyright = "(c) mobile sorcery 2007";
	MAExtent ce = maGetTextSize(copyright);
	aboveCopyrightY = h-EXTENT_Y(ce);
	drawShadowedText((w>>1)-(EXTENT_X(ce)>>1), aboveCopyrightY, TEXT_COLOR, copyright);

}

extern "C" {
/**
 * \brief The entry point.
 */

int MAMain()
{
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);
	int h = EXTENT_Y(e);
	/*
	maSetColor(OUTER_BACKGROUND);
	maFillRect(0, 0, w, h);*/
	drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);

	brickSize = 8;

	beneathLogoY = (h>>1)+9-(PLAY_FIELD_Y>>1)*brickSize;
	Menu mainMenu(TEXT_COLOR, SELECTED_TEXT_COLOR, w>>1, beneathLogoY + MENU_OFFSET, w, h, ALIGN_MID_X);
	mainMenu.addMenuItem("New game");
	mainMenu.addMenuItem("High score");
	mainMenu.addMenuItem("Settings");
	mainMenu.addMenuItem("Quit");
	mainMenu.addMenuListener(new MainMenuListener());

	Menu pauseMenu(TEXT_COLOR, SELECTED_TEXT_COLOR, w>>1, beneathLogoY + MENU_OFFSET, w, h, ALIGN_MID_X);
	pauseMenu.addMenuItem("Resume game");
	pauseMenu.addMenuItem("New game");
	pauseMenu.addMenuItem("High score");
	pauseMenu.addMenuItem("Settings");
	pauseMenu.addMenuItem("Quit");
	pauseMenu.addMenuListener(new PauseMenuListener());

	Menu settingsMenu(TEXT_COLOR, SELECTED_TEXT_COLOR, w>>1, beneathLogoY + MENU_OFFSET, w, h, ALIGN_MID_X);
	settingsMenu.addMenuItem("Set name");
	settingsMenu.addMenuItem("Go back");
	settingsMenu.addMenuListener(new SettingsMenuListener());

	EditBox setNameEditBox(w>>1, beneathLogoY+HIGH_SCORE_OFFSET, MAX_USER_NAME, TEXT_COLOR, 0x444444, ALIGN_MID_X, ALIGN_TOP_Y, "Enter a name: ");
	setNameEditBox.addEditBoxListener(new SetNameEditBoxListener());

	Menu highScoreList(TEXT_COLOR, SELECTED_TEXT_COLOR, w>>1, beneathLogoY, w, aboveCopyrightY - (beneathLogoY), ALIGN_MID_X, 0);
	highScoreList.addMenuListener(new HighScoreListListener());
	highScoreListPtr = &highScoreList;


#ifdef USE_STORE
	loadSettingsFromStore();
#endif
	if(strlen(curSettings.userName)) setNameEditBox.setCurrentText(curSettings.userName);
	populateHighScoreList();

	while(1) {
		if(curGameMode == MENU_INIT) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			curGameMode = MENU_RUN;
		}
		else if(curGameMode == MENU_RUN) {
			mainMenu.show();
			mainMenu.update();

#ifdef MA_PROF_SUPPORT_STYLUS
			if(EventHandler::pointer_pressed ||
				EventHandler::pointer_released) {
				mainMenu.handlePointerPress(EventHandler::point);
			}
#endif	// MA_PROF_SUPPORT_STYLUS
		}
		else if(curGameMode == PAUSE_MENU_INIT) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			curGameMode = PAUSE_MENU_RUN;
		}
		else if(curGameMode == PAUSE_MENU_RUN) {
			pauseMenu.show();
			pauseMenu.update();
		}
		else if(curGameMode == HIGH_SCORE_INIT) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			curGameMode = HIGH_SCORE_RUN;
		}
		else if(curGameMode == HIGH_SCORE_RUN) {
			highScoreList.show();
			highScoreList.update();
		}
		else if(curGameMode == GAME_SETTINGS_INIT) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			curGameMode = GAME_SETTINGS_RUN;
		}
		else if(curGameMode == GAME_SETTINGS_RUN) {
			if(curSettingsMode==SETTINGS_SET_NAME) {
				setNameEditBox.show();
				setNameEditBox.update();
				if(EventHandler::rsk_pressed) {
					curGameMode = GAME_SETTINGS_INIT;
					curSettingsMode = SETTINGS_CHOOSER;
				}
			} else if(curSettingsMode==SETTINGS_CHOOSER) {
				settingsMenu.show();
				settingsMenu.update();
			}
		}
		else if(curGameMode == GAME_INIT) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			initGame((w>>1)-(38), (h>>1)+10, 8);
			curGameMode = GAME_RUN;
		}
		else if(curGameMode == GAME_RESUME) {
			drawBackground(w, h, BKG_PATTERN_X, BKG_PATTERN_Y);
			resumeGame();
			curGameMode = GAME_RUN;
		}
		else if(curGameMode == GAME_RUN) {
			updateField();
			clearPlayer();
			renderPlayer();

#ifdef MA_PROF_SUPPORT_STYLUS
			showNavKeys(EventHandler::point);
#endif	// MA_PROF_SUPPORT_STYLUS

			if(EventHandler::up_pressed ||
			   EventHandler::down_pressed ||
			   EventHandler::left_pressed ||
			   EventHandler::right_pressed ||
			   EventHandler::fire_pressed) {
					timeToNextPlayerUpdate=maGetMilliSecondCount()-100;
					initPlayerUpdateTime();
			}

			updatePlayer();
			clearPlayer();
			renderPlayer();

			showScore();
			/*
			updateGravity();
			checkBoard();
			*/
		}
		else if(curGameMode == GAME_OVER_INIT) {
			showGameOver();
			updateGameOver();
		}

		maUpdateScreen();

		/// keep the backlight alive
		maResetBacklight();
		EventHandler::updateEvents();

		if(curGameMode == GAME_RUN && (EventHandler::star_pressed
#ifdef MA_PROF_SUPPORT_STYLUS
			|| EventHandler::rsk_pressed
#endif	// MA_PROF_SUPPORT_STYLUS
		)) {
			curGameMode = PAUSE_MENU_INIT;
		}

		if(EventHandler::quit || curGameMode == GAME_QUIT) {
			break;
		}
	}

#ifdef USE_STORE
	saveSettingsToStore();
#endif

	return 0;
}
}
