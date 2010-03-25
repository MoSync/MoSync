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

#ifndef STABS_TYPES_H
#define STABS_TYPES_H

#include "stabs_type.h"

//must call before adding any types for that file
void addTypeFile(int file, const std::string& fileName);

void addType(const Type&);

//returns NULL if the type cannot be found.
const TypeBase* findTypeByTuple(Tuple);
const TypeBase* findTypeByTupleAndFile(Tuple t, int file);

const TypeBase* findTypeByNameAndFileGlobal(const std::string& name, int scope);
const TypeBase* findTypeByNameAndTupleAndFileGlobal(const std::string& name, Tuple tuple, int scope);

void clearTypes();

#endif	//STABS_TYPES_H
