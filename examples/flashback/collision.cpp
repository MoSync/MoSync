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

#include "game.h"
#include "resource.h"


void Game::col_prepareRoomState() {
	memset(_col_activeCollisionSlots, 0xFF, sizeof(_col_activeCollisionSlots));
	_col_currentLeftRoom = _res._ctData[CT_LEFT_ROOM + _currentRoom];
	_col_currentRightRoom = _res._ctData[CT_RIGHT_ROOM + _currentRoom];
	for (int i = 0; i != _col_curPos; ++i) {
		CollisionSlot *_di = _col_slotsTable[i];
		uint8 room = _di->ct_pos / 64;
		if (room == _currentRoom) {
			_col_activeCollisionSlots[0x30 + (_di->ct_pos & 0x3F)] = i;
		} else if (room == _col_currentLeftRoom) {
			_col_activeCollisionSlots[0x00 + (_di->ct_pos & 0x3F)] = i;
		} else if (room == _col_currentRightRoom) {
			_col_activeCollisionSlots[0x60 + (_di->ct_pos & 0x3F)] = i;
		}
	}
#ifdef DEBUG_COLLISION
	printf("---\n");
	for (int y = 0; y < 7; ++y) {
		for (int x = 0; x < 16; ++x) {
			printf("%d", _res._ctData[0x100 + _currentRoom * 0x70 + y * 16 + x]);
		}
		printf("\n");
	}
#endif
}

void Game::col_clearState() {
	_col_curPos = 0;
	_col_curSlot = _col_slots;
}

void Game::col_preparePiegeState(LivePGE *pge) {
	debug(DBG_COL, "Game::col_preparePiegeState() pge_num=%d", pge - &_pgeLive[0]);
	CollisionSlot *ct_slot1, *ct_slot2;
	if (pge->init_PGE->unk1C == 0) {
		pge->collision_slot = 0xFF;
		return;
	}
	int i = 0;
	ct_slot1 = 0;
	for (int c = 0; c < pge->init_PGE->unk1C; ++c) {
		ct_slot2 = _col_curSlot;
		if (ct_slot2 + 1 > &_col_slots[255])
			return;
		_col_curSlot = ct_slot2 + 1;
		int16 pos = col_getGridPos(pge, i);
		if (pos < 0) {
			if (ct_slot1 == 0) {
				pge->collision_slot = 0xFF;
			} else {
				ct_slot1->index = 0xFFFF;
			}
			return;
		}
		ct_slot2->ct_pos = pos;
		ct_slot2->live_pge = pge;
		ct_slot2->index = 0xFFFF;
		int16 _ax = col_findSlot(pos);
		if (_ax >= 0) {
			ct_slot2->prev_slot = _col_slotsTable[_ax];
			_col_slotsTable[_ax] = ct_slot2;
			if (ct_slot1 == 0) {
				pge->collision_slot = _ax;
			} else {
				ct_slot1->index = _ax;
			}
			LivePGE *temp_pge = ct_slot2->live_pge;
			if (temp_pge->flags & 0x80) {
				_pge_liveTable2[temp_pge->index] = temp_pge;
				temp_pge->flags |= 4;
			}
			if (ct_slot2->prev_slot) {
				temp_pge = ct_slot2->prev_slot->live_pge;
				if (temp_pge->flags & 0x80) {
					_pge_liveTable2[temp_pge->index] = temp_pge;
					temp_pge->flags |= 4;
				}
			}
		} else {
			ct_slot2->prev_slot = 0;
			_col_slotsTable[_col_curPos] = ct_slot2;
			if (ct_slot1 == 0) {
				pge->collision_slot = _col_curPos;
			} else {
				ct_slot1->index = _col_curPos;
			}
			_col_curPos++;
		}
		ct_slot1 = ct_slot2;
		i += 0x10;
	}
}

