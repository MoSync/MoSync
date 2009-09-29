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

#include "MAUIQueryResult.h"

BOOL MAUIQueryResult_first(MAUIQueryResult* self) {
	if(self->vtable && self->vtable->first)
		return self->vtable->first(self);
	return FALSE;
}

BOOL MAUIQueryResult_absolute(MAUIQueryResult* self, int n) {
	if(self->vtable && self->vtable->absolute)
		return self->vtable->absolute(self, n);
	return FALSE;
}

BOOL MAUIQueryResult_next(MAUIQueryResult* self) {
	if(self->vtable && self->vtable->next)
		return self->vtable->next(self);
	return FALSE;
}

BOOL MAUIQueryResult_previous(MAUIQueryResult* self) {
	if(self->vtable && self->vtable->previous)
		return self->vtable->previous(self);
	return FALSE;
}

char* MAUIQueryResult_getString(MAUIQueryResult* self, char* columnName) {
	if(self->vtable && self->vtable->getString)
		return self->vtable->getString(self, columnName);
	return NULL;
}

int MAUIQueryResult_getInt(MAUIQueryResult* self, char* columnName) {
	if(self->vtable && self->vtable->getInt)
		return self->vtable->getInt(self, columnName);
	return NULL;
}

Handle MAUIQueryResult_getImage(MAUIQueryResult* self, char* columnName) {
	if(self->vtable && self->vtable->getImage)
		return self->vtable->getImage(self, columnName);
	return NULL;
}

int MAUIQueryResult_getColumnIndex(MAUIQueryResult* self, char* columnName) {
	if(self->vtable && self->vtable->getColumnIndex)
		return self->vtable->getColumnIndex(self, columnName);
	return NULL;
}

