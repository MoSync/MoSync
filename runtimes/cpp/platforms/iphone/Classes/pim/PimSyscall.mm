/* Copyright (C) 2011 Mobile Sorcery AB

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

#include "config_platform.h"

#import "syscall.h"
#include "PimDatabase.h"
#import "PimUtil.h"

using namespace MoSyncError;
using namespace Base;


//***************************************************************************
//Variables
//***************************************************************************

PimDatabase* sPimDatabase = NULL;

//***************************************************************************
//Initialization
//***************************************************************************

void MAPimInit() {
	sPimDatabase = [[PimDatabase alloc] init];
}

void MAPimClose() {
	[sPimDatabase release];
    [PimUtils deleteInstance];
}


MAHandle Syscall::maPimListOpen(int listType)
{
    return [sPimDatabase pimListOpen:listType];
}

MAHandle Syscall::maPimListNext(MAHandle list)
{
    return [sPimDatabase pimListNext:list];
}

int Syscall::maPimListClose(MAHandle list)
{
     return [sPimDatabase pimListClose:list];
}

int Syscall::maPimItemCount(MAHandle item)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem count];
}

int Syscall::maPimItemGetField(MAHandle item, int n)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem getFieldID:n];
}

int Syscall::maPimItemFieldCount(MAHandle item, int field)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem fieldCount:field];
}

int Syscall::maPimItemGetAttributes(MAHandle item, int field, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem getAttribute:field indexValue:index];
}


int Syscall::maPimItemSetLabel(const MA_PIM_ARGS* args, int index)
{

    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem setLabel:args indexValue:index];
}

int Syscall::maPimItemGetLabel(const MA_PIM_ARGS* args, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem getLabel:args indexValue:index];
}

int Syscall::maPimFieldType(MAHandle list, int field)
{
    bool singleFieldValue;
    int fieldType;
    int resultCode;

    // Check if the list handle is valid.
    if (MA_PIM_CONTACTS != list)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    resultCode = [[PimUtils sharedInstance] fieldStructure:field
                                                       setType:&fieldType
                                              setIsSingleValue:&singleFieldValue];
    // Check for errors.
    if (MA_PIM_ERR_NONE == resultCode)
    {
        return fieldType;
    }

    return resultCode;
}

int Syscall::maPimItemGetValue(const MA_PIM_ARGS *args, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem getValue:args indexValue:index];
}


int Syscall::maPimItemSetValue(const MA_PIM_ARGS* args, int index, int attributes)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem setValue:args
                  indexValue:index
              valueAttribute:attributes];
}

int Syscall::maPimItemAddValue(const MA_PIM_ARGS* args, int attributes)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem addValue:args
               withAttribute:attributes];
}

int Syscall::maPimItemRemoveValue(MAHandle item, int field, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem removeValue:field atIndex:index];
}

int Syscall::maPimItemClose(MAHandle item)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    return [pimItem close];
}

MAHandle Syscall::maPimItemCreate(MAHandle list)
{
    return [sPimDatabase createItem:list];
}

int Syscall::maPimItemRemove(MAHandle list, MAHandle item)
{
    return [sPimDatabase removeItem:list item:item];
}