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

#include "resource.h"
#include "systemstub.h"
#include "video.h"


Video::Video(Resource *res, SystemStub *stub)
	: _res(res), _stub(stub) {
	_frontLayer = (uint8 *)malloc(GAMESCREEN_W * GAMESCREEN_H);
	memset(_frontLayer, 0, GAMESCREEN_W * GAMESCREEN_H);
	_backLayer = (uint8 *)malloc(GAMESCREEN_W * GAMESCREEN_H);
	memset(_backLayer, 0, GAMESCREEN_W * GAMESCREEN_H);
	_tempLayer = (uint8 *)malloc(GAMESCREEN_W * GAMESCREEN_H);
	memset(_tempLayer, 0, GAMESCREEN_W * GAMESCREEN_H);
	_tempLayer2 = (uint8 *)malloc(GAMESCREEN_W * GAMESCREEN_H);
	memset(_tempLayer2, 0, GAMESCREEN_W * GAMESCREEN_H);
	_screenBlocks = (uint8 *)malloc((GAMESCREEN_W / SCREENBLOCK_W) * (GAMESCREEN_H / SCREENBLOCK_H));
	memset(_screenBlocks, 0, (GAMESCREEN_W / SCREENBLOCK_W) * (GAMESCREEN_H / SCREENBLOCK_H));
	_fullRefresh = true;
	_shakeOffset = 0;
	_charFrontColor = 0;
	_charTransparentColor = 0;
	_charShadowColor = 0;
}

Video::~Video() {
	free(_frontLayer);
	free(_backLayer);
	free(_tempLayer);
	free(_tempLayer2);
	free(_screenBlocks);
}

void Video::markBlockAsDirty(int16 x, int16 y, uint16 w, uint16 h) {
	debug(DBG_VIDEO, "Video::markBlockAsDirty(%d, %d, %d, %d)", x, y, w, h);
	assert(x >= 0 && x + w <= GAMESCREEN_W && y >= 0 && y + h <= GAMESCREEN_H);
	int bx1 = x / SCREENBLOCK_W;
	int by1 = y / SCREENBLOCK_H;
	int bx2 = (x + w - 1) / SCREENBLOCK_W;
	int by2 = (y + h - 1) / SCREENBLOCK_H;
	assert(bx2 < GAMESCREEN_W / SCREENBLOCK_W && by2 < GAMESCREEN_H / SCREENBLOCK_H);
	for (; by1 <= by2; ++by1) {
		for (int i = bx1; i <= bx2; ++i) {
			_screenBlocks[by1 * (GAMESCREEN_W / SCREENBLOCK_W) + i] = 2;
		}
	}
}

void Video::updateScreen() {
	debug(DBG_VIDEO, "Video::updateScreen()");
//	_fullRefresh = true;
	if (_fullRefresh) {
		_stub->copyRect(0, 0, Video::GAMESCREEN_W, Video::GAMESCREEN_H, _frontLayer, 256);
		_stub->updateScreen(_shakeOffset);
		_fullRefresh = false;
	} else {
		int i, j;
		int count = 0;
		uint8 *p = _screenBlocks;
		for (j = 0; j < GAMESCREEN_H / SCREENBLOCK_H; ++j) {
			uint16 nh = 0;
			for (i = 0; i < GAMESCREEN_W / SCREENBLOCK_W; ++i) {
				if (p[i] != 0) {
					--p[i];
					++nh;
				} else if (nh != 0) {
					int16 x = (i - nh) * SCREENBLOCK_W;
					_stub->copyRect(x, j * SCREENBLOCK_H, nh * SCREENBLOCK_W, SCREENBLOCK_H, _frontLayer, 256);
					nh = 0;
					++count;
				}
			}
			if (nh != 0) {
				int16 x = (i - nh) * SCREENBLOCK_W;
				_stub->copyRect(x, j * SCREENBLOCK_H, nh * SCREENBLOCK_W, SCREENBLOCK_H, _frontLayer, 256);
				++count;
			}
			p += GAMESCREEN_W / SCREENBLOCK_W;
		}
		if (count != 0) {
			_stub->updateScreen(_shakeOffset);
		}
	}
	if (_shakeOffset != 0) {
		_shakeOffset = 0;
		_fullRefresh = true;
	}
}

void Video::fullRefresh() {
	debug(DBG_VIDEO, "Video::fullRefresh()");
	_fullRefresh = true;
	memset(_screenBlocks, 0, (GAMESCREEN_W / SCREENBLOCK_W) * (GAMESCREEN_H / SCREENBLOCK_H));
}