uint16 Game::col_getGridPos(LivePGE *pge, int16 dx) {
	int16 x = pge->pos_x + dx;
	int16 y = pge->pos_y;

	int8 c = pge->room_location;
	if (c < 0) return 0xFFFF;

	if (x < 0) {
		c = _res._ctData[CT_LEFT_ROOM + c];
		if (c < 0) return 0xFFFF;
		x += 256;
	} else if (x >= 256) {
		c = _res._ctData[CT_RIGHT_ROOM + c];
		if (c < 0) return 0xFFFF;
		x -= 256;
	} else if (y < 0) {
		c = _res._ctData[CT_UP_ROOM + c];
		if (c < 0) return 0xFFFF;
		y += 216;
	} else if (y >= 216) {
		c = _res._ctData[CT_DOWN_ROOM + c];
		if (c < 0) return 0xFFFF;
		y -= 216;
	}

	x = (x + 8) >> 4;
	y = (y - 8) / 72;
	if (x < 0 || x > 15 || y < 0 || y > 2) {
		return 0xFFFF;
	} else {
		return y * 16 + x + c * 64;
	}
}

int16 Game::col_findSlot(int16 pos) {
	for (uint16 i = 0; i < _col_curPos; ++i) {
		if (_col_slotsTable[i]->ct_pos == pos)
			return i;
	}
	return -1;
}

int16 Game::col_getGridData(LivePGE *pge, int16 dy, int16 dx) {
	if (_pge_currentPiegeFacingDir) {
		dx = -dx;
	}
	const int16 pge_grid_y = _col_currentPiegeGridPosY + dy;
	const int16 pge_grid_x = _col_currentPiegeGridPosX + dx;
	const int8 *room_ct_data;
	int8 next_room;
	if (pge_grid_x < 0) {
		room_ct_data = &_res._ctData[CT_LEFT_ROOM];
		next_room = room_ct_data[pge->room_location];
		if (next_room < 0) return 1;
		room_ct_data += pge_grid_x + 16 + pge_grid_y * 16 + next_room * 0x70;
		return (int16)room_ct_data[0x40];
	} else if (pge_grid_x >= 16) {
		room_ct_data = &_res._ctData[CT_RIGHT_ROOM];
		next_room = room_ct_data[pge->room_location];
		if (next_room < 0) return 1;
		room_ct_data += pge_grid_x - 16 + pge_grid_y * 16 + next_room * 0x70;
		return (int16)room_ct_data[0x80];
	} else if (pge_grid_y < 1) {
		room_ct_data = &_res._ctData[CT_UP_ROOM];
		next_room = room_ct_data[pge->room_location];
		if (next_room < 0) return 1;
		room_ct_data += pge_grid_x + (pge_grid_y + 6) * 16 + next_room * 0x70;
		return (int16)room_ct_data[0x100];
	} else if (pge_grid_y >= 7) {
		room_ct_data = &_res._ctData[CT_DOWN_ROOM];
		next_room = room_ct_data[pge->room_location];
		if (next_room < 0) return 1;
		room_ct_data += pge_grid_x + (pge_grid_y - 6) * 16 + next_room * 0x70;
		return (int16)room_ct_data[0xC0];
	} else {
		room_ct_data = &_res._ctData[0x100];
		room_ct_data += pge_grid_x + pge_grid_y * 16 + pge->room_location * 0x70;
		return (int16)room_ct_data[0];
	}
}

LivePGE *Game::col_findPiege(LivePGE *pge, uint16 arg2) {
	if (pge->collision_slot != 0xFF) {
		CollisionSlot *slot = _col_slotsTable[pge->collision_slot];
		while (slot) {
			if (slot->live_pge == pge) {
				slot = slot->prev_slot;
			} else {
				if (arg2 == 0xFFFF || arg2 == slot->live_pge->init_PGE->object_type) {
					return slot->live_pge;
				} else {
					slot = slot->prev_slot;
				}
			}
		}
	}
	return 0;
}

