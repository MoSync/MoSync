/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "resource.h"
#include "bank.h"
#include "file.h"
#include "serializer.h"
#include "video.h"


// \bug WHY DID I NEED TO add 0:ing of _newPtrId and _curPtrsId
Resource::Resource(Video *vid, const char *dataDir) 
: _vid(vid), _dataDir(dataDir), _curPtrsId(0), _newPtrsId(0) {
}

void Resource::readBank(const MemEntry *me, uint8 *dstBuf) {
	uint16 n = me - _memList;
	debug(DBG_BANK, "Resource::readBank(%d)", n);
#ifdef USE_UNPACKED_DATA
	char bankEntryName[64];
	sprintf(bankEntryName, "ootw-%02X-%d.dump", n, me->type);
	File f;
	if (!f.open(bankEntryName, _dataDir)) {
		error("Resource::readBank() unable to open '%s' file\n", bankEntryName);
	}
	f.read(dstBuf, me->unpackedSize);
#else
	Bank bk(_dataDir);
	if (!bk.read(me, dstBuf)) {
		error("Resource::readBank() unable to unpack entry %d\n", n);
	}
#endif
}

void Resource::readEntries() {	
	File f;
	if (!f.open("memlist.bin", _dataDir)) {
		error("Resource::readEntries() unable to open 'memlist.bin' file\n");
	}
	_numMemList = 0;
	MemEntry *me = _memList;
	while (1) {
		assert(_numMemList < ARRAYSIZE(_memList));
		me->valid = f.readByte();
		me->type = f.readByte();
		me->bufPtr = 0; f.readUint16BE();
		me->unk4 = f.readUint16BE();
		me->rankNum = f.readByte();
		me->bankNum = f.readByte();
		me->bankPos = f.readUint32BE();
		me->unkC = f.readUint16BE();
		me->packedSize = f.readUint16BE();
		me->unk10 = f.readUint16BE();
		me->unpackedSize = f.readUint16BE();
		if (me->valid == 0xFF) {
			break;
		}
		++_numMemList;
		++me;
	}
}

void Resource::load() {
	while (1) {
		MemEntry *it = _memList;
		MemEntry *me = 0;

		// get resource with max rankNum
		uint8 maxNum = 0;
		uint16 i = _numMemList;
		while (i--) {
			if (it->valid == 2 && maxNum <= it->rankNum) {
				maxNum = it->rankNum;
				me = it;
			}
			++it;
		}
		if (me == 0) {
			break; // no entry found
		}

		uint8 *memPtr = 0;
		if (me->type == 2) {
			memPtr = _vidCurPtr;
		} else {
			memPtr = _scriptCurPtr;
			if (me->unpackedSize > _vidBakPtr - _scriptCurPtr) {
				warning("Resource::load() not enough memory");
				me->valid = 0;
				continue;
			}
		}
		if (me->bankNum == 0) {
			warning("Resource::load() ec=0x%X (me->bankNum == 0)", 0xF00);
			me->valid = 0;
		} else {
			debug(DBG_BANK, "Resource::load() bufPos=%X size=%X type=%X pos=%X bankNum=%X", memPtr - _memPtrStart, me->packedSize, me->type, me->bankPos, me->bankNum);
			readBank(me, memPtr);
			if(me->type == 2) {
				_vid->copyPagePtr(_vidCurPtr);
				me->valid = 0;
			} else {
				me->bufPtr = memPtr;
				me->valid = 1;
				_scriptCurPtr += me->unpackedSize;
			}
		}
	}
}

void Resource::invalidateRes() {
	MemEntry *me = _memList;
	uint16 i = _numMemList;
	while (i--) {
		if (me->type <= 2 || me->type > 6) {
			me->valid = 0;
		}
		++me;
	}
	_scriptCurPtr = _scriptBakPtr;
}

void Resource::invalidateAll() {
	MemEntry *me = _memList;
	uint16 i = _numMemList;
	while (i--) {
		me->valid = 0;
		++me;
	}
	_scriptCurPtr = _memPtrStart;
}