void Video::fadeOut() {
	debug(DBG_VIDEO, "Video::fadeOut()");
	for (int step = 16; step >= 0; --step) {
		for (int c = 0; c < 256; ++c) {
			Color col;
			_stub->getPaletteEntry(c, &col);
			col.r = col.r * step >> 4;
			col.g = col.g * step >> 4;
			col.b = col.b * step >> 4;
			_stub->setPaletteEntry(c, &col);
		}
		fullRefresh();
		updateScreen();
		_stub->sleep(50);
	}
}

void Video::setPaletteSlotBE(int palSlot, int palNum) {
	debug(DBG_VIDEO, "Video::setPaletteSlotBE()");
	const uint8 *p = _res->_pal + palNum * 0x20;
	for (int i = 0; i < 16; ++i) {
		uint16 color = READ_BE_UINT16(p); p += 2;
		uint8 t = (color == 0) ? 0 : 3;
		Color c;
		c.r = ((color & 0x00F) << 2) | t;
		c.g = ((color & 0x0F0) >> 2) | t;
		c.b = ((color & 0xF00) >> 6) | t;
		_stub->setPaletteEntry(palSlot * 0x10 + i, &c);
	}
}

void Video::setPaletteSlotLE(int palSlot, const uint8 *palData) {
	debug(DBG_VIDEO, "Video::setPaletteSlotLE()");
	for (int i = 0; i < 16; ++i) {
		uint16 color = READ_LE_UINT16(palData); palData += 2;
		Color c;
		c.b = (color & 0x00F) << 2;
		c.g = (color & 0x0F0) >> 2;
		c.r = (color & 0xF00) >> 6;
		_stub->setPaletteEntry(palSlot * 0x10 + i, &c);
	}
}

void Video::setTextPalette() {
	debug(DBG_VIDEO, "Video::setTextPalette()");
	const uint8 *p = _textPal;
	for (int i = 0; i < 16; ++i) {
		uint16 color = READ_LE_UINT16(p); p += 2;
		Color c;
		c.b = (color & 0x00F) << 2;
		c.g = (color & 0x0F0) >> 2;
		c.r = (color & 0xF00) >> 6;
		_stub->setPaletteEntry(0xE0 + i, &c);
	}
}

void Video::setPalette0xF() {
	debug(DBG_VIDEO, "Video::setPalette0xF()");
	const uint8 *p = _palSlot0xF;
	for (int i = 0; i < 16; ++i) {
		Color c;
		c.r = *p++ >> 2;
		c.g = *p++ >> 2;
		c.b = *p++ >> 2;
		_stub->setPaletteEntry(0xF0 + i, &c);
	}
}

void Video::copyLevelMap(uint16 room) {
	debug(DBG_VIDEO, "Video::copyLevelMap(%d)", room);
	assert(room < 0x40);
	int32 off = READ_LE_UINT32(_res->_map + room * 6);
	if (off == 0) {
		error("Invalid room %d", room);
	}
	bool packed = true;
	if (off < 0) {
		off = -off;
		packed = false;
	}
	const uint8 *p = _res->_map + off;
	_mapPalSlot1 = *p++;
	_mapPalSlot2 = *p++;
	_mapPalSlot3 = *p++;
	_mapPalSlot4 = *p++;
	if (packed) {
		uint8 *vid = _frontLayer;
		for (int i = 0; i < 4; ++i) {
			uint16 sz = READ_LE_UINT16(p); p += 2;
			decodeLevelMap(sz, p, _res->_memBuf); p += sz;
			memcpy(vid, _res->_memBuf, 256 * 56);
			vid += 256 * 56;
		}
	} else {
		for (int i = 0; i < 4; ++i) {
			for (int y = 0; y < 224; ++y) {
				for (int x = 0; x < 64; ++x) {
					_frontLayer[i + x * 4 + 256 * y] = p[256 * 56 * i + x + 64 * y];
				}
			}
		}
	}
	memcpy(_backLayer, _frontLayer, Video::GAMESCREEN_W * Video::GAMESCREEN_H);
}

void Video::decodeLevelMap(uint16 sz, const uint8 *src, uint8 *dst) {
	debug(DBG_VIDEO, "Video::decodeLevelMap() sz = 0x%X", sz);
	const uint8 *end = src + sz;
	while (src < end) {
		int16 code = (int8)*src++;
		if (code < 0) {
			int len = 1 - code;
			memset(dst, *src++, len);
			dst += len;
		} else {
			++code;
			memcpy(dst, src, code);
			src += code;
			dst += code;
		}
	}
}

