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

#include "unpack.h"


static int rcr(UnpackCtx *uc, int CF) {
	int rCF = (uc->chk & 1);
	uc->chk >>= 1;
	if (CF) {
		uc->chk |= 0x80000000;
	}
	return rCF;
}

static int next_chunk(UnpackCtx *uc) {
	int CF = rcr(uc, 0);
	if (uc->chk == 0) {
		uc->chk = READ_BE_UINT32(uc->src); uc->src -= 4;
		uc->crc ^= uc->chk;
		CF = rcr(uc, 1);
	}
	return CF;
}

static uint16 get_code(UnpackCtx *uc, uint8 num_chunks) {
	uint16 c = 0;
	while (num_chunks--) {
		c <<= 1;
		if (next_chunk(uc)) {
			c |= 1;
		}
	}
	return c;
}

static void dec_unk1(UnpackCtx *uc, uint8 num_chunks, uint8 add_count) {
	uint16 count = get_code(uc, num_chunks) + add_count + 1;
	uc->datasize -= count;
	while (count--) {
		*uc->dst = (uint8)get_code(uc, 8);
		--uc->dst;
	}
}

static void dec_unk2(UnpackCtx *uc, uint8 num_chunks) {
	uint16 i = get_code(uc, num_chunks);
	uint16 count = uc->size + 1;
	uc->datasize -= count;
	while (count--) {
		*uc->dst = *(uc->dst + i);
		--uc->dst;
	}
}

bool delphine_unpack(uint8 *dst, const uint8 *src, int len) {
	UnpackCtx uc;
	uc.src = src + len - 4;
	uc.datasize = READ_BE_UINT32(uc.src); uc.src -= 4;
	uc.dst = dst + uc.datasize - 1;
	uc.size = 0;
	uc.crc = READ_BE_UINT32(uc.src); uc.src -= 4;
	uc.chk = READ_BE_UINT32(uc.src); uc.src -= 4;
	debug(DBG_UNPACK, "delphine_unpack() crc=0x%X datasize=0x%X", uc.crc, uc.datasize);
	uc.crc ^= uc.chk;
	do {
		if (!next_chunk(&uc)) {
			uc.size = 1;
			if (!next_chunk(&uc)) {
				dec_unk1(&uc, 3, 0);
			} else {
				dec_unk2(&uc, 8);
			}
		} else {
			uint16 c = get_code(&uc, 2);
			if (c == 3) {
				dec_unk1(&uc, 8, 8);
			} else if (c < 2) {
				uc.size = c + 2;
				dec_unk2(&uc, c + 9);
			} else {
				uc.size = get_code(&uc, 8);
				dec_unk2(&uc, 12);
			}
		}
	} while (uc.datasize > 0);
	return uc.crc == 0;
}