uint8 Game::col_findCurrentCollidingObject(LivePGE *pge, uint8 n1, uint8 n2, uint8 n3, LivePGE **pge_out) {
	if (pge_out) {
		*pge_out = pge;
	}
	if (pge->collision_slot != 0xFF) {
		CollisionSlot *cs = _col_slotsTable[pge->collision_slot];
		while (cs) {
			LivePGE *col_pge = cs->live_pge;
			if (pge_out) {
				*pge_out = col_pge;
			}
			if (col_pge->init_PGE->object_type == n1 ||
				col_pge->init_PGE->object_type == n2 ||
				col_pge->init_PGE->object_type == n3) {
				return col_pge->init_PGE->colliding_icon_num;
			} else {
				cs = cs->prev_slot;
			}
		}
	}
	return 0;
}

int16 Game::col_detectHit(LivePGE *pge, int16 arg2, int16 arg4, col_Callback1 callback1, col_Callback2 callback2, int16 argA, int16 argC) {
	debug(DBG_COL, "col_detectHit()");
	int16 pos_dx, pos_dy, var8, varA;
	int16 collision_score = 0;
	int8 pge_room = pge->room_location;
	if (pge_room < 0 || pge_room >= 0x40) {
		return 0;
	}
	int16 thr = pge->init_PGE->counter_values[0];
	if (thr > 0) {
		pos_dx = -1;
		pos_dy = -1;
	} else {
		pos_dx = 1;
		pos_dy = 1;
		thr = -thr;
	}
	if (_pge_currentPiegeFacingDir) {
		pos_dx = -pos_dx;
	}
	int16 grid_pos_x = (pge->pos_x + 8) >> 4;
	int16 grid_pos_y = (pge->pos_y / 72);
	if (grid_pos_y >= 0 && grid_pos_y <= 2) {
		grid_pos_y *= 16;
		collision_score = 0;
		var8 = 0;
		varA = 0;
		if (argA != 0) {
			var8 = pos_dy;
			grid_pos_x += pos_dx;
			varA = 1;
		}
		while (varA <= thr) {
			if (grid_pos_x < 0) {
				pge_room = _res._ctData[CT_LEFT_ROOM + pge_room];
				if (pge_room < 0) break;
				grid_pos_x += 16;
			}
			if (grid_pos_x >= 16) {
				pge_room = _res._ctData[CT_RIGHT_ROOM + pge_room];
				if (pge_room < 0) break;
				grid_pos_x -= 16;
			}
			int16 slot = col_findSlot(grid_pos_y + grid_pos_x + pge_room * 64);
			if (slot >= 0) {
				CollisionSlot *cs = _col_slotsTable[slot];
				while (cs) {

#ifdef MAPIP //hack? should work without this
					int opTableAddr = *((int*)&callback1);
					collision_score += (*(int (*)(void*, LivePGE *, LivePGE*, int16, int16))opTableAddr)(this, cs->live_pge, pge, arg2, arg4);
#else
					collision_score += (this->*callback1)(cs->live_pge, pge, arg2, arg4);
#endif


					cs = cs->prev_slot;
				}
			}

#ifdef MAPIP //hack? should work without this
					int opTableAddr = *((int*)&callback2);
					int res = (*(int (*)(void*, LivePGE *, int16, int16, int16))opTableAddr)(this, pge, var8, varA, arg2) ;
#else
					int res = (this->*callback2)(pge, var8, varA, arg2);
#endif

			if (res != 0) {
				break;
			}
			grid_pos_x += pos_dx;
			++varA;
			var8 += pos_dy;
		}
	}
	if (argC == -1) {
		return collision_score;
	} else {
		return 0;
	}
}

int Game::col_detectHitCallback1(LivePGE *pge, int16 dy, int16 unk1, int16 unk2) {
	if (col_getGridData(pge, 1, dy) != 0) {
		return 1;
	} else {
		return 0;
	}
}

