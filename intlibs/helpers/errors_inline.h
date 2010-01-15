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

#define IS_IN_SET(set) (code > ERROR_##set##_BASE && code < ERROR_##set##_END)

const char* MoSyncError::subSystemString(int code) {
#define RETURN_SET_STRING(set) if(IS_IN_SET(set)) return #set;
	ERROR_SETS(RETURN_SET_STRING);
	return NULL;
}

#define DECLARE_DESCRIPTIONS(set)\
	static const char* set##_descriptions[MoSyncError::ERROR_##set##_END];

ERROR_SETS(DECLARE_DESCRIPTIONS);

static void initErrors() {
	static bool inited = false;
	if(inited)
		return;
	inited = true;
	const char** curDescs;
	int curBase;
#define SET_INIT_D(val, id, desc) curDescs[val - curBase] = desc;
#define INIT_DESCRIPTIONS(set) curDescs = set##_descriptions;\
	curBase = MoSyncError::ERROR_##set##_BASE; set##_ERRORS(SET_INIT_D);
	ERROR_SETS(INIT_DESCRIPTIONS);
}

const char* MoSyncError::errorDescription(int code) {
	initErrors();
#define RETURN_DESCRIPTION(set)\
	if(IS_IN_SET(set)) return set##_descriptions[code - MoSyncError::ERROR_##set##_BASE];

	ERROR_SETS(RETURN_DESCRIPTION);
	return NULL;
}