void Resource::update(uint16 num) {
	if (num > _numMemList) {
		_newPtrsId = num;
	} else {
		MemEntry *me = &_memList[num];
		if (me->valid == 0) {
			me->valid = 2;
			load();
		}
	}
}

void Resource::setupPtrs(uint16 ptrId) {
	if (ptrId != _curPtrsId) {
		uint8 ipal = 0;
		uint8 icod = 0;
		uint8 ivd1 = 0;
		uint8 ivd2 = 0;
		if (ptrId >= 0x3E80 && ptrId <= 0x3E89) {
			uint16 part = ptrId - 0x3E80;
			ipal = _memListParts[part][0];
			icod = _memListParts[part][1];
			ivd1 = _memListParts[part][2];
			ivd2 = _memListParts[part][3];
		} else {
			error("Resource::setupPtrs() ec=0x%x invalid ptrId", 0xF07);
		}
		invalidateAll();
		_memList[ipal].valid = 2;
		_memList[icod].valid = 2;
		_memList[ivd1].valid = 2;
		if (ivd2 != 0) {
			_memList[ivd2].valid = 2;
		}
		load();
		_segVideoPal = _memList[ipal].bufPtr;
		_segCode = _memList[icod].bufPtr;
		_segVideo1 = _memList[ivd1].bufPtr;
		if (ivd2 != 0) {
			_segVideo2 = _memList[ivd2].bufPtr;
		}
		_curPtrsId = ptrId;
	}
	_scriptBakPtr = _scriptCurPtr;	
}

void Resource::allocMemBlock() {
	_memPtrStart = (uint8 *)malloc(MEM_BLOCK_SIZE);
	_scriptBakPtr = _scriptCurPtr = _memPtrStart;
	_vidBakPtr = _vidCurPtr = _memPtrStart + MEM_BLOCK_SIZE - 0x800 * 16;
	_useSegVideo2 = false;
}

void Resource::freeMemBlock() {
	free(_memPtrStart);
}

void Resource::saveOrLoad(Serializer &ser) {
	uint8 loadedList[64];
	if (ser._mode == Serializer::SM_SAVE) {
		memset(loadedList, 0, sizeof(loadedList));
		uint8 *p = loadedList;
		uint8 *q = _memPtrStart;
		while (1) {
			MemEntry *it = _memList;
			MemEntry *me = 0;
			uint16 num = _numMemList;
			while (num--) {
				if (it->valid == 1 && it->bufPtr == q) {
					me = it;
				}
				++it;
			}
			if (me == 0) {
				break;
			} else {
				assert(p < loadedList + 64);
				*p++ = me - _memList;
				q += me->unpackedSize;
			}
		}
	}
	Serializer::Entry entries[] = {
		SE_ARRAY(loadedList, 64, Serializer::SES_INT8, VER(1)),
		SE_INT(&_curPtrsId, Serializer::SES_INT16, VER(1)),
		SE_PTR(&_scriptBakPtr, VER(1)),
		SE_PTR(&_scriptCurPtr, VER(1)),
		SE_PTR(&_vidBakPtr, VER(1)),
		SE_PTR(&_vidCurPtr, VER(1)),
		SE_INT(&_useSegVideo2, Serializer::SES_BOOL, VER(1)),
		SE_PTR(&_segVideoPal, VER(1)),
		SE_PTR(&_segCode, VER(1)),
		SE_PTR(&_segVideo1, VER(1)),
		SE_PTR(&_segVideo2, VER(1)),
		SE_END()
	};
	ser.saveOrLoadEntries(entries);
	if (ser._mode == Serializer::SM_LOAD) {
		uint8 *p = loadedList;
		uint8 *q = _memPtrStart;
		while (*p) {
			MemEntry *me = &_memList[*p++];
			readBank(me, q);
			me->bufPtr = q;
			me->valid = 1;
			q += me->unpackedSize;
		}
	}	
}
