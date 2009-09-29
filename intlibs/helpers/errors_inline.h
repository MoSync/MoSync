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
	static const char* const set##_descriptions[] = { NULL set##_ERRORS(COMMA_SECOND_DOT) };

ERROR_SETS(DECLARE_DESCRIPTIONS);

const char* MoSyncError::errorDescription(int code) {
#define RETURN_DESCRIPTION(set)\
	if(IS_IN_SET(set)) return set##_descriptions[code - ERROR_##set##_BASE];

	ERROR_SETS(RETURN_DESCRIPTION);
	return NULL;
}