int Game::col_detectHitCallback6(LivePGE *pge, int16 dy, int16 unk1, int16 unk2) {
	return 0;
}

int Game::col_detectHitCallback2(LivePGE *pge1, LivePGE *pge2, int16 unk1, int16 unk2) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == unk2) {
			if ((pge1->flags & 1) == (pge2->flags & 1)) {
				if (col_detectHitCallbackHelper(pge1, unk1) == 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int Game::col_detectHitCallback3(LivePGE *pge1, LivePGE *pge2, int16 unk1, int16 unk2) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == unk2) {
			if ((pge1->flags & 1) != (pge2->flags & 1)) {
				if (col_detectHitCallbackHelper(pge1, unk1) == 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int Game::col_detectHitCallback4(LivePGE *pge1, LivePGE *pge2, int16 unk1, int16 unk2) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == unk2) {
			if ((pge1->flags & 1) != (pge2->flags & 1)) {
				if (col_detectHitCallbackHelper(pge1, unk1) == 0) {
					pge_updateGroup(pge2->index, pge1->index, unk1);
					return 1;
				}
			}
		}
	}
	return 0;
}

int Game::col_detectHitCallback5(LivePGE *pge1, LivePGE *pge2, int16 unk1, int16 unk2) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == unk2) {
			if ((pge1->flags & 1) == (pge2->flags & 1)) {
				if (col_detectHitCallbackHelper(pge1, unk1) == 0) {
					pge_updateGroup(pge2->index, pge1->index, unk1);
					return 1;
				}
			}
		}
	}
	return 0;
}

int Game::col_detectHitCallbackHelper(LivePGE *pge, int16 groupId) {
	InitPGE *init_pge = pge->init_PGE;
	assert(init_pge->obj_node_number < _res._numObjectNodes);
	ObjectNode *on = _res._objectNodesMap[init_pge->obj_node_number];
	Object *obj = &on->objects[pge->first_obj_number];
	int i = pge->first_obj_number;
	while (pge->obj_type == obj->type && on->last_obj_number > i) {
		if (obj->opcode2 == 0x6B) { // pge_op_isInGroupSlice
			if (obj->opcode_arg2 == 0) {
				if (groupId == 1 || groupId == 2) return 0xFFFF;
			}
			if (obj->opcode_arg2 == 1) {
				if (groupId == 3 || groupId == 4) return 0xFFFF;
			}
		} else if (obj->opcode2 == 0x22) { // pge_op_isInGroup
			if (obj->opcode_arg2 == groupId) return 0xFFFF;
		}

		if (obj->opcode1 == 0x6B) { // pge_op_isInGroupSlice
			if (obj->opcode_arg1 == 0) {
				if (groupId == 1 || groupId == 2) return 0xFFFF;
			}
			if (obj->opcode_arg1 == 1) {
				if (groupId == 3 || groupId == 4) return 0xFFFF;
			}
		} else if (obj->opcode1 == 0x22) { // pge_op_isInGroup
			if (obj->opcode_arg1 == groupId) return 0xFFFF;
		}
		++obj;
		++i;
	}
	return 0;
}

int Game::col_detectGunHitCallback1(LivePGE *pge, int16 arg2, int16 arg4, int16 arg6) {
	int16 _ax = col_getGridData(pge, 1, arg2);
	if (_ax != 0) {
		if (!(_ax & 2) || (arg6 != 1)) {
			return _ax;
		}
	}
	return 0;
}

int Game::col_detectGunHitCallback2(LivePGE *pge1, LivePGE *pge2, int16 arg4, int16) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == 1 || pge1->init_PGE->object_type == 10) {
			uint8 id;
			if ((pge1->flags & 1) != (pge2->flags & 1)) {
				id = 4;
				if (arg4 == 0) {
					id = 3;
				}
			} else {
				id = 2;
				if (arg4 == 0) {
					id = 1;
				}
			}
			if (col_detectHitCallbackHelper(pge1, id) != 0) {
				pge_updateGroup(pge2->index, pge1->index, id);
				return 1;
			}
		}
	}
	return 0;
}