void Video::setLevelPalettes() {
	debug(DBG_VIDEO, "Video::setLevelPalettes()");
	if (_unkPalSlot2 == 0) {
		_unkPalSlot2 = _mapPalSlot3;
	}
	if (_unkPalSlot1 == 0) {
		_unkPalSlot1 = _mapPalSlot3;
	}
	setPaletteSlotBE(0x0, _mapPalSlot1);
	setPaletteSlotBE(0x1, _mapPalSlot2);
	setPaletteSlotBE(0x2, _mapPalSlot3);
	setPaletteSlotBE(0x3, _mapPalSlot4);
	if (_unkPalSlot1 == _mapPalSlot3) {
		setPaletteSlotLE(4, _conradPal1);
	} else {
		setPaletteSlotLE(4, _conradPal2);
	}
	// slot 5 is monster palette
	setPaletteSlotBE(0x8, _mapPalSlot1);
	setPaletteSlotBE(0x9, _mapPalSlot2);
	setPaletteSlotBE(0xA, _unkPalSlot2);
	setPaletteSlotBE(0xB, _mapPalSlot4);
	// slots 0xC and 0xD are cutscene palettes
	setTextPalette();
}

void Video::drawSpriteSub1(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub1(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[i] != 0) {
				dst[i] = src[i] | colMask;
			}
		}
		src += pitch;
		dst += 256;
	}
}

void Video::drawSpriteSub2(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub2(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[-i] != 0) {
				dst[i] = src[-i] | colMask;
			}
		}
		src += pitch;
		dst += 256;
	}
}

void Video::drawSpriteSub3(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub3(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[i] != 0 && !(dst[i] & 0x80)) {
				dst[i] = src[i] | colMask;
			}
		}
		src += pitch;
		dst += 256;
	}
}

void Video::drawSpriteSub4(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub4(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[-i] != 0 && !(dst[i] & 0x80)) {
				dst[i] = src[-i] | colMask;
			}
		}
		src += pitch;
		dst += 256;
	}
}

void Video::drawSpriteSub5(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub5(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[i * pitch] != 0 && !(dst[i] & 0x80)) {
				dst[i] = src[i * pitch] | colMask;
			}
		}
		++src;
		dst += 256;
	}
}

void Video::drawSpriteSub6(const uint8 *src, uint8 *dst, int pitch, int h, int w, uint8 colMask) {
	debug(DBG_VIDEO, "Video::drawSpriteSub6(0x%X, 0x%X, 0x%X, 0x%X)", pitch, w, h, colMask);
	while (h--) {
		for (int i = 0; i < w; ++i) {
			if (src[-i * pitch] != 0 && !(dst[i] & 0x80)) {
				dst[i] = src[-i * pitch] | colMask;
			}
		}
		++src;
		dst += 256;
	}
}

void Video::drawChar(uint8 c, int16 y, int16 x) {
	debug(DBG_VIDEO, "Video::drawChar(0x%X, %d, %d)", c, y, x);
	y *= 8;
	x *= 8;
	const uint8 *src = _res->_fnt + (c - 32) * 32;
	uint8 *dst = _frontLayer + x + 256 * y;
	for (int h = 0; h < 8; ++h) {
		for (int i = 0; i < 4; ++i) {
			uint8 c1 = (*src & 0xF0) >> 4;
			uint8 c2 = (*src & 0x0F) >> 0;
			++src;

			if (c1 != 0) {
				if (c1 != 2) {
					*dst = _charFrontColor;
				} else {
					*dst = _charShadowColor;
				}
			} else if (_charTransparentColor != 0xFF) {
				*dst = _charTransparentColor;
			}
			++dst;

			if (c2 != 0) {
				if (c2 != 2) {
					*dst = _charFrontColor;
				} else {
					*dst = _charShadowColor;
				}
			} else if (_charTransparentColor != 0xFF) {
				*dst = _charTransparentColor;
			}
			++dst;
		}
		dst += 256 - 8;
	}
}

const char *Video::drawString(const char *str, int16 x, int16 y, uint8 col) {
	debug(DBG_VIDEO, "Video::drawString('%s', %d, %d, 0x%X)", str, x, y, col);
	int len = 0;
	int offset = y * 256 + x;
	uint8 *dst = _frontLayer + offset;
	while (1) {
		uint8 c = *str++;
		if (c == 0 || c == 0xB || c == 0xA) {
			break;
		}
		uint8 *dst_char = dst;
		const uint8 *src = _res->_fnt + (c - 32) * 32;
		for (int h = 0; h < 8; ++h) {
			for (int w = 0; w < 4; ++w) {
				uint8 c1 = (*src & 0xF0) >> 4;
				uint8 c2 = (*src & 0x0F) >> 0;
				++src;
				if (c1 != 0) {
					*dst_char = (c1 == 0xF) ? col : (0xE0 + c1);
				}
				++dst_char;
				if (c2 != 0) {
					*dst_char = (c2 == 0xF) ? col : (0xE0 + c2);
				}
				++dst_char;
			}
			dst_char += 256 - 8;
		}
		dst += 8; // character width
		++len;
	}
	markBlockAsDirty(x, y, len * 8, 8);
	return str - 1;
}
