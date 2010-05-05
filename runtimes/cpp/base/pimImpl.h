/* Copyright (C) 2010 Mobile Sorcery AB

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

#ifndef PIMIMPL_H
#error "Bad include!"
#endif	//PIMIMPL_H

//This file is designed to be included in the private section of class Syscall.

void pimInit();
void pimClose();
PimItem* pimGetItem(MAHandle h);

MAHandle maPimListOpen(int listType);
MAHandle maPimListNext(MAHandle list);
int maPimListClose(MAHandle list);

int maPimItemCount(MAHandle item);
int maPimItemGetField(MAHandle item, int index);
int maPimItemFieldCount(MAHandle item, int field);
int maPimItemGetAttributes(MAHandle item, int field, int index);

int maPimFieldType(MAHandle list, int field);

int maPimItemGetValue(MA_PIM_ARGS* args, int index);
int maPimItemSetValue(MA_PIM_ARGS* args, int index, int attributes);
int maPimItemAddValue(MA_PIM_ARGS* args, int attributes);
int maPimItemRemoveValue(MAHandle item, int field, int index);
int maPimItemClose(MAHandle item);

MAHandle maPimItemCreate(MAHandle list);
int maPimItemRemove(MAHandle list, MAHandle item);

HashMap<PimList> mPimLists;
int mPimListNextHandle;
int mPimItemNextHandle;