int Game::col_detectGunHitCallback3(LivePGE *pge1, LivePGE *pge2, int16 arg4, int16) {
	if (pge1 != pge2 && (pge1->flags & 4)) {
		if (pge1->init_PGE->object_type == 1 || pge1->init_PGE->object_type == 12 || pge1->init_PGE->object_type == 10) {
			uint8 id;
			if ((pge1->flags & 1) != (pge2->flags & 1)) {
				id = 4;
				if (arg4 == 0) {
					id = 3;
				}
			} else {
				id = 2;
				if (arg4 == 0) {
					id = 1;
				}
			}
			if (col_detectHitCallbackHelper(pge1, id) != 0) {
				pge_updateGroup(pge2->index, pge1->index, id);
				return 1;
			}

		}
	}
	return 0;
}

int Game::col_detectGunHit(LivePGE *pge, int16 arg2, int16 arg4, col_Callback1 callback1, col_Callback2 callback2, int16 argA, int16 argC) {
	int8 pge_room = pge->room_location;
	if (pge_room < 0 || pge_room >= 0x40) return 0;
	int16 thr, pos_dx, pos_dy;
	if (argC == -1) {
		thr = pge->init_PGE->counter_values[0];
	} else {
		thr = pge->init_PGE->counter_values[3];
	}
	if (thr > 0) {
		pos_dx = -1;
		pos_dy = -1;
	} else {
		pos_dx = 1;
		pos_dy = 1;
		thr = -thr;
	}
	if (_pge_currentPiegeFacingDir) {
		pos_dx = -pos_dx;
	}
	int16 grid_pos_x = (pge->pos_x + 8) >> 4;
	int16 grid_pos_y = (pge->pos_y - 8) / 72;
	if (grid_pos_y >= 0 && grid_pos_y <= 2) {
		grid_pos_y *= 16;
		int16 var8 = 0;
		int16 varA = 0;
		if (argA != 0) {
			var8 = pos_dy;
			grid_pos_x += pos_dx;
			varA = 1;
		}
		while (varA <= thr) {
			if (grid_pos_x < 0) {
				pge_room = _res._ctData[CT_LEFT_ROOM + pge_room];
				if (pge_room < 0) return 0;
				grid_pos_x += 0x10;
			}
			if (grid_pos_x >= 0x10) {
				pge_room = _res._ctData[CT_RIGHT_ROOM + pge_room];
				if (pge_room < 0) return 0;
				grid_pos_x -= 0x10;
			}
			int16 slot = col_findSlot(pge_room * 64 + grid_pos_x + grid_pos_y);
			if (slot >= 0) {
				CollisionSlot *cs = _col_slotsTable[slot];
				while (cs) {
					//int r = (this->*callback1)(cs->live_pge, pge, arg2, arg4);



#ifdef MAPIP //hack? should work without this
					int opTableAddr = *((int*)&callback1);
					int r = (*(int (*)(void*, LivePGE *, LivePGE *, int16, int16))opTableAddr)(this, cs->live_pge, pge, arg2, arg4);
#else
					int r = (this->*callback1)(cs->live_pge, pge, arg2, arg4);
#endif					
					if (r != 0) return r;
					cs = cs->prev_slot;
				}
			}

#ifdef MAPIP //hack? should work without this
					int opTableAddr = *((int*)&callback2);
					int res = (*(int (*)(void*, LivePGE *, int16, int16, int16))opTableAddr)(this, pge, var8, varA, arg2) ;
#else
					int res = (this->*callback2)(pge, var8, varA, arg2);
#endif			

			if(res != 0) {
				break;
			}
			grid_pos_x += pos_dx;
			++varA;
			var8 += pos_dy;
		}
	}
	return 0;
}
