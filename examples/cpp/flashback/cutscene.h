/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2007 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __CUTSCENE_H__
#define __CUTSCENE_H__

#include "intern.h"
#include "graphics.h"

struct ModPlayer;
struct Resource;
struct SystemStub;
struct Video;

struct Cutscene {
	typedef void (Cutscene::*OpcodeStub)();

	enum {
		NUM_OPCODES = 15,
		TIMER_SLICE = 15
	};

	static const OpcodeStub _opcodeTable[];
	static const char *_namesTable[];
	static const uint16 _offsetsTable[];
	static const uint16 _cosTable[];
	static const uint16 _sinTable[];
	static const uint8 _creditsData[];
	static const uint16 _creditsCutSeq[];
	static const uint8 _musicTable[];
	static const uint8 _protectionShapeData[];

	Graphics _gfx;
	ModPlayer *_ply;
	Resource *_res;
	SystemStub *_stub;
	Video *_vid;
	Version _ver;

	uint16 _id;
	uint16 _deathCutsceneId;
	bool _interrupted;
	bool _stop;
	uint8 *_polPtr;
	uint8 *_cmdPtr;
	uint8 *_cmdPtrBak;
	uint32 _tstamp;
	uint8 _frameDelay;
	bool _newPal;
	uint8 _palBuf[0x20 * 2];
	uint16 _startOffset;
	bool _creditsSequence;
	uint32 _rotData[4];
	uint8 _primitiveColor;
	uint8 _clearScreen;
	Point _vertices[0x80];
	bool _hasAlphaColor;
	uint8 _varText;
	uint8 _varKey;
	int16 _shape_ix;
	int16 _shape_iy;
	int16 _shape_ox;
	int16 _shape_oy;
	int16 _shape_cur_x;
	int16 _shape_cur_y;
	int16 _shape_prev_x;
	int16 _shape_prev_y;
	uint16 _shape_count;
	uint32 _shape_cur_x16;
	uint32 _shape_cur_y16;
	uint32 _shape_prev_x16;
	uint32 _shape_prev_y16;
	uint8 _textSep[0x14];
	uint8 _textBuf[500];
	const uint8 *_textCurPtr;
	uint8 *_textCurBuf;
	uint8 _textUnk2;
	uint8 _creditsTextPosX;
	uint8 _creditsTextPosY;
	int16 _creditsTextCounter;
	uint8 *_page0, *_page1, *_pageC;

	Cutscene(ModPlayer *player, Resource *res, SystemStub *stub, Video *vid, Version ver);

	void sync();
	void copyPalette(const uint8 *pal, uint16 num);
	void updatePalette();
	void setPalette();
	void initRotationData(uint16 a, uint16 b, uint16 c);
	uint16 findTextSeparators(const uint8 *p);
	void drawText(int16 x, int16 y, const uint8 *p, uint16 color, uint8 *page, uint8 n);
	void swapLayers();
	void drawCreditsText();
	void drawProtectionShape(uint8 shapeNum, int16 zoom);
	void drawShape(const uint8 *data, int16 x, int16 y);
	void drawShapeScale(const uint8 *data, int16 zoom, int16 b, int16 c, int16 d, int16 e, int16 f, int16 g);
	void drawShapeScaleRotate(const uint8 *data, int16 zoom, int16 b, int16 c, int16 d, int16 e, int16 f, int16 g);

	void op_markCurPos();
	void op_refreshScreen();
	void op_waitForSync();
	void op_drawShape();
	void op_setPalette();
	void op_drawStringAtBottom();
	void op_nop();
	void op_skip3();
	void op_refreshAll();
	void op_drawShapeScale();
	void op_drawShapeScaleRotate();
	void op_drawCreditsText();
	void op_drawStringAtPos();
	void op_handleKeys();

	uint8 fetchNextCmdByte();
	uint16 fetchNextCmdWord();
	void mainLoop(uint16 offset);
	void load(uint16 cutName);
	void prepare();
	void startCredits();
	void play();
};

#endif // __CUTSCENE_H__
