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

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "intern.h"

struct File;

struct LocaleData {
	enum Id {
		LI_01_CONTINUE_OR_ABORT = 0,
		LI_02_TIME,
		LI_03_CONTINUE,
		LI_04_ABORT,
		LI_05_COMPLETED,
		LI_06_LEVEL,
		LI_07_START,
		LI_08_SKILL,
		LI_09_PASSWORD,
		LI_10_INFO,
		LI_11_QUIT,
		LI_12_SKILL_LEVEL,
		LI_13_EASY,
		LI_14_NORMAL,
		LI_15_EXPERT,
		LI_16_ENTER_PASSWORD1,
		LI_17_ENTER_PASSWORD2,
		LI_18_RESUME_GAME,
		LI_19_ABORT_GAME,
		LI_20_LOAD_GAME,
		LI_21_SAVE_GAME,
		LI_22_SAVE_SLOT,

		LI_NUM
	};

	static const char *_textsTableFR[];
	static const char *_textsTableEN[];
	static const char *_textsTableDE[];
	static const char *_textsTableSP[];
	static const uint8 _stringsTableFR[];
	static const uint8 _stringsTableEN[];
	static const uint8 _stringsTableDE[];
	static const uint8 _stringsTableSP[];
};

struct Resource {
	typedef void (Resource::*LoadStub)(File *);

	enum ObjectType {
		OT_MBK    = 0x00,
		OT_PGE    = 0x01,
		OT_PAL    = 0x02,
		OT_CT     = 0x03,
		OT_MAP    = 0x04,
		OT_SGD    = 0x05,
		OT_SPC    = 0x06,
		OT_RP     = 0x07,
		OT_DEMO   = 0x08,
		OT_ANI    = 0x09,
		OT_OBJ    = 0x0A,
		OT_TBN    = 0x0B,
		OT_SPR    = 0x0C,
		OT_TAB    = 0x0D,
		OT_ICN    = 0x0E,
		OT_FNT    = 0x0F,
		OT_TXTBIN = 0x10,
		OT_CMD    = 0x11,
		OT_POL    = 0x12,
		OT_SPRM   = 0x13,
		OT_OFF    = 0x14,

		OT_NUM    = 0x15
	};

	static const uint16 _voicesOffsetsTable[];

	const char *_dataPath;
	Version _ver;
	char _entryName[30];
	uint8 *_fnt;
	MbkEntry *_mbk;
	uint8 *_mbkData;
	uint8 *_icn;
	uint8 *_tab;
	uint8 *_spc; // BE
	uint16 _numSpc;
	uint8 _rp[0x4A];
	uint8 *_pal; // BE
	uint8 *_ani;
	uint8 *_tbn;
	int8 _ctData[0x1D00];
	uint8 *_spr1;
	uint8 *_spr_off[1287]; // 0-0x22F + 0x28E-0x2E9 ... conrad, 0x22F-0x28D : junkie
	uint8 _sprm[0x8411]; // MERCENAI.SPR size
	uint16 _pgeNum;
	InitPGE _pgeInit[256];
	uint8 *_map;
	uint16 _numObjectNodes;
	ObjectNode *_objectNodesMap[255];
	uint8 *_memBuf;
	SoundFx *_sfxList;
	uint8 _numSfx;
	uint8 *_cmd;
	uint8 *_pol;
	uint8 *_cine_off;
	uint8 *_cine_txt;
	uint8 *_voiceBuf;
	char **_extTextsTable;
	const char **_textsTable;
	uint8 *_extStringsTable;
	const uint8 *_stringsTable;

	Resource(const char *dataPath, Version ver);
	~Resource();

	void clearLevelRes();
	void load_FIB(const char *fileName);
	void load_MAP_menu(const char *fileName, uint8 *dstPtr);
	void load_PAL_menu(const char *fileName, uint8 *dstPtr);
	void load_SPR_OFF(const char *fileName, uint8 *sprData);
	void load_CINE();
	void load_TEXT();
	void free_TEXT();
	void load(const char *objName, int objType);
	void load_CT(File *pf);
	void load_FNT(File *pf);
	void load_MBK(File *pf);
	void load_ICN(File *pf);
	void load_SPR(File *pf);
	void load_SPRM(File *pf);
	void load_RP(File *pf);
	void load_SPC(File *pf);
	void load_PAL(File *pf);
	void load_MAP(File *pf);
	void load_OBJ(File *pf);
	void free_OBJ();
	void load_PGE(File *pf);
	void load_ANI(File *pf);
	void load_TBN(File *pf);
	void load_CMD(File *pf);
	void load_POL(File *pf);
	void load_VCE(int num, int segment, uint8 **buf, uint32 *bufSize);
	const uint8 *getGameString(int num) {
		return _stringsTable + READ_LE_UINT16(_stringsTable + num * 2);
	}
	const char *getMenuString(int num) {
		return (num >= 0 && num < LocaleData::LI_NUM) ? _textsTable[num] : "";
	}
};

#endif // __RESOURCE_H__
